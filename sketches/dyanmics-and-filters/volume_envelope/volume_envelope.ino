/**
 * This is an implementation of a typical flanger pedal.   
 * 
 * Left pot: depth - the depth of the flanger effect
 * Center pot: delay time - modulation rate
 * Right pot: feedback - the feedback has a big impact on the sound.  Full counter-clockwise is -1.0
 *                       center is 0.0, and full clock-wise is 1.0.
 * 
 * Left footswitch: bypass - turns on and off the effect
 * Right footswitch: tap - tap it a few times at a set interval to update the flanger modulation rate
 * 
 * This effect uses a tiny amount of the available processing power and memory.
 * It's provided as an example of how to use the various features of the fx_variable_delay block
 * 
 */
#include <dreammakerfx.h>

fx_variable_delay fm_guit(1.0, 0.4, 0.0, 20.0, 0.25, 1.0, OSC_TRIANGLE, true); 
fx_variable_delay fm_osc(1.0, 0.4, 0.0, 20.0, 0.0, 1.0, OSC_SINE, false); 
fx_adsr_envelope  env(125.0, 10.0, 10.0, 75.0, 1.0);

fx_oscillator   osc1(OSC_TRIANGLE, 1.0, 1.0); 

fx_biquad_filter  out_filt(1200.0, 2.0, BIQUAD_TYPE_LPF);

fx_gain       out_env(1.0), gain_del(1.0);

fx_delay      del(1000.0, 3000.0, 0.7, 0.0, 0.50, false);
fx_mixer_2    mix_2;

void setup() {
  
  // Initialize the pedal!
  pedal.init(true, true);

  // Route audio from instrument -> my_variable_delay -> amp
  pedal.route_audio(pedal.instr_in, out_env.input);
  pedal.route_audio(out_env.output, del.input);
  pedal.route_audio(del.output, mix_2.input_1);
  pedal.route_audio(pedal.instr_in, mix_2.input_2);
  pedal.route_audio(mix_2.output, pedal.amp_out);  


  pedal.route_control(env.value, out_env.gain);
  pedal.route_control(pedal.new_note, env.start);
  
  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  if (true) {
    pedal.print_instance_stack();
    pedal.print_routing_table();
    pedal.print_param_tables();
  }
  // Run this effect
  pedal.run();

}

int now;
void loop() {

  if (millis() > now + 1000) {
    Serial.println(dsp_status.note_freq);
    Serial.println(dsp_status.loading_percentage);
    Serial.println(dsp_status.new_note);
    now = millis();
  }
  float guit_freq = dsp_status.note_freq;
  //osc1.set_frequency(guit_freq);
  //fm_osc.set_rate_hz(guit_freq*1.01);

  if (dsp_status.note_freq) {
    out_filt.set_freq(min(100 + dsp_status.note_duration_ms, 2000));
  } else {
    out_filt.set_freq(100);
  }

  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    //fm_guit.set_depth(pedal.pot_left.val);     
  } 

  // Center pot controls rate
  if (pedal.pot_center.has_changed()) { 
    //fm_osc.set_depth(pedal.pot_center.val); 
  } 

  // Right pot controls the feedback (-1.0 to 1.0)
  if (pedal.pot_right.has_changed()) {
   //fm_guit.set_feedback(1.0 - pedal.pot_right.val*2.0);
  }

   // Run pedal service to take care of stuff
  pedal.service();  
}
