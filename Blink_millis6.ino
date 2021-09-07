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

const uint16_t BLINK_INTERVAL = 500;  // Time interval between toggling LED in milliseconds
const uint8_t LED = 13;               // LED is connected to D13

uint32_t blink_time;                  // Global variable to store the time that LED last changed state

/*
** Returns a boolean value that indicates whether the current time, t, is later than some prior 
** time, t0, plus a given interval, dt.  The condition accounts for timer overflow / wraparound.
*/
bool it_is_time(uint32_t t, uint32_t t0, uint16_t dt) {
  return ((t >= t0) && (t - t0 >= dt)) ||                   // The first disjunct handles the normal case
            ((t < t0) && (t + (uint32_t)(~t0) + 1 >= dt));  //   while the second handles the overflow case
}

void setup() {
  pinMode(LED, OUTPUT);               // Configure LED pin as a digital output
  digitalWrite(LED, HIGH);            // Set LED high initially
  blink_time = millis();              // Remember the current value of the millis timer
}

void loop() {
  uint32_t t;                         // Local variable to store the current value of the millis timer

  t = millis();                       // Get the current value of the millis timer
  if (it_is_time(t, blink_time, BLINK_INTERVAL)) {  // If BLINK_INTERVAL ms have elspsed since blink_time, 
    digitalWrite(LED, !digitalRead(LED));           //   toggle the state of LED1 and
    blink_time = t;                                 //   set blink_time to the current millis timer value
  }
}
