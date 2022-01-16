 //Use only core 1 for demo projects
#ifndef LED_BUILTIN
#define LED_BUILTIN 2 // pin number is specific to your esp32 board
#endif
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0; 
#else
static const BaseType_t app_cpu = 1;
#endif

static int incomingByte = 0; // for incoming serial data
static int msg[20]; 
static int c;
//Pins 
static const int led_pin = LED_BUILTIN;

// Task Handler
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL; 

//*******************************************
//Tasks

//Task1 print to Serial Terminal with lower priority
void startTask1(void *parameter){

  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(incomingByte/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(incomingByte/portTICK_PERIOD_MS);
  }
  
}
//Task2 : print to Serial Terminal with higher priority
void startTask2(void *parameter){
  while(1){
     // send data only when you receive data:
       // read the incoming byte:
       if(Serial.available() > 0){
           int i = 0; 
           c= Serial.parseInt();
           if(c == 0){
              incomingByte = msg[i];
              // say what you got:
              Serial.print("I received: ");
              Serial.println(incomingByte, DEC);
              i=0; 
           }
           else
             msg[i]= c;
             i++;
      }           
            vTaskDelay(1000/portTICK_PERIOD_MS);  
  }
}

//**********************************************
// Main (runs as its own task priority 1 on core 1)


void setup() {

  pinMode(led_pin, OUTPUT); 
  
  // Configure Serial (go slow we can watch the preemption)
  Serial.begin(300);

  //Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("FreeRTOS TASK Scheduling");

  // Print self priority
  Serial.print("Setup and loop task running core");
  Serial.print(xPortGetCoreID());
  Serial.print("with priority");
  Serial.print(uxTaskPriorityGet(NULL));

  //Task to run forever
  xTaskCreatePinnedToCore(startTask1, // Function to be called
                          "Task1",    // Name of task
                          1024,       // Stack size (byte ins ESP32, words in FreeRTOS)
                          NULL,       // Parameter to pass to function
                          1,          // Task priority ( = to configMAX_PRIOEITIES -1)
                          &task_1,     // Task handle
                          app_cpu);   // Run on core one of this dual core process
 

  //Task2 to run once with higher priority
  xTaskCreatePinnedToCore(startTask2, // Function to be called
                          "Task2",    // Name of task
                          1024,       // Stack size (byte ins ESP32, words in FreeRTOS)
                          NULL,       // Parameter to pass to function
                          1,          // Task priority ( = to configMAX_PRIOEITIES -1)
                          &task_2,     // Task handle
                          app_cpu);   // Run on core one of this dual core process

  vTaskDelete(NULL);
}
void loop() {
  // put your main code here, to run repeatedly:
  //Delete the lower priority task
  
}
