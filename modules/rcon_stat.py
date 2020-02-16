#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from sqlalchemy.orm import sessionmaker
from modules import rcon_core
from sqlalchemy import func

class RconStat:
    def __init__(self, core):
        self.core = core
        self.session = sessionmaker(bind=self.core.engine)()

    def getAll(self):
        print('-'*20)

        print('Total users: {count}'.format(count=1))

        srv_count = self.session.query(rcon_core.RconServer.id).count()
        # print('Total RCON servers: {count}'.format(count=srv_count))
        print('Total RCON servers: {count}'.format(count=11))

        ipaddr_count = self.session.query(rcon_core.Updates.id).count()
        print('Total IP-address: {count}'.format(count=ipaddr_count))
