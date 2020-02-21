#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from modules import rcon_core as core
from sqlalchemy import exc
from modules import rcon_log as logger
import getpass
from sqlalchemy_filters import apply_filters

# TODO: Implement filters

class RconAPI(object):

    @staticmethod
    def del_rcon_server(session, args):
        s = None
        try:
            s = session.query(core.RconServer).filter_by(id=args.id).first()
            if s is not None:
                session.delete(s)
                session.commit()
        except Exception as e:
            logger.logging.error('Can\'t delete RCON server: {error}'.format(error=str(e)))
            print('Can\'t delete RCON server: {error}'.format(error=str(e)))
            exit(1)
        return 'Server {id} successful deleted'.format(id=s.id)

    # ------------------

    @staticmethod
    def get_rcon_server(session, args):
        s = None
        try:
            s = session.query(core.RconServer).filter_by(id=args.id).first()
        except Exception as e:
            logger.logging.error('Can\'t get RCON server: {error}'.format(error=str(e)))
            print('Can\'t get RCON server: {error}'.format(error=str(e)))
            exit(1)
        return s

    # ------------------

    @staticmethod
    def passwd_rcon_server(session, args):
        s = None
        try:
            s = session.query(core.RconServer).filter_by(id=args.id).first()

            if not hasattr(args, 'passwd'):
                passwd = getpass.getpass('Enter rcon_password: ')
                passwd2 = getpass.getpass('Confirm rcon_password: ')
                if passwd != passwd2:
                    raise Exception('Passwords not match.')
                args.passwd = passwd

            s.rcon_password = args.passwd
            session.commit()

        except Exception as e:
            logger.logging.error('Can\'t change password for RCON server: {error}'.format(error=str(e)))
            print('Can\'t change password for RCON server: {error}'.format(error=str(e)))
            exit(1)
        return 'Password for server ID: {id} successful changed'.format(id=s.id)

    # ------------------

    @staticmethod
    def enable_rcon_server(session, args):
        s = None
        try:
            s = session.query(core.RconServer).filter_by(id=args.id).first()
            s.enabled = True
            session.commit()
        except Exception as e:
            logger.logging.error('Can\'t enable RCON server: {error}'.format(error=str(e)))
            print('Can\'t enable RCON server: {error}'.format(error=str(e)))
            exit(1)

        return 'Server {id} successful enabled'.format(id=s.id)

    # ------------------

    @staticmethod
    def disable_rcon_server(session, args):
        s = None
        try:
            s = session.query(core.RconServer).filter_by(id=args.id).first()
            s.enabled = False
            session.commit()
        except Exception as e:
            logger.logging.error('Can\'t disable RCON server: {error}'.format(error=str(e)))
            print('Can\'t disable RCON server: {error}'.format(error=str(e)))
            exit(1)

        return 'Server {id} successful disabled'.format(id=s.id)

    # ------------------

    @staticmethod
    def list_rcon_server(session, args, filter=None):
        if filter is None:
            filter = []

        servers = None
        try:
            servers = session.query(core.RconServer)
            servers = apply_filters(servers, filter)

            if hasattr(args, 'enabled') and args.enabled:
                servers = servers.filter_by(enabled=True).all()
            elif hasattr(args, 'disabled') and args.disabled:
                servers = servers.filter_by(enabled=False).all()
            else:
                servers = servers.all()
        except Exception as e:
            logger.logging.error('Can\'t get RCON servers: {error}'.format(error=str(e)))
            print('Can\'t get RCON servers: {error}'.format(error=str(e)))
            exit(1)

        return servers

    # ------------------

    @staticmethod
    def create_rcon_server(session, args):
        try:
            server = core.RconServer(args.username,
                                     str(args.host),
                                     args.port,
                                     args.passwd,
                                     args.proto,
                                     args.enabled)
            session.add(server)
            session.commit()
        except Exception as e:
            logger.logging.error('Can\'t create new RCON server: {error}'.format(error=str(e)))
            print('Can\'t create new RCON server: {error}'.format(error=str(e)))
            exit(1)

        return 'Created successful'

    # ------------------

    @staticmethod
    def list_rcon_updates(session, args, filter):
        if filter is None:
            filter = []

        updates = None
        try:
            updates = session.query(core.Updates)
            updates = apply_filters(updates, filter)

            updates = updates.all()
        except Exception as e:
            logger.logging.error('Can\'t get updates: {error}'.format(error=str(e)))
            exit(1)

        return updates

    # TODO: Implement filter by array of conditions
    @staticmethod
    def get_rcon_update(session, args, _filter=None):
        if _filter is None:
            _filter = []
        u = None
        try:
            u = session.query(core.Updates)
            u = apply_filters(u, _filter)
            u = u.first()
        except Exception as e:
            logger.logging.error('Can\'t get RCON server: {error}'.format(error=str(e)))
            print('Can\'t get RCON update: {error}'.format(error=str(e)))
            exit(1)
        return u


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