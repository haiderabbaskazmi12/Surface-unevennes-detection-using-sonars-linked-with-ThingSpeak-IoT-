#include <NewPing.h>
 
#define TRIGGER_PINL  12
#define ECHO_PINL     11

#define TRIGGER_PINM  10
#define ECHO_PINM     9

#define TRIGGER_PINR  8
#define ECHO_PINR     7

#define MAX_DISTANCE 200
 
NewPing sonarL(TRIGGER_PINL, ECHO_PINL, MAX_DISTANCE);
NewPing sonarM(TRIGGER_PINM, ECHO_PINM, MAX_DISTANCE);
NewPing sonarR(TRIGGER_PINR, ECHO_PINR, MAX_DISTANCE);

void setup() 
{
  Serial.begin(115200);
}
 
void loop() 
{
  delay(50);
  Serial.print("Ping: ");
  Serial.print(sonarL.ping_cm());
  Serial.print("cm");
  Serial.print("\t");

  Serial.print("Ping: ");
  Serial.print(sonarM.ping_cm());
  Serial.print("cm");
  Serial.print("\t");
  
  Serial.print("Ping: ");
  Serial.print(sonarR.ping_cm());
  Serial.println("cm");
}
