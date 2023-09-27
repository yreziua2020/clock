#define numElectrodes 4
  
  void MPR121_inic() 
  {
 // MPR121.setTouchThreshold(100); //уровень нажатя  //было 40
  //MPR121.setReleaseThreshold(20); //уровень отпускания
 // MPR121.updateTouchData(); //читаем начальное значениеж
 // MPR121.setNumDigPins(4);  //колличество пинов для выхода и идет подсчет с зади в роде 
 // MPR121.pinMode(11, OUTPUT);
//  MPR121.pinMode(10, OUTPUT);
 // MPR121.pinMode(9, OUTPUT);
 // MPR121.pinMode(8, OUTPUT);
  }

void MPR121_update()
{
    
//MPR121.updateTouchData();
  
  // if (MPR121.isNewTouch(0))  {MPR121.digitalWrite(11, 1); delay(100); }
  // if (MPR121.isNewRelease(0)){MPR121.digitalWrite(11, 0); delay(100);}
   
 //   if (MPR121.isNewTouch(1))  {MPR121.digitalWrite(10, 1); delay(100);  if (hour > 5 && hour != 23  ) { mp3_play_file_in_fol (12,random(100));} } //mp3_play_fol_ran (10);
  //  if (MPR121.isNewRelease(1)){MPR121.digitalWrite(10, 0); delay(100);}
   
//   if (MPR121.isNewTouch(2))  {MPR121.digitalWrite(9, 1); delay(100);pr_bip_vre3=1;   }   // mp3_volume_dec(); mp3_volume_dec();
//  if (MPR121.isNewRelease(2)){MPR121.digitalWrite(9, 0); delay(100);}
   
 //    if (MPR121.isNewTouch(3))   {MPR121.digitalWrite(8, 1); if (hour > 5 && hour != 23  ) {if (f_mp3) {f_mp3=0;mp3_stop (); } else f_mp3=1; } }   //mp3_volume_inc();delay(100);
  //   if (MPR121.isNewRelease(3)) {MPR121.digitalWrite(8, 0); delay(100);}
   
//   if (MPR121.isNewTouch(4))  {mp3_play_fol_ran (4);}
    //  if          (MPR121.isNewTouch(i))  {if (i == 0) { bip();  }
     // } else if (MPR121.isNewRelease(i)) {if (i == 1) { bip(); }
    //  }
    
   // }
}
