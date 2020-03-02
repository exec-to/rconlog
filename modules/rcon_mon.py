#!/usr/bin/python3
# -*- coding: UTF-8 -*-
from modules.rcon_api import RconAPI as api
from modules.rcon_cli import RconCLI as cli
from modules.rcon_client import utils
from modules import rcon_log as logger
from modules import rcon_core as core


class RconMonitor():
    def __init__(self, session, args):
        self.args = args
        self.session = session
        _filter = [
            {'field': 'enabled', 'op': '==', 'value': True}
        ]

        self.servers = api.list_rcon_server(self.session, self.args, _filter)

        for server in self.servers:
            # cli.get_rcon_server(server)
            # TODO: Handle socket timeout
            try:
                ipv4_list = utils.get_ipv4_list(
                    server.rcon_host,
                    int(server.rcon_port),
                    server.rcon_password,
                    server.rcon_proto)

            except Exception as e:
                logger.logging.error('Critical error with RCON server ID {id}: {error}'.format(error=str(e),id=server.id))
                continue

            for ip in ipv4_list:
                _filter = [
                    {'field': 'gamehost', 'op': '==', 'value': server.rcon_host},
                    {'field': 'gameport', 'op': '==', 'value': server.rcon_port},
                    {'field': 'ipaddr', 'op': '==', 'value': ip},
                ]

                update = api.get_rcon_update(self.session, self.args, _filter)
                if not update:
                    update = core.Updates(server.rcon_host, server.rcon_port, ip, server.id)
                    api.create_rcon_update(self.session, update, commit=False)
                    # logger.logging.info('{ip} created'.format(ip = update.ipaddr, date=update.created_at))

                    subnet = '{}.0/24'.format('.'.join(str(ip).split('.')[:3]))
                    _filter = [
                        {'field': 'gamehost', 'op': '==', 'value': server.rcon_host},
                        {'field': 'gameport', 'op': '==', 'value': server.rcon_port},
                        {'field': 'username', 'op': '==', 'value': server.username},
                        {'field': 'subnet', 'op': '==', 'value': subnet}
                    ]

                    rule = api.list_rcon_rule(self.session, None, _filter)
                    if not rule:
                        rule = core.FirewallRule(server.rcon_host,
                                                 server.rcon_port,
                                                 server.username,
                                                 subnet)
                        api.create_firewall_rule(self.session, rule, commit=False)

        self.session.commit()


        # load custom subnets to database
        api.load_custom_subnets(self.session)