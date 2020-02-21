from modules.rcon_client import mcrcon
import socket
import re

def get_ipv4_list(host, port, passwd, proto):
    sock = None
    if proto == 'tcp':
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    elif proto == 'udp':
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    sock.connect((host, port))

    # TODO: Add Error handler
    try:
        # Log in
        data = mcrcon.login(sock, passwd, proto)
        if not data['login']:
            print("Incorrect auth")
            return

        request = 'status'
        response = ''

        if proto == 'tcp':
            response = mcrcon.command(sock, request)
        elif proto == 'udp':
            response = mcrcon.udp_command(sock, request, data['master_key'], passwd)

    finally:
        sock.close()

    ipv4_list = extract_ipv4(response, host)
    return ipv4_list


def extract_ipv4(raw_input, exclude='127.0.0.1'):
    ipv4_list = []

    ignore_list = ['0.0.0.0', '1.37.4.0', '1.1.2.7', '255.255.255.255', exclude]

    for entry in raw_input.split('\n'):
        match = re.search(r'((?:\d{1,3}[.]){3}\d{1,3})', entry)
        if match:
            ipv4 = match.group(1)
            if not str(ipv4) in ignore_list:
                ipv4_list.append(str(ipv4))

    return ipv4_list