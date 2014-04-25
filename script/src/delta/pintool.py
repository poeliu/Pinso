import optparse
from maple.core import config
from maple.core import pintool

class Analyzer(object):
    def __init__(self, name):
        self.name = name
        self.knob_types = {}
        self.knob_defaults = {}
        self.knob_helps = {}
        self.knob_metavars = {}
        self.knobs = {}
    def register_knob(self, name, type, default, help, metavar=''):
        self.knob_types[name] = type
        self.knob_defaults[name] = default
        self.knob_helps[name] = help
        self.knob_metavars[name] = metavar
        self.knobs[name] = default

class SinstAnalyzer(Analyzer):
    def __init__(self):
        Analyzer.__init__(self, 'sinst_analyzer')
        self.register_knob('enable_sinst', 'bool', False, 'whether enable shared inst analyzer')
        self.register_knob('unit_size', 'int', 4, 'the monitoring granularity in bytes', 'SIZE')

class Observer(Analyzer):
    def __init__(self):
        Analyzer.__init__(self, 'observer')
        self.register_knob('enable_observer', 'bool', False, 'whether enable iroot observer')
        self.register_knob('shadow_observer', 'bool', False, 'whether the observer is shadow')
        self.register_knob('sync_only', 'bool', False, 'whether only monitor synchronization accesses')
        self.register_knob('unit_size', 'int', 4, 'the monitoring granularity in bytes', 'SIZE')
        self.register_knob('complex_idioms', 'bool', False, 'whether target complex idioms')
        self.register_knob('vw', 'int', 1000, 'the vulnerability window (# dynamic inst)', 'SIZE')

class Predictor(Analyzer):
    def __init__(self):
        Analyzer.__init__(self, 'predictor')
        self.register_knob('enable_predictor', 'bool', False, 'whether enable iroot predictor')
        self.register_knob('sync_only', 'bool', False, 'whether only monitor synchronization accesses')
        self.register_knob('unit_size', 'int', 4, 'the monitoring granularity in bytes', 'SIZE')
        self.register_knob('complex_idioms', 'bool', False, 'whether target complex idioms')
        self.register_knob('vw', 'int', 1000, 'the vulnerability window (# dynamic inst)', 'SIZE')
        self.register_knob('racy_only', 'bool', False, 'whether only consider sync and racy memory dependencies')
        self.register_knob('predict_deadlock', 'bool', False, 'whether predict and trigger deadlocks (experimental)')

class Profiler(pintool.Pintool):
    def __init__(self, name):
        pintool.Pintool.__init__(self, name)
        self.register_knob('ignore_lib', 'bool', False, 'whether ignore accesses from common libraries')
        self.register_knob('memo_failed', 'bool', True, 'whether memoize fail-to-expose iroots')
        self.register_knob('debug_out', 'string', 'stdout', 'the output file for the debug messages', 'PATH') 
        self.register_knob('sinfo_in', 'string', 'sinfo.db', 'the input static info database path', 'PATH')
        self.register_knob('sinfo_out', 'string', 'sinfo.db', 'the output static info database path', 'PATH')
        self.register_knob('iroot_in', 'string', 'iroot.db', 'the input iroot database path', 'PATH')
        self.register_knob('iroot_out', 'string', 'iroot.db', 'the output iroot database path', 'PATH')
        self.register_knob('memo_in', 'string', 'memo.db', 'the input memoization database path', 'PATH')
        self.register_knob('memo_out', 'string', 'memo.db', 'the output memoization database path', 'PATH')
        self.register_knob('sinst_in', 'string', 'sinst.db', 'the input shared inst database path', 'PATH')
        self.register_knob('sinst_out', 'string', 'sinst.db', 'the output shared inst database path', 'PATH')
        self.analyzers = {}
        self.add_analyzer(SinstAnalyzer())
        self.add_analyzer(Observer())
        self.add_analyzer(Predictor())
    def add_analyzer(self, a):
        self.analyzers[a.name] = a
        for k, v in a.knobs.iteritems():
            self.knobs[k] = v
            self.knob_types[k] = a.knob_types[k]
            self.knob_defaults[k] = a.knob_defaults[k]
            self.knob_helps[k] = a.knob_helps[k]
            self.knob_metavars[k] = a.knob_metavars[k]

class BaseProfiler(Profiler):
    def __init__(self):
        Profiler.__init__(self, 'delta_profiler')
        self.register_knob('type1', 'bool', True, 'which pattern type to be observer')
    def so_path(self):
        return config.build_home(self.debug) + '/delta_profiler.so'

class RandSchedProfiler(Profiler):
    def __init__(self):
        Profiler.__init__(self, 'delta_randsched_profiler')
        self.register_knob('strict', 'bool', False, 'whether use non-preemptive priorities')
        self.register_knob('cpu', 'int', 0, 'which cpu to run on', 'CPU_ID')
        self.register_knob('delay', 'bool', True, 'whether inject delay instead of changing priorities at each change point')
        self.register_knob('float', 'bool', True, 'whether the number of change points depends on execution length')
        self.register_knob('float_interval', 'int', 100000, 'average number of memory accesses between two change points', 'N')
        self.register_knob('num_chg_pts', 'int', 3, 'number of change points (when float is set to False)', 'N')
        self.register_knob('rand_history', 'string', 'rand.histo', 'the rand history file path', 'PATH')
    def so_path(self):
        return config.build_home(self.debug) + '/delta_randsched_profiler.so'

class Scheduler(pintool.Pintool):
    def __init__(self):
        pintool.Pintool.__init__(self, 'delta_scheduler')
        self.register_knob('ignore_lib', 'bool', False, 'whether ignore accesses from common libraries')
        self.register_knob('strict', 'bool', False, 'whether use non-preemptive priorities')
        self.register_knob('cpu', 'int', 0, 'which cpu to run on', 'CPU_ID')
        self.register_knob('unit_size', 'int', 4, 'the monitoring granularity in bytes', 'SIZE')
        self.register_knob('vw', 'int', 1000, 'the vulnerability window (# dynamic inst)', 'SIZE')
        self.register_knob('random_seed',  'int', 0, 'the random seed (0 means using current time)', 'SEED')
        self.register_knob('target_iroot', 'int', 0, 'the target iroot (0 means choosing any)', 'ID')
        self.register_knob('target_idiom', 'int', 0, 'the target idiom (0 means any idiom)', 'IDIOM')
        self.register_knob('yield_with_delay', 'bool', True, 'whether inject delays for async iroots')
        self.register_knob('misc_info', 'string', 'misc.out', 'the misc information output file path', 'PATH')
        self.register_knob('test_history', 'string', 'test.histo', 'the test history file path', 'PATH')
        self.register_knob('debug_out', 'string', 'stdout', 'the output file for the debug messages', 'PATH') 
        self.register_knob('sinfo_in', 'string', 'sinfo.db', 'the input static info database path', 'PATH')
        self.register_knob('sinfo_out', 'string', 'sinfo.db', 'the output static info database path', 'PATH')
        self.register_knob('iroot_in', 'string', 'iroot.db', 'the input iroot database path', 'PATH')
        self.register_knob('iroot_out', 'string', 'iroot.db', 'the output iroot database path', 'PATH')
        self.register_knob('memo_in', 'string', 'memo.db', 'the input memoization database path', 'PATH')
        self.register_knob('memo_out', 'string', 'memo.db', 'the output memoization database path', 'PATH')
        self.register_knob('sinst_in', 'string', 'sinst.db', 'the input shared inst database path', 'PATH')
        self.register_knob('sinst_out', 'string', 'sinst.db', 'the output shared inst database path', 'PATH')
        self.analyzers = {}
        self.add_analyzer(SinstAnalyzer())
        self.add_analyzer(Observer())
    def add_analyzer(self, a):
        self.analyzers[a.name] = a
        for k, v in a.knobs.iteritems():
            self.knobs[k] = v
            self.knob_types[k] = a.knob_types[k]
            self.knob_defaults[k] = a.knob_defaults[k]
            self.knob_helps[k] = a.knob_helps[k]
            self.knob_metavars[k] = a.knob_metavars[k]
    def so_path(self):
        return config.build_home(self.debug) + '/delta_scheduler.so'

