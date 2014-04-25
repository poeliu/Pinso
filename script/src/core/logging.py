import sys
from maple.core import config

_message_level = 1

def err(msg):
    sys.stderr.write('[%s] %s' % (config.tool_name(), msg))
    sys.exit(1)

def msg(msg, level=1):
    if level <= _message_level:
        sys.stdout.write('[%s] %s' % (config.tool_name(), msg))

def set_message_level(level):
    global _message_level
    _message_level = level

