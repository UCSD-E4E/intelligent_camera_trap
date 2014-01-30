import time

week    = ["mon","tue", "wed", "thu", "fri", "sat", "sun"]

def getCurrentTime ():
   TM_YEAR  = 0
   TM_MON   = 1
   TM_MDAY  = 2
   TM_HOUR  = 3
   TM_MIN   = 4
   TM_SEC   = 5
   TM_WDAY  = 6
   TM_YDAY  = 7
   TM_ISDST = 8

   clockTime = time.localtime(time.time())

   #Massage to get leading zeros

   newFormatTime = {}
   newFormatTime["YEAR"]        =  "%s" %clockTime[TM_YEAR]        

   if clockTime[TM_MON] < 10:
       newFormatTime["MONTH"] = "0%s" %(clockTime[TM_MON])
   else:
       newFormatTime["MONTH"] = "%s" %(clockTime[TM_MON])

   if clockTime[TM_MDAY] < 10:
       newFormatTime["MONTHDAY"] = "0%s" %(clockTime[TM_MDAY])
   else:
       newFormatTime["MONTHDAY"] = "%s" %(clockTime[TM_MDAY])

   if clockTime[TM_HOUR] < 10:
       newFormatTime["HOUR"] = "0%s" %(clockTime[TM_HOUR])
   else:
       newFormatTime["HOUR"] = "%s" %(clockTime[TM_HOUR])

   if clockTime[TM_MIN] < 10:
       newFormatTime["MINUTE"] = "0%s" %(clockTime[TM_MIN])
   else:
       newFormatTime["MINUTE"] = "%s" %(clockTime[TM_MIN])

   if clockTime[TM_SEC] < 10:
       newFormatTime["SEC"] = "0%s" %(clockTime[TM_SEC])
   else:
       newFormatTime["SEC"] = "%s" %(clockTime[TM_SEC])

   newFormatTime["WEEKDAY"]     =  "%s" %week[clockTime[TM_WDAY]]     
   newFormatTime["YDAY"]        =  "%s" %clockTime[TM_YDAY]        
   newFormatTime["ISDST"]       =  "%s" %clockTime[TM_ISDST]       

   return newFormatTime



