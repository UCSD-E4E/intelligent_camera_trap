#!/bin/bash
USER="ubuntu"

echo "We're setting up the ICT for user $USER..."

# Make sure root is running the script
if [ "$(id -u)" != "0" ]; then
   echo "ERROR!"  
   echo "This script must be run as root."
   echo "Did not begin installation." 1>&2
   echo "Exiting..."
   exit 1
fi

printf "\n1.) Copying the RTC driver into /usr/share..."
cp -u /home/$USER/intelligent_camera_trap/BBB/stm/clock_init.sh /usr/share
printf "done\n"


if [ ! -f /etc/screenrc ]; then
   echo "ERROR!"
   echo "screen is not installed."  
   echo "Did not complete installation."
   echo "Exiting..."
   exit 1
fi

#write out current screenrc
cat /etc/screenrc > my_screenrc
# add our desired lines if they don't exist
if grep --silent "logfile flush 1" my_screenrc
then
   printf "\n'logfile flush 1' already written.\n"
else
   echo "logfile flush 1" >> my_screenrc
fi

if grep --silent "logfile /home/$USER/screen_log.txt" my_screenrc
then
   printf "\n'logfile /home/$USER/screen_log.txt' already written.\n"
else
   echo "logfile /home/$USER/screen_log.txt" >> my_screenrc
fi

# write our screenrc back to /etc/screenrc
cat my_screenrc > /etc/screenrc
rm my_screenrc

printf "done\n"


printf "\n3a.) Creating the directory /home/root..."
mkdir -p /home/root
printf "done\n"

printf "\n3b.) Installing our setup.sh script /home/root..."
# write all our lines into setup.sh file in here
echo "chmod a+rw /dev/ttyO*" > /home/root/setup.sh
echo "chmod a+rw /dev/video*" >> /home/root/setup.sh
#echo "/usr/share/rtc_ds1307/clock_init.sh" >> /home/root/setup.sh
echo "echo ITC_setup_complete > /dev/shm/ITC_setup_complete" >> /home/root/setup.sh
chmod +x /home/root/setup.sh
printf "done\n"


printf "\n4.) Creating our run.sh script in current direcory..."
echo "/usr/bin/python /home/$USER/intelligent_camera_trap/BBB/stm/Main.py" > run.sh
chmod +x run.sh
printf "done\n"

printf "\n5.) Adding root's cronjob..."
#write out current crontab
crontab -u root -l > mycron
#echo new cron into cron file
if grep --silent "@reboot /home/root/setup.sh" mycron
then
   printf "\nroot's setup.sh cronjob is already installed.  Moving on...\n"
else
   echo "@reboot /home/root/setup.sh" >> mycron
fi
if grep --silent "@reboot /usr/bin/screen -L -dmS remote_system /home/$USER/intelligent_camera_trap/BBB/stm/run.sh" mycron

then 
   printf "\nroot's screen cronjob already installed.  Moving on...\n"
else
   echo "@reboot /usr/bin/screen -L -dmS remote_system /home/$USER/intelligent_camera_trap/BBB/stm/run.sh" >> mycron
fi
#install new cron file
crontab -u root mycron
rm mycron
printf "done\n"


#printf "\nWe'd like to reboot the system to finish installation\n"
#read -r -p "Reboot now? [y/N] " response
#case $response in
    #[yY][eE][sS]|[yY]) 
        #echo "Rebooting now..."
        #;;
    #*)
        #echo "Exiting..."
        #exit 1
        #;;
#esac


