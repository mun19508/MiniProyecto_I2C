// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
//int count = 0;
String dato_PIC = "";
int LEDcount1 = 0;
int LEDcount2 = 0;
float Ax, Ay, Az;
// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 15000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *accX = io.feed("AccX");
AdafruitIO_Feed *accY = io.feed("AccY");
AdafruitIO_Feed *accZ = io.feed("AccZ");
AdafruitIO_Feed *P1 = io.feed("Piloto1");
AdafruitIO_Feed *P2 = io.feed("Piloto2");
void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial2.begin(9600);

  // wait for serial monitor to open
  //while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  P1->onMessage(dataPiloto1);
  P2->onMessage(dataPiloto2);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  //  counter->get();

}

void loop() {
  while (Serial2.available()) {
    char recibido = Serial2.read();
    if (recibido != '\n') dato_PIC.concat(recibido);
    else {
      Ax = (dato_PIC.substring(0, 7)).toFloat();
      Ay = (dato_PIC.substring(7, 14)).toFloat();
      Az = (dato_PIC.substring(14, 21)).toFloat();
      dato_PIC = "";
    }
  }
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // save count to the 'counter' feed on Adafruit IO
    Serial.print("sending -> x:");
    Serial.println(Ax);
    accX->save(Ax);
    Serial.print("sending -> y:");
    Serial.println(Ay);
    accY->save(Ay);
    Serial.print("sending -> z:");
    Serial.println(Az);
    accZ->save(Az);

    // increment the count by 1
    //count++;

    // after publishing, store the current time
    lastUpdate = millis();
  }

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void dataPiloto1(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  char* dato =  data->value(); // se obtiene el valor enviado;
  Serial.println(dato);
  if (*dato == '1') {
    Serial2.print('a'); //encender luz 1
    Serial.println('a');
  }
  else {
    Serial2.print('b'); // apagar luz 1
    Serial.println('b');
  }
  return;
}

void dataPiloto2(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  char* dato =  data->value(); // se obtiene el valor enviado;
  Serial.println(dato);
  if (*dato == '1') {
    Serial2.println('c'); //encender luz 2
    Serial.println('c');
  }
  else {
    Serial2.println('d'); // apagar luz 2
    Serial.println('d');
  }
  return;
}
