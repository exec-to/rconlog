#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import sqlalchemy as db
from modules import rconlog_config as config
from sqlalchemy import Table, Column, Integer, String, MetaData, Boolean, ForeignKey, DateTime
from sqlalchemy.orm import mapper
from sqlalchemy.orm import relationship
from sqlalchemy.ext.declarative import declarative_base
from modules import rcon_log as logger
import datetime

Base = declarative_base()


class RconServer(Base):
    __tablename__ = 'rcon_servers'
    id = Column('id', Integer, primary_key=True)
    username = Column(String(20), nullable=False)
    rcon_host = Column(String(20), nullable=False)
    rcon_port = Column(String(5), nullable=False)
    rcon_password = Column(String(64), nullable=False)
    rcon_proto = Column(String(4), nullable=False, default='tcp')
    enabled = Column(Boolean, default=False)
    updates = relationship("Updates")

    def __init__(self, username, rcon_host, rcon_port, rcon_password, rcon_proto, enabled):
        self.username = username
        self.rcon_host = rcon_host
        self.rcon_port = rcon_port
        self.rcon_password = rcon_password
        self.rcon_proto = rcon_proto
        self.enabled = enabled


class Updates(Base):
    __tablename__ = 'updates'
    id = Column('id', Integer, primary_key=True, autoincrement=True, nullable=False)
    gamehost = Column(String(16), nullable=False) # duplicate of rcon_host
    gameport = Column(Integer, nullable=False, default=0)  # duplicate of rcon_port
    ipaddr = Column(String(15), nullable=False)
    created_at = Column(DateTime, default=datetime.datetime.utcnow, nullable=False)
    rcon_server_id = Column(Integer, ForeignKey('rcon_servers.id'), nullable=False, default=1)

    def __init__(self, gamehost, gameport, ipaddr, rcon_server_id):
        self.gamehost = gamehost
        self.gameport = gameport
        self.ipaddr = ipaddr
        self.rcon_server_id = rcon_server_id

class Core:
    def __init__(self):
        # logger.logging.info('Init Mysql engine with {con}'.
        #                   format(con='mysql+mysqlconnector://{user}:{passwd}@{host}:{port}/{db}'
        #                          .format_map(config.database)))

        self.engine = db.create_engine('mysql+mysqlconnector://{user}:{passwd}@{host}:{port}/{db}'
                                       .format_map(config.database), echo=False)

        Base.metadata.create_all(self.engine)
