## About The Multi-threading solution:
The existence of **two cores** on the ESP32 chip is a crucial characteristic that distinguishes it from its predecessor, the ESP8266. This means that **two processes can run concurrently on two separate cores**. Of course, you might argue that with FreeRTOS/any other analogous RTOS, parallel operation can be accomplished on a single thread.
In our previous code we were able to successfully collect data from our DHT11 sensor and then upload it onto our firebase realtime database. However, we can easily notice that the data displayed in our database lags behind the data collected by the sensor with a certain delay .  As result it would be more than convenient to make use of multi-core architecture to run the data collection and the data upload processes concurrently in order to significantly reduce that delay.

##  The dual core architecture of the ESP32 :
The ESP32 comes with 2 Xtensa 32-bit LX6 microprocessors, so it’s dual core:

* Core 0
* Core 1

![image](https://user-images.githubusercontent.com/86969450/132525666-822278f6-c8ac-4742-a3b0-b1230b8233d8.png)
  
  
* We should note that both the `setup()` and `loop()` are running on core 1. We can test that with the `xPortGetCoreID()` function & uploading the following sketch to the ESP32.

  ```cpp
  void setup() {
  Serial.begin(115200);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
  }

  void loop() {
  Serial.print("loop() running on core ");
  Serial.println(xPortGetCoreID());
  }
  ```
 * Now we should get these results:

![image](https://user-images.githubusercontent.com/86969450/132528907-e2c6b966-9133-40c4-bc86-df8a13389ad0.png)

##  Software implementation :


