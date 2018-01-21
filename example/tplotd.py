import binascii
import socket
import struct
import sys
import time

import plotly.plotly as py
import plotly.tools as tls
import plotly.graph_objs as go


# WARNING: you need to config your API key before first use. See ~/.plotly/.credentials file.


#######################
#    ZONA GIORNO
#######################

print >>sys.stderr, 'Setting plotly: Zona Giorno...'

# Data stream on Plotly
stream_TaGiorno = dict(token='frxfxqesod', maxpoints=10000)
stream_ValvolaGiorno = dict(token='d8r55gnaez', maxpoints=10000)
stream_TEsterna = dict(token='j3cczgsj6q', maxpoints=10000)

# Setup Plotly graph
trace_TaGiorno = go.Scatter(x=[], y=[], stream=stream_TaGiorno, name='Ta giorno')
trace_ValvolaGiorno = go.Scatter(x=[], y=[], stream=stream_ValvolaGiorno, yaxis='y2', name='Valvola giorno', marker=dict(color='rgb(148, 103, 189)'))
trace_TEsterna = go.Scatter(x=[], y=[], stream=stream_TEsterna, name='T esterna', marker=dict(color='rgb(0, 103, 0)'))

data = [trace_TaGiorno, trace_ValvolaGiorno, trace_TEsterna]
layout = go.Layout(
    title='Grafico temperatura reparto giorno',
    yaxis=dict(
        title='*C'
    ),
    yaxis2=dict(
        title='On/Off',
        titlefont=dict(
            color='rgb(148, 103, 189)'
        ),
        tickfont=dict(
            color='rgb(148, 103, 189)'
        ),
        overlaying='y',
        side='right'
    )
)

fig = go.Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='temperatura-rep-giorno', fileopt='extend', auto_open=False)

# Setup Stream object
s_Ta = py.Stream(stream_id='frxfxqesod')
s_V = py.Stream(stream_id='d8r55gnaez')
s_Te = py.Stream(stream_id='j3cczgsj6q')



#######################
#    ZONA NOTTE
#######################

print >>sys.stderr, 'Setting plotly: Zona Notte...'

# Data stream on Plotly
stream_TaNotte = dict(token='z6wiwrjgs4', maxpoints=10000)
stream_ValvolaNotte = dict(token='27ziv50bie', maxpoints=10000)
stream_TEsterna = dict(token='j3cczgsj6q', maxpoints=10000)

# Setup Plotly graph
trace_TaNotte = go.Scatter(x=[], y=[], stream=stream_TaNotte, name='Ta notte')
trace_ValvolaNotte = go.Scatter(x=[], y=[], stream=stream_ValvolaNotte, yaxis='y2', name='Valvola notte', marker=dict(color='rgb(148, 103, 189)'))
trace_TEsterna = go.Scatter(x=[], y=[], stream=stream_TEsterna, name='T esterna', marker=dict(color='rgb(0, 103, 0)'))

data = [trace_TaNotte, trace_ValvolaNotte, trace_TEsterna]
layout = go.Layout(
    title='Grafico temperatura reparto notte',
    yaxis=dict(
        title='*C'
    ),
    yaxis2=dict(
        title='On/Off',
        titlefont=dict(
            color='rgb(148, 103, 189)'
        ),
        tickfont=dict(
            color='rgb(148, 103, 189)'
        ),
        overlaying='y',
        side='right'
    )
)

fig = go.Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='temperatura-rep-notte', fileopt='extend', auto_open=False)

# Setup Stream object
s_TaN = py.Stream(stream_id='z6wiwrjgs4')
s_VN = py.Stream(stream_id='27ziv50bie')
s_Te = py.Stream(stream_id='j3cczgsj6q')





# Create a UDS socket
print >>sys.stderr, 'Opening socket...'
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
    s_Ta.open()
    s_V.open()
    s_Te.open()
    
    s_TaN.open()
    s_VN.open()
    
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
            
            retry = 3
            
            while retry:
            
                try:
                    if track == 'Ta_giorno':
                        s_Ta.write(dict(x=t, y=value))
                    elif track == 'Valvola_giorno':
                        s_V.write(dict(x=t, y=value))
                    elif track == 'T_ext':
                        s_Te.write(dict(x=t, y=value))
                    if track == 'Ta_notte':
                        s_TaN.write(dict(x=t, y=value))
                    elif track == 'Valvola_notte':
                        s_VN.write(dict(x=t, y=value))
                        
                    break
                    
                except:
                    print >>sys.stderr, 'WARNING: stream write failed, retrying...'
                    time.sleep(3)
                    retry -= 1
                    if (retry == 0):
                        print >>sys.stderr, 'ERROR: stream write failed, stop retrying. Data lost.'
                        
            print >>sys.stderr, '***********'
            
        except KeyboardInterrupt:
            print 'Exiting..'
            sys.exit()

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
    
    print >>sys.stderr, 'closing Stream'
    s_Ta.close()
    s_V.close()
    s_Te.close()
    
    s_TaN.close()
    s_VN.close()
    print >>sys.stderr, 'done.'
