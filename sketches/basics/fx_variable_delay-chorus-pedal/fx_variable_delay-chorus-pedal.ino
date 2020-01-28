#include <dreammakerfx.h>

fx_variable_delay chorus (1.0,          // Initial rate of 1 Hz (1 cycle / second)
                          0.1,          // Initial depth of 0.1
                          0.0,          // No feedback needed for chorus
                          1.0,          // Delay line length (also effects depth) in milliseconds
                          0.6,          // Clean mix set to 0.6
                          0.6,          // Delayed mix set to 0.6
                          OSC_SINE,     // Initial oscillator is sine wave
                          false);       // Don't use an external signal for oscillator

void setup() {
  
  // put your setup code here, to run once:
  pedal.init();

  // Route audio
  pedal.route_audio(pedal.instr_in, chorus.input);
  pedal.route_audio(chorus.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap modulation rate 
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  // Run this effect
  pedal.run();

}
  
void loop() {

  // Service 
  pedal.service();

  // If new mod rate has been tapped in, use that to control vibrato rate
  if (pedal.new_tap_interval()) { 
    chorus.set_rate_hz(pedal.get_tap_freq_hz());
  } 

  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    chorus.set_depth(pedal.pot_left.val * 0.5);     
  } 

  // Center pot controls rate
  if (pedal.pot_center.has_changed()) { 
    float rate_hz = pedal.pot_center.val* 6.0;
    pedal.set_tap_blink_rate_hz(rate_hz);
    chorus.set_rate_hz(rate_hz);     
  } 
  
  // Right pot sets the oscillator type
  if (pedal.pot_right.has_changed()) {
    if (pedal.pot_right.val < 0.33) {
      chorus.set_lfo_type(OSC_SINE);
    } else if (pedal.pot_right.val < 0.66) {
      chorus.set_lfo_type(OSC_TRIANGLE);
    } else {
      chorus.set_lfo_type(OSC_RANDOM);
    }
  }
  
}
