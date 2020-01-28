/**
 * This is an implementation of a parametric filtering pedal.   
 * 
 * Left pot: filter center frequency
 * Center pot: filter gain (-12dB full counterclockwise to +12dB full clockwise)
 * Right pot: filter width
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: nothing
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_biquad_filter block
 * 
 */
#include <dreammakerfx.h>

fx_ring_mod ring_mod(220.0,   // Freq
                      1.0,    // Depth        
                      true);  // Enable filter

void setup() {
  
  // put your setup code here, to run once:
  pedal.init();

  // Route audio
  pedal.route_audio(pedal.instr_in, ring_mod.input);
  pedal.route_audio(ring_mod.output, pedal.amp_out);  

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}


void loop() {


  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    
  }
    

  // Center pot filter gain from -12dB to 12dB
  if (pedal.pot_center.has_changed()) { 
  } 

  // Right pot controls filter width
  if (pedal.pot_right.has_changed()) {
  }

  // Run pedal service to take care of stuff
  pedal.service();  

}
