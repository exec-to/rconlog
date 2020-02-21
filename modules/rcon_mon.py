#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules.rcon_api import RconAPI as api
from modules.rcon_cli import RconCLI as cli


class RconMonitor():
    def __init__(self, session, args):
        self.args = args
        self.session = session
        _filter = [
            {'field': 'enabled', 'op': '==', 'value': True}
        ]

        self.servers = api.list_rcon_server(self.session, self.args, _filter)
        cli.list_rcon_server(self.servers)

        _filter = [
            {'field': 'rcon_server_id', 'op': '==', 'value': 1},
            {'field': 'ipaddr', 'op': '==', 'value': '176.49.196.133'}
        ]
        update = api.get_rcon_update(self.session, self.args, _filter)
        print (update.id)
        # self.updates = api.list_rcon_updates(self.session, self.args)
