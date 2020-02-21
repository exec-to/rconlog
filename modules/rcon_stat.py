#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from sqlalchemy.orm import sessionmaker
from modules import rcon_core as core


class RconStat:
    def __init__(self, core):
        self.core = core
        self.session = sessionmaker(bind=self.core.engine)()

    def getAll(self):
        print('-'*20)

        users_count = self.session.query(core.RconServer.username).group_by(core.RconServer.username).count()
        print('Total users: {count}'.format(count=users_count))

        srv_count = self.session.query(core.RconServer.id).count()
        # print('Total RCON servers: {count}'.format(count=srv_count))
        print('Total RCON servers: {count}'.format(count=srv_count))

        ipaddr_count = self.session.query(core.Updates.id).count()
        print('Total IP-address: {count}'.format(count=ipaddr_count))
