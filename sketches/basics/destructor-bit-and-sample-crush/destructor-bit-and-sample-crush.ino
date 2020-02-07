/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Bit and Sample Rate Crusher

Effect description: 
Uses the BIT_CRUSHER and the SAMPLE_RATE_CRUSHER models in the destructor to both 
reduce the bit resolution of the audio samples (bit crusher) and reduce the sample
rate (sample rate crusher).  A sample rate crusher does not use what's called an
anti-aliasing filter when it reduces the sample rate and as a result, it produces
all sorts of artifacts of the original signal at different frequencies.  


Left pot label: Bit crush depth
Left pot function: Sets the number of bits to drop off of each sample

Center pot label: Tone
Center pot function: Sets tone of the low-pass filter following the desctuctor

Right pot label: Sample rate crush depth
Right pot function: Sets the amount to reduce the sample rate by.

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Not used
Right footswitch function: Nothing

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.1
Sketch version: 1.0
*/ 
#include <dreammakerfx.h>
                              
fx_destructor       bit_crusher(0.0,            // Zero intensity initially
                                BIT_CRUSHER);   // Destructor model - bit crusher!

fx_destructor       sample_rate_crusher(0.0,                    // Zero intensity initially
                                        SAMPLE_RATE_CRUSHER);   // Destructor model - sample rate crushers


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
  pedal.route_audio(pedal.instr_in, bit_crusher.input);
  pedal.route_audio(bit_crusher.output, sample_rate_crusher.input);
  pedal.route_audio(sample_rate_crusher.output, tone_filter.input);
  pedal.route_audio(tone_filter.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}

void loop() {

  // Left pot is distortion drive
  if (pedal.pot_left.has_changed()) {
    // Parameter 1 on the bit crusher is mapped to the intensity
    bit_crusher.set_param_1(pedal.pot_left.val_log);
  }

  // Center pot is tone knob from 200 to 3600Hz
  if (pedal.pot_center.has_changed()) {
    tone_filter.set_freq(200.0 + pedal.pot_center.val*3400.0);
  }

  if (pedal.pot_right.has_changed()) {
    // Parameter 1 on the sample rate crusher is mapped to the intensity
    sample_rate_crusher.set_param_1(pedal.pot_right.val);
  }

  // Run pedal service to take care of stuff
  pedal.service();

}
