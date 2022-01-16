

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0; 
#else
static const BaseType_t app_cpu = 1;
#endif

// some string to print
const char msg[] = "Nicola Ferrante ESP32";

// Task Handler
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL; 

//*******************************************
//Tasks

//Task1 print to Serial Terminal with lower priority
void startTask1(void *parameter){

  //Count number of characters in string
  int msg_len = strlen(msg);

  //Print string to Terminal
  while(1){
    Serial.println();
    for(int i = 0; i< msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
//Task2 : print to Serial Terminal with higher priority
void startTask2(void *parameter){
  while(1){
    Serial.print("*");
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

//**********************************************
// Main (runs as its own task priority 1 on core 1)


void setup() {
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
                          2,          // Task priority ( = to configMAX_PRIOEITIES -1)
                          &task_2,     // Task handle
                          app_cpu);   // Run on core one of this dual core process
 
}

void loop() {
  // put your main code here, to run repeatedly:
  for( int i=0; i<3; i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }

  //Delete the lower priority task
  if(task_1 != NULL){
    vTaskDelete(task_1);
    task_1 = NULL; 
    
  }
}
