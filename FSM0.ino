/*
** Copyright (c) 2021, Bradley A. Minch
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met: 
** 
**     1. Redistributions of source code must retain the above copyright 
**        notice, this list of conditions and the following disclaimer. 
**     2. Redistributions in binary form must reproduce the above copyright 
**        notice, this list of conditions and the following disclaimer in the 
**        documentation and/or other materials provided with the distribution. 
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
** POSSIBILITY OF SUCH DAMAGE.
*/

const uint16_t BLINK_INTERVAL = 500;  // Time interval between toggling LED1 in milliseconds
const uint8_t GREEN_LED = 13;         // GREEN_LED is connected to D13
const uint8_t YELLOW_LED = 12;        // YELLOW_LED is connected to D12
const uint8_t RED_LED = 11;           // RED_LED is connected to D11
const uint8_t SW1 = 10;               // SW1 is connected to D10
const uint8_t SW2 = 9;                // SW2 is connected to D9

enum states {                         // Define enumerated type for state machine states
  GREEN, 
  YELLOW, 
  RED 
};

states state;                         // Global variable to store current state
uint32_t yellow_time;                 // Global variable to store the time that YELLOW_LED last changed state
uint16_t yellow_count;                // Global variable to store the number of times YELLOW_LED has changed 
                                      //   states

void setup() {
  pinMode(GREEN_LED, OUTPUT);         // Configure GREEN_LED pin as a digital output
  pinMode(YELLOW_LED, OUTPUT);        // Configure YELLOW_LED pin as a digital output
  pinMode(RED_LED, OUTPUT);           // Configure RED_LED pin as a digital output
  pinMode(SW1, INPUT);                // Configure SW1 pin as a digital input
  pinMode(SW2, INPUT);                // Configure SW2 pin as a digital input
  
  digitalWrite(GREEN_LED, HIGH);      // Set GREEN_LED high initially
  digitalWrite(YELLOW_LED, LOW);      // Set YELLOW_LED low initially
  digitalWrite(RED_LED, LOW);         // Set RED_LED low initially

  state = GREEN;
}

void loop() {
  uint32_t t;                         // Local variable to store the current value of the millis timer

  switch (state) {
    case GREEN:
      if (digitalRead(SW1) == HIGH) {   // If SW1 is pressed,
        digitalWrite(GREEN_LED, LOW);   //   turn off GREEN_LED, 
        state = YELLOW;                 //   switch to the YELLOW state, 
        digitalWrite(YELLOW_LED, HIGH); //   turn on YELLOW_LED, 
        yellow_time = millis();         //   record the value of the millis timer, and 
        yellow_count = 0;               //   initialize the count variable
      }
      
      break;

    case YELLOW:
      t = millis();      
      if (t >= yellow_time + BLINK_INTERVAL) {
        digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
        yellow_time = t;
        yellow_count++;
      }
      
      if (digitalRead(SW2) == HIGH) {   // If SW2 is pressed,
        digitalWrite(YELLOW_LED, LOW);  //   turn off YELLOW_LED,
        state = GREEN;                  //   switch to the GREEN state, and 
        digitalWrite(GREEN_LED, HIGH);  //   turn on GREEN_LED
      } else if (yellow_count == 20) {  // else if YELLOW_LED has toggled 20 times, 
        digitalWrite(YELLOW_LED, LOW);  //   turn off YELLOW_LED, 
        state = RED;                    //   switch to the RED state, and 
        digitalWrite(RED_LED, HIGH);    //   turn on RED_LED
      }
      
      break;

    case RED:
      // Abandon all hope ye who enter here...
      break;
  }
}
