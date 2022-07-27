# Penulisan-Ilmiah
"Rancang Bangun Alat Monitoring Padatan Terlarut, Suhu dan Pengontrol Level Air Otomatis Kolam Koi Berbasis IoT"

# Blok Diagram
![image](https://user-images.githubusercontent.com/91959410/181244019-8e55b793-d27c-487a-b214-bf2febac265d.png)

# Skematik
![image](https://user-images.githubusercontent.com/91959410/181244183-1f53ff97-ad2f-4214-9c49-b07e70d7d201.png)

# Dokumentasi
#### Wemos D1 R32 Pinout
![wemos-d1-r32-wifi-bluetooth-uno-esp32-432307](https://user-images.githubusercontent.com/91959410/181250512-25018773-5e45-4b36-af2f-0615a91dadf8.jpg)

#### Sensor TDS


    void TDSnTemp() 
    {  

    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    Serial.print("Temperature is: "); 
    Serial.println(temp);
    gravityTds.setTemperature(temp); 
    gravityTds.update();  
    tdsValue = gravityTds.getTdsValue();  
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, tdsValue);
