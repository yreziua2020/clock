void kurs_z(const uint16_t opros) {
    static unsigned long timer_kurs = millis();
        if (millis() - timer_kurs > (opros*60000)) {
        timer_kurs = millis();
        if(_Dkurs){Serial.println("курс валют");} 
        kurs();
    }
}

void kurs(){

  WiFiClientSecure httpsClient;
 //WiFiClientSecure httpsClient;    //Declare object of class WiFiClient
  if(_Dkurs){Serial.println(host2);  Serial.printf("Using fingerprint '%s'\n", fingerprint);}
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(5000); // 10 Seconds
  delay(1000);
  
  if(_Dkurs){Serial.print("HTTPS Connecting Start");}  //дальше не проходит
  int r=0; //retry counter
  while((!httpsClient.connect(host2, httpsPort2)) && (r < 30))  { delay(100); if(_Dkurs){Serial.print(".");}  r++; }   if(r==30) { if(_Dkurs){Serial.println(" Connection failed");} }  else {   if(_Dkurs){ Serial.println(" Connected to web finich");} }

 Serial.println("111111");
  String   Link; 
  Link = "/p24api/pubinfo?json&exchange&coursid=5";  if(_Dkurs){Serial.print("requesting URL: ");  Serial.println(host2+Link);}

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" + "Host: " + host2 + "\r\n" + "Connection: close\r\n\r\n");
  if(_Dkurs){Serial.println("request sent");}
                 
//  while (httpsClient.connected()) {  String line = httpsClient.readStringUntil('\n');     if (line == "\r") {      Serial.println("headers received");    break;    }  }
//  Serial.println("reply was:");    Serial.println("=========="); 


  int repeatCounter = 10;  while (!httpsClient.available() && repeatCounter--) {    delay(500); Serial.print("*"); }
  char endOfHeaders[] = "\r\n\r\n";  if (!httpsClient.find(endOfHeaders)) {    Serial.println(F("Invalid response"));  httpsClient.stop();  return;  }
  

   // while (client.connected() && client.available()) {  line = client.readStringUntil('\r');    Serial.println(line);   delay(100);}
  
 // delay(1500);

  Serial.println("lfikj kb cele");
  while(httpsClient.available()){   //пока есть информация читаем   тмкже может применяться httpsClient.connect() - без параметров чтобы проверит законекчен ли клиент
    httpsClient.readStringUntil('\r');
    //StaticJsonDocument<1024> doc;      // StaticJsonDocument<512> doc;

   DynamicJsonDocument doc(1024);
  
    DeserializationError error = deserializeJson(doc, httpsClient);   //десереализация из httpsClient в doc  это процесс восстановления объекта из этих байт
   if (error) {  if(_Dkurs){ Serial.print(F("deserializeJson() failed: "));   Serial.println(error.f_str());}   return;   }  //проверка на ошибку если возникает ошиька то пичатаем код ошибки
   
    // ReadLoggingStream loggingStream(httpsClient, Serial);
    //  DeserializationError err = deserializeJson(doc, loggingStream); //выводит весь джейсон

    if(_Dkurs){Serial.print("Memory usage: ");Serial.println(doc.memoryUsage());}        //  сколько памяти использовано
    JsonArray array = doc.as<JsonArray>();  //создаем жейсон обьект
    if(_Dkurs){Serial.print("Array json size: ");Serial.println(array.size());}         //array size
 
    if(_Dkurs){Serial.print("doc= ");Serial.println(httpsClient);}         //array size
     
      for (int i=0;i<(array.size());i++)
          if (doc[i]["ccy"] == "USD")  {
            USDbuy =  doc[i]["buy"];
            USDsale = doc[i]["sale"];
            if(_Dkurs){Serial.print("USD buy  ");Serial.println(USDbuy);   Serial.print("USD sale ");Serial.println(USDsale);}
          }

    EURbuy =  doc[1]["buy"];    EURsale = doc[1]["sale"];    if(_Dkurs){Serial.print("EUR buy  ");Serial.println(EURbuy);    Serial.print("EUR sale ");Serial.println(EURsale);}
    BTCbuy =  doc[2]["buy"];    BTCsale = doc[2]["sale"];    if(_Dkurs){Serial.print("BTC buy  ");Serial.println(BTCbuy);    Serial.print("BTC sale ");Serial.println(BTCsale);}
    doc.clear();
    delay(1000);
  }     //while
 
/*  
  String line;
  Serial.println("==========");
 // while (httpsClient.available()) { line = httpsClient.readStringUntil('\n');    Serial.println(line);   }
    while (httpsClient.available()) { datarx += httpsClient.readStringUntil('\n'); }  Serial.println(datarx);  datarx = "";  
  Serial.println("==========");  Serial.println("closing connection");
  delay(2000);  //GET Data at every 2 seconds
*/
  
  httpsClient.stop();
  Kurs_disp="Курс наличный ";
  Kurs_disp+="  USD sale= ";
  Kurs_disp+=USDsale;
  Kurs_disp+="USD buy= ";
}
