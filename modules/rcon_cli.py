#!/usr/bin/python3
# -*- coding: UTF-8 -*-

class RconCLI(object):

    @staticmethod
    def get_rcon_server(s):
        if s is None:
            print('Error: RCON Server not exist.')
            return
        print('')
        print('RCON Server ID: {id}'.format(id=s.id))
        print('Host: {host}:{port} ({proto}), User: {user}, Active: {state}'
              .format(host=s.rcon_host, port=s.rcon_port, proto=s.rcon_proto, user=s.username, state=s.enabled))
        print('--'*30)

    @staticmethod
    def test_rcon_server(s):
        print(s)


    @staticmethod
    def list_rcon_server(servers):
        for s in servers:
            RconCLI.get_rcon_server(s)


    # @staticmethod
    # def list_rcon_updates(updates):
    #     for u in updates:
    #         RconCLI.get_rcon_server(u)
    #
    #
    # @staticmethod
    # def get_rcon_update(u):
    #     if u is None:
    #         print('Error: RCON update not exist.')
    #         return
    #     print('RCON Server ID: {id}'.format(id=s.id))
    #     print('Host: {host}:{port} ({proto}), User: {user}, Active: {state}'
    #           .format(host=s.rcon_host, port=s.rcon_port, proto=s.rcon_proto, user=s.username, state=s.enabled))
    #     print('--')


    @staticmethod
    def create_rcon_server(message):
        print(message)

    @staticmethod
    def del_rcon_server(message):
        print(message)

    @staticmethod
    def enable_rcon_server(message):
        print(message)

    @staticmethod
    def disable_rcon_server(message):
        print(message)

    @staticmethod
    def passwd_rcon_server(message):
        print(message)

    # -- Firewall methods
    @staticmethod
    def create_rcon_firewall(message):
        print(message)

    @staticmethod
    def del_rcon_firewall(message):
        print(message)

    @staticmethod
    def enable_rcon_firewall(message):
        print(message)

    @staticmethod
    def disable_rcon_firewall(message):
        print(message)

    @staticmethod
    def get_rcon_firewall(f):
        if f is None:
            print('Error: Firewall not exist.')
            return
        print('')
        print('Firewall ID: {id}'.format(id=f.id))
        print('Host: {host}, name: {name}, type: {type}, active: {state}'
              .format(host=f.host, name=f.name, type=f.type, state=f.enabled))
        print('--' * 30)

    @staticmethod
    def list_rcon_firewall(firewalls):
        for f in firewalls:
            RconCLI.get_rcon_firewall(f)


    # -- Database methods
    @staticmethod
    def sync_rcon_db(message):
        pass