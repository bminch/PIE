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

function Serial_cmd_gui(dev)

dev_name = inputname(1);

fig_handles = get(0, 'children');
dev_gui_fig = findobj(fig_handles, 'tag', [dev_name, '_gui_fig']);
if ~isempty(dev_gui_fig)
    figure(dev_gui_fig);
    return;
end

set(0, 'units', 'pixels');
screen_rect = get(0, 'screensize');
screen_width = screen_rect(3); 
screen_height = screen_rect(4);

fig = figure;
fig.Units = 'pixels';
fig.Position = [(screen_width / 2 - 200), (screen_height / 2 - 90), 400, 180];
fig.NumberTitle = 'off';
fig.Name = ['Serial_cmd GUI (', dev_name, ')'];
fig.Resize = 'off';
fig.Tag = [dev_name, '_gui_fig'];
fig.DeleteFcn = @stop_update_timer;

red_btn = uicontrol(fig);
red_btn.Style = 'pushbutton';
red_btn.String = 'TOGGLE RED';
red_btn.Position = [160, 150, 80, 24];
red_btn.Callback = @toggle_red_callback;

    function toggle_red_callback(src, event)
        dev.set_red(uint16(1 - dev.get_red()));
    end

green_slider = uicontrol(fig);
green_slider.Style = 'slider';
green_slider.Min = 0;
green_slider.Max = 1000;
green_slider.Value = dev.get_green();
green_slider.SliderStep = [1, 10];
green_slider.Position = [50, 120, 300, 24];
green_slider.Callback = @green_slider_callback;

    function green_slider_callback(src, event)
        dev.set_green(uint16(src.Value));
    end

yellow_slider = uicontrol(fig);
yellow_slider.Style = 'slider';
yellow_slider.Min = 0;
yellow_slider.Max = 1000;
yellow_slider.Value = dev.get_yellow();
yellow_slider.SliderStep = [1, 10];
yellow_slider.Position = [50, 90, 300, 24];
yellow_slider.Callback = @yellow_slider_callback;

    function yellow_slider_callback(src, event)
        dev.set_yellow(uint16(src.Value));
    end

sw1_text = uicontrol(fig);
sw1_text.Style = 'text';
sw1_text.Position = [50, 60, 300, 24];
sw1_text.String = 'SW1 is currently ?';

sw2_text = uicontrol(fig);
sw2_text.Style = 'text';
sw2_text.Position = [50, 30, 300, 24];
sw2_text.String = 'SW2 is currently ?';

pot_text = uicontrol(fig);
pot_text.Style = 'text';
pot_text.Position = [50, 0, 300, 24];
pot_text.String = 'POT is currently ????';

update_timer = timer;
update_timer.ExecutionMode = 'fixedSpacing';
update_timer.Period = 0.05;
update_timer.StartDelay = 0;
update_timer.TimerFcn = @update_status;
start(update_timer);

    function update_status(src, event)
        sw1_text.String = sprintf('SW1 is currently %d', dev.get_sw1());
        sw2_text.String = sprintf('SW2 is currently %d', dev.get_sw2());
        pot_text.String = sprintf('POT is currently %04d', dev.get_pot());
    end

    function stop_update_timer(src, event)
        stop(update_timer);
        delete(update_timer);
    end

end
