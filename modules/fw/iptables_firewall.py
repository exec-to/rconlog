#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules.python_ipset.ipset.wrapper import IPSet
from modules.rcon_api import RconAPI as api
from modules import rcon_log as logger
import iptc

class IptablesFirewall:
    def __init__(self, name, session):
        self.name = name
        self.session = session

    def update_users_ipsets(self):
        # -- get enabled users

        _filter = [
            {'field': 'enabled', 'op': '==', 'value': True}
        ]

        users = api.list_rcon_users(self.session, None, _filter)
        for user in users:
            # ipset for current user (destination servers)
            dst_setname = "{}-dst".format(user.username)
            dst_ipset = IPSet(set_name=dst_setname, set_type="hash:ip", set_family="inet", ignore_existing=True)

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
                    # {'field': 'gamehost', 'op': '==', 'value': server.rcon_host},
                    # {'field': 'gameport', 'op': '==', 'value': server.rcon_port},
                    {'field': self.name, 'op': '==', 'value': False}
                ]

                rules = api.list_rcon_rule(self.session, None, _filter)
                for rule in rules:
                    src_ipset.add(rule.subnet)
                    api.update_rcon_rule(self.session, id=rule.id, updates=[{self.name: True}])

            self.session.commit()

    def update_custom_ipsets(self):
        rcon_custom = IPSet(set_name="rcon-custom", set_type="hash:net", set_family="inet", ignore_existing=True)
        subnets = api.list_custom_subnets(self.session)
        for subnet in subnets:
            if not rcon_custom.test(subnet.subnet):
                rcon_custom.add(subnet.subnet)


    def verify_users_iptables(self):
        # -- get all users

        _filter = [
            # {'field': 'enabled', 'op': '==', 'value': True}
        ]

        # -- add rules for enabled users and delete rules for disabled users

        users = api.list_rcon_users(self.session, None, _filter)
        possible_to_remove = []

        for user in users:
            uname = user.username
            dst_ipset = '{}-dst'.format(uname)
            src_ipset = '{}-src'.format(uname)
            src_custom = 'rcon-custom'

            if user.enabled and user.protected:
                table = iptc.Table(iptc.Table.FILTER)

                if not table.is_chain(uname):
                    iptc.easy.add_chain('filter', uname)
                    user_chain = iptc.Chain(table, uname)

                    # -- accept from custom networks
                    rule = iptc.Rule()
                    rule.target = rule.create_target("ACCEPT")
                    m = rule.create_match("set")
                    m.match_set = [src_custom, 'src']
                    user_chain.append_rule(rule)

                    # -- accept from user-based whitelist networks
                    rule = iptc.Rule()
                    rule.target = rule.create_target("ACCEPT")
                    m = rule.create_match("set")
                    m.match_set = [src_ipset, 'src']
                    user_chain.append_rule(rule)

                    # -- drop the rest networks
                    rule = iptc.Rule()
                    rule.create_target("DROP")
                    user_chain.append_rule(rule)

                    # -- jump forwarded packets to user destination chain
                    rule = iptc.Rule()
                    rule.target = rule.create_target(uname)
                    m = rule.create_match("set")
                    m.match_set = [dst_ipset, 'dst']
                    chain = iptc.Chain(iptc.Table(iptc.Table.FILTER), "FORWARD")
                    chain.insert_rule(rule)

            elif not (user.enabled and user.protected):
                if user.username not in possible_to_remove:
                    possible_to_remove.append(user.username)

        if possible_to_remove:
            for user in possible_to_remove:
                _filter = [
                    {'field': 'username', 'op': '==', 'value': user},
                    {'field': 'protected', 'op': '==', 'value': True}
                ]
                user_is_protected = api.list_rcon_users(self.session, None, _filter)

                if not user_is_protected:
                    uname = user
                    dst_setname = '{}-dst'.format(uname)
                    src_setname = '{}-src'.format(uname)

                    table = iptc.Table(iptc.Table.FILTER)
                    chain = iptc.Chain(table, "FORWARD")
                    for rule in chain.rules:
                        if rule.target and rule.target.name == uname:
                            chain.delete_rule(rule)
                            # print('prepare to remove iptables rule')

                    if table.is_chain(uname):
                        iptc.easy.delete_chain('filter', uname, flush=True)

                    dst_ipset = IPSet(set_name=dst_setname, set_type="hash:ip", set_family="inet", ignore_existing=True)
                    dst_ipset.destroy()

                    src_ipset = IPSet(set_name=src_setname, set_type="hash:ip", set_family="inet", netmask=24,
                                      ignore_existing=True)
                    src_ipset.destroy()

                    # flush user rules
                    logger.logging.info('Flush rules')
                    api.flush_user_rules(self.session, uname)

