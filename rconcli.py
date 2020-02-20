#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import rconlog
import sys


def main():
    cli = ['--mode', 'cli']
    prog, *cmd = sys.argv
    sys.argv = [prog] + cli + cmd
    rconlog.main()


if __name__ == '__main__':
    main()
