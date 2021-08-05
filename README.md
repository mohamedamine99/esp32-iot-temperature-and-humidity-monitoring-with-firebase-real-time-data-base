# esp32-iot-temperature-and-humidity-monitoring-with-firebase-real-time-data-base


## 1- About the project

ESP32 is a powerful hardware platform for IoT applications and is widely used for prototyping and development of IoT applications.

This prject will demonstrate the fastest method to connect your ESP32 to Google’s Firebase backend. Firebase has become a very convenient option for developers to rapidly prototype their ideas, in this tutorial we lay the ground work for the development of a realtime temperature and humidity monitoring solution. 

This tutorial further demostrates how to interface the DHT11 temperature and humidity sensor with the ESP32 using Arduino IDE and how to upload sensor data on google Firebase's  realtime database . 



## Table of contents

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#Hardware-Requirements">Hardware-Requirements</a></li>
        <li><a href="#Software-Requirements">Software-Requirements</a></li>
         <li><a href="#Backend-Setup">Backend-Setup</a></li>
      </ul>
    </li>
    <li><a href="#Hardware-implementation">Hardware-implementation</a></li>
         <ul>
        <li><a href="#DHT11-Sensor">DHT11-Sensor</a></li>
        <li><a href="#RGB-LEDs">RGB-LEDs</a></li>
      </ul>
    </li>
    <li><a href="#Software-implementation">Software-implementation</a></li>
    <li><a href="#Results">Results</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



## 2- Getting Started

### 2.1 Hardware Requirements:
* ESP32 development board (any ESP32 board is okay, for this project we will be using the ESP32 DevKit v1).
* DHT11 temperature and humidity sensor or sensor module .
* x2 RGB LEDs
* x6 390 ohm resistors
* one 10K resistor (in case you have the sensorwithout the module since the module has a built-in resistor) 
* Breadboard
* Wires

### 2.2 Software Requirements:
* Arduino IDE (If you haven't already installed or configured it using esp32, check this awesome [tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) by Rui Santos)
* DHT11 sensor library : DHTNEW for arduino by RobTillaart , you can find it [here](https://github.com/RobTillaart/DHTNEW)
* Wifi library (should be installed by default)
* Firebase esp library : Firebase Realtime Database Arduino Library for ESP32 by Mobizt, you can find it [here](https://github.com/mobizt/Firebase-ESP32)



### 2.3 Backend Setup:
* In order to use Google's Firebase you need a Google account.
* to get started head over to Firebase, and sign in using your Google account. 
    
To start a new project, select the **Add project option**. Enter your project name and click the blue **Continue button**. I’ve called my project ***ESP32 dht11***.

![image](https://user-images.githubusercontent.com/86969450/128199695-032f0780-9eb1-434f-ba12-ff788f63cdd1.png)
  
You can **disable the Enable Google Analytics** for this project option since it will not be needed this for this project. Finally, select **Create project**.

![image](https://user-images.githubusercontent.com/86969450/128200652-9f3f4287-25e9-4d89-b5fc-1fde095e33f6.png)
  
 Firebase will begin setting up your project. Once completed select **Continue**, and you will be taken to your projects overview page on the Firebase console.

![image](https://user-images.githubusercontent.com/86969450/128201585-a1c022bd-b68b-4d10-9796-814dce610e97.png)
    

First we need to setup authentification methods for the project.    
Select the **Authentication** menu option on the top left, and you will be taken to the authentication page. Select the **Get Started** button.
    
![image](https://user-images.githubusercontent.com/86969450/128202578-e31509fa-6bb9-42ac-bc99-0010971a75d9.png)
    
 ![image](https://user-images.githubusercontent.com/86969450/128202995-bea71292-332a-4df8-8d5b-b0648c30414a.png)
    
 **Enable** both **Anonymous** Sign In and **Email/Password** as shown above and select the **Save** option.
 In fact , we’re going to start by using the Anonymous sign in for our ESP32 devices, and later on in the tutorial we will devise a more advanced sign in method via   Email/Password .
 
 The next step is to establish a database to store all of our sensor data. To do so, go to the **Realtime Database** menu option on the top left.   
 this will take you to the Realtime Database page. Select the **Create Database** button to initialise the database creation menu.
 ![image](https://user-images.githubusercontent.com/86969450/128204621-3323434c-87a2-4499-b1d7-9040a2938e94.png)

 ![image](https://user-images.githubusercontent.com/86969450/128208145-e484b28f-c19e-482f-998b-21fb7205f0d0.png)

In the database creation menu you can choose a location that is closest to you, and select **Next**.    
You will be presented with the option to initialise your database in locked mode or test mode. Select **test mode** for now.
    
The main difference is that in test mode a database access rule is placed allowing unauthorised access to your database for thirty days. If you plan to take your project into production, you will need to disable this in the future. Select the **Enable button**.
    
![image](https://user-images.githubusercontent.com/86969450/128208581-d35ed4c2-b057-4097-b7f2-f330be4c4dba.png)
    
 Finally, a new page with your new empty database should appear, now we're all set !
    
 ![image](https://user-images.githubusercontent.com/86969450/128210675-e9d4ae16-d796-4f11-801e-2c8261d21e3a.png)
    
Before we continue, there are two elements you should copy and save in our embedded application for future use. 
The first thing we'll need is our Realtime Database URL, which you can get by copying the URL of the Realtime Database page (in our case, it'll be https://esp32-dht11-fbf8d-default-rtdb.firebaseio.com/).
    
![image](https://user-images.githubusercontent.com/86969450/128211096-600f3a0c-725d-44c7-be46-15f9b6f4893a.png)
    
The second item we need to store is the project’s API key.\
To acquire your API key, go to the project settings page by clicking the settings icon on the top right and then Project settings from the menu, as shown below:
    
![image](https://user-images.githubusercontent.com/86969450/128211721-b732ab04-8b8d-4de1-b986-0b706d82b58e.png)

![image](https://user-images.githubusercontent.com/86969450/128211611-800f7843-0806-43fc-ad20-35e5d12ea8ab.png)

**Now we're all ready to start working on our embedded application !**

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 3- the Hardware:
### Understanding the hardware :
Before building our circuit we need to understand the hardware we're using :

### 3-1 the DHT11 humidity and temperature sensor :
![image](https://user-images.githubusercontent.com/86969450/128260693-fb78d2f9-0f95-48f1-9bd8-9bf41570ac61.png)

The DHT sensors are made of two parts, a capacitive humidity sensor and a thermistor. There is also a very basic chip inside that does some analog to digital conversion and spits out a digital signal with the temperature and humidity. The digital signal is fairly easy to read using any microcontroller.

#### DHT11 Pinout Identification and Configuration:
 
![image](https://user-images.githubusercontent.com/86969450/128260826-39c05af8-e2ee-4c76-b41a-012059d6e8c5.png)

**NB : In our case we'll be using the DHT11 sensor module**

#### Technical details:

* Operating Voltage: 3.5V to 5.5V
* Operating current: 0.3mA (measuring) 60uA (standby)
* Output: Serial data
* Temperature Range: 0°C to 50°C
* Humidity Range: 20% to 90%
* Resolution: Temperature and Humidity both are 16-bit
* Accuracy: ±1°C and ±1%
  
### 3-2 the RGB LEDs :

 ![image](https://user-images.githubusercontent.com/86969450/128232493-7d108dc6-889b-4f1e-8cc7-02b301025d9e.png)
An RGB LED is basically an LED package that can produce almost any color. It can be used in different applications such as outdoor decoration lighting, stage lighting designs, home decoration lighting, LED matrix display, and more.

RGB LEDs are made up of three internal LEDs (Red, Green, and Blue) that may be combined to produce nearly any color. To produce different colors, we must adjust the brightness of each internal LED and combine the three color outputs.
We'll use PWM to adjust the intensity of the red, green, and blue LEDs individually, and the key is that because the LEDs are all so close together inside, our eyes will see the mixture of colors rather than the individual ones.

**NB: the LEDs used in our project are common Cathode**

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 4- Software implementation:
**NB:   
1- you can test your sensor individually via the sensor library link provided [here](https://github.com/RobTillaart/DHTNEW) you can find an implementation example [here](https://github.com/RobTillaart/DHTNew/blob/master/examples/dhtnew_suppressError/dhtnew_suppressError.ino)**\
**2-you can test your Firebase individually via the library link provided [here](https://github.com/mobizt/Firebase-ESP32) you can find an implementation example of anonymous authentification [here](https://github.com/mobizt/Firebase-ESP32/blob/master/examples/Authentications/SignInAsGuest/AnonymousSignin/AnonymousSignin.ino)**  


* Now let's begin with an anonymous authentification code where we upload the data read from the sensor to our database  :
note that this example will create a new anonymous user with  different UID (user ID) every time you run this example.

 First things first , we should begin by including all the libraries we're intending to use :
 
   ```
#include <dhtnew.h> // humidity and temperature sensor library

/* the below if defined and endif bloc will detect the current esp board (32 or 8266) and include the adequate libraries 
if it detects esp32 the it will includeesp32 librairies else it will include esp8266 libraries*/

#if defined(ESP32)
#include <WiFi.h> //wifi library 
#include <FirebaseESP32.h> //firebase for ESP32 library
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info this will mostly help us debug the progress and the state of token generation 
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions. 
#include "addons/RTDBHelper.h"

  ```
 Now we need to capture our WiFi credentials. Replace `WIFI_ID` with your WiFi Identifier, and `WIFI_PASSWORD` with your WiFi Password.
    
**Note: It’s never a good idea to hardcode password information in your embedded application, for production cases you need to apply a device provision strategy that includes a secure device registration process.**

 ```
 /* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_ID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
 ```
Next, we'll create a constant to store our API key; as previously said, you can retrieve your Firebase project API key from the projects settings page.     
`API_KEY` should be replaced with your API key. Replace `URL` with your own Firebase Realtime Database URL.

 ```
#define API_KEY "API_KEY"

/* 3. If work with RTDB, define the RTDB URL */

#define DATABASE_URL "URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
 ```
 We need to define a unique device ID which can used to differentiate data coming from multiple sensors.
 the same should be applied for the Locations
  ```
#define DEVICE_ID "dev-1"

#define LOCATION "Living Room"
 ```
Now we define our 2 rgb LEDs pins
    
 ```
 // Led 1 Pins
#define Led_1_Red 25
#define Led_1_Green 26
#define Led_1_Blue 27

// Led 2 Pins
#define Led_2_Red 2
#define Led_2_Green 4
#define Led_2_Blue 5
 ```
Each RGB LED has 4 pins : one for gnd and 3 pins as input to determine the output color


   
 Next we initialise 3 objects courtesy of the `FirebaseESP32` library which will be critical to linking our application to Firebase.

 ```
 /* 4. Define the Firebase Data object */
FirebaseData fbdo;

/* 5. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 6. Define the FirebaseConfig data for config data */
FirebaseConfig config;
 ``` 
Following that, we define a few global variables that will be useful.

 ``` 
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
int Sensor_Error_Code=0;
 ```
 We also need to create two Firebase Json objects that will hold our sensor data .
 ```
// Json objects containing data
FirebaseJson Tempreature_json;

FirebaseJson Humidity_json;
 ```
 We also need to declare string variables indicating the paths to our json objects containg the sensor data.
 ```
 // declaring string variables containing the data paths
String path="";//base path 
String temp_path="";
String hum_path="";
```
  Finally let's not forget to create a DHT11 sensor object and attach it to an analog pin (15 in our case).
 ```
//creating sensor object attached to pin 15
DHTNEW Sensor(15);
```
Now let's take a look at our setup function :
```
void setup()
{ 
  //Starting serial communication with our ESP32 board
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
```
As you can see First we need to initailize and establish serial communication with our ESP32 board using `Serial.begin(115200)`.  
note that we have used multiple functions In order to initialize our hardware.    
the first function is `Led_init()` which initializes our RGB LEDs as outputs and attaches them to their respective pins.
```
void Led_Init()
{
  
pinMode(Led_1_Red,OUTPUT);
pinMode(Led_1_Green,OUTPUT);
pinMode(Led_1_Blue,OUTPUT);

pinMode(Led_2_Red,OUTPUT);
pinMode(Led_2_Green,OUTPUT);
pinMode(Led_2_Blue,OUTPUT);
    
}
```
The next step is to setup a connection between our board and the wifi network.In order to do that we daclare a function named `WiFi_init()`.  
We make use of the built in WiFi API provided by the Arduino framework.  
We use the `WiFi.begin(WIFI_SSID, WIFI_PASSWORD)`  function to initialise a WiFi connection using our credentials `WIFI_SSID` and `WIFI_PASSWORD`.  
After that check every 300 ms to see if the connection has been successfully established using the `WiFi.status()` function in this case we pirnt the local IP adress using
`WiFi.localIP()`.  
For more detailed informations about WiFi API you might want to check out the [Arduino WiFi API documentation](https://www.arduino.cc/en/Reference/WiFi).

```
void WiFi_init()
{

  //attempting to connect with the wifi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
    Serial.print("Connecting to Wi-Fi");

    //while not connected 
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    
    //Print the local IP adress if connection is successfully established
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
   
}
```
Next, we'll create a Firebase initialization function that connects to the Firebase backend, authenticates our device, and sets up our Firebase library.  
We start by passing our API key and database URL to the configuration object, along with enabling WiFi reconnection on upon setup.  
We then attempt to sign up anonymously as a new user using the function `Firebase.signUp(&config, &auth, "", "")`.  
In order to accomplish anonymous authentification we pass to the function empty mail and password.
After signing up sucessfully we initialze our Firebase library using `Firebase.begin(&config, &auth)`.  
Now we need to determine the **paths** to where to upload our data :
* the base path `path` includes the user ID generated after a successfull sign up ,we use this instruction ` auth.token.uid.c_str()` in order to get the UID.
* temperature path which contains the json objects that holds sensor tempertaure data `temp_path` .
* humidity path which contains the json objects that holds sensor humidity data `hum_path` .  

**For more details you can check the Library link and documentation [here](https://github.com/mobizt/Firebase-ESP32) you can find an implementation example of anonymous authentification [here](https://github.com/mobizt/Firebase-ESP32/blob/master/examples/Authentications/SignInAsGuest/AnonymousSignin/AnonymousSignin.ino)**  

```
void FireBase_init(){
Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the API key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL */
    config.database_url = DATABASE_URL;

    Firebase.reconnectWiFi(true);
    Serial.print("Sign up new user... ");

    /* Sign up */
    if (Firebase.signUp(&config, &auth, "", ""))
    {
        Serial.println("ok");
        signupOK = true;

    }
    else
        Serial.printf("%s\n", config.signer.signupError.message.c_str());

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    Firebase.begin(&config, &auth);
    path = auth.token.uid.c_str(); //<- user uid
    path+="/";

    // determining the paths
    
    path+=LOCATION;
    temp_path=path+"/temperature";
    hum_path=path+"/humidity";
}
```
The next function is `Json_init()` which initializes our json objects keys and values and assigns them to thier respectives paths
```
void Json_init()
{
Tempreature_json.add("Device ID",DEVICE_ID);
Tempreature_json.add("Value",temperature);
Tempreature_json.add("Warning","NONE");
Tempreature_json.add("Sensor State","OK");

Humidity_json.add("Device ID",DEVICE_ID);
Humidity_json.add("Value",humidity);
Humidity_json.add("Warning","NONE");
Humidity_json.add("Sensor State","OK");
}

```
and finally we initialize our sensor. Sometimes the DHT11 sensors fails to read values correctly and as a result it returns -999.0 so in order to avoid such inconveniences the functions we tend to use the `setSuppressError(true)` function: its main role is to replace the -999.0 error value by the latest value read by the sensor.
  
**For more detailed informations you can check the sensor library link provided [here](https://github.com/RobTillaart/DHTNEW) you can find an implementation example [here](https://github.com/RobTillaart/DHTNew/blob/master/examples/dhtnew_suppressError/dhtnew_suppressError.ino)**

```
void Sensor_init()
{
    Sensor.setSuppressError(true);// avoiding spikes and error values for more detailed info check this  

}

```

Now that we have completed the setup functions, lets get to the loop function.  
As we can see we we use the `if` statement to check whether we are well connected and signed up and to keep our functions running at a constant rate, in our case very 2 seconds.  
We used `Update_Sensor_readings()` function in order to read sensor values and  `Update_data()` in order to upload them to our Firebase realtime database.

```
void loop()
{
    if (millis() - dataMillis > 2000 && signupOK && Firebase.ready())
    {
        dataMillis = millis();
        Update_Sensor_readings();
        Update_data(); 
    }

}
```
Now let's dive into our `Update_Sensor_readings()` function.  
this functions updates error_code and sensor values that will later be uploaded to our database.

```
void Update_Sensor_readings()
{
 // if (millis() - Sensor.lastRead() > 2000){
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
```
Now the `Led_Signal()` function:
```
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
  
```
Now let's explore the `Update_data()` function that will update our data on the Firebase realtime database.
we begin by setting the json values of temperature and humidity acquired by our sensor, then we compare them with the threshhold , set warning strings and signal diodes accordingly using the `if` statements.  

the system has 2 rgb leds : one for temperature and one for humidity
 *  determining Led signaling outputs dependq on values read by the sensor and the thresholds
 *  Blue led 1 indicates high levels of humidity 
 *  Red led 1 indicates low levels of humidity 
 *  green led 1 indicates that the level of humidity is within human indoor confort range
 *  Blue led 2 indicates low levels of temperature 
 *  Red led 2 indicates high levels of temperature 
 *  green led 2 indicates that the level of temperature is within human indoor confort range

We then set the sensor state value using the `switch`, `case` statement.
Finally we upload our data to the database using `Firebase.updateNode()` .
```
void Update_data()
{

  // setting the values of temperature and humidity
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
}
```

---------------------------------------------------------------------------------------------------------------------------------------------------
### 5- Executing program

![image](https://user-images.githubusercontent.com/86969450/128270751-faebf612-2e97-496e-97e4-26935ed65734.png)
  
![image](https://user-images.githubusercontent.com/86969450/128270765-7b770f5d-9586-4d57-836d-7cde2104b34d.png)
  
![image](https://user-images.githubusercontent.com/86969450/128270796-33b1d534-90a0-4a03-b039-fd7f2855bb86.png)
  
![image](https://user-images.githubusercontent.com/86969450/128270911-228a45fa-8b23-4afe-9a9e-a36505edee9d.png)

  




