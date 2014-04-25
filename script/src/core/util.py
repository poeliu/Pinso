import os
import signal
from maple.core import logging

def basename(path):
    return os.path.basename(path)

def kill_process(pid):
    try:
        os.kill(proc.pid, signal.SIGKILL)
    except OSError:
        logging.msg('kill process %d error\n' % proc.pid)

