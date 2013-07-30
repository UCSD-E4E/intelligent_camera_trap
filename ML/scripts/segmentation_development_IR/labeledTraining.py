#Modify inputted .csv file.  Allows you to add a "blob count" to the end
#of any given range of lines (or 1 line if you specify just a range from
# 1 to 1, for example). Blob counts are automatically updated/overwritten
#if you come back to the file and change the blob count.
#ASSUMES THE .csv will look like this: data,data,data,data,blobCount

import fnmatch
import sys
import traceback

fileName = raw_input("Enter the name of the file you'd like to modify: ")
validFileName = fnmatch.fnmatch(fileName, '*.csv')
if validFileName:
	fileName = str(fileName)
else:
	print "Bad file name, exiting"
	exit()

while True:
	s = raw_input("Enter tihe range of lines you'd like to modify separated by a space: firstLine secondLine\n")
	firstLine,secondLine = [int(i) for i in s.split(' ')]

	if firstLine == secondLine:
		try:
			with open(fileName, 'r+') as file:
				data = file.readlines()
				if firstLine >= 1 and firstLine <= len(data):
					s = raw_input("Enter the number of blobs to expect for these frames\n")
				else:
					print "Enter a valid range \n"
					continue
				try: 
					numBlobs = int(s)				
				except:				
					print "Enter a valid number."	
					continue

				noNewLines = data[(firstLine - 1)].rstrip('\n')
				commaSplit = noNewLines.split(',')
				if commaSplit[len(commaSplit) - 1] == '':  #haven't added a blob count yet
					data[(firstLine - 1)] = noNewLines + str(numBlobs) + '\n'
				else: #need to replace our previous blob count
					commaSplit[len(commaSplit) - 1] = str(numBlobs) + '\n'
					print ','.join(commaSplit)					
					data[(firstLine - 1)] = ','.join(commaSplit)		
					
			with open(fileName, 'w') as file:
				file.writelines(data)
			print "Done with those changes.\n\n"
		except:
			traceback.print_exc(file=sys.stdout)			
			print "I/O Error.  Exiting..."
			exit()
	
	else:
		try:		
			with open(fileName, 'r+') as file:
				data = file.readlines()
				if (firstLine >= 1 and firstLine <= len(data)) and (secondLine >= 1 and secondLine <= len(data)) and (firstLine < secondLine):
					s = raw_input("Enter the number of blobs to expect for these frames\n")
				else:
					print "Enter a valid range combination\n"
					continue				
	
				try: 
					numBlobs = int(s)				
				except:				
					print "Enter a valid number."	
					continue
			for i in range((firstLine - 1), secondLine):		
				noNewLines = data[i].rstrip('\n')
				commaSplit = noNewLines.split(',')
				if commaSplit[len(commaSplit) - 1] == '':  #haven't added a blob count yet
					data[i] = noNewLines + str(numBlobs) + '\n'
				else: #need to replace our previous blob count
					commaSplit[len(commaSplit) - 1] = str(numBlobs) + '\n'
					data[i] = ','.join(commaSplit)	
			
			with open(fileName, 'w') as file:
				file.writelines(data)
			print "Done with those changes.\n\n"
		except:
			traceback.print_exc(file=sys.stdout)						
			print "I/O Error. Exiting..."
			exit()
