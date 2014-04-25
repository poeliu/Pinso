import os
import sys

def tool_name():
    return 'MAPLE'

def pin_home():
    if not os.environ.has_key('PIN_HOME'):
        sys.exit('please specify the environment variable PIN_HOME')
    else:
        return os.environ['PIN_HOME']

def pkg_home():
    path = __file__
    return path[0:path.rfind('/script')]

def build_home(debug):
    if debug:
        return pkg_home() + '/build-debug'
    else:
        return pkg_home() + '/build-release'

def benchmark_home(name):
    bench_home = {}
    bench_home['memcached'] = '/opt/memcached/memcached-1.4.4'
    return bench_home[name]

