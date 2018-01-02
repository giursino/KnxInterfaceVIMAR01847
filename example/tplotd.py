import binascii
import socket
import struct
import sys

import plotly.plotly as py
from plotly.graph_objs import *


# WARNING: you need to config your API key before first use. See ~/.plotly/.credentials file.


# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = '/tmp/01847-temp'
print >>sys.stderr, 'connecting to %s' % server_address
try:
    sock.connect(server_address)
except socket.error, msg:
    print >>sys.stderr, msg
    sys.exit(1)

try:

    unpacker = struct.Struct('32s 32s f')
    
    while True:
        try:
            data = sock.recv(68)
            print >>sys.stderr, 'received "%s"' % binascii.hexlify(data)

            unpacked_data = unpacker.unpack(data)
            print >>sys.stderr, 'unpacked:', unpacked_data

            t=unpacked_data[0].split(b'\0',1)[0]
            track=unpacked_data[1].split(b'\0',1)[0]
            value=unpacked_data[2]

            print 'time=%s' % t 
            print 'track=%s' % track
            print 'value=%s' % value

            #new_data = Scatter(x=t, y=value )

            #data = Data( [ new_data ] )

            #plot_url = py.plot(data, filename='temperatura', fileopt='extend', auto_open=False)
        
        except KeyboardInterrupt:
            print 'Exiting..'
            sys.exit()

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
