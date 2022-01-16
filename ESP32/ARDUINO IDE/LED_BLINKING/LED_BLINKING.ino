 //Use only core 1 for demo projects
#ifndef LED_BUILTIN
#define LED_BUILTIN 2 // pin number is specific to your esp32 board
#endif
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0; 
#else
static const BaseType_t app_cpu = 1;
#endif

//Pins 
static const int led_pin = LED_BUILTIN;

//Our task: blink a LED
void toggleLED( void *parameter){
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT); 

  //Task to run forever
  xTaskCreatePinnedToCore( //UseXtaskCreate() in vanilla FreeRTOS
             toggleLED,   // Function to be called
             "Toggle LED",// Nome of task
             1024,        // Stack size (byte ins ESP32, words in FreeRTOS)
             NULL,        // Parameter to pass to function
             1,           // Task priority ( = to configMAX_PRIOEITIES -1)
             NULL,        // Task handle
             app_cpu);    // Run on core one of this dual core process

     // If this was Vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
     // main after setting up your tasks.

     
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
