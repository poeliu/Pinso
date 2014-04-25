import sys
from maple.proto.core import static_info_pb2
from maple.proto.delta import ilist_pb2
from maple.proto.delta import slist_pb2
from maple.proto.idiom import iroot_pb2
from maple.proto.idiom import memo_pb2
from maple.proto.idiom import history_pb2

class ReadProtoBuf(object):
    def __init__(self, input_file):
        self.infile = input_file

    def ReadiRoot(self):
  	ifile = open(self.infile, "rb")
        iroot_db = iroot_pb2.iRootDBProto()
        iroot_db.ParseFromString(ifile.read())
        ifile.close()

	ofile = open(self.infile+'.display', 'w')
        for event in iroot_db.event:
	    ofile.write("Event:"+str(hex(event.id))) 
	    ofile.write(' ')
	    ofile.write("Inst:"+str(hex(event.inst_id)))
            ofile.write(' ')
	    if event.type == 1:
		ofile.write("READ")
	    if event.type == 2:
		ofile.write("WRITE")
	    if event.type == 3:
		ofile.write("LOCK")
	    if event.type == 4:
		ofile.write("UNLOCK")
	    ofile.write('\n')
  	for iroot in iroot_db.iroot:
	    ofile.write("iRoot: "+str(hex(iroot.id)))
            ofile.write(' ')
	    ofile.write("Idiom: "+str(hex(iroot.idiom)))
            ofile.write(' ')
   	    #ofile.write(str(iroot.event_id))
            #ofile.write('\n')
            for event in iroot.event_id:
                 ofile.write(str(hex(event)))
		 ofile.write(' ')
            ofile.write('\n')
	ofile.close()

    def ReadStaticInfo(self):
  	ifile = open(self.infile, "rb")
        sinfo_db = static_info_pb2.StaticInfoProto()
        sinfo_db.ParseFromString(ifile.read())
        ifile.close()

	ofile = open(self.infile+'.display', 'w')
        for image in sinfo_db.image:
	    ofile.write("image:"+str(hex(image.id))) 
	    ofile.write(' ')
	    ofile.write(str(image.name))
	    ofile.write('\n')
  	for inst in sinfo_db.inst:
	    ofile.write("id:"+str(hex(inst.id)))
            ofile.write(' ')
	    ofile.write("Image:"+str(hex(inst.image_id)))
            ofile.write(' ')
   	    ofile.write("offset:"+str(hex(inst.offset)))
            ofile.write(' ')
            ofile.write('\n')
	ofile.close()
  
    def ReadiList(self):
        print "read_ilist"
