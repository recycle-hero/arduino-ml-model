#include <Servo.h> 
#include <SoftwareSerial.h>
SoftwareSerial jevoisSerial(8, 9); // RX, TX

Servo trapdoorServo;

void setup() 
{ 
  trapdoorServo.attach(A0);
  trapdoorServo.write(0);  // set servo to mid-point

  // Use normal serial for debugging
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

//  Serial.println("Main serial ready");

  jevoisSerial.begin(9600);
} 


// Buffer for received serial port bytes:
#define MAX_LINE_LENGTH 128
char lineFromJevois[MAX_LINE_LENGTH + 1];

void loop() {
   if (Serial.available()) {
//      trapdoorServo.write(0);
//      delay(100);
//      trapdoorServo.write(100);
//      delay(100);
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
          String lastDigits = "";
          if (strLineFromJevois.substring(colonIndex) == "100") {
            lastDigits = "100";
          }

          else {
            lastDigits = strLineFromJevois.substring(len - 3);
          }
        
        String label = strLineFromJevois.substring(3, len - 4);

        if (lastDigits.toInt() > 80 && label == "bottle") {
           // wait 1 second before opening trapdoor
           delay(1000);
           trapdoorServo.write(90);
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
