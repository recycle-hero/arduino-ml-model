#include <Servo.h>

Servo trapdoorServo;

void setup()
{
  trapdoorServo.attach(A0);
  // set servo to mid-point
  trapdoorServo.write(0);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

#define MAX_LINE_LENGTH 128
#define START_INDEX_OF_JEVOIS_LABEL 3
#define THRESHOLD_PROBABILITY_OF_MATCH 80
#define SERVO_ANGLE_TO_OPEN_TRAPDOOR 90
char lineFromJevois[MAX_LINE_LENGTH + 1];

void loop() {
   if (Serial.available()) {
      byte len = Serial.readBytesUntil('\n', lineFromJevois, MAX_LINE_LENGTH);
      lineFromJevois[len] = 0;

      if (lineFromJevois[0] == 'N' && lineFromJevois[1] == 'O') {
        String strLineFromJevois = String(lineFromJevois);
        int colonIndex = 0;
        for (int i=0; i < len; i++) {
          if (String(strLineFromJevois.charAt(i)) == ":") {
            colonIndex = strLineFromJevois[i];
          }
        }
        //Assign the number to be the characters after the colon to the end of the string
        String lastDigits = strLineFromJevois.substring(colonIndex);

        String label = strLineFromJevois.substring(START_INDEX_OF_JEVOIS_LABEL, colonIndex);

        if (lastDigits.toInt() > THRESHOLD_PROBABILITY_OF_MATCH && label == "bottle") {
           // wait 1 second before opening trapdoor
           delay(1000);
           trapdoorServo.write(SERVO_ANGLE_TO_OPEN_TRAPDOOR);
           // leave trapdoor open for 5 seconds
           delay(5000);
        } else {
           trapdoorServo.write(0);
        }
      } else {
        trapdoorServo.write(0);
      }
   } else {
      trapdoorServo.write(0);
   }
}
