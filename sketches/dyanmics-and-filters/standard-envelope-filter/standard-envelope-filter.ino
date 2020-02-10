/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Describe a bit about how your effect works.  Feel free to use multiple lines, 
just don't delete or change any of the field names. 

Effect name: Add a clever name
Effect description:  What does it do

Left pot label: left pot label
Left pot function: describe left pot function

Center pot label: center pot label
Center pot function: describe center pot function

Right pot label: right pot label
Right pot function: describe right pot function

Left footswitch label: 
Left footswitch function: describe left footswitch function

Right footswitch label:
Right footswitch function: describe right footswitch function

Youtube Url: optional, add a URL to a youtube video
Soundcloud Url: optional, add a URL to a soundcloud audio clip

Created by: your alias
DreamMakerFx package version: DreamMakerFx package version used x.y.z (e.g. 1.4.2)
Version: version of your sketch (e.g. 1.0)
*/

/**
This is an example of an envelope filter or "auto-wah".  It uses the amplitude
of the incoming signal to the center frequency of a bandpass filter in real time.
The louder you play, the higher the filter is swept.


                          +------------+
                          |            |
Instr In +-------+------->+ Bandpass   +-------------------------> Amp Out
                 |        | filter     |
                 |        +-----+------+
                 |              ^
                 |              |  Control center frequency of filter
                 |        +-----+------+
                 |        |            |
                 +------->+ Envelope   |
                          | tracker    |
                          +------------+


*/
#include <dreammakerfx.h>

fx_envelope_tracker   envy_track1(10,     // 10 ms attack
                                  100,    // 100 ms release
                                  false,  // not triggered
                                  4000.0, // envelope scale
                                  200.0); // envelope offset 

fx_envelope_tracker   envy_track2(10,     // 10 ms attack
                                  100,    // 100 ms release
                                  false,  // not triggered
                                  6000.0, // envelope scale
                                  600.0); // envelope offset 

fx_envelope_tracker   envy_track3(10,     // 10 ms attack
                                  100,    // 100 ms release
                                  false,  // not triggered
                                  3000.0, // envelope scale
                                  1200.0); // envelope offset 


                                  
fx_biquad_filter      wah_filter_1(300.0,                 // 300 Hz starting frequency
                                 4.0,   
                                 BIQUAD_TYPE_BPF,
                                 BIQUAD_ORDER_4);      // Type is bandpass

fx_biquad_filter      wah_filter_2(300.0,                 // 300 Hz starting frequency
                                 4.0,   
                                 BIQUAD_TYPE_BPF,
                                 BIQUAD_ORDER_4);      // Type is bandpass

fx_biquad_filter      wah_filter_3(300.0,                 // 300 Hz starting frequency
                                 4.0,   
                                 BIQUAD_TYPE_BPF,
                                 BIQUAD_ORDER_4);      // Type is bandpass                                 

fx_mixer_3             mix3;

fx_gain                gain_up(2.0), gain1(1.0), gain2(1.5), gain3(2.0);

void setup() {
  
  // put your setup code here, to run once:
  pedal.init(true, true);

  // for template, just pass audio from input to output jack
  pedal.route_audio(pedal.instr_in, wah_filter_1.input);
  pedal.route_audio(pedal.instr_in, wah_filter_2.input);
  pedal.route_audio(pedal.instr_in, wah_filter_3.input);
  
  pedal.route_audio(wah_filter_1.output, gain1.input);
  pedal.route_audio(wah_filter_2.output, gain2.input);
  pedal.route_audio(wah_filter_3.output, gain3.input);
  
  pedal.route_audio(gain1.output, mix3.input_1);
  pedal.route_audio(gain2.output, mix3.input_2);
  pedal.route_audio(gain3.output, mix3.input_3);
  pedal.route_audio(mix3.output, gain_up.input);
  pedal.route_audio(gain_up.output, pedal.amp_out);

  pedal.route_audio(pedal.instr_in, envy_track1.input);
  pedal.route_audio(pedal.instr_in, envy_track2.input);
  pedal.route_audio(pedal.instr_in, envy_track3.input);
  
  pedal.route_control(envy_track1.envelope, wah_filter_1.freq);  
  pedal.route_control(envy_track2.envelope, wah_filter_2.freq);  
  pedal.route_control(envy_track3.envelope, wah_filter_3.freq);  

  // Left footswitch bypasses effect
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Optional code to print out the routing details to console
  if (true) {
    pedal.print_instance_stack();
    pedal.print_routing_table();
    pedal.print_param_tables();
  }
  
  // Run this effect
  pedal.run();

}
  
void loop() {

  // Add an event if pot 0 has changed
  if (pedal.pot_left.has_changed()) { 
    envy_track1.set_attack_speed_ms(pedal.pot_left.val*1000.0);
    envy_track2.set_attack_speed_ms(pedal.pot_left.val*1000.0);
    envy_track3.set_attack_speed_ms(pedal.pot_left.val*1000.0);

  } 

  // Add an event if pot 0 has changed
  if (pedal.pot_center.has_changed()) { 
    envy_track1.set_env_scale(3000.0*pedal.pot_center.val*3.0);
    envy_track2.set_env_scale(6000.0*pedal.pot_center.val*3.0);
    envy_track3.set_env_scale(3000.0*pedal.pot_center.val*3.0);
  } 

  // Add an event if pot 2 has changed
  if (pedal.pot_right.has_changed()) { 
  } 

  // Service 
  pedal.service();
}
