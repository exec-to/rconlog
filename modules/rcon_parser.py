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



    def stat(self):
        print('>> stat')
        parser = argparse.ArgumentParser(description='Get RCON statistics')
        parser.add_argument('--verbose', '-v', help='Print statistic in verbose mode', action='store_true')
        args = parser.parse_args(sys.argv[3:])
        if args.verbose:
            print('>> verbose stat')


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

        server_enable = cmd_server.add_parser('enable', help='Enable RCON server')
        server_enable.add_argument('id', help='rcon server id')

        server_disable = cmd_server.add_parser('disable', help='Disable RCON server')
        server_disable.add_argument('id', help='rcon server id')

        server_get = cmd_server.add_parser('get', help='Get RCON server')
        server_get.add_argument('id', help='rcon server id')

        server_passwd = cmd_server.add_parser('passwd', help='Change password for RCON server')
        server_passwd.add_argument('id', help='rcon server id')

        server_list = cmd_server.add_parser('list', help='List RCON servers')
        server_list.add_argument('--all', help='List all RCON servers', action='store_true', default=True)
        server_list.add_argument('--enabled', help='List enabled RCON servers', action='store_true')
        server_list.add_argument('--disabled', help='List disabled RCON servers', action='store_true')

        # - firewall --------------------

        cmd_firewall_sub = commands_subparsers.add_parser('firewall', help='RCON firewall')
        cmd_firewall = cmd_firewall_sub.add_subparsers(help='Commands for RCON firewalls control', dest='command2')

        # firewall_create
        # firewall_get
        # firewall_del
        # firewall_list
        # etc
        # ---------------------

        args = parser.parse_args(sys.argv[3:])

        full_command = '{cmd2}_rcon_{cmd1}'.format(cmd1=args.command, cmd2=args.command2)
        args.full_command = full_command

        if not hasattr(api, full_command) or args.command2 is None:
            print('Unrecognize command {cmd}'.format(cmd=args.command))
            print('usage: rconcli.py server [-h] {create,del,enable,disable,get,list} ...')
            exit(1)

        if args.command2 == 'create':
            passwd = getpass.getpass('Enter rcon_password: ')
            passwd2 = getpass.getpass('Confirm rcon_password: ')
            if passwd != passwd2:
                print('Passwords not match.')
                exit(1)
            args.passwd = passwd

        self.args = args
