#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules import rconlog_config as config
from .rcon_api import RconAPI as api
from .python_ipset.ipset.wrapper import IPSet
import xml.etree.ElementTree as ET
import ipaddress
from modules import rcon_log as logger


class RconFirewall():
    def __init__(self, session, args):
        self._fw_name = config.default['FW_NAME']
        self.args = args
        self.session = session

        # -- get firewall status (exit when disabled)
        _filter = [
            {'field': 'name', 'op': '==', 'value': self._fw_name}
        ]

        [firewall] = api.list_rcon_firewall(self.session, self.args, _filter)
        if not firewall.enabled:
            exit(0)

        # -- get enabled users

        _filter = [
            {'field': 'enabled', 'op': '==', 'value': True}
        ]

        self.users = api.list_rcon_users(self.session, self.args, _filter)
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
            servers = api.list_rcon_server(self.session, self.args, _filter)

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
            src_ipset = IPSet(set_name=src_setname, set_type="hash:ip", set_family="inet", netmask=24, ignore_existing=True)
            for server in servers:
                # get only new rules
                _filter = [
                    {'field': 'username', 'op': '==', 'value': user.username},
                    {'field': 'gamehost', 'op': '==', 'value': server.rcon_host},
                    {'field': 'gameport', 'op': '==', 'value': server.rcon_port},
                    {'field': self._fw_name, 'op': '==', 'value': False}
                ]

                rules = api.list_rcon_rule(self.session, self.args, _filter)
                for rule in rules:
                    src_ipset.add(rule.subnet)
                    api.update_rcon_rule(self.session, id=rule.id, updates=[{self._fw_name: True}])

            self.session.commit()

        # TODO: implement cli for add custom subnets
        rcon_custom = IPSet(set_name="rcon-custom", set_type="hash:ip", set_family="inet", netmask=24, ignore_existing=True)
        # import iptables module

        # load custom subnets (steam, internal)

        # + get subnets by firewall.name where state is 0 (column firewall_#)

        # + check ipset users chain (dst server ip list)
        #   + get users and servers for each users
        #     + check servers is added to ipset users chain, delete when disabled

        # + check ipset chain is exist (src host ip list) (incoming traffic)
        #   + add subnets to chain

        # check user iptables chain (iptables -N u4372)

        # check iptables rule in FORWARD table (iptables -C FORWARD -m set --match-set u4372-to dst -j u4372-from)
        # iptables -I FORWARD -m set --match-set u4372-to dst -j u4372
        # iptables -I u4372 -m set --match-set u4372-from src -j ACCEPT
        # iptables -A u4372 -j DROP (append to end chain)

