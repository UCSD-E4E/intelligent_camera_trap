#!/bin/bash
ACTIVITY="A"
HEARTBEAT="B"
TRACKING="C"
POWERLEDON="D"
POWERLEDOFF="E"
SUSPENDACK="F"
HIBERNATEACK="G"
POWEROFFACK="H"


POWEROFF="HC1X"
SUSPEND="HC1S"
HIBERNATE="HC1H"
ACKNOWLEDGE="HC1A"
COUNT=10
serialInput="A"

#Setup COM1
sudo chmod 0777 /dev/ttyS1
stty -F /dev/ttyS1 57600 -echo 

echo $ACTIVITY > /dev/ttyS1
sleep 1
echo $HEARTBEAT > /dev/ttyS1
sleep 1
echo $TRACKING > /dev/ttyS1
sleep 1

echo SUSPEND THEN POWEROFF
read -n4 serialInput < /dev/ttyS1 &
while [ $COUNT -gt 0 ]; do
	#let COUNT=COUNT-1
	read -n4 -t10 serialInput < /dev/ttyS1 				
	echo $serialInput

	if [ ${#serialInput} -eq 4 ]
	then
 		if   [ $serialInput == $POWEROFF ]
		then
			echo ENTERING_POWEROFF
			echo $POWEROFFACK > /dev/ttyS1
			./cmd-poweroff.sh
		
		elif [ $serialInput == $SUSPEND ]
		then
			echo ENTERING_SUSPEND
			echo $SUSPENDACK > /dev/ttyS1
			sleep 3
			./cmd-suspend.sh
			
		elif [ $serialInput == $ACKNOWLEDGE ]
		then
 			echo ACKNOWLEDGE

		fi

	else
		echo "Chucking along"
	
	fi	
	
	echo $HEARTBEAT > /dev/ttyS1
	sleep 0.25
	#Get suspended!	
	#echo $TRACKING > /dev/ttyS1
	#sleep 0.25	
done

