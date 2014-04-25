from maple.core import logging
from maple.core import util
from maple.proto.core import static_info_pb2

class Image(object):
    def __init__(self, proto, db):
        self.proto = proto
        self.db = db
        self.offset_map = {}
    def id(self):
        return self.proto.id
    def name(self):
        return self.proto.name
    def basename(self):
        return util.basename(self.proto.name)
    def add_inst(self, inst):
        self.offset_map[inst.offset()] = inst
    def __str__(self):
        content = []
        content.append('%2d' % self.id())
        content.append('%s' % self.name())
        return ' '.join(content)

class Inst(object):
    def __init__(self, proto, db):
        self.proto = proto
        self.db = db
    def id(self):
        return self.proto.id
    def image(self):
        return self.db.image_map[self.proto.image_id]
    def offset(self):
        return self.proto.offset
    def debug_info(self):
        if not self.proto.HasField('debug_info'):
            return '%21s' % ''
        else:
            file = util.basename(self.proto.debug_info.file_name)
            line = self.proto.debug_info.line
            content = []
            content.append('%15s' % file)
            content.append('+%04d' % line)
            return ' '.join(content)
    def __str__(self):
        content = []
        content.append('%6d' % self.id())
        content.append('%15s' % self.image().basename())
        content.append('0x%08x' % self.offset())
        content.append('%s' % self.debug_info())
        return ' '.join(content)

class StaticInfo(object):
    def __init__(self):
        self.proto = static_info_pb2.StaticInfoProto()
        self.image_map = {}
        self.inst_map = {}
    def load(self, db_name):
        try:
            f = open(db_name, 'rb')
            self.proto.ParseFromString(f.read())
            f.close()
            for image_proto in self.proto.image:
                image = Image(image_proto, self)
                self.image_map[image.id()] = image
            for inst_proto in self.proto.inst:
                inst = Inst(inst_proto, self)
                self.inst_map[inst.id()] = inst
                self.image_map[inst.image().id()].add_inst(inst)
        except IOError:
            logging.msg('static info not found\n')
    def find_image(self, image_id):
        return self.image_map[image_id]
    def find_inst(self, inst_id):
        return self.inst_map[inst_id]
    def display_image_table(self, f):
        for image in self.image_map.itervalues():
            f.write(str(image))
            f.write('\n')
    def display_inst_table(self, f):
        for inst in self.inst_map.itervalues():
            f.write(str(inst))
            f.write('\n')

