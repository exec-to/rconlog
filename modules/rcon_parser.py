#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import argparse
import sys
import getpass


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
            self.args = args = parser.parse_args(sys.argv[3:])


    def stat(self):
        print('>> stat')
        parser = argparse.ArgumentParser(description='Get RCON statistics')
        parser.add_argument('--verbose', '-v', help='Print statistic in verbose mode', action='store_true')
        args = parser.parse_args(sys.argv[3:])
        if args.verbose:
            print('>> verbose stat')


    def cli(self):
        print('>> cli')
        parser = argparse.ArgumentParser(
            description='Record changes to the repository')
        # prefixing the argument with -- means it's optional
        parser.add_argument('command', help='Subcommand to run')
        parser.add_argument('username', help='rcon username')
        parser.add_argument('host', help='rcon host')
        parser.add_argument('port', type=int, help='rcon port')
        parser.add_argument('--proto', help='rcon protocol', default='tcp',  choices={'tcp', 'udp'})

        args = parser.parse_args(sys.argv[3:])

        passwd = getpass.getpass('Enter rcon_password: ')
        passwd2 = getpass.getpass('Confirm rcon_password: ')
        if passwd != passwd2:
            print('Passwords not match.')
            exit(1)

        args.passwd = passwd
        self.args = args


