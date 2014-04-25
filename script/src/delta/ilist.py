import subprocess
from maple.core import config
from maple.core import logging
from maple.proto.delta import ilist_pb2

def command_mv_ilist(test_history):
    cmd_list = []
    cmd_list.append('/bin/mv')
    cmd_list.append(config.pkg_home() + '/script/ilist.db')
    cmd_list.append(config.pkg_home() + '/script/ilist.db.'+str(test_history).zfill(3))
    return cmd_list

def move_ilist(test_history):
    command = command_mv_ilist(test_history)
    proc = subprocess.Popen(command, shell=False)
    proc.wait()

class iList(object):
    def __init__(self, merging_number):
        self.merging_number = merging_number
    def get_command_cat(self, i):
        c = []
        c.append("cat")
        c.append(config.pkg_home() + '/script/ilist.txt.merge.' + str(i).zfill(3))
        c.append(config.pkg_home() + '/script/ilist.txt.' + str(i+1).zfill(3))
        return c
  
    def get_command_sort(self, i):
        c = []
        c.append("sort")
        c.append("-d")
        c.append("-k1")
        c.append("-u")
        c.append(config.pkg_home() + '/script/ilist.txt.merge.' + str(i+1).zfill(3))
        return c

    def get_command_diff(self):
        c = []
        c.append("diff")
        c.append("-Naur")
        c.append(config.pkg_home() + '/script/ilist.txt.err')
        c.append(config.pkg_home() + '/script/ilist.txt.merge.sort.' + str(self.merging_number).zfill(3))
        return c

    def merge(self):
        proc_copy = subprocess.Popen(["cp", config.pkg_home() + '/script/ilist.txt.001', 
                                     config.pkg_home() + '/script/ilist.txt.merge.001'])
        proc_copy.wait()
        for i in range(1, self.merging_number):
            cat_command = self.get_command_cat(i)
            output_file_merge = open(config.pkg_home() + '/script/ilist.txt.merge.'+str(i+1).zfill(3), 'w')
            proc_merge = subprocess.Popen(cat_command, shell = False, stdout = output_file_merge)  
            proc_merge.wait()
            output_file_merge.close()
   
            sort_command = self.get_command_sort(i)
            output_file_sort = open(config.pkg_home() + '/script/ilist.txt.merge.sort.' + str(i+1).zfill(3), 'w')
            proc_sort = subprocess.Popen(sort_command, shell = False, stdout = output_file_sort)
            proc_sort.wait()
            output_file_sort.close()
         
        proc_del = subprocess.Popen(['rm', '-rf', config.pkg_home() + '/script/ilist.txt.merge.???'])
        proc_del.wait()

    def display(self):
        for i in range(1, int(self.merging_number)+1):
            input_file = open(config.pkg_home() + '/script/ilist.db.' + str(i).zfill(3), "rb")
            ilist_db = ilist_pb2.iListProto()
            ilist_db.ParseFromString(input_file.read())
            input_file.close()
            #output to txt file, can be read by human
            output_file = open(config.pkg_home() + '/script/ilist.txt.' + str(i).zfill(3), 'w')
            for entry in ilist_db.entry:
                output_file.write(str(entry.iroot_id))
                output_file.write('\n')
            output_file.close()

    def diff(self):
        diff_command = self.get_command_diff() 
        output_file_diff = open(config.pkg_home() + '/script/ilist.txt.err.'+str(self.merging_number).zfill(3) 
                                + '.diff', 'w')
        proc_diff = subprocess.Popen(diff_command, shell = False, stdout = output_file_diff)
        proc_diff.wait()
        output_file_diff.close()
