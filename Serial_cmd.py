#
## Copyright (c) 2021, Bradley A. Minch
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met: 
## 
##     1. Redistributions of source code must retain the above copyright 
##        notice, this list of conditions and the following disclaimer. 
##     2. Redistributions in binary form must reproduce the above copyright 
##        notice, this list of conditions and the following disclaimer in the 
##        documentation and/or other materials provided with the distribution. 
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
## ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
## LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
## CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
## POSSIBILITY OF SUCH DAMAGE.
#

import serial
import serial.tools.list_ports as list_ports
import string, array

class Serial_cmd:

    Arduino_IDs = ((0x2341, 0x0043), (0x2341, 0x0001), 
                   (0x2A03, 0x0043), (0x2341, 0x0243), 
                   (0x0403, 0x6001), (0x1A86, 0x7523))

    def __init__(self, port = ''):
        if port == '':
            self.dev = None
            self.connected = False
            devices = list_ports.comports()
            for device in devices:
                if (device.vid, device.pid) in Serial_cmd.Arduino_IDs:
                    try:
                        self.dev = serial.Serial(device.device, 115200)
                        self.connected = True
                        print('Connected to {!s}...'.format(device.device))
                    except:
                        pass
                if self.connected:
                    break
        else:
            try:
                self.dev = serial.Serial(port, 115200)
                self.connected = True
            except:
                self.dev = None
                self.connected = False

    def write(self, command):
        if self.connected:
            self.dev.write('{!s}\r'.format(command).encode())

    def read(self):
        if self.connected:
            return self.dev.readline().decode()

    def get_green(self):
        if self.connected:
            self.write('GREEN?')
            return int(self.read(), 16)

    def set_green(self, val):
        if self.connected:
            self.write('GREEN!{:X}'.format(int(val)))

    def get_yellow(self):
        if self.connected:
            self.write('YELLOW?')
            return int(self.read(), 16)

    def set_yellow(self, val):
        if self.connected:
            self.write('YELLOW!{:X}'.format(int(val)))

    def get_red(self):
        if self.connected:
            self.write('RED?')
            return int(self.read(), 16)

    def set_red(self, val):
        if self.connected:
            self.write('RED!{:X}'.format(int(val)))

    def get_sw1(self):
        if self.connected:
            self.write('SW1?')
            return int(self.read(), 16)

    def get_sw2(self):
        if self.connected:
            self.write('SW2?')
            return int(self.read(), 16)

    def get_pot(self):
        if self.connected:
            self.write('POT?')
            return int(self.read(), 16)

