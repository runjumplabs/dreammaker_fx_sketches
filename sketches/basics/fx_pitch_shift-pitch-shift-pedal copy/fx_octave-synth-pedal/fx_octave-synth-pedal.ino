/**
 * This is an implementation of a pitch shift pedal.   
 * 
 * Left pot: Dry / clean signal mix
 * Center pot: Pitch shift setting (oct down, fifth down, third down, third up, fifth up, octave up)
 * Right pot: wet / octave signal mix
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: nothing
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_pitch_shift block
 * 
 * 
 */
#include <dreammakerfx.h>

fx_octave    oct(OSC_TRIANGLE, 0.3, 0.7, 0.5, 0.2); 

void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init();

  // Route audio from instrument -> pitch_shift -> amp
  pedal.route_audio(pedal.instr_in, oct.input);
  pedal.route_audio(oct.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

   // Run this effect
  pedal.run();
}

void loop() {

  // Left pot changes dry signal gain
  if (pedal.pot_left.has_changed()) {
  }
  
  // Center pot changes pitch shift amount
  if (pedal.pot_center.has_changed()) {
  }  
  
  // Right pot changes mix gain
  if (pedal.pot_center.has_changed()) {
  }    
  
  // Service pedal
  pedal.service();
}
