DHT dht(DHTPIN, DHTTYPE);

void UpdateDHT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (!(isnan(h) || isnan(t) || isnan(f))) 
  {
    float hic = dht.computeHeatIndex(t, h, false);

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, hic);
  }
}

void DHTCall()
{
  processing = 1;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    terminal.println("Failed to read from DHT sensor!");
  }
  else
  {
    float hif = dht.computeHeatIndex(f, h);
    // tinh chi so nhiet
    float hic = dht.computeHeatIndex(t, h, false);

    terminal.print(F("Humidity: "));
    terminal.println(h);
    terminal.print(F("%  Temperature: "));
    terminal.print(t);
    terminal.print(F("°C "));
    terminal.print(f);
    terminal.print(F("°F  Heat index: "));
    terminal.print(hic);
    terminal.print(F("°C "));
    terminal.print(hif);
    terminal.println(F("°F"));
    terminal.flush();
  }
  delay(1000);
  processing = 0;
}

void AcessWater()
{
    dateTime = NTPch.getNTPtime(7.0, 0); // Timezone
  
    // check dateTime.valid before getting time
    if(dateTime.valid)
    {
      NTPch.printDateTime(dateTime);
  
      byte actualHour = dateTime.hour;      // Gio
      byte actualMinute = dateTime.minute;  // Phut
      byte actualSecond = dateTime.second;  // Giay

      terminal.print("Time: ");
      terminal.print(actualHour);
      terminal.print(":");
      terminal.print(actualMinute);
      terminal.print(":");
      terminal.println(actualSecond);
    }

    terminal.println("Starting...");
    DHTCall();
    processing = 1;
    digitalWrite(Relay, HIGH); //On
    for(int i = 30; i >= 0; i--)
    {
      terminal.print("Time remaining: ");
      terminal.print(i);
      terminal.print(" s\n");
      terminal.flush();
      delay(1000);
    }
    digitalWrite(Relay, LOW); //Off
    terminal.println("Done.");
    terminal.flush();
    processing = 0;
}
BLYNK_WRITE(V6){
     int p = param.asInt();
   if (p == 1) {
       //terminal.println("PUMP ON");
       delay(200);
       digitalWrite(Relay, LOW);   
    
   }
   else if (p == 0) {
       //terminal.println("PUMP OFF");
       delay(200);
       digitalWrite(Relay, HIGH);
   }
}

BLYNK_WRITE(V3)
{
   if (String("restart") == param.asStr() || String("reboot") == param.asStr()) 
  {
    terminal.print("Restarting");
    for(byte i = 1; i <= 3; i++)
    {
      delay(1000);
      terminal.print('.');
    }
    terminal.flush(); // Ensure everything is sent
    ESP.restart();
  }
  
  else if (String("water") == param.asStr())
  {
    AcessWater();
  }
  
  else if (String("clrscr") == param.asStr())
  {
    terminal.clear();
  }
  
  else if (String("temperature") == param.asStr() || String("temp") == param.asStr())
  {
    DHTCall();
  }
  
  else if (String("help") == param.asStr())
  {
    terminal.println("Command option:");
    terminal.println("version");
    terminal.println("restart");
    terminal.println("water");
    terminal.println("clrscr");
    terminal.println("temp");
    terminal.print("help\n");
  }
  
  else if (String("version") == param.asStr())
  {
    terminal.print("Esp8266 IoT-Farm ");
    terminal.println(Ver);
    terminal.print("Upload date: ");
    terminal.println(dateup);
    terminal.println("Processor: NodeMCU.");
    terminal.print("NT131.M11-Nhom19");
    terminal.flush();
  }
  
  else
  {
    // Send it back
    terminal.println("Invalid command line.");
  }

  terminal.flush();
}
