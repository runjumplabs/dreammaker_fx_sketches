/**
 * This is an implementation of a typical flanger pedal.   
 * 
 * Left pot: depth - the depth of the flanger effect
 * Center pot: delay time - modulation rate
 * Right pot: feedback - the feedback has a big impact on the sound.  Full counter-clockwise is -1.0
 *                       center is 0.0, and full clock-wise is 1.0.
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: tap - tap it a few times at a set interval to update the flanger modulation rate
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_variable_delay block
 * 
 */
#include <dreammakerfx.h>

fx_variable_delay flangey(1.0,            // Initial oscillator rate of 1Hz (1 cycle / second)
                          0.5,            // Initial depth of 0.5
                          0.4,            // Initial feedback of 0.4
                          OSC_TRIANGLE);  // Use a triangle oscillator

void setup() {
  
  // Initialize the pedal!
  pedal.init();

  // Route audio from instrument -> my_variable_delay -> amp
  pedal.route_audio(pedal.instr_in, flangey.input);
  pedal.route_audio(flangey.output, pedal.amp_out);  

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap loop length
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  // Run this effect
  pedal.run();

}


void loop() {

  // If new tempo has been tapped in, use that to control flange rate
  if (pedal.new_tap_interval()) { 
    flangey.set_rate_hz(pedal.get_tap_freq_hz());
  } 

  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    flangey.set_depth(pedal.pot_left.val);     
  } 

  // Center pot controls rate
  if (pedal.pot_center.has_changed()) { 
    float rate_hz = pedal.pot_center.val* 6.0;
    pedal.set_tap_blink_rate_hz(rate_hz);
    flangey.set_rate_hz(rate_hz);     
  } 

  // Right pot controls the feedback (-1.0 to 1.0)
  if (pedal.pot_right.has_changed()) {
   flangey.set_feedback(1.0 - pedal.pot_right.val*2.0);
  }

   // Run pedal service to take care of stuff
  pedal.service();  
}
