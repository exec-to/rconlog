#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules import rconlog_config as config
from modules.rcon_api import RconAPI as api

class RconFirewall():
    def __init__(self, session, args):
        name = config.default['FW_NAME']
        self.args = args
        self.session = session

        # -- get firewall status (exit when disabled)
        _filter = [
            {'field': 'name', 'op': '==', 'value': name}
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
            print(user.username)



        # load ipset module

        # load iptables module

        # load custom subnets (steam, internal)

        # get subnets by firewall.name where state is 0 (column firewall_#)

        # check ipset users chain (dst server ip list)

        # get users and servers for each users
          # check servers is added to ipset users chain, delete when disabled

        # check ipset chain is exist (src host ip list) (incoming traffic)
          # add subnets to chain

        # check user iptables chain (iptables -N u4372)

        # check iptables rule in FORWARD table (iptables -C FORWARD -m set --match-set u4372-to dst -j u4372-from)
        # iptables -I FORWARD -m set --match-set u4372-to dst -j u4372
        # iptables -I u4372 -m set --match-set u4372-from src -j ACCEPT
        # iptables -A u4372 -j DROP (append to end chain)

