//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sensorsAll() {
  if(pressBmp<min_dav) {min_dav=pressBmp;} //min давление больше чем текучее то огда приравниваем
  if(pressBmp>max_dav) {max_dav=pressBmp;}
  sensorsDs18b20();
 // sensorsSi7021();
  sensorsBmp();
  sensorsBme();
//  sensorsDht();
  sensors();
}
//--------------------------------senn-----------------------------------------
void sensors() {
  t0 = (sensorDom == 0 ? 0 : sensorDom == 1 ? tempDs18b20 : sensorDom == 2 ? celsiusSi7021 : sensorDom == 3 ? tempBmp : sensorDom == 4 ? tempBme : sensorDom == 5 ? tempDht : sensorDom == 6 ? tMqtt1 : sensorDom == 7 ? tMqtt2 : sensorDom == 8 ? tMqtt3 : 0);
  if (sensorDom) t0 += corrTempD;
//yyyyy

  t1 = int(t0);
  t2 = int(t0 * 10 * (t0 > 0 ? 1 : -1)) % 10;
  tt2= int(t0 * 100 * (t0 > 0 ? 1 : -1)) % 10;
  t3 = (sensorUl == 0 ? 0 : sensorUl == 1 ? tempDs18b20 : sensorUl == 2 ? celsiusSi7021 : sensorUl == 3 ? tempBmp : sensorUl == 4 ? tempBme : sensorUl == 5 ? tempDht : sensorUl == 6 ? tMqtt1 : sensorUl == 7 ? tMqtt2 : sensorUl == 8 ? tMqtt3 : sensorUl == 9 ? tempNM : 0);
   //t3='1234';
  if (sensorUl) t3 += corrTempU;
  t4 = int(t3 * -1);
  t5 = int(t3 * 10 * (t3 > 0 ? 1 : -1)) % 10;
  t6 = (sensorHome == 0 ? 0 : sensorHome == 1 ? tempDs18b20 : sensorHome == 2 ? celsiusSi7021 : sensorHome == 3 ? tempBmp : sensorHome == 4 ? tempBme : sensorHome == 5 ? tempDht : sensorHome == 6 ? tMqtt1 : sensorHome == 7 ? tMqtt2 : sensorHome == 8 ? tMqtt3 : sensorHome == 9 ? tempNM : 0);
  if (sensorHome) t6 += corrTempH;
  t7 = int(t6 * -1);
  t8 = int(t6 * 10 * (t6 > 0 ? 1 : -1)) % 10;
  
  h0 = (sensorHumi == 2 ? humSi7021 : sensorHumi == 4 ? humBme : sensorHumi == 5 ? humiDht : 0);
  if (sensorHumi) h0 += corrHumi;
  h1 = int(h0);
  h2 = int(h0 * 10 * (h0 > 0 ? 1 : -1)) % 10;
  p0 = int(sensorPrAl == 3 ? pressBmp : pressBme); //присваеваем значение давления в зависисоти от дачика p0 = или pressBmp : pressBme
  if (sensorPrAl) p0 += corrPress;

    p1 = p0 / 10000;
    p2 = (p0 - p1 * 10000) / 1000;
    p3 = (p0 - p1 * 10000 - p2 * 1000) / 100;
    p4 = (p0 - p1 * 10000 - p2 * 1000 - p3 * 100) / 10;
    p5 = (p0 - p1 * 10000 - p2 * 1000 - p3 * 100 - p4 * 10);
  

  ///if (printCom) {    Serial.print(String(p1));Serial.print(String(p2));Serial.print(String(p3));Serial.print(String(p4));Serial.println(String(p5)); }
  ////if (printCom) {    printTime(); if (sensorDom) Serial.println("sensorDom  = " + String(t0)); if (sensorUl) Serial.println("          sensorUl   = " + String(t3));  if (sensorHome) Serial.println("          sensorHome = " + String(t6));   if (sensorHumi) Serial.println("          sensorHumi = " + String(h0));    if (sensorPrAl) Serial.println("          sensorPrAl = " + String(p0));  }
}//--------------------------------------------------------------------------
void sensorsDs18b20() {  //1
   if (printCom) { Serial.println("Опрос 18b20");}
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if (!ds.search(addr)) {                               // Стартуємо функцію пошук першого або наступного датчика в addr-возвращаем ответ датчика
    ds.reset_search();                                  // Якщо добігли кінця адресного простору, то скидуємо адрес на початок простору
    delay(250);                                         // Пауза
    return;                                             // Виходимо з підпрограми
  }
  if (OneWire::crc8(addr, 7) != addr[7]) return;          // перевіряємо 7 байт в addr - он содержит crc8
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(750);
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3;
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;
    else if (cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
  }
  tempDs18b20 = (float)raw / 16.00;
 ////// if (printCom) {    printTime();    Serial.println("Temperature DS18B20: " + String(tempDs18b20) + " *C");  }
}//--------------------------------------------------------------------------
void sensorsSi7021() {  //2
  if (si7021 == false) return;
  humSi7021 = sensor.readHumidity();
  celsiusSi7021 = sensor.readTemperature();
  if (printCom) {    printTime();    Serial.println("Temperature Si7021: " + String(celsiusSi7021) + " *C,  Humidity: " + String(humSi7021) + " %");  }
}
//----------------------dav----------------------------------------------------
void sensorsBmp() {  //3
  if (bmp280 == true) {
    tempBmp = bmp.readTemperature();
    //pressBmp = bmp.readPressure() * (pressSys == 1 ? 133.32239 : 1); /// 133.32239; // (pressSys == 0 ? 0.007500613026439 : 1);
    pressBmp = bmp.readPressure() / (pressSys == 1 ? 1.3332239 : 1);
    pressBmp = (int) pressBmp;
    altBmp = bmp.readAltitude(1013.25);
    if (printCom) {  printTime(); if (bmp280 == true) Serial.println("Temperature BMP280: " + String(tempBmp) + " *C,  Pressure: " + String(pressBmp) + (pressSys == 1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBmp) + " m");    }
  }

  //-------------------------180
  if (BMP180 == true) {
    tempBmp = bmp180.readTemperature();
    pressBmp = bmp180.readPressure() * (pressSys == 0 ? 0.007500613026439 : 1);
    pressBmp = (int) pressBmp;
    altBmp = bmp180.readAltitude(101500);
    if (printCom) {  printTime();  if (BMP180 == true) Serial.println("Temperature BMP180: " + String(tempBmp) + " *C,  Pressure: " + String(pressBmp) + (pressSys == 1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBmp) + " m");    }
  }
}
//----------------------------BME280----------------------------------------------
void sensorsBme() {  //4
  if (bme280 == false) return;
  tempBme = bme.readTempC();          //bme.readTempF()
  humBme = bme.readHumidity();
  pressBme = (bme.readPressure()*100) / (pressSys == 1 ? 1.3332239 : 1);
  pressBme = (int) pressBme;
  altBme = bme.readAltitudeFeet();   //bme.readAltitudeMeter()  bme.readAltitudeFeet()
  if (printCom) {    printTime();    Serial.println("Temperature BME280: " + String(tempBme) + " *C,  Humidity: " + String(humBme) + " %,  Pressure: " + String(int(pressBme)) + (pressSys == 1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBme) + " m");  }
}//--------------------------------------------------------------------------
void sensorsDht() {   //5
  int err = SimpleDHTErrSuccess;
  int err2 = SimpleDHTErrSuccess;
  byte temp11 = 0;
  byte humi11 = 0;
  if ((err2 = dht22.read2(pinDHT, &tempDht, &humiDht, NULL)) == SimpleDHTErrSuccess) {
    humiDht = (int) (humiDht);
    /////if (printCom) {printTime(); Serial.println("Temperature DHT22: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");  }
  }
  if ((err = dht11.read(pinDHT, &temp11, &humi11, NULL)) == SimpleDHTErrSuccess) {
    tempDht = temp11;
    humiDht = humi11;
    if (printCom) {printTime();  Serial.println("Temperature DHT11: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");  }
  }
}
