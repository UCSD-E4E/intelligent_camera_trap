
def getGridIndexOf(row, column):
	index = (((column-1)*4) + (row - 1))	
	return index




print "Enter your row and column, to determine the Grid Index you're at"

while True:
	s = raw_input('Insert 2 integers separated by a space (Row Column): ')
	row,column = [int(i) for i in s.split(' ')]
	
	print getGridIndexOf(row,column)
	print ""


