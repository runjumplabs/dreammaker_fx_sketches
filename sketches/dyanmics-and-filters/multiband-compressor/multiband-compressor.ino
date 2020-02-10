#include "dm_fx.h"

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

fx_biquad_filter  crossover_filt_lower(400.0,             // 400.0 Hz
                                       FILTER_WIDTH_WIDE, 
                                       BIQUAD_TYPE_LPF);  // Lowpass
fx_biquad_filter  crossover_filt_upper(400.0,             // 400.0 Hz
                                       FILTER_WIDTH_WIDE, 
                                       BIQUAD_TYPE_HPF);  // Highpass

fx_mixer_2 mixer;

bool  effect_bypassed = false;

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

  pedal.run();

  // pedal on initially
  digitalWrite(PIN_FOOTSW_LED_1, HIGH); // Turn on LED
    
}

void loop() {
  // put your main code here, to run repeatedly:

  // Add an event if pot 0 has changed
  if (pedal.pot_0.has_changed()) { 

    // Pot 0 changes the cross over frequency of the filters
    crossover_filt_lower.set_freq(100.0 + 1000.0*pedal.pot_0.val);
    crossover_filt_upper.set_freq(100.0 + 1000.0*pedal.pot_0.val);
    Serial.print("Crossover changed to: ");
    Serial.print(100.0 + 1000.0*pedal.pot_0.val);
    Serial.println("Hz");
    
  } 

  // Add an event if pot 1 has changed
  if (pedal.pot_1.has_changed()) { 
    comp_upper.set_attack(pedal.pot_1.val*100.0 + 10.0);
    comp_lower.set_attack(pedal.pot_1.val*100.0 + 10.0);

    Serial.print("Attack changed to: ");
    Serial.print(pedal.pot_1.val*100.0 + 10.0);
    Serial.println("ms");
  } 

  // Add an event if pot 2 has changed (DOESN'T WORK ON FIRST GEN HARDWARE YET)
  if (pedal.pot_2.has_changed()) { 
    comp_upper.set_threshold(-12.0 - (pedal.pot_2.val*60));
    comp_lower.set_threshold(-12.0 - (pedal.pot_2.val*60));

    comp_upper.set_output_gain(2 + pedal.pot_2.val*15);
    comp_lower.set_output_gain(2 + pedal.pot_2.val*15);

    Serial.print("Sustain changed to: ");
    Serial.println(pedal.pot_2.val);

  } 

  // Service 
  pedal.service();

  // Check if bypass status has changed and update LEDs
  if (effect_bypassed) {
    digitalWrite(PIN_FOOTSW_LED_1, LOW);  // Turn off LED
  } else {
    digitalWrite(PIN_FOOTSW_LED_1, HIGH);  // Turn off LED
  }
  

}

// Footswitch 1 - typically used for enable/bypass
void footswitch_1_pressed() { 
  
  // Bypass code
  if (effect_bypassed) {
    // If we are currently bypassed, switch pedal to active mode and turn on LED next to SW1
    pedal.enable_fx();
  } else {
    // If we are currently active, switch pedal to bypassed mode and turn off LED next to SW1
    pedal.bypass_fx();
  }
  effect_bypassed = !effect_bypassed; // Toggle bypassed state variable
}
