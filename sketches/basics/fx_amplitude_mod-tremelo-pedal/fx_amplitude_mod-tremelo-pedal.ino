/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Tremelo example using amplitude modulator 

Effect description:  This is an example that shows how to use the fx_amplitude_mod
effect to create a tremelo pedal effects.  The ADSR envelope standads for attack-decay-
sustain-release and controls how quickly a note fades and out.  The sketch also
demonstrates how to route the new note pedal event to the ADSR envelope so each time 
you play a new note, the ADSR envelope is applied.

Left pot label: Depth
Left pot function: The depth of the tremelo effect

Center pot label: Modulation rate
Center pot function: Rate of amplitude modulation

Right pot label: Type of modulation
Right pot function: From counterclockwise to clockwise is sine -> triangle -> square -> random

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Tap rate
Right footswitch function: Tap this switch at a tempo to set the rate of the tremelo

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.4.2
Sketch version: 1.0
*/  
#include <dreammakerfx.h>

// Add your fx module declarations here
fx_amplitude_mod  tremy(1.0,       // Rate is 1.0Hz (1 cycle per second)
                        0.5,       // Initial depth is 0.5 (50% volume reduction)
                        0.0,       // Initial phase is 0 degrees
                        OSC_SINE,  // Sine oscillator
                        false);    // Not using an external oscillator

void setup() {
  
  // Initialize the pedal!
  pedal.init();

  // Route audio from instrument -> fx_amplitude_mod -> amp
  pedal.route_audio(pedal.instr_in, tremy.input);
  pedal.route_audio(tremy.output, pedal.amp_out);  

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap loop length
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  // Run this effect
  pedal.run();

}

void loop() {

  // If new tempo has been tapped in, use that to control tremelo rate
  if (pedal.new_tap_interval()) { 
    tremy.set_rate_hz(pedal.get_tap_freq_hz());
  } 

  // Left pot controls depth
  if (pedal.pot_left.has_changed()) { 
    tremy.set_depth(pedal.pot_left.val);     
  } 

  // Center pot controls rate
  if (pedal.pot_center.has_changed()) { 
    float rate_hz = pedal.pot_center.val* 10.0;
    pedal.set_tap_blink_rate_hz(rate_hz);
    tremy.set_rate_hz(rate_hz);     
  } 

  // Right pot sets the oscillator type
  if (pedal.pot_right.has_changed()) {
    if (pedal.pot_right.val < 0.25) {
      tremy.set_lfo_type(OSC_SINE);
    } else if (pedal.pot_right.val < 0.5) {
      tremy.set_lfo_type(OSC_TRIANGLE);
    } else if (pedal.pot_right.val < 0.75) {
      tremy.set_lfo_type(OSC_SQUARE_SOFT);
    } else {
      tremy.set_lfo_type(OSC_RANDOM);
    }      
  }

   // Run pedal service to take care of stuff
  pedal.service();  
  
}
