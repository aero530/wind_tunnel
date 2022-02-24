import time
import serial
import re
import sys

import serial.tools.list_ports
	
def read_all(port, chunk_size=200):
    """Read all characters on the serial port and return them."""
    if not port.timeout:
        raise TypeError('Port needs to have a timeout set!')

    read_buffer = b''

    while True:
        # Read in chunks. Each chunk will wait as long as specified by
        # timeout. Increase chunk_size to fail quicker
        byte_chunk = port.read(size=chunk_size)
        read_buffer += byte_chunk
        if not len(byte_chunk) == chunk_size:
            break

    return read_buffer
	
for n, (port, desc, hwid) in enumerate(sorted(serial.tools.list_ports.comports()), 1):
    print(n)
    print(port)
    print(desc)
    print(hwid)
    print(" ")

# configure the serial connections (the parameters differs on the device you are connecting to)
windTunnel = serial.Serial(
    port='COM8',
    baudrate=57600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS,
	timeout=.1
)

windTunnel.isOpen()

windTunnel.write(b'd')
windTunnel.write(b'\n')
time.sleep(2);

# -----------------------------------------------------

stepDuration = float(sys.argv[1])
stepSize = float(sys.argv[2])
percentMin = float(sys.argv[3])
percentMax = float(sys.argv[4])
filename = sys.argv[5]


delim = ','

# -----------------------------------------------------

#fileDate = time.strftime("%Y-%m-%d_%H-%M-%S", time.gmtime())

o = open('{0}_Tunnel.csv'.format(filename),'w', newline="\r\n")
o.write('timestamp'+delim+'percentSetting'+delim+'PWM1'+delim+'PWM2\n')

percent = percentMin
windTunnelPercent = 0
windTunnelPWM1 = 0
windTunnelPWM2 = 0

print('Step Duration ' + str(stepDuration))
print('Step Size ' + str(stepSize))
print('Test Length ' + str((percentMax-percentMin)/stepSize*stepDuration) + ' seconds')
print(percent)

startTime = time.time()

while percent <= percentMax :
	o.write(str(time.time()) + delim + str(windTunnelPercent) + delim + str(windTunnelPWM1) + delim + str(windTunnelPWM2) +'\n')
	if (time.time() - startTime > stepDuration) :
		percent += stepSize
		
		windTunnel.write(b's')
		windTunnel.write(b'=')	
		for c in str(percent):
			windTunnel.write(c.encode())
		windTunnel.write(b'\n')
		print(percent)
		time.sleep(0.01)
		
		
		needWindTunnelReading = True
		while needWindTunnelReading :
			windTunnel.write(b'g')
			windTunnel.write(b'\n')
			windTunnelReading = read_all(windTunnel).decode().replace('\n', '').replace('\r', '').replace(' ','').replace('\t','').split(',')	
			if (len(windTunnelReading) == 3) :
				needWindTunnelReading = False
		windTunnelPercent = windTunnelReading[0]
		windTunnelPWM1 = windTunnelReading[1]
		windTunnelPWM2 = windTunnelReading[2]
		startTime = time.time();
	
	
	time.sleep(.05)

time.sleep(stepDuration)

o.close()

windTunnel.write(b'd')
windTunnel.write(b'\n')

time.sleep(0.1)

windTunnel.close()
