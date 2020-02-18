#!/usr/bin/python3

import argparse
import sys
from modules.process_manager import ProcessManager as manager


class Parser(object):

    def __init__(self):
        parser = argparse.ArgumentParser(
            description='Pretends to be git',
            usage='''git <command> [<args>]

The most commonly used git commands are:
   commit     Record changes to the repository
   fetch      Download objects and refs from another repository
''')
        parser.add_argument('--mode', dest='mode', action='store', nargs=1, required=True, default='stat',
                             choices={'server', 'monitor', 'firewall', 'web', 'stat', 'cli'},
                             help='Run app in selected mode')

        self.ns = parser.parse_args(sys.argv[1:3])

        if not hasattr(self, *self.ns.mode):
            print('Unrecognized mode')
            parser.print_help()
            exit(1)

        getattr(self, *self.ns.mode)()


    def stat(self):
        parser = argparse.ArgumentParser(description='Record changes to the repository')
        parser.add_argument('--verbose', '-v', help='Print statistic in verbose mode', action='store_true')
        args = parser.parse_args(sys.argv[3:])
        print('>> stat')
        if args.verbose:
            print('>> verbose stat')


    def cli(self):
        print('>> cli')
        parser = argparse.ArgumentParser(
            description='Record changes to the repository')
        # prefixing the argument with -- means it's optional
        parser.add_argument('command', help='Subcommand to run')
        parser.add_argument('host', help='rcon host')
        print(sys.argv[3:])
        args = parser.parse_args(sys.argv[3:])

        command = args.command.replace('-', '_')
        print(command)
        if not hasattr(self, command):
            print ('Unrecognized command')
            parser.print_help()
            exit(1)

        cmd = Command(command, host)
        pm = manager(*self.ns.mode)
        pm.run_command(cmd)
        # use dispatch pattern to invoke method with same name
        # getattr(self, cmd)(args.host)

    # def add_rcon_server(self, host):
    #     print('>> add-rcon-server {host}'.format(host=host))
    #     command = Command('add-rcon-server', host)
    #     pm = manager(*self.ns.mode)
    #     pm.run_command(command)

class Command():
    def __init__(self, command, args):
        self.command = command
        self.args = args

if __name__ == '__main__':
    Parser()
