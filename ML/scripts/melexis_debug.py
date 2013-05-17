import numpy as np
import scipy
import fnmatch
import sys
import serial
import Image

def formImage(in_vector):
	img_max = max(in_vector)
	img_min = min(in_vector)
	
	#normalize intensities to range [0,255]
	for i in range(len(in_vector)):
		in_vector[i] -= img_min
		in_vector[i] *= (255/(img_max-img_min))
	
	#stack normalized intensity vector into image matrix
	reshaped = in_vector.reshape(4,16, order='F').copy()
	img = Image.fromarray(reshaped)
	
	#Now,  scale it up for viewability
	img = img.resize([30*16, 30*4])
	return img
	

#check for command line arguments
num_args = len(sys.argv)
if num_args == 3:
	br_str = fnmatch.filter(sys.argv, 'b*')
	port_str = fnmatch.filter(sys.argv, 'tty*')
else:	
	print ("Example Usage: python melexis_debug.py ttyACM0 b115200")
	exit()

ser = serial.Serial()
ser.port = "/dev/" + port_str[0]

rate = int(br_str[0].replace("b", ""));

print ("Opening port:" + ser.port + " at b" + str(rate))

#Set Serial Properties
ser.baudrate = rate
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = 2 
ser.writeTimeout = 2

try: 
	ser.open()

except Exception, e:
	print "Error opening port :( "+ str(e)
	exit()

if ser.isOpen():

	try:
		#clean out buffers
		ser.flushInput()
		ser.flushOutput()
		
		#make sure we read both the ambient and the data each cycle
		read_amb = False
		read_dat = False
		
		while True:
			response = ser.readline()
			if response.startswith("Ambient"):
				ambient = float(response.strip('Ambient T='))
				read_amb = True
			elif response.startswith("IR:"):
				data = response.strip("IR: ")
				read_dat = True

			if read_amb and read_dat:
				print "Ambient: " + str(ambient)
				#transform input string into float array
				data_list = data.split()
				data_vector = map(float, data_list) 
				
				print "Data: "
				print data_vector
				
				#turn vector of values into a human-viewable image
				img = formImage(np.asarray(data_vector))
				img.show()
				
				read_amb = False
				read_dat = False 
			
	except Exception, e1:
		print "Error in communication with port: " + str(e1)

else:
	print "Cannot open serial port"	

