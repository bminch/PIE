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

const uint16_t GREEN_PERIOD = 1000;   // Period of green LED flashing in milliseconds
const uint16_t YELLOW_INTERVAL_MAX = 1000;  // Maximum interval between yellow LED toggling in milliseconds
const uint8_t GREEN_LED = 13;         // GREEN_LED is connected to D13
const uint8_t YELLOW_LED = 12;        // YELLOW_LED is connected to D12
const uint8_t RED_LED = 11;           // RED_LED is connected to D11
const uint8_t SW1 = 10;               // SW1 is connected to D10
const uint8_t SW2 = 9;                // SW2 is connected to D9
const uint8_t POT = 0;                // POT is connected to A0

uint32_t green_time, yellow_time;
uint16_t green_interval = 500, yellow_interval = 500;
String command = "";

bool it_is_time(uint32_t t, uint32_t t0, uint16_t dt);
bool str2hex(char *str, uint16_t *val);

void setup() {
  pinMode(GREEN_LED, OUTPUT);         // Configure GREEN_LED pin as a digital output
  pinMode(YELLOW_LED, OUTPUT);        // Configure YELLOW_LED pin as a digital output
  pinMode(RED_LED, OUTPUT);           // Configure RED_LED pin as a digital output
  pinMode(SW1, INPUT);                // Configure SW1 pin as a digital input
  pinMode(SW2, INPUT);                // Configure SW2 pin as a digital input
  
  digitalWrite(GREEN_LED, HIGH);       // Set GREEN_LED low initially
  digitalWrite(YELLOW_LED, HIGH);      // Set YELLOW_LED low initially
  digitalWrite(RED_LED, LOW);         // Set RED_LED low initially

  green_time = millis();
  yellow_time = green_time;

  Serial.begin(115200);
}

void loop() {
  uint32_t t;

  t = millis();
  if (it_is_time(t, green_time, green_interval)) {
    digitalWrite(GREEN_LED, LOW);
  }

  if (it_is_time(t, green_time, GREEN_PERIOD)) {
    digitalWrite(GREEN_LED, HIGH);
    green_time = t;
  }

  if (it_is_time(t, yellow_time, yellow_interval)) {
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    yellow_time = t;
  }

  if (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\r') {
      parse_command();
      command = "";
    } else {
      command += ch;
    }
  }
}

void parse_command() {
  uint16_t val;

  //Serial.print("[");
  //Serial.print(command);
  //Serial.print("]\r\n");

  if (command.equals("GREEN?")) {
    Serial.print(green_interval, HEX);
    Serial.print("\r\n");
  } else if (command.startsWith("GREEN!")) {
    if (str2hex(command.substring(6), &val)) {
      green_interval = (val < GREEN_PERIOD) ? val : GREEN_PERIOD;
    }
  } else if (command.equals("YELLOW?")) {
    Serial.print(yellow_interval, HEX);
    Serial.print("\r\n");
  } else if (command.startsWith("YELLOW!")) {
    if (str2hex(command.substring(7), &val)) {
      yellow_interval = (val < YELLOW_INTERVAL_MAX) ? val : YELLOW_INTERVAL_MAX;
    }
  } else if (command.equals("RED?")) {
    Serial.print(digitalRead(RED_LED), HEX);
    Serial.print("\r\n");
  } else if (command.startsWith("RED!")) {
    if (str2hex(command.substring(4), &val)) {
      digitalWrite(RED_LED, val ? HIGH : LOW);
    }
  } else if (command.equals("SW1?")) {
    Serial.print(digitalRead(SW1), HEX);
    Serial.print("\r\n");
  } else if (command.equals("SW2?")) {
    Serial.print(digitalRead(SW2), HEX);
    Serial.print("\r\n");
  } else if (command.equals("POT?")) {
    Serial.print(analogRead(POT), HEX);
    Serial.print("\r\n");
  }
}

/*
** Returns a boolean value that indicates whether the current time, t, is later than some prior 
** time, t0, plus a given interval, dt.  The condition accounts for timer overflow / wraparound.
*/
bool it_is_time(uint32_t t, uint32_t t0, uint16_t dt) {
  return ((t >= t0) && (t - t0 >= dt)) ||         // The first disjunct handles the normal case
            ((t < t0) && (t + (~t0) + 1 >= dt));  //   while the second handles the overflow case
}

/*
** Converts a string, str, into a 16-bit unsigned integer that is returned 
** using call by reference via val, skipping over any initial space or tab 
** characters.  The function returns a boolean value that is true if the 
** conversion succeeded.  The conversion is considered failed if there is 
** no valid sequence of hex digits or if the sequence of hex digits does 
** not end with the end of the string.
*/
bool str2hex(String str, uint16_t *val) {
  uint8_t pos = 0;

  if (str.length() == 0)
    return false;

  while ((str.charAt(pos) == ' ') || (str.charAt(pos) == '\t')) {
    pos++;
  }

  *val = 0;
  while (pos < str.length()) {
    if ((str.charAt(pos) >= '0') && (str.charAt(pos) <= '9')) {
      *val = (*val << 4) + (str.charAt(pos) - '0');
    } else if ((str.charAt(pos) >= 'a') && (str.charAt(pos) <= 'f')) {
      *val = (*val << 4) + 10 + (str.charAt(pos) - 'a');
    } else if ((str.charAt(pos) >= 'A') && (str.charAt(pos) <= 'F')) {
      *val = (*val << 4) + 10 + (str.charAt(pos) - 'A');
    } else {
      return false;
    }
    pos++;
  }

  return true;
}
