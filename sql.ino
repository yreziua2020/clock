void bd_sql() {
 
if (!ESPclient.connect(DB_host,Port)) {
    //Serial.print(DB_host);
    //Serial.println(" - Connection failed.");
    ESPclient.stop();
  }
    if (ESPclient.connect(DB_host, Port))
    {
    //Serial.println("database connection established");  
    ESPclient.print( "GET /php/wp-scale.php?A=Vp3wDWbP&ID=");
    ESPclient.print(UserID);
    ESPclient.print("&T1=");
    ESPclient.print(tempDs18b20); //data
    ESPclient.print("&T2=");
    ESPclient.print(pressBmp);
    ESPclient.print("&T3=");
    ESPclient.print("0");
    ESPclient.print("&T4=");
    ESPclient.print("0");
    ESPclient.print("&T5=");
    //TempDs18[4].tDs=0;
    ESPclient.print("0");
    ESPclient.print("&P=");
    ESPclient.print(database_pass);
    ESPclient.println( " HTTP/1.1");
    ESPclient.print( "Host:" );
    ESPclient.println(DB_host);
    ESPclient.println( "Connection: close" );
    ESPclient.println();
    ESPclient.println();
    ESPclient.stop();
    
    }

 
  
}
