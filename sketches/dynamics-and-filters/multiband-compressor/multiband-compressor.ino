/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*

Effect name: Multi-band compressor
Effect description:  Two-band compressor which uses a low-pass and high-pass
filter to separate the audio into two components, and then applies
independent compressors to each

Left pot label: Crossover freq
Left pot function: Crossover frequency between the two filters

Center pot label: Threshold
Center pot function: Compression threshold

Right pot label: Gain
Right pot function: Compressor output gain

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label:
Right footswitch function: describe right footswitch function

Youtube Url: 
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.3
Sketch version: 1.0
*/
#include <dreammakerfx.h>


/**
 Multiband compressor example uses two compressors which each act on a certain
 band of the signal.
 
                   +--------+     +------------+    +-------+
                   |        |     |            |    |       |
              +--->+  HPF   +---->+ Compressor +--->+       |
              |    |        |     |            |    |       |
              |    +--------+     +------------+    |       |
Instr In +----+                                     | Mixer +----> Amp Out
              |    +--------+     +------------+    |       |
              |    |        |     |            |    |       |
              +--->+  LPF   +---->+ Compressor +--->+       |
                   |        |     |            |    |       |
                   +--------+     +------------+    +-------+

Created with http://asciiflow.com/

 */


fx_compressor comp_upper( -30.0,    // Threshold in dB
                          8,        // Ratio (1:8)
                          10.0,     // Attack (10ms)
                          100.0,    // Release (100ms)
                          2.0);     // Output gain (2x);

fx_compressor comp_lower( -40.0,    // Threshold in dB
                          32,        // Ratio (1:8)
                          10.0,     // Attack (10ms)
                          100.0,    // Release (100ms)
                          2.0);     // Output gain (2x);

fx_biquad_filter  crossover_filt_lower(400.0,             // Initial frequency of  400.0 Hz
                                       1.0, 
                                       BIQUAD_TYPE_LPF,
                                       BIQUAD_ORDER_6);  
fx_biquad_filter  crossover_filt_upper(400.0,             // Initial frequency of 400.0 Hz
                                       1.0,
                                       BIQUAD_TYPE_HPF,   // Highpass
                                       BIQUAD_ORDER_6);  

fx_mixer_2 mixer;


void setup() {
  // put your setup code here, to run once:

  pedal.init();

  pedal.route_audio(pedal.instr_in, crossover_filt_lower.input);
  pedal.route_audio(pedal.instr_in, crossover_filt_upper.input);

  pedal.route_audio(crossover_filt_lower.output, comp_lower.input);
  pedal.route_audio(crossover_filt_upper.output, comp_upper.input);

  pedal.route_audio(comp_lower.output, mixer.input_1);
  pedal.route_audio(comp_upper.output, mixer.input_2);

  pedal.route_audio(mixer.output, pedal.amp_out);

  // Left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  pedal.run();

    
}

void loop() {
  // put your main code here, to run repeatedly:

  // Add an event if pot 0 has changed
  if (pedal.pot_left.has_changed()) { 

    // Pot 0 changes the cross over frequency of the filters
    crossover_filt_lower.set_freq(200.0 + 1000.0*pedal.pot_left.val_log_inv);
    crossover_filt_upper.set_freq(200.0 + 1000.0*pedal.pot_left.val_log_inv);    
  } 

  // Add an event if pot 1 has changed
  if (pedal.pot_center.has_changed()) { 
    comp_upper.set_threshold(-12.0 - (pedal.pot_center.val*40));
    comp_lower.set_threshold(-12.0 - (pedal.pot_center.val*40));

  } 

  // Add an event if pot 2 has changed (DOESN'T WORK ON FIRST GEN HARDWARE YET)
  if (pedal.pot_right.has_changed()) { 

    comp_upper.set_output_gain(2 + pedal.pot_right.val_log_inv*15);
    comp_lower.set_output_gain(2 + pedal.pot_right.val_log_inv*15);

  } 

  // Service 
  pedal.service();


}
