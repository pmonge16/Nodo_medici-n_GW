#include <Wire.h>
#include "RTClib.h"  //Biblioteca RTC
#include <DHT.h>;    //Biblioteca sensor de humedad
#include <Adafruit_Sensor.h>


//Constantes
#define DHTPIN 2          //conexión del sensor de humedad
#define DHTTYPE DHT22     // DHT 22  (AM2302)


// Inicializar los sensores
DHT dht(DHTPIN, DHTTYPE); 
RTC_DS3231 rtc;          


// contiene la fecha de cada medida tomada
fecha = []; 
int hora = 0;

//trama final
trama = [];

//variables de medición
int profundidad_media        = 0;
float humedad_media          = 0;
float temperatura_media      = 0;
float profundidad_desviacion = 0;
float humedad_desviacion     = 0;
float temperatura_desviacion = 0;


//variable adicionales
bool error   = false;
int contador = 0;

void setup () 
{
  Serial.begin(9600);
  delay(3000); // esperar a la consola para comenzar

  //inicializar el sensor de humedad
  dth.begin();

  if (! rtc.begin()) {
    error = true;
    while (1);
  }
  
  if (rtc.lostPower()) {
    //Problemas de alimentación, se procede a ajustar el tiempo  
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Las siguiente líneas ajustan el tiempo y fecha específicos
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
}

void loop () 
{
    if (contador == 0){
      DateTime now = rtc.now();
      
      }

    if (now.unixtime() == 30){
      
      contador == 0
      }
    Serial.print(now.unixtime());
    Serial.print(" seconds/");
    Serial.print(now.unixtime() / 86400L);
    Serial.println(" days since 1/1/1970");
    
   //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    
    Serial.println();
    delay(1000);
}
