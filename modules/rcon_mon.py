#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules.rcon_api import RconAPI as api
from modules.rcon_cli import RconCLI as cli

class RconMonitor():
    def __init__(self, servers):
        cli.list_rcon_server(servers)
        # self.updates = {}
