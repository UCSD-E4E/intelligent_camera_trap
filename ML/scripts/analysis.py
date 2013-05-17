import csv
import numpy 
import scipy
import Image
import matplotlib

data = numpy.loadtxt('sealab-celcius-real.csv', delimiter=',')
print data.shape

test_row = data[5,range(1, 65)]
print test_row.shape

mean = test_row.mean()
print mean
print test_row
for i in range(0,64):
	if test_row[i] < mean*1.1:
		test_row[i] = 0
	else:
		test_row[i] = 255

#print numpy.array([[1,2,3], [4,5,6]]).shape

reshaped = test_row.reshape(4,16,order='F').copy()
print reshaped.shape

img = Image.fromarray(reshaped)
img.show()

def adaptiveThresh(in_mat, k):
   mean = in_mat.mean()
	for i in range(0,len(in_mat)):
		if in_mat[i] < mean*k:
			in_mat[i] = 0
		else:
			in_mat[i] = 255
	return in_mat

def imgTransform(in_mat)
	reshaped = in_mat.reshape(4,16,order='F').copy()
	img = Image.fromarray(reshaped)
	return img


#FULL RUN:
for i in range(0,30):
	sample = data[i, range(1,65)]
	thresh_mat = adaptive_thresh(sample, 1.1)
	thresh_img = imgTransform(thresh_mat)
	
	
