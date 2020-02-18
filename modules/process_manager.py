#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import operator
from modules import rcon_core
from sqlalchemy.orm import sessionmaker
from modules import rcon_stat
from test import Command


class ProcessManager:
    def __init__(self, mode):
        self._mode = mode
        self.core = rcon_core.Core()
        self.session = sessionmaker(bind=self.core.engine)()

    def run(self):
        operator.methodcaller(self._mode)(self)

    def run_command(self, cmd):
        print(cmd.command, cmd.host)

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
        print('run cli')
        # user = rcon_core.User("u4372", True)
        # print(user)
        # self.session.add(user)
        # self.session.commit()
        update = self.session.query(rcon_core.Updates).filter_by(gamehost='109.237.110.151').first()
        print(update.gamehost, update.gameport, update.ipaddr, update.created_at, update.rcon_server_id)
        # print(user)
        # print(user.id)

    def firewall(self):
        print('run firewall')