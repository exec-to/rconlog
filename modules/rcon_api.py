#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from modules import rcon_core as core
from sqlalchemy import exc
from modules import rcon_log as logger

class RconAPI(object):

    @staticmethod
    def create_rcon_server(self):
        print('>> create_rcon_server')

    @staticmethod
    def del_rcon_server(self):
        print('>> create_rcon_server')

    # ------------------

    @staticmethod
    def get_rcon_server(session, args):
        s = session.query(core.RconServer).filter_by(id=args.id).first()
        return s

    @staticmethod
    def print_get_rcon_server(s):
        if s is None:
            print('Error: RCON Server not exist.')
            return
        print('RCON Server ID: {id}'.format(id=s.id))
        print('Host: {host}:{port} ({proto}), User: {user}, Active: {state}'
              .format(host=s.rcon_host, port=s.rcon_port, proto=s.rcon_proto, user=s.username, state=s.enabled))
        print('--')

    # ------------------

    @staticmethod
    def list_rcon_server(session, args):
        servers = session.query(core.RconServer)

        if args.enabled:
            servers = servers.filter_by(enabled=True).all()
        elif args.disabled:
            servers = servers.filter_by(enabled=False).all()
        elif args.all:
            servers = servers.all()

        return servers

    @staticmethod
    def print_list_rcon_server(servers):
        for s in servers:
            RconAPI.print_get_rcon_server(s)

    # ------------------

    @staticmethod
    def create_rcon_server(session, args):
        # TODO: add try catch

        try:
            server = core.RconServer(args.username,
                                     str(args.host),
                                     args.port,
                                     args.passwd,
                                     args.proto,
                                     args.enabled)
            session.add(server)
            session.commit()
        except exc.SQLAlchemyError as e:
            logger.logging.error('Can\'t create new RCON server: {error}'.format(error=str(e)))
            print('Can\'t create new RCON server: {error}'.format(error=str(e)))
            exit(1)

        return 'Created successful'

    @staticmethod
    def print_create_rcon_server(message):
        print(message)

    # ------------------



# RCON API Functions

# create_rcon_server
# del_rcon_server
# update_rcon_server
# get_rcon_server(id)
# list_rcon_servers
# enable
# disable

# create_firewall
# del_firewall
# update_firewall
# get_firewall(id)
# list_firewalls

# create_subnet
# del_subnet
# update_subnet
# get_subnet(id)
# list_subnets

# create_iptables_rule
# del_iptables_rule
# update_iptables_rule
# get_iptables_rule(id)
# list_iptables_rules

# create_ipset_rule
# del_ipset_rule
# update_ipset_rule
# get_ipset_rule(id)
# list_ipset_rules