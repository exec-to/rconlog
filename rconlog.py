#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import argparse
from modules import rcon_log as logger
from modules import process_manager
import os, sys
from modules.rcon_parser import RconParser as Parser

#                          RCON-based logger application
#                          -----------------------------
#                          server - run program on monitor and web server
#                          monitor - run program on logger server
#                          firewall - run program on firewall server
#                          web - run web-based admin panel
#                          stat - print logger statistics
#                          cli - enter cli mode


def main():
    parser = Parser()

    pid = str(os.getpid())
    pidfile = "/tmp/rcon_{mode}.pid".format(mode=parser.mode)

    if os.path.isfile(pidfile):
        logger.logging.info("%s already exists, exiting", pidfile)
        sys.exit()

    with open(pidfile, 'w') as f:
        f.write(pid)

    try:
        logger.logging.info('Starting rconlog in %s mode.', parser.mode)
        pm = process_manager.ProcessManager(parser.mode, parser.args)
        pm.run()

    finally:
        os.unlink(pidfile)


if __name__ == '__main__':
    main()


# specification
# rcon_cli - модуль интерфейса командной строки
# rcon_monitor - вызывает rcon_client, обновляет БД
# rcon_stat - модуль выводит статистику по своей работе
# rcon_client - клиент rcon, получает данные от одного игрового сервера
# rcon_control - flask-админка для управления
# rcon_ipset - модуль для управления таблицами ipset (use wrapper module)
# rcon_iptables - модуль для управления правилами iptables
# rcon_firewall - модуль для управления фаерволлом
# rcon_config - конфигурация наблюдаемых игровых серверов
# rconlog_config - конфигурация приложения (БД, фаерволлы и т.п.)
# rcon_log - модуль логгирования
# cron_core
# supervisord


