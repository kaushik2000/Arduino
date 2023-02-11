// This code is used to test CDM7160 Co2 sensor with Arduino Nano - Using UART communication
// Connect as follows:
// Nano     |   CDM7160
// ---------------------
// 5V       |   VCC
// GND      |   GND
// D10      |   Tx
// D11      |   Rx
// MSE      |   No Connection for UART

#include <SoftwareSerial.h>
#define DECODE false // Set to true --> Display all info to the serial monitor | Set to false --> Display only ppm value

SoftwareSerial mySerial(10, 11); // Soft. RX-10, TX-11 of Nano --> TX, RX of CDM respectively
byte message[] = {0xFE, 0x44, 0x00, 0x08, 0x02, 0x9F, 0x25}; // Device code (FE - All Devices) | Function Code - 44 | No. of Readout bytes - 02 | Co2 Concentration - 08 02 | Error check - 9F 25
bool flag = true;
long int MSB = 0, LSB = 0, PPM = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (DECODE) Serial.println("Greetings"); // Test Serial Output
  mySerial.begin(9600); // CDM works at 9600 bits per second (Baud Rate)
  delay(1000);
}

void loop() { // run over and over
  //  Input   : FE 44 00 08 02 9F 25
  //  Output  : FE 44 02 ~~ ~~ ee ee

  MSB = 0; LSB = 0; PPM = 0;
  if (DECODE)
  {
    Serial.print("Sending Code: ");
    // Print input codes for our reference
    Serial.print(message[0], HEX); Serial.print(" ");
    Serial.print(message[1], HEX); Serial.print(" ");
    Serial.print(message[2], HEX); Serial.print(" ");
    Serial.print(message[3], HEX); Serial.print(" ");
    Serial.print(message[4], HEX); Serial.print(" ");
    Serial.print(message[5], HEX); Serial.print(" ");
    Serial.print(message[6], HEX); Serial.println();
  }
  mySerial.write(message, sizeof(message)); // Sends the codes to CDM7160
  flag = true;
  delay(1000);

  if (DECODE) Serial.print("Starting to receive data: ");
  while (flag)
  {
    if (mySerial.read() == 254) // Start only when you receive 0xFE = (Decimal)254
    {
      if (DECODE) Serial.println("Device has responded");
      mySerialRead(); mySerialRead(); // Read the response for function and readout bytes
      MSB = mySerial.read();
      LSB = mySerial.read();
      PPM = (MSB * 100) + LSB;
      if (DECODE) Serial.print("PPM Value: ");
      if (DECODE) {
        Serial.print(PPM);
      }
      else {
        Serial.println(PPM);
      }
      if (DECODE) Serial.println(" ppm"); // Use serial monitor to view PPM / Serial plotter for a clear visual
      mySerialRead(); mySerialRead(); // Read error check bytes
      if (DECODE) Serial.println("Error check bits received");
      if (DECODE) Serial.println("\n------  -------  ------\n ");
      flag = false;
    }
  }
}

void display()
{
  if (mySerial.available()) {
    //    Serial.print("Read from CDM7160: ");
    Serial.print(mySerial.read(), HEX); Serial.print(" ");
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

void mySerialRead()
{
  if (mySerial.available()) {
    mySerial.read();
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
