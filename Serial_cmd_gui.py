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

import tkinter as tk
import Serial_cmd

class Serial_cmd_gui:

    def __init__(self):
        self.dev = Serial_cmd.Serial_cmd()
        if self.dev.dev is not None:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title('Serial_cmd GUI')
            self.root.protocol('WM_DELETE_WINDOW', self.shut_down)
            fm = tk.Frame(self.root)
            tk.Button(fm, text = 'TOGGLE RED', command = self.toggle_red).pack(side = tk.LEFT)
            fm.pack(side = tk.TOP)
            green_slider = tk.Scale(self.root, from_ = 0, to = 1000, orient = tk.HORIZONTAL, showvalue = tk.FALSE, command = self.set_green_callback)
            green_slider.set(500)
            green_slider.pack(side = tk.TOP)
            yellow_slider = tk.Scale(self.root, from_ = 0, to = 1000, orient = tk.HORIZONTAL, showvalue = tk.FALSE, command = self.set_yellow_callback)
            yellow_slider.set(500)
            yellow_slider.pack(side = tk.TOP)
            self.sw1_status = tk.Label(self.root, text = 'SW1 is currently ?')
            self.sw1_status.pack(side = tk.TOP)
            self.sw2_status = tk.Label(self.root, text = 'SW2 is currently ?')
            self.sw2_status.pack(side = tk.TOP)
            self.pot_status = tk.Label(self.root, text = 'POT is currently ????')
            self.pot_status.pack(side = tk.TOP)
            self.update_job = self.root.after(500, self.update_status)

    def toggle_red(self):
        self.dev.set_red(1 - self.dev.get_red())

    def set_green_callback(self, value):
        self.dev.set_green(int(value))

    def set_yellow_callback(self, value):
        self.dev.set_yellow(int(value))

    def update_status(self):
        self.sw1_status.configure(text = 'SW1 is currently {!s}'.format(self.dev.get_sw1()))
        self.sw2_status.configure(text = 'SW2 is currently {!s}'.format(self.dev.get_sw2()))
        self.pot_status.configure(text = 'POT is currently {:04d}'.format(self.dev.get_pot()))
        self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        if self.update_job is not None:
            self.root.after_cancel(self.update_job)
        self.root.destroy()

if __name__=='__main__':
    gui = Serial_cmd_gui()
    gui.root.mainloop()

