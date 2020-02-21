/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Flip-flip Octaviser

Effect description: 
Uses the FLIP FLOP OCTAVE model in the destructor to emulate a flip flop circuit to
generate tones at the frequency you're playing at or octaves below.  This is basically
how the original Boss OC-2 pedals worked.  


Left pot label: Octave
Left pot function: Sets the frequency of the flip-flop.  Full left is same as note you're playing
                   and full right is 2 octaves down.

Center pot label: Tone
Center pot function: Sets tone of the low-pass filter following the desctuctor

Right pot label: Clean mix
Right pot function: Sets the amount of clean signal mixed in.

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Not used
Right footswitch function: Nothing

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.3
Sketch version: 1.0
*/ 
#include <dreammakerfx.h>
                              
fx_destructor       destruct(1.0,                 // Octaves down
                             0.0,                 // Clean mix
                             1.0,                 // Output gain
                             FLIP_FLOP_OCTAVE);   // Destructor model - flip flop

fx_biquad_filter    tone_filter(800.0,              // Initial filter cutoff freq
                                1.0,                // Standard resonance 
                                BIQUAD_TYPE_LPF,    // Filter type is lowpass
                                BIQUAD_ORDER_6);    // 6th order filter


fx_gain             out_gain(1.0);
void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init();

  // Route audio through distortion, tone filter and then output gain
  pedal.route_audio(pedal.instr_in, destruct.input);
  pedal.route_audio(destruct.output, tone_filter.input);
  pedal.route_audio(tone_filter.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}

void loop() {

  // Left pot is distortion drive
  if (pedal.pot_left.has_changed()) {
    // Parameter 1 on the flip flop octave model is mapped to octave drop
    destruct.set_param_1(pedal.pot_left.val * 3);
  }

  // Center pot is tone knob from 200 to 1200Hz
  if (pedal.pot_center.has_changed()) {
    tone_filter.set_freq(200.0 + pedal.pot_center.val*1400.0);
  }

  if (pedal.pot_right.has_changed()) {
    // Parameter 2 on the flip flop octave is mapped to clean mix
    destruct.set_param_2(pedal.pot_right.val);
  }

  // Run pedal service to take care of stuff
  pedal.service();

}
