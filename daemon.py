import os
import sys
import time

from daemon_pid import write_pid, pid_alive


def loop():
    while True:
        print('loop')
        time.sleep(1)


def daemonize(args):
    if args and args[-1] == '&':
        args = args[:-1]
    if not args:
        print('no args')
        return
    pid = pid_alive()
    op = args[0]
    if op == 'start':
        if not pid:
            new_pid = os.getpid()
            loop()
            write_pid(new_pid)
            print('created', pid)
        print('running')
    elif op == 'stop':
        if pid:
            os.system('kill -9 %d' % pid)
            print('killed', pid)
        print('stopped')
    elif op == 'status':
        print('status is', pid)


if __name__ == '__main__':
    args = sys.argv[1:]
    daemonize(args)
