# /usr/share/rtc_ds1307/clock_init.s
# sudo crontab @reboot

echo ds1307 0x68 > /sys/class/i2c-adapter/i2c-1/new_device
hwclock -s -f /dev/rtc1
hwclock -w
