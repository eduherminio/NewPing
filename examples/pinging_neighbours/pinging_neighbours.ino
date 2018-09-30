/*
Also note, the 15 Sensors Example Sketch is designed to ping all the sensors and then do something with the results once all the sensors have been polled. Some want to ping each sensor and then do something right away maybe between any two pings. To do that, we no longer need the oneSensorCycle() function nor the if() statement in loop() that calls oneSensorCycle(). The following sketch calls the pingResult() function every time there's a ping within range. Because this sketch still keeps the cm array, you can look at neighboring ping results in the cm array to do whatever calculations you need to do.
*/
/*
Remember, to analyze the ping results, you do that in the pingResults() function in the above sketches or in the oneSensorCycle() function in the 15 Sensors Example Sketch. Also, none of these will properly work if you do any delay statements at any point in your sketch.

If you ever want to stop the pings in your sketch, for example to do something that requires delays or takes longer than 33ms to process, do the following:

for (uint8_t i = 0; i < SONAR_NUM; i++) pingTimer[i] = -1;

To start the pings again, do the following:

pingTimer[0] = millis();
for (uint8_t i = 1; i < SONAR_NUM; i++) pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
*/
#include <NewPing.h>

#define SONAR_NUM      3 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(4, 5, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(6, 7, MAX_DISTANCE),
  NewPing(8, 9, MAX_DISTANCE)
};

void setup() {
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()) {
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    pingResult(currentSensor);
  }
}

void pingResult(uint8_t sensor) { // Sensor got a ping, do something with the result.
  // The following code would be replaced with your code that does something with the ping result.
  Serial.print(sensor);
  Serial.print(" ");
  Serial.print(cm[sensor]);
  Serial.println("cm");
}