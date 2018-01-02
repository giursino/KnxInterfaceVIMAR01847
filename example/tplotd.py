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

    unpacker = struct.Struct('I f')
    
    while True:
        data = sock.recv(1024)
        print >>sys.stderr, 'received "%s"' % binascii.hexlify(data)

        unpacked_data = unpacker.unpack(data)
        print >>sys.stderr, 'unpacked:', unpacked_data


        new_data = Scatter(x=unpacked_data[0], y=unpacked_data[1] )

        data = Data( [ new_data ] )

        plot_url = py.plot(data, filename='prova', fileopt='extend', auto_open=False)

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
