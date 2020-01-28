/**
 * This is an implementation of a typical compressor pedal   
 * 
 * Left pot: depth - the threshold where compression kicks in
 * Center pot: delay time - the amount to compress after threshold crossed
 * Right pot: type of modulation - output gain
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: gain boost - press to double instrument volume when effect engaged
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_compressor block
 * 
 */
#include <dreammakerfx.h>

fx_compressor compressor( -30.0,    // Threshold in dB
                          8,        // Ratio (1:8)
                          10.0,     // Attack (10ms)
                          100.0,    // Release (100ms)
                          2.0);     // Output gain (2x);

void setup() {

  // Initialize the pedal!
  pedal.init();

  // Route audio from instrument -> fx_compressor -> amp
  pedal.route_audio(pedal.instr_in, compressor.input);
  pedal.route_audio(compressor.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();
    
}

// Use this to save our gain setting for when gain boost footswitched released
float pot_gain_setting = 0;


void loop() {

  // The right footswitch is being used as a momentary gain boost of 2x
  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
    compressor.set_output_gain(pot_gain_setting*2.0);      
  }
  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
    compressor.set_output_gain(pot_gain_setting);
  }

  // Left pot changes threshold from -12dB to -72dB
  if (pedal.pot_left.has_changed()) { 
      compressor.set_threshold(-12.0 - (pedal.pot_left.val*60));
  } 

  // Center pot sets compression ratio
  if (pedal.pot_center.has_changed()) { 
    compressor.set_ratio(pedal.pot_center.val*100.0);
  } 

  // Right pot controls compressor output gain
  if (pedal.pot_right.has_changed()) { 
    pot_gain_setting = pedal.pot_right.val*6;
    compressor.set_output_gain(pot_gain_setting);
  } 

  // Service 
  pedal.service();

}
