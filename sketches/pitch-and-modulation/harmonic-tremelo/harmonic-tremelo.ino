#include "dreammakerfx.h"

/*
A harmonic tremelo is basically a pair of tremelos that are 180 degrees out of 
phase.  The first tremelo is run through a low pass filter and the second
tremelo is run through a high pass filter.  The result is a tremelo type 
sounds but the volume remains rather constant.

             +-------+    +----------------------+    +-------+
             |       |    |                      |    |       |
             | LPF   |    | Tremelo              |    |       |
         +-->+ 500Hz +--->+ (0 initial phase)    +--->+       +-+
         |   |       |    |                      |    |       | |
         |   +-------+    +----------------------+    |       | |
Instr In |                                            | Mix 2 |  > Amp Out
         |   +-------+    +----------------------+    |       | |
         |   |       |    |                      |    |       | |
         +-->+ HPF   +--->+ Tremelo              +--->+       +-+
             | 500Hz |    | (180  initial phase) |    |       |
             |       |    |                      |    |       |
             +-------+    +----------------------+    +-------+

 */

// Amplitude modulator #1
fx_amplitude_mod    mod1(1.0,     // Rate (Hz)
                         0.8,     // Depth (0.0->1.0)
                         0,       // Initial phase (degrees)
                         OSC_SINE,// Oscillator type
                         false);  // Don't use external LFO
// Amplitude modulator #2                         
fx_amplitude_mod    mod2(1.0,     // Rate (Hz)
                         0.8,     // Depth (0.0->1.0)
                         180,     // Initial phase (degrees)
                         OSC_SINE,// Oscillator type 
                         false);  // Don't use external LFO

// Filters for splitting low and high ends
fx_biquad_filter    lpf(500.0,              // Initial filter cutoff freq
                        1.0,                // Normal resonance
                        BIQUAD_TYPE_LPF,    // Filter type
                        BIQUAD_ORDER_6);
                        
fx_biquad_filter    hpf(500.0,              // Initial filter cutoff freq
                        1.0,                // Normal resonance
                        BIQUAD_TYPE_HPF,    // Filter type
                        BIQUAD_ORDER_6);


// Simple two channel mixer
fx_mixer_2    mix2;

 void setup() {

  pedal.init(true);

  pedal.route_audio(pedal.instr_in, lpf.input);
  pedal.route_audio(pedal.instr_in, hpf.input);
  pedal.route_audio(lpf.output, mod1.input);
  pedal.route_audio(hpf.output, mod2.input);
  
  pedal.route_audio(mod1.output, mix2.input_1);
  pedal.route_audio(mod2.output, mix2.input_2);

  pedal.route_audio(mix2.output, pedal.amp_out);

  // Left foot switch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);
  
  // Run this effect
  pedal.run(); 
}


void loop() {

  static int now = millis();

  // Run pedal service to take care of stuff
  pedal.service();

  // If a new tempo has been tapped in, update the pedal
  if (pedal.new_tap_interval()) { 
    mod1.set_rate_hz(pedal.get_tap_freq_hz());
    mod2.set_rate_hz(pedal.get_tap_freq_hz());
  } 


  // Pot 0 / right pot changes the rate of the tremelo from 0 to 4Hz
  if (pedal.pot_0.has_changed()) {
    float new_rate_hz = pedal.pot_0.val*4.0;
    mod1.set_rate_hz(new_rate_hz);
    mod2.set_rate_hz(new_rate_hz);
    pedal.set_tap_blink_rate_hz(new_rate_hz);
  }

  // Pot 1 / middle pot changes cross over frequency between 300 and 1700Hz
  if (pedal.pot_1.has_changed()) {
    lpf.set_freq(300.0 + pedal.pot_1.val*700.0);
    hpf.set_freq(300.0 + pedal.pot_1.val*700.0);
  }

  // Pot 2 / left sets the depth of the modulation
  if (pedal.pot_2.has_changed()) {
    mod1.set_depth(pedal.pot_2.val);
    mod2.set_depth(pedal.pot_2.val);
    
  }
}
