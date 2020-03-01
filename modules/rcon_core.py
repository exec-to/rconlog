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
    protected = Column(Boolean, default=True)
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


class FirewallRule(Base):
    __tablename__ = 'firewall_rules'
    id = Column('id', Integer, primary_key=True, autoincrement=True, nullable=False)
    gamehost = Column(String(16), nullable=False) # duplicate of rcon_host
    gameport = Column(Integer, nullable=False, default=0)  # duplicate of rcon_port
    username = Column(String(10), nullable=False)
    subnet = Column(String(20), nullable=False)
    firewall_1 = Column(Boolean, default=False, nullable=False)
    firewall_2 = Column(Boolean, default=False, nullable=False)
    firewall_3 = Column(Boolean, default=False, nullable=False)
    firewall_4 = Column(Boolean, default=False, nullable=False)
    firewall_5 = Column(Boolean, default=False, nullable=False)
    firewall_6 = Column(Boolean, default=False, nullable=False)
    firewall_7 = Column(Boolean, default=False, nullable=False)
    firewall_8 = Column(Boolean, default=False, nullable=False)
    firewall_9 = Column(Boolean, default=False, nullable=False)
    firewall_10 = Column(Boolean, default=False, nullable=False)
    firewall_11 = Column(Boolean, default=False, nullable=False)
    firewall_12 = Column(Boolean, default=False, nullable=False)
    firewall_13 = Column(Boolean, default=False, nullable=False)
    firewall_14 = Column(Boolean, default=False, nullable=False)
    firewall_15 = Column(Boolean, default=False, nullable=False)
    firewall_16 = Column(Boolean, default=False, nullable=False)

    def __init__(self, gamehost, gameport, username, subnet):
        self.gamehost = gamehost
        self.gameport = gameport
        self.username = username
        self.subnet = subnet

class CustomSubnet(Base):
    __tablename__ = 'custom_subnets'
    id = Column('id', Integer, primary_key=True, autoincrement=True, nullable=False)
    subnet = Column(String(20), nullable=False)

    def __init__(self, subnet):
        self.subnet = subnet


class Firewall(Base):
    __tablename__ = 'firewalls'
    id = Column('id', Integer, primary_key=True, autoincrement=True, nullable=False)
    host = Column(String(16), nullable=False)
    name = Column(String(16), nullable=False)
    type = Column(String(16), nullable=False, default='iptables')
    enabled = Column(Boolean, default=False)

    def __init__(self, host, name, type, enabled):
        self.host = host
        self.name = name
        self.type = type
        self.enabled = enabled



class Core:
    def __init__(self):
        # logger.logging.info('Init Mysql engine with {con}'.
        #                   format(con='mysql+mysqlconnector://{user}:{passwd}@{host}:{port}/{db}'
        #                          .format_map(config.database)))

        self.engine = db.create_engine('mysql+mysqlconnector://{user}:{passwd}@{host}:{port}/{db}'
                                       .format_map(config.database), echo=False)

        Base.metadata.create_all(self.engine)
