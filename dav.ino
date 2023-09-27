void dav_opros  (void)
{   if (printCom) {  printTime();    Serial.print("Опрос давления2\r\n");    } 
    pressure2 = (aver_sens());                          // найти текущее давление по среднему арифметическому // сдвинуть массив давлений КРОМЕ ПОСЛЕДНЕЙ ЯЧЕЙКИ на шаг назад
    for (byte i = 0; i < 6; i++) { pressure_array[i] = pressure_array[i + 1]; 
    //if (printCom){Serial.print("дав ");Serial.println(String(pressure_array[i]));}
    }
    pressure_array[5] = pressure2;                    // последний элемент массива теперь - новое давление
    //if (printCom){Serial.print("дав ");Serial.println(String(pressure_array[5]));}

    sumX = 0;
    sumY = 0;
    sumX2 = 0;
    sumXY = 0;
    for (int i = 0; i < 6; i++) {                    // для всех элементов массива
      sumX += time_array[i];
      sumY += (long)pressure_array[i];
      sumX2 += time_array[i] * time_array[i];
      sumXY += (long)time_array[i] * pressure_array[i];
    }
   // if (printCom){Serial.print("sumX="); Serial.println(String(sumX)); };
   // if (printCom){Serial.print("sumY="); Serial.println(String(sumY)); };
  //  if (printCom){Serial.print("sumX2="); Serial.println(String(sumX2)); };
  //  if (printCom){Serial.print("sumXY="); Serial.println(String(sumXY)); };
    
    a = 0;
    a = (long)6 * sumXY;             // расчёт коэффициента наклона приямой
    unsigned long tmp_a=(long)sumX * sumY;
    a = a - tmp_a;
    a = (float)a / (6 * sumX2 - sumX * sumX);
    // Вопрос: зачем столько раз пересчитывать всё отдельными формулами? Почему нельзя считать одной большой?
    // Ответ: а затем, что ардуинка не хочет считать такие большие числа сразу, и обязательно где-то наё*бывается,
    // выдавая огромное число, от которого всё идёт по пи*зде. Почему с матами? потому что устал отлаживать >:O
    delta = a * 6;                   // расчёт изменения давления 

   for (byte i = 0; i < 5; i++) { pressure_del[i] = pressure_del[i + 1];    //в масив кидаем дельту
   //if (printCom){Serial.print("Delta "); Serial.println(String(pressure_del[i])); };
   }
    pressure_del[5] = delta;                    // последний элемент массива теперь - новое давление
    //if (printCom){Serial.print("Delta "); Serial.println(String(pressure_del[5])); };
}

unsigned long aver_sens() {
  pressure2 = 0;
 //for (byte i = 0; i < 10; i++) { if ( sensorPrAl== 3){pressure2 += bmp.readPressure();} else {pressure2 += bme.readPressure();}  }
 for (byte i = 0; i < 10; i++) { pressure2 += (sensorPrAl == 3 ? bmp.readPressure() : bme.readPressure()*100);  }
 
 // for (byte i = 0; i < 10; i++) {    pressure2 += bmp.readPressure();  }
  aver_pressure = pressure2 / 10;
  return aver_pressure;
}
