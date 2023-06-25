import os

PID = os.path.join(os.path.dirname(__file__), '.pid')


def read_pid():
    if not os.path.isfile(PID):
        return 0
    with open(PID, 'r') as f:
        content = f.read()
        if content.isdigit():
            return int(content)
        else:
            return 0


def write_pid(pid):
    with open(PID, 'w') as f:
        f.write(str(pid))


def pid_alive():
    pid = read_pid()
    if pid:
        ret = os.system('ps -ef | grep %d | grep -v grep' % pid)
        if ret:
            return pid
    return 0
