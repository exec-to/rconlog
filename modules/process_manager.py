#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules import rcon_core
from sqlalchemy.orm import sessionmaker
from modules import rcon_stat
from modules import rcon_log as logger
from modules.rcon_api import RconAPI as api
from modules.rcon_cli import RconCLI as cli
from modules.rcon_mon import RconMonitor
from modules.rcon_firewall import RconFirewall

class ProcessManager:
    def __init__(self, mode, args):
        self._mode = mode
        self._args = args
        try:
            self.core = rcon_core.Core()
            self.session = sessionmaker(bind=self.core.engine)()
        except Exception as e:
            logger.logging.error('Can\'t initialize process manager. Error: {}'.format(str(e)))

    def run(self):
        if hasattr(self, self._mode):
            getattr(self, self._mode)()
        else:
            logger.logging.error('Can\'t find handler for {mode}'.format(mode=self._mode))

    def stat(self):
        stat = rcon_stat.RconStat(self.core)
        stat.getAll()

    def server(self):
        print('run server')

    def monitor(self):
        # TODO: argparse filter [stote_const] append filter
        RconMonitor(self.session, self._args)

    def web(self):
        print('run web')

    def cli(self):
        cmd = self._args.full_command
        data = getattr(api, cmd)(self.session, self._args)
        getattr(cli, cmd)(data)

    def firewall(self):
        fw = RconFirewall(self.session, self._args)