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

fx_biquad_filter parametric(500.0,                // Initial center frequency
                            1.0,                  // standard resonance
                            0.0,                  // Set initial gain at 0 (no effect)
                            BIQUAD_TYPE_PEAKING,  // Peaking filter
                            TRANS_VERY_SLOW,      // Slow transitions so we don't hear any zipper noise from pots
                            BIQUAD_ORDER_4);      // 4th order filter (medium intense filtering effect)

void setup() {
  
  // put your setup code here, to run once:
  pedal.init();

  // Route audio
  pedal.route_audio(pedal.instr_in, parametric.input);
  pedal.route_audio(parametric.output, pedal.amp_out);  

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap modulation rate 
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  // Run this effect
  pedal.run();

}


void loop() {


  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    parametric.set_freq(100.0 + pedal.pot_left.val*1200);     
  } 

  // Center pot filter gain from -12dB to 12dB
  if (pedal.pot_center.has_changed()) { 
    parametric.set_gain(-12.0 + pedal.pot_center.val*24.0);
  } 

  // Right pot controls filter width
  if (pedal.pot_right.has_changed()) {
      parametric.set_resonance(0.25 + pedal.pot_right.val*3.0);
  }

  // Run pedal service to take care of stuff
  pedal.service();  

}
