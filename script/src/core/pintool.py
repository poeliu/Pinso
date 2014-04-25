import optparse

class Pin(object):
    """ The wrapper class for PIN binary instrumentation tool
    """
    def __init__(self, home_path):
        self.home_path = home_path
        self.logging_mode = False
        self.debugging_mode = False
        self.knobs = {}
    def pin(self):
        return self.home_path + '/pin'
    def options(self):
        c = []
        if self.logging_mode:
            c.append('-xyzzy')
            c.append('-mesgon')
            c.append('log_signal')
            c.append('-mesgon')
            c.append('log_syscall')
        if self.debugging_mode:
            c.append('-pause_tool')
            c.append('10')
        for k, v in self.knobs.iteritems():
            c.append('-' + k)
            c.append(str(v))
        return c

class Pintool(object):
    """ The abstract class for PIN tools.
    """
    def __init__(self, name):
        self.name = name
        self.debug = False
        self.knob_types = {}
        self.knob_defaults = {}
        self.knob_helps = {}
        self.knob_metavars = {}
        self.knobs = {}
        self.option_prefix = ''
    def so_path(self):
        pass
    def options(self):
        c = []
        c.append('-t')
        c.append(self.so_path())
        for k, v in self.knobs.iteritems():
            c.append('-' + k)
            if self.knob_types[k] == 'bool':
                if v:
                    c.append('1')
                else:
                    c.append('0')
            else:
                c.append(str(v))
        return c
    def register_knob(self, name, type, default, help, metavar=''):
        self.knob_types[name] = type
        self.knob_defaults[name] = default
        self.knob_helps[name] = help
        self.knob_metavars[name] = metavar
        self.knobs[name] = default
    def register_cmdline_options(self, parser):
        parser.add_option(
                '--%sdebug' % self.option_prefix,
                action='store_true',
                dest='%sdebug' % self.option_prefix,
                default=False,
                help='whether in debugging mode [default: False]')
        for k, v in self.knobs.iteritems():
            opt_str = '--%s%s' % (self.option_prefix, k)
            if self.knob_types[k] == 'bool':
                if self.knob_defaults[k] == False:
                    parser.add_option(
                            '--%s%s' % (self.option_prefix, k),
                            action='store_true',
                            dest='%s%s' % (self.option_prefix, k),
                            default=False,
                            help='%s [default: False]' % self.knob_helps[k])
                else:
                    parser.add_option(
                            '--no_%s%s' % (self.option_prefix, k),
                            action='store_false',
                            dest='%s%s' % (self.option_prefix, k),
                            default=True,
                            help='%s [default: True]' % self.knob_helps[k])
            else:
                parser.add_option(
                        '--%s%s' % (self.option_prefix, k),
                        action='store',
                        type=self.knob_types[k],
                        dest='%s%s' % (self.option_prefix, k),
                        default=self.knob_defaults[k],
                        metavar=self.knob_metavars[k],
                        help='%s [default: %s]' % \
                                (self.knob_helps[k],
                                 str(self.knob_defaults[k])))
    def set_cmdline_options(self, options, args):
        self.debug = eval('options.%sdebug' % self.option_prefix)
        for k in self.knobs.keys():
            self.knobs[k] = eval('options.%s%s' % (self.option_prefix, k))
