# esp32-iot-temperature-and-humidity-monitoring-with-firebase-real-time-data-base
------------------------------------------------------------------------


ESP32 is a powerful hardware platform for IoT applications and is widely used for prototyping and development of IoT applications.

This prject will demonstrate the fastest method to connect your ESP32 to Google’s Firebase backend. Firebase has become a very convenient option for developers to rapidly prototype their ideas, in this tutorial we lay the ground work for the development of a realtime temperature and humidity monitoring solution. 

This tutorial further demostrates how to interface the DHT11 temperature and humidity sensor with the ESP32 using Arduino IDE and how to upload sensor data on google Firebase's  realtime database . 

------------------------------------------------------------------------

## Table of contents
------------------------------------------------------------------------

## Getting Started

### Hardware Requirements:
* ESP32 development board (any ESP32 board is okay, for this project we will be using the ESP32 DevKit v1).
* DHT11 temperature and humidity sensor.
* x2 RGB LEDs
* x6 390 ohm resistors
* one 10K resistor
* Breadboard
* Wires

### Software Requirements:
* Arduino IDE (If you haven't already installed or configured it using esp32, check this awesome [tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) by Rui Santos)
* DHT11 sensor library : DHTNEW for arduino by RobTillaart , you can find it [here](https://github.com/RobTillaart/DHTNEW)
* Wifi library (should be installed by default)
* Firebase esp library : Firebase Realtime Database Arduino Library for ESP32 by Mobizt, you can find it [here](https://github.com/mobizt/Firebase-ESP32)



### Backend Setup:
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

## Software implementation:
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

