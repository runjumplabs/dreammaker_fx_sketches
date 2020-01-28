/**
 * This is an implementation of a typical delay / echo pedal.   
 * 
 * Left pot: depth - the depth of the tremelo effect
 * Center pot: delay time - modulation rate
 * Right pot: type of modulation - from counterclockwise to clockwise is sine -> triangle -> square -> random
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: tap - tap it a few times at a set interval to update the vibrato modulation rate
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_amplitude_mod block
 * 
 * 
 */
#include <dreammakerfx.h>

fx_adsr_envelope  env(250.0, 10.0, 10.0, 500.0, 1.0);

void setup() {
  
  // Initialize the pedal!
  pedal.init(true, true, true);

  // Route audio from instrument -> fx_amplitude_mod -> amp
  pedal.route_audio(pedal.instr_in, env.input);
  pedal.route_audio(env.output, pedal.amp_out);  

  pedal.route_control(pedal.new_note, env.start);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}

void loop() {


  // Left pot controls depth
  if (pedal.pot_left.has_changed()) { 
  //  tremy.set_depth(pedal.pot_left.val);     
  } 

  // Center pot controls rate
  if (pedal.pot_center.has_changed()) { 
  } 

  // Right pot sets the oscillator type
  if (pedal.pot_right.has_changed()) {
  }

   // Run pedal service to take care of stuff
  pedal.service();  
  
}
