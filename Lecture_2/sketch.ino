#include <Arduino.h> //Essential for running Arduino sketches on ESP32.

#define LED1 2  //Using #define makes the code easier to read and change later.
#define LED2 4
#define LED3 5

void task1(void *pvParameters) { //This defines a FreeRTOS task function. Every task has this format.
  TickType_t xLastWakeTime = xTaskGetTickCount();  //Records the current time (in ticks) when the task starts
  const TickType_t period = 500 / portTICK_PERIOD_MS; //Sets the task period in FreeRTOS ticks.
  for (;;) { //Infinie loop
    digitalWrite(LED1, HIGH); //LED1 on
    vTaskDelay(200 / portTICK_PERIOD_MS); //  Keeps LED1 on for 200 ms (computation time, like task execution).
    digitalWrite(LED1, LOW); // LED1 off
    vTaskDelayUntil(&xLastWakeTime, period); // Waits until the next period starts.
  } // Key Concept: vTaskDelayUntil helps create rate-monotonic periodic tasks.
}

// Same structure as task1, but with different periods and computation times:
// task2: period = 200 ms, computation = 100 ms → highest priority (shortest period).
// task3: period = 1000 ms, computation = 300 ms → lowest priority (longest period).
// By RM scheduling, tasks with shorter periods get higher priorities.

void task2(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t period = 200 / portTICK_PERIOD_MS;
  for (;;) {
    digitalWrite(LED2, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(LED2, LOW);
    vTaskDelayUntil(&xLastWakeTime, period);
  }
}

void task3(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t period = 1000 / portTICK_PERIOD_MS;
  for (;;) {
    digitalWrite(LED3, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(LED3, LOW);
    vTaskDelayUntil(&xLastWakeTime, period);
  }
}

void setup() {
  pinMode(LED1, OUTPUT); //configure GPIO pins as outputs so they can drive LEDs.
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
 
  xTaskCreate(task1, "Task1", 1024, NULL, 1, NULL); //Priority → 1 (lowest) to 3 (highest).
  xTaskCreate(task2, "Task2", 1024, NULL, 3, NULL);
  xTaskCreate(task3, "Task3", 1024, NULL, 2, NULL);
  //FreeRTOS priority determines preemption. Higher priority tasks can interrupt lower priority ones.
}
void loop() {}
// loop() runs repeatedly in standard Arduino sketches, but in FreeRTOS, all work is done inside tasks.
// Here, loop() is empty because the scheduler runs tasks independently.
