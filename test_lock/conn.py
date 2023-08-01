from subprocess import Popen
from threading import Thread
import random
import shlex
import sys
cmds = [shlex.split('./client start'), shlex.split('./client stop')]

def run():
    cmd = random.choice(cmds)
    proc = Popen(cmd)
    try:
        out, err = proc.communicate()
    except Exception as e:
        print("error", e)
    else:
        print(cmd, out,err)

n = 10
if len(sys.argv) > 1:
    n = int(sys.argv[1])
ts = []
for i in range(n):
    t = Thread(target=run)
    t.start()
    ts.append(t)
for t in ts:
    t.join()
