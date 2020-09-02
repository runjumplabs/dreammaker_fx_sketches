/******************************************************************************
   DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
  Effect name: Crushinatrix
  Effect description: When you need to push your 11 to 11. A grotesque boost to further aggravate your neighbors and alienate your loved ones.

  Left pot label: Clipping threshold
  Left pot function: Turning this up decreases the clipping threshold.

  Center pot label: Tone
  Center pot function: Sculpt your tone.

  Right pot label: Input drive
  Right pot function: Influences the character of the underlying distortion.

  Left footswitch label: Bypass
  Left footswitch function: Bypasses the effect

  Right footswitch label: N/A
  Right footswitch function: N/A

  Youtube Url: 
  Soundcloud Url: https://soundcloud.com/modusjonens/crushinator-audio-tour

  Created by: joedougherty
  DreamMakerFx package version: 1.6.6
  Sketch version: 1.0
*/


/**

                    +-----------+    +--------+    +---------------+
                    |           |    |        |    |               |
  (PATH A)    +---->+ tighten   +--->+  grit  +--->+ tone_filter   +------+
              |     | (comp)    |    | (dist) |    | (med. filter) |      |
              |     +-----------+    +--------+    +---------------+      |
              |                                                      +----+-----+
  Instr In +--+                                                      |          |
              |                                                      | tee      +->Amp Out
              |                                                      | (mixer)  |        
              |                                                      +----+-----+
              |     +-----------+                                         |
              |     |           |                                         |
  (PATH B)    +---->+ dirt      +-----------------------------------------+
                    | (dist)    |
                    +-----------+


  Created with http://asciiflow.com/

*/

#include <dreammakerfx.h>


fx_compressor     tighten(-30.0,      // Initial threshold in dB
                          8,          // Initial ratio (1:8)
                          20.0,       // Attack (20ms)
                          1000.0,     // Release (1000ms)
                          2.0);       // Initial output gain (2.0x);

fx_destructor     grit(0.1,
                       2.0,
                       SMOOTH_FUZZ);

fx_biquad_filter  tone_filter(800.0,                // Initial filter cutoff freq
                              FILTER_WIDTH_MEDIUM,  // Filter width
                              BIQUAD_TYPE_BPF);     // Filter type is bandpass

fx_destructor     dirt(0.15,
                       4.0,
                       SMOOTHER_CLIP);

fx_mixer_2        tee;

void setup() {

  pedal.init();

  // Signal PATH A
  pedal.route_audio(pedal.instr_in, tighten.input);
  pedal.route_audio(tighten.output, grit.input);
  pedal.route_audio(grit.output, tone_filter.input);
  pedal.route_audio(tone_filter.output, tee.input_1);

  // Signal PATH B
  pedal.route_audio(pedal.instr_in, dirt.input);
  pedal.route_audio(dirt.output, tee.input_2);

  // Mix Path A + Path B and output
  pedal.route_audio(tee.output, pedal.amp_out);

  // Left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  pedal.run();

}


// An implementation of map [https://www.arduino.cc/reference/en/language/functions/math/map/]
// to work with floats.
// Source: https://circuitcrush.com/arduino/2015/03/09/arduino-map-floats-or-ints.html
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {

  // Left knob controls the Clipping threshold of grit
  // It is set up to run from 1.0 (highest threshold) to 0.5 (lowest threshold)
  // (Note that this may be the inverse of what you would anticipate!)
  if (pedal.pot_left.has_changed()) {
    float new_val = mapfloat(pedal.pot_left.val, 0.0, 1.0, 1.0, 0.5);
    grit.set_param_1(new_val);
  }

  // Center pot is tone knob from 200 to 1200Hz
  if (pedal.pot_center.has_changed()) {
    tone_filter.set_freq(200.0 + pedal.pot_center.val * 1400.0);
  }

  // Right knob controls the Input Drive. Ranges from 2.0 to 10.0.
  if (pedal.pot_right.has_changed()) {
    float new_val = mapfloat(pedal.pot_right.val * 100.0, 0, 100, 2.0, 10.0);
    grit.set_param_2(new_val);
  }

  // Run pedal service to take care of stuff
  pedal.service();

}
