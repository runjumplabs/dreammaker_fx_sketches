/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Parametric filter 

Effect description:  This is an example that shows how to use the fx_biquad_filter
effect to create a parametric filter.  

Left pot label: Frequency
Left pot function: The filter center frequency

Center pot label: Gain
Center pot function: The gain of the filter (-12dB full counterclockwise to +12dB full clockwise)

Right pot label: Width
Right pot function: The width / resonance of the filter

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Nothing
Right footswitch function: Nothing

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.4.2
Sketch version: 1.0
*/   
#include <dreammakerfx.h>

// Add your fx module declarations here
fx_biquad_filter parametric(500.0,                // Initial center frequency
                            1.0,                  // standard resonance
                            0.0,                  // Set initial gain at 0 (no effect)
                            BIQUAD_TYPE_PEAKING,  // Peaking filter
                            TRANS_VERY_SLOW,      // Slow transitions so we don't hear any zipper noise from pots
                            BIQUAD_ORDER_4);      // 4th order filter (medium intense filtering effect)

void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
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
  // put your main code here, to run repeatedly:
 
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
