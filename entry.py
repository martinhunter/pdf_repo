import os
import sys


def process(args):
    if args[1] == 'daemon':
        os.system('python3 daemon.py {} &'.format(' '.join(args[2:])))
        return 'daemon operated'
    else:
        return 'unknown'

help_msg = """example:
    python3 server
    python3 
"""

def entry():
    args = sys.argv[1:]
    if not args:
        print(help_msg)
    if args[1] == 'server':
        from server import start_server
        ip = "127.0.0.1"
        port = 8080
        if len(args) >= 3:
            ip = args[2]
        if len(args) >= 4 and args[3].isdigit():
            port = int(args[3])
        start_server(ip, port)
    else:
        process(args)


if __name__ == '__main__':
    entry()
