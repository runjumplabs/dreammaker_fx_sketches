/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Standard Vibrato 

Effect description:  This pedal sketch implements a typical vibrato pedal.  It relies
on the fx_variable_delay to changes the pitch of the incoming signal using an LFO 
(low-frequency oscillator). 

Left pot label: Depth 
Left pot function: Depth of the vibrato effect

Center pot label: Rate
Center pot function: Modulation / vibrato rate

Right pot label: LFO type
Right pot function: The type of LFO waveform used to change the pitch.
Full left is sine, center is triangle and right is random

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Tap tempo
Right footswitch function: Tap it a few times to set the vibrato tempo

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.2
Sketch version: 1.0
*/   
#include <dreammakerfx.h>

fx_variable_delay vibrato(1.0,          // Initial rate of 1 Hz (1 cycle / second)
                          0.5,          // Initial depth of 0.5
                          0.0,          // No feedback needed for vibrato
                          5.0,          // Delay line length (also effects depth) in milliseconds
                          0.0,          // Clean mix set to 0 since we just want to hear pitch shifted
                          1.0,          // Delayed mix to 1.0
                          OSC_SINE,     // Initial oscillator is sine wave
                          false);       // Don't use an external signal for oscillator

void setup() {
  
  // put your setup code here, to run once:
  pedal.init();

  // Route audio through vibrato 
  pedal.route_audio(pedal.instr_in, vibrato.input);
  pedal.route_audio(vibrato.output, pedal.amp_out);  

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap modulation rate 
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  // Run this effect
  pedal.run();

}


void loop() {


  // If new mod rate has been tapped in, use that to control vibrato rate
  if (pedal.new_tap_interval()) { 
    vibrato.set_rate_hz(pedal.get_tap_freq_hz());
  } 

  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    vibrato.set_depth(pedal.pot_left.val_log_inv);     
  } 

  // Center pot controls rate
  if (pedal.pot_center.has_changed()) { 
    float rate_hz = pedal.pot_center.val* 8.0;
    pedal.set_tap_blink_rate_hz(rate_hz);
    vibrato.set_rate_hz(rate_hz);     
  } 

  // Right pot sets the oscillator type
  if (pedal.pot_right.has_changed()) {
    if (pedal.pot_right.val < 0.33) {
      vibrato.set_lfo_type(OSC_SINE);
    } else if (pedal.pot_right.val < 0.66) {
      vibrato.set_lfo_type(OSC_TRIANGLE);
    } else {
      vibrato.set_lfo_type(OSC_RANDOM);
    }
  }

  // Run pedal service to take care of stuff
  pedal.service();  

}
