/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Compressor / Sustainer

Effect description:  This is an example that shows how to use the fx_compressor 
effect to create a compressor sustainer pedal

Left pot label: Threshold
Left pot function: The threshold where compression kicks in

Center pot label: Compression Rate
Center pot function: The amount to compress after threshold crossed

Right pot label: Gain
Right pot function: The output gain of the compressor

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Nothing
Right footswitch function: Nothing

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.2
Sketch version: 1.0
*/    
#include <dreammakerfx.h>

// Add your fx module declarations here
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
