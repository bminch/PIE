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
const uint8_t CMD_BUFFER_LEN = 128;

uint32_t green_time, yellow_time;
uint16_t green_interval = 500, yellow_interval = 500;
uint16_t cmd_buffer_pos = 0;
char cmd_buffer[CMD_BUFFER_LEN];

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
      cmd_buffer[cmd_buffer_pos] = '\0';
      parse_cmd_buffer();
      cmd_buffer_pos = 0;
    } else if (cmd_buffer_pos == CMD_BUFFER_LEN - 1) {
      cmd_buffer_pos = 0;
      cmd_buffer[cmd_buffer_pos] = ch;
      cmd_buffer_pos++;
    } else {
      cmd_buffer[cmd_buffer_pos] = ch;
      cmd_buffer_pos++;
    }
  }
}

void parse_cmd_buffer() {
  uint16_t val;

  //Serial.print("[");
  //Serial.print(cmd_buffer);
  //Serial.print("]\r\n");

  if (strcmp(cmd_buffer, "GREEN?") == 0) {
    Serial.print(green_interval, HEX);
    Serial.print("\r\n");
  } else if (strncmp(cmd_buffer, "GREEN!", 6) == 0) {
    if (str2hex(cmd_buffer + 6, &val)) {
      green_interval = (val < GREEN_PERIOD) ? val : GREEN_PERIOD;
    }
  } else if (strcmp(cmd_buffer, "YELLOW?") == 0) {
    Serial.print(yellow_interval, HEX);
    Serial.print("\r\n");
  } else if (strncmp(cmd_buffer, "YELLOW!", 7) == 0) {
    if (str2hex(cmd_buffer + 7, &val)) {
      yellow_interval = (val < YELLOW_INTERVAL_MAX) ? val : YELLOW_INTERVAL_MAX;
    }
  } else if (strcmp(cmd_buffer, "RED?") == 0) {
    Serial.print(digitalRead(RED_LED), HEX);
    Serial.print("\r\n");
  } else if (strncmp(cmd_buffer, "RED!", 4) == 0) {
    if (str2hex(cmd_buffer + 4, &val)) {
      digitalWrite(RED_LED, val ? HIGH : LOW);
    }
  } else if (strcmp(cmd_buffer, "SW1?") == 0) {
    Serial.print(digitalRead(SW1), HEX);
    Serial.print("\r\n");
  } else if (strcmp(cmd_buffer, "SW2?") == 0) {
    Serial.print(digitalRead(SW2), HEX);
    Serial.print("\r\n");
  } else if (strcmp(cmd_buffer, "POT?") == 0) {
    Serial.print(analogRead(POT), HEX);
    Serial.print("\r\n");
  }
}

/*
** Returns a boolean value that indicates whether the current time, t, is later than some prior 
** time, t0, plus a given interval, dt.  The condition accounts for timer overflow / wraparound.
*/
bool it_is_time(uint32_t t, uint32_t t0, uint16_t dt) {
  return ((t >= t0) && (t - t0 >= dt)) ||                   // The first disjunct handles the normal case
            ((t < t0) && (t + (uint32_t)(~t0) + 1 >= dt));  //   while the second handles the overflow case
}

/*
** Converts a null-terminated (C-style) string that is pointed to by str 
** into a 16-bit unsigned integer that is returned using call by reference 
** via val, skipping over any initial space or tab characters.  The function 
** returns a boolean value that is true if the conversion succeeded.  The 
** conversion is considered failed if there is no valid sequence of hex 
** digits or if the sequence of hex digits does not end with the end of 
** the string.
*/
bool str2hex(char *str, uint16_t *val) {
  uint8_t pos = 0;

  if (str[pos] == '\0')
    return false;

  while ((str[pos] == ' ') || (str[pos] == '\t')) {
    pos++;
  }

  *val = 0;
  while (str[pos] != '\0') {
    if ((str[pos] >= '0') && (str[pos] <= '9')) {
      *val = (*val << 4) + (str[pos] - '0');
    } else if ((str[pos] >= 'a') && (str[pos] <= 'f')) {
      *val = (*val << 4) + 10 + (str[pos] - 'a');
    } else if ((str[pos] >= 'A') && (str[pos] <= 'F')) {
      *val = (*val << 4) + 10 + (str[pos] - 'A');
    } else {
      return false;
    }
    pos++;
  }

  return true;
}
