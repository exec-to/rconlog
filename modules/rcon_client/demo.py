
import argparse
import socket
import mcrcon
import re

def extract_ipv4(raw_input, exclude='127.0.0.1'):

    ipv4_list = []

    ignore_list = ['0.0.0.0', '255.255.255.255', exclude]

    for entry in raw_input.split('\n'):
        match = re.search(r'((?:\d{1,3}[.]){3}\d{1,3})', entry)
        if match:
            ipv4 = match.group(1)
            if not str(ipv4) in ignore_list:
                ipv4_list.append(str(ipv4))

    return ipv4_list

def main():
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("host")
    parser.add_argument("port", type=int)
    parser.add_argument("password")
    parser.add_argument("protocol", choices={'tcp', 'udp'})
    args = parser.parse_args()

    # Connect
    sock = None
    if args.protocol == 'tcp':
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    elif args.protocol == 'udp':
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    sock.connect((args.host, args.port))

    try:
        # Log in
        data = mcrcon.login(sock, args.password, args.protocol)
        if not data['login']:
            print("Incorrect auth")
            return

        request = 'status'
        response = ''

        if args.protocol == 'tcp':
            response = mcrcon.command(sock, request)
        elif args.protocol == 'udp':
            response = mcrcon.udp_command(sock, request, data['master_key'], args.password)

    finally:
        sock.close()

    ipv4_list = extract_ipv4(response, args.host)


if __name__ == '__main__':
    main()
