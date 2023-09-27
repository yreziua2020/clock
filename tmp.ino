
//void bip3()       {   if (digitalRead(MP3_PIN)==1) {mp3_play_file_in_fol (2,12);}}  //для громкости увиличение уменьшение
//void bip4()       {   if (digitalRead(MP3_PIN)==1) {mp3_play_file_in_fol (2,3);} }  //естли играет смкелодия то звук ненужен
//void bip()        {  mp3_set_volume (gromk);delay(100); mp3_play_file_in_fol (11,3);}   //201
//-------------------bbbbbbb--------------------------------------------------------------------------
//void bip()          {   command2(Fold,11,13); }   //201  двойное нажатие нопки

//void vver(){delay(1000);  command(15,6,6);wait_play(); }
/*

//------------------------------------ говорит время при обновлении времени с веб страици---------------

void bip_v7re4()   // проговариваем время с веба
{ uint8_t wr_tmp;
  if(Serial.available()||(st_pler==0))
  { wr_tmp=answer(100);
    if ( (st_pler==0)||(wr_tmp==1))
    { 
      delay(100);
      st_pler=1;    //нужно для того чтоб включить первую мелодию не дожидаясь ответа
        switch (++caun_zv)    
            {
            case 1:{command2(15,7,212);delay(500); break;}
          //case 2:{if(wr_tmp==1) {command2(15,7,212); delay(100);}else {command2(15,7,dayOfWeek+30); delay(100);} break;}
            case 2:{if(wr_tmp==1) {command2(15,7,212);delay(500);}else {command2(15,7,dayOfWeek+30); delay(100);} break;}
            //case 2:{if(wr_tmp==1) {command2(15,7,dayOfWeek+30);delay(100);}else {command2(15,7,dayOfWeek+30); delay(100);} break;}
            case 3:{command2(15,7,day+40); delay(100); break;} 
            case 4:{command2(15,7,month+80); delay(100); break;}
            case 5:{command2(15,7,hour); delay(100);  break;}
            case 6:{command2(15,7,minute+100);delay(100);  break;}
            case 7:{caun_zv=0; pr_bip_vre3=0;st_pler=0;delay(100); break;}
            default:{  break;}
         
            }
    }
    
 }

}


 p1 = p0 / 10000;
    p2 = (p0 - p1 * 10000) / 1000;
    p3 = (p0 - p1 * 10000 - p2 * 1000) / 100;
    p4 = (p0 - p1 * 10000 - p2 * 1000 - p3 * 100) / 10;
    p5 = (p0 - p1 * 10000 - p2 * 1000 - p3 * 100 - p4 * 10);
  

  //p0 = p0/100;  //я добавил 
   p2 = (p0 - p1 * 1000) / 100;
  p3 = (p0 - p1 * 1000 - p2 * 100) / 10;
  p4 = p0 % 10;


*/










/*void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
*/
