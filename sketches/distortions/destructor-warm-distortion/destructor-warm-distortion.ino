/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Warm Distortion 

Effect description:  This is an example that shows how to use the fx_destructor
effect to create a smooth, tube-like distortion sound. To make this more interesting,
try adding additional destructor and tone (fx_biquad_filter) elements either in series
or in parallel.

Left pot label: Drive
Left pot function: Sets input drive of distortion

Center pot label: Tone
Center pot function: Sets tone of distortion

Right pot label: Gain
Right pot function: Sets output gain of distortion

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Drive boost
Right footswitch function: Sets drive to max while held down

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.3
Sketch version: 1.0
*/ 
#include <dreammakerfx.h>
                              
fx_destructor       destruct(0.1,           // Clipping level (from 0 to 1.0) - lower is heavier distortion
                             4.0,           // Input drive 
                             SMOOTH_CLIP);  // Distortion function = fuzz

fx_biquad_filter    tone_filter(800.0,              // Initial filter cutoff freq
                                2.0,                // Standard resonance 
                                BIQUAD_TYPE_LPF);   // Filter type is lowpass

void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init(true, true);

  // Route audio through distortion, tone filter and then output gain
  pedal.route_audio(pedal.instr_in, destruct.input);
  pedal.route_audio(destruct.output, tone_filter.input);
  pedal.route_audio(tone_filter.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  if (false) {
      pedal.print_instance_stack();
      pedal.print_routing_table();
      pedal.print_param_tables();
    }

  // Run this effect
  pedal.run();

}

float pot_drive = 1.0;
void loop() {

  // When right footswitch is pressed, max drive!
  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
    destruct.set_input_drive(16);
  }
  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
    // When footswitch is released, restore previous value
    destruct.set_input_drive(pot_drive);
  }

  // Left pot is distortion drive
  if (pedal.pot_left.has_changed()) {
    pot_drive = 1 + pedal.pot_left.val_log_inv*16.0;
    destruct.set_input_drive(pot_drive);
  }

  // Center pot is tone knob from 200 to 1200Hz
  if (pedal.pot_center.has_changed()) {
    tone_filter.set_freq(200.0 + pedal.pot_center.val*1400.0);
  }

  // Right pot is output gain
  if (pedal.pot_right.has_changed()) {
    destruct.set_output_gain(1.0 + pedal.pot_right.val*6.0);
  }

  // Run pedal service to take care of stuff
  pedal.service();

}
