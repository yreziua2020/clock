void wifiConnect() {    //printTime -вывод времени в ком порт
  if (printCom) { printTime();  Serial.print("Connecting WiFi (ssid=" + String(ssid.c_str()) + "  pass=" + String(password.c_str()) + ") ");  }
  if (!firstStart) printStringWithShift("WiFi", 15);  //при старте пичатаем
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS )) {    if (printCom) {Serial.println("STA Failed to configure");}   }             //  "Не удалось задать статический IP-адрес"
  WiFi.disconnect();   WiFi.mode(WIFI_STA);  
  WiFi.begin("home", "30011980");
// WiFi.begin(ssid.c_str(), password.c_str());
  for (int i = 1; i < 221; i++) 
  {
    if (WiFi.status() == WL_CONNECTED) 
    {  WIFI_connected = true;      
       if (printCom) {Serial.print(" IP adress : "); Serial.println(WiFi.localIP());}
        if (!firstStart) 
        { 
          String aaa=WiFi.localIP().toString() + "                ";  if(NUM_MAX > 4) {for (int j=0; j<NUM_MAX - 4; j++) aaa += "  "; }   clr(); printStringWithShift(" IP: ", 15); printStringWithShift(aaa.c_str(), 25);
          bip_privet();
          printStringWithShift("ХАЙ!!! ", 25);
         }  //Печатаем ip
          firstStart=1; timeUpdateNTP(); amountNotStarts=0;   return;
    }
    bip_Wi_Fi();  
   // mp3_play_file_in_fol (6,i);wait_play();  
    
    if (printCom) Serial.print(".");
    if (!firstStart)  { int j=0;  while (j<500) {if (j%10 == 0) showAnimWifi(i);   j++; delay(1);   }   }  // showAnimWifi анимация для вайфай
     delay (800);//800
  } //for
  
  WiFi.disconnect();   if (printCom) Serial.println(" Not connected!!!");
  amountNotStarts++;
  if (printCom) {    Serial.print("Amount of the unsuccessful connecting = ");    Serial.println(amountNotStarts);  }
  if (amountNotStarts > 221) { amountNotStarts=0;    firstStart=0;   
  #ifdef ESP32
    ESP.restart();
  #else
    ESP.reset(); 
  #endif 
  }   //рестарт   модуля
 ESP.reset(); 
  
 /*  
  if (!firstStart) 
  {   WiFi.mode(WIFI_AP);    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());    
      if (printCom) { printTime(); Serial.println("Start AP mode!!!"); Serial.print("          Wifi AP IP : ");  Serial.println(WiFi.softAPIP());}
      updateTime();    printStringWithShift(tPoint.c_str(), 35);    // tPoint="Подключитесь к точке доступа";
    //firstStart=1;
  }
*/
  
}
