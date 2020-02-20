#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from modules import rcon_core
from sqlalchemy.orm import sessionmaker
from modules import rcon_stat
from modules import rcon_log as logger
from modules.rcon_api import RconAPI as api

class ProcessManager:
    def __init__(self, mode, args):
        self._mode = mode
        self._args = args
        self.core = rcon_core.Core()
        self.session = sessionmaker(bind=self.core.engine)()

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
        print('run monitor')

    def web(self):
        print('run web')

    def cli(self):
        cmd = self._args.full_command
        data = getattr(api, cmd)(self.session, self._args)
        getattr(api, 'print_{cmd}'.format(cmd=cmd))(data)

    def firewall(self):
        print('run firewall')
        print(self._args)