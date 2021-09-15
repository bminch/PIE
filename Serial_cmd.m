%%
%% Copyright (c) 2021, Bradley A. Minch
%% All rights reserved.
%%
%% Redistribution and use in source and binary forms, with or without
%% modification, are permitted provided that the following conditions are met: 
%% 
%%     1. Redistributions of source code must retain the above copyright 
%%        notice, this list of conditions and the following disclaimer. 
%%     2. Redistributions in binary form must reproduce the above copyright 
%%        notice, this list of conditions and the following disclaimer in the 
%%        documentation and/or other materials provided with the distribution. 
%%
%% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
%% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
%% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
%% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
%% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
%% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
%% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
%% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
%% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
%% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
%% POSSIBILITY OF SUCH DAMAGE.
%%

classdef Serial_cmd < handle
    properties
        dev
        port
    end
    methods
        function obj = Serial_cmd(varargin)
            if nargin == 0
                com_ports = serialportlist('available');

                Arduino_IDs = {'VID_2341&PID_0043',... 
                               'VID_2341&PID_0001',... 
                               'VID_2A03&PID_0043',... 
                               'VID_2341&PID_0243',... 
                               'VID_0403&PID_6001',... 
                               'VID_1A86&PID_7523' };

                key = 'HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\';
                [~, vals] = dos(['REG QUERY ', key, ' /s /f "FriendlyName"']);
                vals = textscan(vals, '%s', 'delimiter', '\t');
                vals = cat(1, vals{:});

                Arduinos = {};
                for i = 1:length(vals)
                    for j = 1:length(Arduino_IDs)
                        if contains(vals{i}, Arduino_IDs{j})
                            ans = extractBetween(vals{i + 1}, '(', ')');
                            Arduinos{end + 1} = ans{1};
                        end
                    end
                end

                port = '';
                for i = 1:length(com_ports)
                    for j = 1:length(Arduinos)
                        if strcmp(com_ports(i), Arduinos{j})
                            port = com_ports(i);
                        end
                    end
                end
                obj.port = port;
            else
                obj.port = varargin{1};
            end
            obj.dev = serialport(obj.port, 115200);
            configureTerminator(obj.dev, 'CR/LF', 'CR');
        end
        
        function ret = get_green(obj)
            writeline(obj.dev, 'GREEN?');
            ret = hex2dec(readline(obj.dev));
        end
        
        function set_green(obj, val)
            writeline(obj.dev, ['GREEN!', dec2hex(val)]);
        end
        
        function ret = get_yellow(obj)
            writeline(obj.dev, 'YELLOW?');
            ret = hex2dec(readline(obj.dev));
        end
        
        function set_yellow(obj, val)
            writeline(obj.dev, ['YELLOW!', dec2hex(val)]);
        end
        
        function ret = get_red(obj)
            writeline(obj.dev, 'RED?');
            ret = hex2dec(readline(obj.dev));
        end
        
        function set_red(obj, val)
            writeline(obj.dev, ['RED!', dec2hex(val)]);
        end
        
        function ret = get_sw1(obj)
            writeline(obj.dev, 'SW1?');
            ret = hex2dec(readline(obj.dev));
        end
        
        function ret = get_sw2(obj)
            writeline(obj.dev, 'SW2?');
            ret = hex2dec(readline(obj.dev));
        end
        
        function ret = get_pot(obj)
            writeline(obj.dev, 'POT?');
            ret = hex2dec(readline(obj.dev));
        end
    end
end
