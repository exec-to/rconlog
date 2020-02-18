#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import argparse
from modules import rcon_log as logger
from modules import process_manager
import sys

#                          RCON-based logger application
#                          -----------------------------
#                          server - run program on monitor and web server
#                          monitor - run program on logger server
#                          firewall - run program on firewall server
#                          web - run web-based admin panel
#                          stat - print logger statistics
#                          cli - enter cli mode


def create_parser():
    _parser = argparse.ArgumentParser(description='RCON-based logger application')
    _parser.add_argument('--mode', dest='mode', action='store',  nargs=1, required=True, default='stat',
                         choices={'server', 'monitor', 'firewall', 'web', 'stat', 'cli'},
                         help='Run app in selected mode')
    return _parser


def main():
    parser = create_parser()
    ns = parser.parse_args()

    logger.logging.info('Starting rconlog in %s mode.', *ns.mode)
    pm = process_manager.ProcessManager(*ns.mode)
    pm.run()


if __name__ == '__main__':
    main()


# specification
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


