#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import sqlalchemy as db
from modules import rconlog_config as config
from sqlalchemy import Table, Column, Integer, String, MetaData, Boolean, ForeignKey
from sqlalchemy.orm import mapper
from modules import rcon_log as logger

class User(object):
    def __init__(self, userid, enabled):
        self.userid = userid
        self.enabled = enabled

    def __repr__(self):
        return "<User('%s','%s','%s')>" % (self.id, self.userid, self.enabled)


class Core:
    def __init__(self):
        logger.logging.info('Init Mysql engine with {con}'.
                            format(con='mysql+mysqlconnector://{user}:{passwd}@{host}:{port}/{db}'
                                  .format_map(config.database)))

        self.engine = db.create_engine('mysql+mysqlconnector://{user}:{passwd}@{host}:{port}/{db}'
                                  .format_map(config.database), echo=True)

        metadata = MetaData()
        users_table = Table('users', metadata,
                            Column('id', Integer, primary_key=True),
                            Column('userid', String(20)),
                            Column('enabled', Boolean)
                            )
        metadata.create_all(self.engine)
        mapper(User, users_table)
