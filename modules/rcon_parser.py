#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import argparse
import sys
import getpass
from ipaddress import ip_address
from modules.rcon_api import RconAPI as api


class RconParser(object):

    def __init__(self):
        parser = argparse.ArgumentParser(description='RCON-based logger application')
        parser.add_argument('--mode', dest='mode', action='store', nargs=1, required=True, default='stat',
                             choices={'server', 'monitor', 'firewall', 'web', 'stat', 'cli'},
                             help='Run app in selected mode')

        args = parser.parse_args(sys.argv[1:3])

        [self.mode] = args.mode
        self.args = None
        if hasattr(self, self.mode):
            getattr(self, self.mode)()
        else:
            self.args = args # = parser.parse_args(sys.argv[3:])
            # print(self.args)

        self.args.filter = []


    def stat(self):
        parser = argparse.ArgumentParser(description='Get RCON statistics')
        parser.add_argument('--verbose', '-v', help='Print statistic in verbose mode', action='store_true')
        args = parser.parse_args(sys.argv[3:])
        if args.verbose:
            print('>> verbose stat')

        self.args = args


    def cli(self):
        parser = argparse.ArgumentParser(
            description='RCON Logger command line interface')
        # prefixing the argument with -- means it's optional
        commands_subparsers = parser.add_subparsers(help='Available commands', dest='command')

        # - server ---------------------

        cmd_server_sub = commands_subparsers.add_parser('server', help='RCON server')
        cmd_server = cmd_server_sub.add_subparsers(help='Commands for RCON server control', dest='command2')

        server_create = cmd_server.add_parser('create', help='Create RCON server')
        server_create.add_argument('username', help='rcon username')
        server_create.add_argument('host', help='rcon host', type=ip_address)
        server_create.add_argument('port', type=int, help='rcon port')
        server_create.add_argument('--proto', help='rcon protocol, default TCP', default='tcp',  choices={'tcp', 'udp'})
        server_create.add_argument('--enabled', help='Enable, default disabled', action='store_true')

        server_del = cmd_server.add_parser('del', help='Delete RCON server')
        server_del.add_argument('id', help='rcon server id')

        # TODO: enable -all, disable --all
        server_enable = cmd_server.add_parser('enable', help='Enable RCON server')
        server_enable.add_argument('id', help='rcon server id')

        server_disable = cmd_server.add_parser('disable', help='Disable RCON server')
        server_disable.add_argument('id', help='rcon server id')

        server_protect = cmd_server.add_parser('protect', help='Set protect property for RCON server')
        server_protect.add_argument('protected', choices={'on', 'off'}, help='protected state')
        server_protect.add_argument('id', help='rcon server id')

        # TODO: db clean - clean old data for removed users

        server_get = cmd_server.add_parser('get', help='Get RCON server')
        server_get.add_argument('id', help='rcon server id')

        server_test = cmd_server.add_parser('test', help='Get status from RCON server')
        server_test.add_argument('id', help='rcon server id')

        server_passwd = cmd_server.add_parser('passwd', help='Change password for RCON server')
        server_passwd.add_argument('id', help='rcon server id')

        server_list = cmd_server.add_parser('list', help='List RCON servers')
        server_list.add_argument('--all', help='List all RCON servers', action='store_true', default=True)
        server_list.add_argument('--enabled', help='List enabled RCON servers', action='store_true')
        server_list.add_argument('--disabled', help='List disabled RCON servers', action='store_true')

        # - firewall --------------------

        cmd_firewall_sub = commands_subparsers.add_parser('firewall', help='Firewall')
        cmd_firewall = cmd_firewall_sub.add_subparsers(help='Commands for firewalls control', dest='command2')

        # firewall_create
        firewall_create = cmd_firewall.add_parser('create', help='Create Firewall')
        firewall_create.add_argument('host', help='Firewall IP-address')
        firewall_create.add_argument('name', help='Firewall name, format \'firewall_1\' from 1 to 16')
        firewall_create.add_argument('--type', help='Firewall type', default='iptables', choices={'iptables', 'cisco_asa'})
        firewall_create.add_argument('--enabled', help='Enable firewall, default disabled', action='store_true')

        firewall_del = cmd_firewall.add_parser('del', help='Delete firewall')
        firewall_del.add_argument('id', help='firewall id')

        # TODO: enable -all, disable --all
        firewall_enable = cmd_firewall.add_parser('enable', help='Enable firewall')
        firewall_enable.add_argument('id', help='firewall id')

        firewall_disable = cmd_firewall.add_parser('disable', help='Disable firewall')
        firewall_disable.add_argument('id', help='firewall id')

        firewall_get = cmd_firewall.add_parser('get', help='Get firewall')
        firewall_get.add_argument('id', help='firewall id')

        firewall_list = cmd_firewall.add_parser('list', help='List firewalls')
        firewall_list.add_argument('--all', help='List all firewalls', action='store_true', default=True)
        firewall_list.add_argument('--enabled', help='List enabled firewalls', action='store_true')
        firewall_list.add_argument('--disabled', help='List disabled firewalls', action='store_true')

        # ---------------------

        cmd_db_sub = commands_subparsers.add_parser('db', help='Database')
        cmd_db = cmd_db_sub.add_subparsers(help='Operation with database', dest='command2')

        db_sync = cmd_db.add_parser('sync', help='synchronize database')
        db_sync.add_argument('--quiet', help='quiet mode')

        args = parser.parse_args(sys.argv[3:])

        if not hasattr(args, 'command2'):
            print('Use {} -h for print help'.format(sys.argv[0]))
            exit(1)

        full_command = '{cmd2}_rcon_{cmd1}'.format(cmd1=args.command, cmd2=args.command2)
        args.full_command = full_command

        if not hasattr(api, full_command) or args.command2 is None:
            print('Unrecognize command {cmd}'.format(cmd=args.command))
            print('usage: rconcli.py server [-h] {create,del,enable,disable,get,list} ...')
            exit(1)

        if args.command == 'server' and args.command2 == 'create':
            passwd = getpass.getpass('Enter rcon_password: ')
            passwd2 = getpass.getpass('Confirm rcon_password: ')
            if passwd != passwd2:
                print('Passwords not match.')
                exit(1)
            args.passwd = passwd

        self.args = args
