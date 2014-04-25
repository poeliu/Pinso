import os
import sys
import subprocess
import optparse
from maple.core import config
from maple.core import logging
from maple.core import pintool
from maple.core import static_info
from maple.core import testing
from maple.idiom import iroot
from maple.delta import pintool as delta_pintool
from maple.delta import testing as delta_testing
from maple.delta import ilist as delta_ilist
from maple.delta import display

_separator = '---'

def get_prefix(pin, tool):
    c = []
    c.append(pin.pin())
    c.extend(pin.options())
    c.extend(tool.options())
    c.append('--')
    return c

def valid_display_set():
    result = set()
    for name in dir(sys.modules[__name__]):
        idx = name.find('__display_')
        if idx != -1:
            result.add(name[idx+10:])
    return result

def valid_display():
    return display in valid_display_set()

def display_usage():
    usage = 'usage: <script> display [options] <object>\n\n'    
    usage = 'valid objectrs are:\n'
    for display in valid_display_set():
        usage += '  %s\n' % display
        for display in valid_display_set():
            usage += '  %s\n' % display
        return usage 

def register_display_options(parser):
    parser.add_option(
            '-i', '--input',
            action='store',
            type='string',
            dest='input',
            default='stdin',
            metavar='PATH',
            help='the input file name')
    parser.add_option(
            '-o', '--output',
            action='store',
            type='string',
            dest='output',
            default='stdout',
            metavar='PATH',
            help='the output file name')
    parser.add_option(
            '--sinfo_in',
            action='store',
            type='string',
            dest='sinfo_in',
            default='sinfo.db',
            metavar='PATH',
            help='the input static info database path')
    parser.add_option(
            '--sinfo_out',
            action='store',
            type='string',
            dest='sinfo_out',
            default='sinfo.db',
            metavar='PATH',
            help='the output static info database path')
    parser.add_option(
            '--iroot_in',
            action='store',
            type='string',
            dest='iroot_in',
            default='iroot.db',
            metavar='PATH',
            help='the input iroot database path')
    parser.add_option(
            '--iroot_out',
            action='store',
            type='string',
            dest='iroot_out',
            default='iroot.db',
            metavar='PATH',
            help='the output iroot database path')
    parser.add_option(
            '--memo_in',
            action='store',
            type='string',
            dest='memo_in',
            default='memo.db',
            metavar='PATH',
            help='the input memoization database path')
    parser.add_option(
            '--memo_out',
            action='store',
            type='string',
            dest='memo_out',
            default='memo.db',
            metavar='PATH',
            help='the output memoization database path')

def __command_display(argv):
    pb_reader = display.ReadProtoBuf(sys.argv[2])
    fname_ = sys.argv[2].split("/")[-1]
    if fname_ == "iroot.db":
        pb_reader.ReadiRoot()
    if fname_ == "sinfo.db":
        pb_reader.ReadStaticInfo()
    #parser = optparse.OptionParser(display_usage())
    #register_display_options(parser)
    #(options, args) = parser.parse_args(argv)
    #if len(args) != 1 or not valid_display(args[0]):
    #    parser.print_help()
    #    sys.exit()

def separate_opt_prog(argv):
    if not _separator in argv:
        return argv, []
    else:
        opt_argv = argv[0:argv.index(_separator)]
        prog_argv = argv[argv.index(_separator)+1:]
        return opt_argv, prog_argv

def register_profile_cmdline_options(parser, prefix=''):
    parser.add_option('--%smode' % prefix, 
                      action = 'store', 
                      type = 'string', 
                      dest = '%smode' % prefix, 
                      default = 'runout', 
                      metavar = 'MODE', 
                      help = 'the profile mode: runout, timeout, stable')
    parser.add_option('--%sthreshold' % prefix,
                      action = 'store',
                      type = 'int',
                      dest = '%sthreshold' % prefix, 
                      default = 1,
                      metavar = 'N', 
                      help = 'the threshold (depends on mode)')

def __command_profile(argv):
    pin = pintool.Pin(config.pin_home())
    profiler = delta_pintool.BaseProfiler()
    profiler.knob_defaults['enable_observer'] = True
    # parse cmdline options
    usage = 'usage: <script> profile [options] --- program'
    parser = optparse.OptionParser(usage)
    register_profile_cmdline_options(parser)
    profiler.register_cmdline_options(parser)
    (opt_argv, prog_argv) = separate_opt_prog(argv)
    if len(prog_argv) == 0:
        parser.print_help()
        sys.exit(0)
    (options, args) = parser.parse_args(opt_argv)
    profiler.set_cmdline_options(options, args)
    ##run profile
    test = testing.InteractiveTest(prog_argv)
    test.set_prefix(get_prefix(pin, profiler))
    testcase = delta_testing.ProfileTestCase(test, options.mode, options.threshold, profiler)
    testcase.run()
    if testcase.is_fatal():
        logging.err('fatal error detected\n')
    else:
        logging.err('threshold reached\n')

def __command_randprofile(argv):
    pin = pintool.Pin(config.pin_home())
    profiler = delta_pintool.RandSchedProfiler()
    profiler.knob_defaults['enable_observer'] = True
    # parse cmdline options
    usage = 'usage: <script> profile [options] --- program'
    parser = optparse.OptionParser(usage)
    register_profile_cmdline_options(parser)
    profiler.register_cmdline_options(parser)
    (opt_argv, prog_argv) = separate_opt_prog(argv)
    if len(prog_argv) == 0:
        parser.print_help()
        sys.exit(0)
    (options, args) = parser.parse_args(opt_argv)
    profiler.set_cmdline_options(options, args)
    ##run profile
    test = testing.InteractiveTest(prog_argv)
    test.set_prefix(get_prefix(pin, profiler))
    testcase = delta_testing.ProfileTestCase(test, options.mode, options.threshold, profiler)
    testcase.run()
    if testcase.is_fatal():
        logging.err('fatal error detected\n')
    else:
        logging.err('threshold reached\n')

def register_diff_cmdline_options(parser):
    parser.add_option('--number', 
                       action = 'store',
                       dest = 'merging_number',
                       type = 'int', 
                       help = 'number of merging ilist file') 

def __command_diff(argv):
    usage = 'usage: <script> diff [options]'
    parser = optparse.OptionParser(usage)
    register_diff_cmdline_options(parser)
    (options, args) = parser.parse_args(sys.argv)
    if options.merging_number:
       merging_number = options.merging_number
    ilist = delta_ilist.iList(merging_number) 
    ilist.display()
    ilist.merge()
    ilist.diff()
 
def register_schedule_cmdline_options(parser, prefix=''):
    pass

def __command_schedule(argv):
    pin = pintool.Pin(config.pin_home())
    scheduler = delta_pintool.Scheduler()
    scheduler.knob_defaults['enable_observer'] = True
    # parse cmdline options
    usage = 'usage: <script> schedule [options] --- program'
    parser = optparse.OptionParser(usage)
    register_profile_cmdline_options(parser)
    profiler.register_cmdline_options(parser)
    (opt_argv, prog_argv) = separate_opt_prog(argv)
    if len(prog_argv) == 0:
        parser.print_help()
        sys.exit(0)
    (options, args) = parser.parse_args(opt_argv)
    profiler.set_cmdline_options(options, args)
    ##run profile
    test = testing.InteractiveTest(prog_argv)
    test.set_prefix(get_prefix(pin, scheduler))
    testcase = idiom_testing.ActiveTestCase(test, options.mode, options.threshold, scheduler)
    testcase.run()
    if testcase.is_fatal():
        logging.err('fatal error detected\n')
    else:
        logging.err('threshold reached\n')

def valid_command_set():
    result = set()
    for name in dir(sys.modules[__name__]):
        idx = name.find('__command_')
        if idx != -1:
            result.add(name[idx+10:])
    return result

def valid_command(command):
    return command in valid_command_set()

def command_usage():
    usage = 'usage: <script> <command> [options] [args]\n\n'
    usage += 'valid commands are:\n'
    for command in valid_command_set():
        usage += '  %s\n' % command
    return usage

def main(argv):
    if len(argv) < 1:
        logging.err(command_usage())
    command = argv[0]
    logging.msg('performing command: %s ...\n' % command, 2)
    if valid_command(command):
        eval('__command_%s(argv[1:])' % command)
    else:
        logging.err(command_usage())

if __name__ == '__main__':
    main(sys.argv[1:])
