#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules.python_ipset.ipset.wrapper import IPSet
from modules.rcon_api import RconAPI as api

class IptablesFirewall:
    def __init__(self, name, session):
        self.name = name
        self.session = session

    def update_users_ipsets(self):
        # -- get enabled users

        _filter = [
            {'field': 'enabled', 'op': '==', 'value': True}
        ]

        self.users = api.list_rcon_users(self.session, None, _filter)
        for user in self.users:
            # ipset for current user (destination servers)
            dst_setname = "{}-dst".format(user.username)
            dst_ipset = IPSet(set_name=dst_setname, set_type="hash:ip", set_family="inet", ignore_existing=True)

            # TODO: Implement cli for set protected property
            _filter = [
                {'field': 'username', 'op': '==', 'value': user.username},
                {'field': 'enabled', 'op': '==', 'value': True},
                {'field': 'protected', 'op': '==', 'value': True},
            ]
            servers = api.list_rcon_server(self.session, None, _filter)

            enabled_list = []
            for server in servers:
                dst_ipset.add(server.rcon_host)
                enabled_list.append(server.rcon_host)

            dst_list = []
            try:
                dst_list = dst_ipset.list_get()
            except Exception as e:
                logger.logging.error(
                    'Can\'t get ipset list {set}. Error: {error}'.format(set=dst_setname, error=str(e)))

            remove_list = list(set(dst_list) - set(enabled_list))

            for disabled in remove_list:
                dst_ipset.remove(disabled)

            # ipset for current user (source hosts)
            src_setname = "{}-src".format(user.username)
            src_ipset = IPSet(set_name=src_setname, set_type="hash:ip", set_family="inet", netmask=24,
                              ignore_existing=True)
            for server in servers:
                # get only new rules
                _filter = [
                    {'field': 'username', 'op': '==', 'value': user.username},
                    {'field': 'gamehost', 'op': '==', 'value': server.rcon_host},
                    {'field': 'gameport', 'op': '==', 'value': server.rcon_port},
                    {'field': self.name, 'op': '==', 'value': False}
                ]

                rules = api.list_rcon_rule(self.session, None, _filter)
                for rule in rules:
                    src_ipset.add(rule.subnet)
                    api.update_rcon_rule(self.session, id=rule.id, updates=[{self.name: True}])

            self.session.commit()

    def update_custom_ipsets(self):
        # TODO: implement cli for add custom subnets
        rcon_custom = IPSet(set_name="rcon-custom", set_type="hash:net", set_family="inet", ignore_existing=True)
        subnets = api.list_custom_subnets(self.session)
        for subnet in subnets:
            if not rcon_custom.test(subnet.subnet):
                rcon_custom.add(subnet.subnet)