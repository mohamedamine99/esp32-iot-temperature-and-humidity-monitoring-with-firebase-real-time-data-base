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

 


