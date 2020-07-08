//Iniciliazar bibliotecas necesarias para el proyecto
#include <Adafruit_Sensor.h>    //Sensores de Adafruit
#include <ComponentObject.h>
#include <RangeSensor.h>       
#include <SparkFun_VL53L1X.h>   //Sensor de distancia
#include <vl53l1x_class.h>      //Sensor de distancia
#include <vl53l1_error_codes.h> //Sensor de distancia
#include <Wire.h>               //Protocolos de comunciación serial: I2C
#include "SparkFun_VL53L1X.h"   //Sensor de distancia
#include "RTClib.h"             // Adafruit RTC library
#include <DHT.h>                //Biblioteca sensor de humedad
#include <OneWire.h>            //Sensor de temperatura
#include <DallasTemperature.h>  //Sensor de temperatura


//Constantes y pines
#define DHTPIN 2          //conexión del sensor de humedad
#define DHTTYPE DHT22     // DHT 22  (AM2302)
#define ONE_WIRE_BUS 3    // conexión del sensor de temperatura
#define ID 1              // Número de ID del nodo

//Pines opcionales
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3


//Objetos
// Inicializar los sensores
DHT dht(DHTPIN, DHTTYPE); 
RTC_DS3231 rtc;         
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

SFEVL53L1X distanceSensor;
//Descomentar si se desean usar los pines adicionales
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

// contiene la fecha de cada medida tomada
//fecha = []; 
int Hor = 0;
uint8_t Min;
uint8_t t_muestra;
uint8_t seg;
uint8_t s_muestra;
int Sec = 0;



//variables de medición
int profundidad_media        = 0;
float humedad_media          = 0;
float temperatura_media      = 0;
float profundidad_desviacion = 0;
float humedad_desviacion     = 0;
float temperatura_desviacion = 0;


//variable adicionales
bool  condicion   = false;
int   contador    = 0;
bool  transmision = false;
float trama         [12];
bool inicial      = 0;

void setup(void)
{
  Wire.begin();
  Serial.begin(9600);
  //Inicializar sensores
  dht.begin();
  if (! rtc.begin()) {
    Serial.println("No es posible encontrar RTC");
    while (1);
  }
  if (distanceSensor.begin() == 0) //Inicia en 0 si se inicializó correctamente
  {
    Serial.println("Sensor online!");
  }
}

void limpiar_variables(void){
//variables de medición
  profundidad_media        = 0;
  humedad_media          = 0;
  temperatura_media      = 0;
  profundidad_desviacion = 0;
  humedad_desviacion     = 0;
  temperatura_desviacion = 0;
  inicial =                0;
  //variable adicionales
  condicion   = false;
  contador    = 0;
  transmision = false; 
  }


void loop(void)
{
  int t_muestreo = 0;     //el tiempo de muestreo en minutos
  int t_trans    = 2;    // el tiempo de transmisión en minutos
  int mascara    = 0;     // variable tipo máscara para contener temporalmente los diferentes valores
  uint8_t t_inicial;
  
  
  //Primera iteración, establece parámetros iniciales de tiempo
  // para poder establecer referencia temporal de cada medición
  if (inicial == 0){
    Serial.println("ENTRE...");
    DateTime now = rtc.now();
    Min = now.minute();
    seg = now.second();
    inicial = 1;
    if (condicion == false){
      t_inicial = now.minute();
      condicion = true;
      Serial.print("Tiempo inicial: ");
      Serial.println(t_inicial);
      }
    }

  //Tiempo actual de cada medida
  DateTime now = rtc.now();
  t_muestra = now.minute();
  s_muestra = now.second();
  

  //Si se cumple el tiempo de transmision
  if (((t_muestra - Min) > t_muestreo) && s_muestra == seg){
    
    distanceSensor.startRanging(); //Inicia la medición
    while (!distanceSensor.checkForDataReady()) {
      delay(1);
    }
    //Sensor de profundidad
    mascara                 = distanceSensor.getDistance();
    profundidad_media      = (profundidad_media + (2000 - mascara))/2; //Se obtiene el resultado de la medición
    profundidad_desviacion   += pow(((2000 - mascara) - profundidad_media),2);
    Serial.print("Profundidad: ");
    Serial.println(profundidad_media);
    Serial.println(profundidad_desviacion);
    Serial.println((2000 - mascara) - profundidad_media);
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();

    mascara             = dht.readHumidity();
    humedad_media      += mascara/2;
    humedad_desviacion += pow((mascara - humedad_media),2);
    Serial.print("Humedad: ");
    Serial.println(humedad_media);
 
    sensors.requestTemperatures();
    mascara                 = sensors.getTempCByIndex(0);
    temperatura_media      += mascara/2;
    temperatura_desviacion += pow((mascara - temperatura_media),2);
    Serial.print("Temperatura: "); 
    Serial.println(temperatura_media);

    contador++;
    inicial = 0;
    }  
    
    if (abs(int(now.minute()) - int(t_inicial)) > t_trans){
      Serial.print("WHAT: ");
      
      temperatura_desviacion = sqrt(temperatura_desviacion/contador);
      humedad_desviacion     = sqrt(humedad_desviacion/contador);
      profundidad_desviacion   = sqrt(profundidad_desviacion/contador);
      trama[0]  = now.day();
      Serial.println(trama[0]);
      trama[1]  = now.month();
      Serial.println(trama[1]);
      trama[2]  = now.year();
      Serial.println(trama[2]);
      trama[3]  = now.hour();
      Serial.println(trama[3]);
      trama[4]  = now.minute();
      Serial.println(trama[4]);
      trama[5]  = profundidad_media;
      Serial.println(trama[5]);
      trama[6]  = profundidad_desviacion;
      Serial.println(trama[6]);
      trama[7]  = humedad_media;
      Serial.println(trama[7]);
      trama[8]  = humedad_desviacion;
      Serial.println(trama[8]);
      trama[9]  = temperatura_media;
      Serial.println(trama[9]);
      trama[10] = temperatura_desviacion;
      Serial.println(trama[10]);
      trama[11] = ID;
      Serial.println(trama[11]);
      
      transmision = true;
      limpiar_variables();
      
      }
 
  delay(1000);
}
