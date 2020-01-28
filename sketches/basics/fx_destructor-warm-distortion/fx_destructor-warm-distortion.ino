/**
 * This is an implementation of a basic distortion pedal   
 * 
 * Left pot: depth - the threshold where compression kicks in
 * Center pot: delay time - the amount to compress after threshold crossed
 * Right pot: type of modulation - output gain
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: drive boost - hold down to set drive to max
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_compressor block
 * 
 */
#include <dreammakerfx.h>
                              
fx_destructor       destruct(0.1,           // Clipping level (from 0 to 1.0) - lower is heavier distortion
                             4.0,           // Input drive 
                             SMOOTH_CLIP);  // Distortion function = fuzz

fx_biquad_filter    tone_filter(800.0,              // Initial filter cutoff freq
                                1.0,                // Standard resonance 
                                BIQUAD_TYPE_BPF);   // Filter type is bandpass


fx_gain             out_gain(1.0);
void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init(true, true);

  // Route audio through distortion, tone filter and then output gain
  pedal.route_audio(pedal.instr_in, destruct.input);
  pedal.route_audio(destruct.output, tone_filter.input);
  pedal.route_audio(tone_filter.output, out_gain.input);
  pedal.route_audio(out_gain.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}

float pot_drive = 1.0;
void loop() {

  // When right footswitch is pressed, max drive!
  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
    destruct.set_param_2(16);
  }
  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
    destruct.set_param_2(pot_drive);
  }

  // Left pot is distortion drive
  if (pedal.pot_left.has_changed()) {
    pot_drive = 1 + pedal.pot_left.val_log*16.0;
    destruct.set_param_2(pot_drive);
  }

  // Center pot is tone knob from 200 to 1200Hz
  if (pedal.pot_center.has_changed()) {
    tone_filter.set_freq(200.0 + pedal.pot_center.val*1400.0);
  }

  if (pedal.pot_right.has_changed()) {
    out_gain.set_gain(0.3333 + pedal.pot_right.val*3.0);
  }

  // Run pedal service to take care of stuff
  pedal.service();

}
