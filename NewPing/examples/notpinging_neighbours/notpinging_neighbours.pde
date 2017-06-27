/*
If, however, you don't really care about comparing neighboring ping results and just want to ping multiple sensors and anytime there's something within range you want to trigger something, you can totally get rid of the cm array.
The following code is probably the easiest to understand, as it simply pings each sensor and when there's a ping within range, the pingResult() function is called.
You get the sensor number and the cm distance which you can do something with.
It will only call pingResult() when one of the sensors "hears" something within range.
*/

#include <NewPing.h>

#define SONAR_NUM      3
#define MAX_DISTANCE 200
#define PING_INTERVAL 33

unsigned long pingTimer[SONAR_NUM];
uint8_t currentSensor = 0;

NewPing sonar[SONAR_NUM] = {
  NewPing(4, 5, MAX_DISTANCE),
  NewPing(6, 7, MAX_DISTANCE),
  NewPing(8, 9, MAX_DISTANCE)
};

void setup() {
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;
  for (uint8_t i = 1; i < SONAR_NUM; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
      sonar[currentSensor].timer_stop();
      currentSensor = i;
      sonar[currentSensor].ping_timer(echoCheck);
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.
}

void echoCheck() {
  if (sonar[currentSensor].check_timer())
    pingResult(currentSensor, sonar[currentSensor].ping_result / US_ROUNDTRIP_CM);
}

void pingResult(uint8_t sensor, int cm) {
  // The following code would be replaced with your code that does something with the ping result.
  Serial.print(sensor);
  Serial.print(" ");
  Serial.print(cm);
  Serial.println("cm");
}

/*Remember, to analyze the ping results, you do that in the pingResults() function in the above sketches or in the oneSensorCycle() function in the 15 Sensors Example Sketch. Also, none of these will properly work if you do any delay statements at any point in your sketch.

If you ever want to stop the pings in your sketch, for example to do something that requires delays or takes longer than 33ms to process, do the following:

for (uint8_t i = 0; i < SONAR_NUM; i++) pingTimer[i] = -1;

To start the pings again, do the following:

pingTimer[0] = millis();
for (uint8_t i = 1; i < SONAR_NUM; i++) pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;

Hope this helps!
*/