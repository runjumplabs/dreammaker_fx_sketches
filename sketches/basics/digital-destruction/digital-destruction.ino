/**
 * This is an implementation of a digital destruction pedal that uses a bit crusher and a sample rate crusher
 * 
 * Left pot: bit crush
 * Center pot: sample rate crush 
 * Right pot: type of modulation - output gain
 * 
 * Left footswitch: bypass - turns on and off the effect
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_compressor block
 * 
 */
#include <dreammakerfx.h>
                              
fx_destructor       destruct_bits(8,              // shift by 8 bits
                                  0.0,            // Not used by bit crusher
                                  BIT_CRUSHER);   // Destruction type function = bit crusher

fx_destructor       destruct_sample(4.0,              // Initial filter cutoff freq
                                    0.0,                // Standard resonance 
                                    SAMPLE_RATE_CRUSHER);   // Destruction type function = sample rate crusher


fx_gain             out_gain(1.0);
void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init();

  // Route audio through destructors and then output gain stage
  pedal.route_audio(pedal.instr_in, destruct_bits.input);
  pedal.route_audio(destruct_bits.output, destruct_sample.input);
  pedal.route_audio(destruct_sample.output, out_gain.input);
  pedal.route_audio(out_gain.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  pedal.run();

}

void loop() {

  // Left pot is distortion drive
  if (pedal.pot_left.has_changed()) {
    destruct_bits.set_param_1(pedal.pot_left.val);
  }

  // Center pot is tone knob from 200 to 1200Hz
  if (pedal.pot_center.has_changed()) {
    destruct_sample.set_param_1(pedal.pot_center.val);
  }

  if (pedal.pot_right.has_changed()) {
    out_gain.set_gain(0.3333 + pedal.pot_right.val*3.0);
  }

  // Run pedal service to take care of stuff
  pedal.service();

}
