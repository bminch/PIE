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

typedef void (*STATE_HANDLER_T)();    // Defined type to store pointer to a state handler function

void green();                         // Forward declarations of state handler functions
void yellow();
void red();

STATE_HANDLER_T prior_state, state;   // Global variables to store the prior and current states
uint32_t yellow_time;                 // Global variable to store the time that YELLOW_LED last changed state
uint16_t yellow_count;                // Global variable to store the number of times YELLOW_LED has changed states

void green() {
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);
  }

  // Perform state tasks

  // Check for state transitions
  if (digitalRead(SW1) == HIGH) {
    state = yellow;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);
  }
}

void yellow() {
  uint32_t t;
  
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(YELLOW_LED, HIGH);
    yellow_time = millis();
    yellow_count = 0;
  }

  // Perform state tasks
  t = millis();
  if (t >= yellow_time + BLINK_INTERVAL) {
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    yellow_time = t;
    yellow_count++;
  }

  // Check for state transitions
  if (digitalRead(SW2) == HIGH) {
    state = green;
  } else if (yellow_count == 20) {
    state = red;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(YELLOW_LED, LOW);
  }
}

void red() {
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(RED_LED, HIGH);
  }

  // Perform state tasks

  // Check for state transitions

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(RED_LED, LOW);
  }
}

void setup() {
  pinMode(GREEN_LED, OUTPUT);         // Configure GREEN_LED pin as a digital output
  pinMode(YELLOW_LED, OUTPUT);        // Configure YELLOW_LED pin as a digital output
  pinMode(RED_LED, OUTPUT);           // Configure RED_LED pin as a digital output
  pinMode(SW1, INPUT);                // Configure SW1 pin as a digital input
  pinMode(SW2, INPUT);                // Configure SW2 pin as a digital input
  
  digitalWrite(GREEN_LED, LOW);       // Set GREEN_LED low initially
  digitalWrite(YELLOW_LED, LOW);      // Set YELLOW_LED low initially
  digitalWrite(RED_LED, LOW);         // Set RED_LED low initially

  prior_state = NULL;
  state = green;
}

void loop() {
  state();
}
