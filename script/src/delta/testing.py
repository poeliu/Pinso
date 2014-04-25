from maple.core import config
from maple.core import logging
from maple.core import static_info
from maple.core import pintool
from maple.core import testing
from maple.idiom import iroot
from maple.delta import ilist

class CmdlineTest(testing.CmdlineTest):
    def __init__(self):
        pass
    def tear_down(self):
        # copy files
        pass

class ProfileTestCase(testing.DeathTestCase):
    def __init__(self, test, mode, threshold, profiler):
        testing.DeathTestCase.__init__(self, test, mode, threshold)
        self.profiler = profiler
        self.memo_size = 0
        self.stable_cnt = 0
    def threshold_check(self):
        if testing.DeathTestCase.threshold_check(self):
            return True
        if self.mode == 'stable':
            sinfo = static_info.StaticInfo()
            sinfo.load(self.profiler.knobs['sinfo_out'])
            iroot_db = iroot.iRootDB(sinfo)
            iroot_db.load(self.profiler.knobs['iroot_out'])
        return False
    def before_each_test(self):
        logging.msg('=== profile iteration %d ===\n' % len(self.test_history))
    def after_each_test(self):
        ilist.move_ilist(len(self.test_history)) 
