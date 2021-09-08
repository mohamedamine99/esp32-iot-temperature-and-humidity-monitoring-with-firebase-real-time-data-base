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

The Arduino IDE supports FreeRTOS for the ESP32, which is a Real Time Operating system. This allows us to handle several **tasks** in parallel that run independently.

###  Understanding tasks:

* Tasks are snippets of code that carry out a certain action. Blinking an LED, sending a network request, measuring sensor readings, publishing sensor readings,etc...  
* Tasks are used to assign certain pieces of code to a specific core.  
* When creating a task you can chose in which core it will run, as well as its priority.
* Priority values start at 0, in which 0 is the lowest priority.
* The tasks with the highest priority will be executed first by the processor.

###  Creating tasks (example):
1. Create a task handle. for example Task1:

  ```cpp 
  TaskHandle_t Task1;
  ```

2. In the `setup()` create a task assigned to a specific core using the `xTaskCreatePinnedToCore` function.

  ```cpp 
xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */
```
3. After creating the task, we should write a function that contains the task's code. we must create the `Task1code()` function. This is how the task function's structure should look like:

  ```cpp 
  
  Void Task1code( void * vParameter) {
  for(;;) {
   // Code for task 1 
    
     }
   }
  
  ```
 In case we want to delete the created task, we can use the `vTaskDelete()` function, with the task handle (Task1) as argument:
 ```cpp 
 vTaskDelete(Task1);
 ```
  
  ###  Adding tasks to our firebase code :
  Now let's apply these concepts to our firebase code
 1. We start by creating our tasks handles. Our main tasks are collecting data and updating our database.
 
  ```cpp 
 TaskHandle_t Collecting_data_handler;
 TaskHandle_t Updating_data_handler;
  ```

2. Now we create our tasks and pin each one to a core (0 or 1) in our `setup()` function.

  ```cpp 
    xTaskCreatePinnedToCore(Collect_data,
                            "Collecting_data",
                            1000,
                            NULL,
                            2,
                            &Collecting_data_handler,
                            0);
delay(500);

    xTaskCreatePinnedToCore(Updating_data,
                            "Updating_data",
                            20000,
                            NULL,
                            5,
                            &Updating_data_handler,
                            1);
Serial.println("tasks created");
 
delay(500);
 ```
 3. We create our functions code:
```cpp 
    void Collect_data(void * voidParams)
{
  for(;;)
  {
    Update_Sensor_readings();
    delay(2000);
    yield();

   }     
}

void Updating_data(void * voidParas){

for(;;)
  {

        Update_data(); 
        delay(2000) ;   
        yield();

      }
}
```
4. Now in order to avoid any watchdog errors we should delete the `loop()` function:
```cpp 

  void loop()
  {
   vTaskDelete(NULL);
  }

```
**explanation**:  
Sometimes we might get this watchdog error :
```
TasE (10179) task_wdt: Task watchdog got triggered. The following tasks did not reset the watchdog in time:
E (10179) task_wdt:  - IDLE0 (CPU 0)
E (10179) task_wdt: Tasks currently running:
E (10179) task_wdt: CPU 0: Task1
E (10179) task_wdt: CPU 1: loopTask
E (10179) task_wdt: AbortinGuru Meditation Error: Core  0 panic'ed (Interrupt wdt timeout on CPU0)
```
 this error indicates that we have at least one task that's starving (waited for too long and still not executed).  
 In fact, the `loop()` funtions is pinned by default to core 1 with priority value of 1 .
 This means that CPU1 is very busy doing our empty `loop()` over and over and over again, so the other task (`Updating_data()` which is also pinned to CPU1) is starved of CPU time.
