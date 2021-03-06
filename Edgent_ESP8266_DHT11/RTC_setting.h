NTPtime NTPch("ch.pool.ntp.org");   // Server NTP

bool getTimeWatering()
{
  dateTime = NTPch.getNTPtime(7.0, 0); // Timezone
  
  // check dateTime.valid 
  if(dateTime.valid)
  {
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;      // Hour
    byte actualMinute = dateTime.minute;  // Minute
    byte actualSecond = dateTime.second;  // Second

    if((actualHour >= startWaterTime && actualHour <= endWaterTime) && ((actualHour % timeBTW == 0) && (actualMinute == 0) && (actualSecond <= 25)))
      return true;
  }
  return false;
}
