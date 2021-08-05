
#include <dhtnew.h>

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif


//Provide the token generation process info.
#include "addons/TokenHelper.h"

//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "xxxxxxxxxxxxxxxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxxxxxxxxxxxx"

#define API_KEY "xxxxxxxxxxxxxxxxx"
/* 3. If work with RTDB, define the RTDB URL */
#define DATABASE_URL "xxxxxxxxxxxxxxxxxxxx" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define USER_EMAIL "xxxxxxxxxxxxxxxx@gmail.com"
#define USER_PASSWORD "xxxxxxxxxxxx"

#define DEVICE_ID "dev-1"

#define LOCATION "Living Room"

#define Led_1_Red 25
#define Led_1_Green 26
#define Led_1_Blue 27

#define Led_2_Red 2
#define Led_2_Green 4
#define Led_2_Blue 5


/* 4. Define the Firebase Data object */
FirebaseData fbdo;

/* 5. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 6. Define the FirebaseConfig data for config data */
FirebaseConfig config;

// Json objects containing data
FirebaseJson Tempreature_json;

FirebaseJson Humidity_json;

// Sensor data
float humidity=0.0;
float temperature=0.0;

unsigned long dataMillis = 0;

bool signupOK = false;

// thresholds for temperature & humidity 
//these values represent indoor human confort range
float hum_min_threshhold=30.0;
float hum_max_threshhold=60.0;

float temp_min_threshhold=20.0;
float temp_max_threshhold=30.0;

//notifications concerning sensor values
String SensorState = "OK";
String Warning = "NONE";




// declaring string variables containing the data paths
String path="/Users data/";//global path 
String temp_path="";
String hum_path="";

//
int Sensor_Error_Code=0;

//creating sensor object attached to pin 15
DHTNEW Sensor(15);


void setup()
{   
    Serial.begin(115200);
    //initializations 
  

    Led_Init();
    Sensor_init();
    WiFi_init();
    FireBase_init();
    Json_init();
    
    Serial.println(path);
    Serial.println(temp_path);
    Serial.println(hum_path);
    
    Serial.println(String(millis()));
}
/**********************************************************************************************************/
void loop()
{
    if (millis() - dataMillis > 2000 && Firebase.ready())
    {
        dataMillis = millis();
        Update_Sensor_readings();
        Update_data(); 
        Serial.println("updated reached");       
    }

}

/**************************************************************************************************************/
void WiFi_init()
{
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
   
}
/******************************************************************************************************************/
void FireBase_init(){
Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the API key (required) */
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    /* Assign the RTDB URL */
    config.database_url = DATABASE_URL;

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);

    Serial.print("Signing in ... ");

    /* Sign up */

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    config.max_token_generation_retry = 5;

    Firebase.begin(&config, &auth);
    Serial.println(" firebase begin"); 
    Serial.println( Firebase.ready());
    Serial.println( "WAITING FOR FIREBASE TOKEN GENERATION");
    while (!Firebase.ready()){ Serial.print("..");delay(300);    };


    path += auth.token.uid.c_str(); //<- user uid
    path+="/";

    // determining the paths
    
    path+=LOCATION;
    temp_path=path+"/temperature";
    hum_path=path+"/humidity";
}
/****************************************************************************************************************/
void Sensor_init()
{
    Sensor.setSuppressError(true);//avoiding spikes 

}
/*********************************************************************************************************************/

void Led_Init()
{
  
pinMode(Led_1_Red,OUTPUT);
pinMode(Led_1_Green,OUTPUT);
pinMode(Led_1_Blue,OUTPUT);

pinMode(Led_2_Red,OUTPUT);
pinMode(Led_2_Green,OUTPUT);
pinMode(Led_2_Blue,OUTPUT);
    
}

/*******************************************************************************************************************/

void Update_Sensor_readings()
{
  if (millis() - Sensor.lastRead() > 2000){
Sensor_Error_Code = Sensor.read();
humidity=Sensor.getHumidity();
temperature= Sensor.getTemperature();
Serial.println("/*****/");
Serial.println(Sensor_Error_Code);
Serial.println("/*****/");
Serial.println("temperature : ");
Serial.print(temperature);
Serial.println("/*****/");
Serial.println("humidity : ");
Serial.print(humidity);
Serial.println("/*****/");
  }
  
}

/********************************************************************************************************************/
void Led_Signal(String Led,byte r,byte g,byte b)
{
  if (Led=="Led 1")
  {
    digitalWrite(Led_1_Red ,r);
    digitalWrite(Led_1_Green ,g);
    digitalWrite(Led_1_Blue ,b);
  
  }
  else if (Led=="Led 2")
  {
    digitalWrite(Led_2_Red ,r);
    digitalWrite(Led_2_Green ,g);
    digitalWrite(Led_2_Blue ,b);
    
  }
}
  
/****************************************************************************************************************/
void Json_init()
{
Tempreature_json.add("Device ID",DEVICE_ID);
Tempreature_json.add("Value",temperature);
Tempreature_json.add("Warning","NONE");
Tempreature_json.add("Sensor State","OK");


//Tempreature_json.add("Min Threshhold",temp_min_threshhold);
//Tempreature_json.add("Max Threshhold",temp_max_threshhold);


Humidity_json.add("Device ID",DEVICE_ID);
Humidity_json.add("Value",humidity);
Humidity_json.add("Warning","NONE");
Humidity_json.add("Sensor State","OK");

//Humidity_json.add("Min Threshhold",hum_min_threshhold);
//Humidity_json.add("Max Threshhold",hum_max_threshhold); 
}

void Update_data()
{
  
Tempreature_json.set("Value",temperature);
Humidity_json.set("Value",humidity);


/* the system has 2 rgb leds : one for temperature and one for humidity
 *  determining Led signaling outputs dependq on values read by the sensor and the thresholds
 *  Blue led 1 indicates high levels of humidity 
 *  Red led 1 indicates low levels of humidity 
 *  green led 1 indicates that the level of humidity is within human indoor confort range
 */

if (humidity>hum_max_threshhold)
{
  Humidity_json.set("Warning","HUMIDITY ABOVE 60 !!" );
  Led_Signal("Led 1",0,0,255);  
  }
else if (humidity<hum_min_threshhold){
  Humidity_json.set("Warning","HUMIDITY BELOW 30 !!"  );
  Led_Signal("Led 1",255,0,0);
  }
else {
  Humidity_json.set("Warning","NONE");
  Led_Signal("Led 1",0,255,0);
  }


/*
 *  Blue led 2 indicates low levels of temperature 
 *  Red led 2 indicates high levels of temperature 
 *  green led 1 indicates that the level of temperature is within human indoor confort range
 */
        
if (temperature>temp_max_threshhold)
{
  Tempreature_json.set("Warning","TEMPERATURE ABOVE 30 !!");
  Led_Signal("Led 2",255,0,0);
  }
else if (temperature<temp_min_threshhold)
{
  Tempreature_json.set("Warning","TEMPERATURE BELOW 20 !!");
  Led_Signal("Led 2",0,0,255);
  }
else {
  Tempreature_json.set("Warning","NONE");
  Led_Signal("Led 2",0,255,0);
  }

 switch (Sensor_Error_Code)
  {
    case DHTLIB_OK:
      SensorState="OK";
      break;
    case DHTLIB_ERROR_CHECKSUM:
      SensorState="Checksum error";
      break;
    case DHTLIB_ERROR_TIMEOUT_A:
      SensorState="Time out A error";
      break;
    case DHTLIB_ERROR_TIMEOUT_B:
      SensorState="Time out B error";
      break;
    case DHTLIB_ERROR_TIMEOUT_C:
      SensorState="Time out C error";
      break;
    case DHTLIB_ERROR_TIMEOUT_D:
      SensorState="Time out D error";
      break;
    case DHTLIB_ERROR_SENSOR_NOT_READY:
      SensorState="Sensor not Ready : check sensor connections ";
      break;
    case DHTLIB_ERROR_BIT_SHIFT:
      SensorState="Bit shift error";
      break;
    case DHTLIB_WAITING_FOR_READ:
      SensorState="Waiting for read";
      break;
    default:
      SensorState="Unknown ";
      break;
  }
  
  Tempreature_json.set("Sensor State",SensorState);
  Humidity_json.set("Sensor State",SensorState);


Serial.println("updating");

Firebase.updateNode(fbdo, temp_path , Tempreature_json);
Firebase.updateNode(fbdo, hum_path , Humidity_json);

//if (!Firebase.setJSON(fbdo, hum_path, Humidity_json))Serial.println("no set json");
}



int get_hum_max_thresh(){

  if (Firebase.getInt(fbdo, (hum_path+"/Max Threshhold")) )
  {

    if (fbdo.dataType() == "int") {
     // Serial.println(fbdo.intData());
    //Serial.println(fbdo.dataType());

    }

  } else {
    Serial.println(fbdo.errorReason());
    Serial.println(".................................");

    Serial.println(fbdo.dataType());

  }
}
