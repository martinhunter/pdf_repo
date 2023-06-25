import os
import sys


def process(args):
    if args[1] == 'daemon':
        os.system('python3 daemon.py {} &'.format(' '.join(args[2:])))
        return 'daemon operated'
    else:
        return 'unknown'


def entry():
    args = sys.argv[1:]

    if args[1] == 'server':
        from server import start_server
        start_server("127.0.0.1", 8080)
    else:
        process(args)


if __name__ == '__main__':
    entry()
