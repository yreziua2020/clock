//------------------------------------ говорит время при обновлении времени с веб страици---------------
void bip_vre4()   // проговариваем время с веба одинаковое добавил только проговор минут
{ 
static uint8_t otp_kom; //отправить команду если  ноль то отправляем комаду плееру
uint8_t b = 0;
     
    if(otp_kom==0)   
    {  
        myTimer_pl= millis(); //запускаем отсчет ели вдруг не будет ответа чтобы не стопорить а все обновить
        switch (++caun_zv)    
            {
          case 1:{delay(100);clr();showAnimClock(); refreshAll();command2(15,7,212);break;}
          case 2:{if (one_fl==1||one_f2==1||one_f3==1||one_f4==1 ){caun_zv++;} else {command2(15,7,dayOfWeek+30);  break;} }
          case 3:{if (one_fl==1||one_f2==1||one_f3==1||one_f4==1 ){caun_zv++;} else {command2(15,7,day+40);break;}  }
          case 4:{if (one_fl==1||one_f2==1||one_f3==1||one_f4==1 ){caun_zv++;} else {command2(15,7,month+80); break;}}
          case 5:{command2(15,7,hour); break;}
          case 6:{ if (f_kuku==1 ){caun_zv=0;  f_kuku=0;  break;} else {command2(15,7,minute+100); break;}}  //когда куку не возпроизводило минуты , минуты только при вебе 
          case 7:{caun_zv=0; pr_bip_vre3=0; one_fl=2; one_f2=2; one_f3=2; one_f4=2; break;}
          default:{  break;}
         }
         otp_kom=1; //
     } 
      else
      {
        if (millis() - myTimer_pl >= 3000) {  otp_kom=0;i_bat = 0; } // Serial.println("Привышенно время ожидания ответа для команды"); для того чтобы обнолить если вдруг не прийдет ответ, чтобы следующая команда выполнилась, а также повторно с работала    
      }
        while((Serial.available())&&(i_bat < 20) ) //ждем пока не появляться данные в порте, как только появяться вычитываем 20 байт два ответа по 10
        {   //unsigned long t_tmp = millis()  ;
          delay(5); b=Serial.read(); delay(1); ansbuf[i_bat]=b;  i_bat++; delay(1);   //можно не вычитывать не испльзую нужен фак окончание передачи данных
          if( i_bat == 20) { i_bat = 0;  otp_kom=0;     }   //после получения ответа в 20 байт можно отправлять следующую команду  => otp_kom=0;
          //if (millis() - t_tmp >= 3000) {i_bat = 0;  otp_kom=0; break;} //для того чоб выйти с цикла по привышению времени на всякий случай думаю лишнее 
          //если даные закочаться выйдет, а если даные незакончятся то выйдет по условию что получено 20  байт
        } 
}

//----------------------------------------------------------------------------------------------------------------------------------------
void command2(int8_t cmd, int8_t Para_MSB, int8_t Para_LSB)   //без ответа о файле вроде
{
 // f_otv=1;
    //7EFF060F0007D4EF
  cmdbuf[0] = 0x7e;
  cmdbuf[1] = 0xFF;
  cmdbuf[2] = 0x06;
  cmdbuf[3] = cmd;
  cmdbuf[4] = 0x00;
  cmdbuf[5] = Para_MSB;
  cmdbuf[6] = Para_LSB;
  cmdbuf[7] = 0xef;
  for (uint8_t i = 0; i < 8; i++) {   Serial.write(cmdbuf[i]);   }
}
//-------------------------------------------------------------------------------------
void command_Ot(int8_t cmd, int8_t Para_MSB, int8_t Para_LSB)  //для громкости 
{
  cmdbuf[0] = 0x7e;
  cmdbuf[1] = 0xFF;
  cmdbuf[2] = 0x06;
  cmdbuf[3] = cmd;
  cmdbuf[4] = 0x01;
  cmdbuf[5] = Para_MSB;
  cmdbuf[6] = Para_LSB;
  cmdbuf[7] = 0xef;
  for (uint8_t i = 0; i < 8; i++) {  Serial.write(cmdbuf[i]);   }
 
}

///-------------------------------------------------------- ПРИВЕТ --------------------------------------
void bip_privet()  {
#ifdef _ZVUK  
  int8_t pov_grom;  //для повышения громкости еслине максимум
    ////if (gromk<25 ) {pov_grom=gromk+3;} else pov_grom=30; //если громкость меньше 25 то увеличиваем на 5 если бельше то прото увиличиваем на максимум
     if ((hour>=kuOn  && hour<kuOff)&& dayOfWeek!=7&&dayOfWeek!=1) {command2(Volu,0,gromk);delay(200);command2(Fold,7,216);}   //216if (Wiat_otv(100)) {delay(100);command2(Fold,7,216);} else {command2(Fold,7,218);}
     else if (hour>=kuOn+4 && hour<kuOff){                          command2(Volu,0,gromk);delay(200);command2(Fold,7,216);} //if (Wiat_otv(100)) {delay(100);command2(Fold,7,216);} else {command2(Fold,7,218);}
#endif  
}  
//--------------------------------------------------  Вайфай ------------------------------------------
void bip_Wi_Fi() { 
#ifdef _ZVUK  
     if ((hour>=kuOn  && hour<kuOff)&& dayOfWeek!=7&&dayOfWeek!=1)    
     {command_Ot(Volu,0,gromk);delay(200);command2(Fold,7,210); } //20wait_play();if (Wiat_otv(100)) {delay(100);command2(Fold,7,210);} else {command2(Fold,7,218);}
     else 
     if (hour>=kuOn+4 && hour<kuOff){
     command_Ot(Volu,0,gromk);delay(200);command2(Fold,7,210);}     //20wait_play();if (Wiat_otv(100)) {delay(100);command2(Fold,7,210);} else {command2(Fold,7,218);}
#endif  
 }  //звук аськи при соединии с файфаем
//---------------------------------------------------------------------------------------------------------
void bip2()  {     //при внисения изминеий на странице
#ifdef _ZVUK   
  command2(Volu,0,gromk);delay(100); command2(Fold,7,218);  
#endif  
} 
void bip_restart() {  //211- типа завершения
#ifdef _ZVUK
  printStringWithShift(" R_Web", 25); command2(Volu,0,gromk);delay(200);command2(Fold,7,211);delay(3400);
#endif
}  
void bip_restart_up(){ 
#ifdef _ZVUK  
  printStringWithShift(" R_Upd", 25); command2(Volu,0,gromk);delay(200);command2(Fold,7,219);
#endif  
}                                    //тмпа зарчдки оружия
void bip_restart2()  { 
#ifdef _ZVUK   
  printStringWithShift("Resta", 15);  command2(Volu,0,gromk);delay(200);command2(Fold,7,218);delay(3400);
#endif
}  //199 -зарядки оружея/211- типа типа пилинг подводной лодки

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//-----------старое только вроде----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------праздники напоминаия---------------------------------------------
void bip_prazn()  {   command2(Volu,0,gromk); delay(100); command2(Fold,7,198);  delay(500);}   //11
//--------------------------------будильник
void bip_bud_vs()  //Вызываеться один раз когда сбрасываються все флаги будильника
{     unsigned long t_tmp = millis() ;  
         
     delay(100); command2(ADVE,0,hour);  //проиграть файль из рекламы
     t_tmp = millis() ;
     while(!Serial.available()) 
      {  //пока нет даных ждеи,но если привышено будет время ожидание то прекращаем цикл
        yield(); //использую чтобы сбросить сторожевой таймер в цикличиских выражениях, а работает в паре с delay, в ней пожно вызывать обработки кнопопок пока работает  delay
        if (millis() - t_tmp >= 8000) { break;} //для того чоб выйти с цикла по привышению времени на всякий случай
       }
     i_bat = 0;
     while((Serial.available())&&(i_bat < 20) ) {delay(1); //также сбрасывает сторожевой таймер
      //b=Serial.read(); delay(1); ansbuf[i_bat]=b;  
      i_bat++; delay(1); 
      if( i_bat == 20) { break;  }
      }//если даные закочаться выйдет, а если даные незакончятся то выйдет по условию что получено 20  байт
    Serial.println("первая фраза");
    
     delay(1500); command2(ADVE,0,minute+100);  //проиграть файль из рекламы
     t_tmp = millis() ;
     while(!Serial.available()) 
      {  //пока нет даных ждеи,но если привышено будет время ожидание то прекращаем цикл
        yield(); //использую чтобы сбросить сторожевой таймер в цикличиских выражениях, а работает в паре с delay, в ней пожно вызывать обработки кнопопок пока работает  delay
        if (millis() - t_tmp >= 8000) { break;} //для того чоб выйти с цикла по привышению времени на всякий случай
       }
     i_bat = 0;
     while((Serial.available())&&(i_bat < 20) ) {delay(1); //также сбрасывает сторожевой таймер
      b=Serial.read(); delay(1); ansbuf[i_bat]=b;  
      i_bat++; delay(1); 
      if( i_bat == 20) { break;  }
      }//если даные закочаться выйдет, а если даные незакончятся то выйдет по условию что получено 20  байт
 
}
//----------------------------------------------------------------------------------------------------------------
void bip_budil()
 { 
#ifdef _ZVUK
 
      if(fl_bud_mp==0)      //чтобы сработола только один раз   обнулим при обнулении alarm_stat
      {   
          //command2(Volu,0,gromk);delay(200);
         if  ((gromk-15) >0) voll=(gromk-15); else  voll=10;
                       command2(Volu,0,voll);delay(200);
          //mp3_play_file_in_fol (7,trek);  
            trek= random(222,242);  //случайное  число в таких приделах
          command2(Fold,7,trek);  //проиграть файль trek из папки 7 
         // delay(100);    //8
         fl_bud_mp=1; 
         //if (trek++>242) {trek=222;}  

      } else  {   if (voll++<gromk) {delay(100); command2(Volu,0,voll); } }  //использую else чтобы в первый раз не менять громкость а только включить трек //для плаввнго включения громкости //17
#endif 
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

 
