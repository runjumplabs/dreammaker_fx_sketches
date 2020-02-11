/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*

Effect name: The Separator 
Effect description:  Sends all low notes through a delay so you can create rhythmic 
repeating low patterns by playing low notes.  And then play over them.  The effect
uses a tight low-pass filter to cut off all frequencies above 100-300Hz (controlled
via left pot).  The output of this runs through a compressor to make the bass notes 
solid with even volume.  And then the output of the compressor runs into the delay.

Left pot label: Low pass cutoff
Left pot function: The cutoff frequency of filter feeding delay

Center pot label: Delay time
Center pot function: Length of time between echoes

Right pot label: Bass gain
Right pot function: Output level of repeating bass notes

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Tap delay
Right footswitch function: Tap it a few times at a set interval and the delay will lock on

Youtube Url: https://youtu.be/YtXOiU8ff68
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.2
Sketch version: 1.0
*/
#include <dreammakerfx.h>

/*

                +----------+   +------------+   +----------+   +-------+
                |          |   |            |   |          |   |       |
Instr In +--+-->+ Low-pass +-->+ Compressor +-->+  Delay   +-->+       |   +--> Amp out
            |   |          |   |            |   |          |   |       |   |
            |   +----------+   +------------+   +----------+   | Mix 2 |   |
            |                                                  |       +---+
            |                                                  |       |
            +------------------------------------------------->+       |
                                                               |       |
                                                               +-------+

  Created with http://asciiflow.com/


 */

fx_biquad_filter    low_cutoff(250.0,              // Initial filter cutoff freq
                        1.0,                // Normal resonance
                        BIQUAD_TYPE_LPF,    // Filter type
                        BIQUAD_ORDER_8);

fx_compressor       compressor(-30,       // Threshold in dB
                                40,       // Ratio
                                20,       // Attack time (ms)
                                1000,     // Release time (ms)
                                1.0);     // Output gain

fx_pitch_shift      pitch_down(0.50);       // Pitch shift down by one octave


fx_delay            bass_delay(3000.0, 0.93);
fx_gain             bass_gain(2.0);

fx_mixer_2          mix2;                   // Mixer utility
                        
void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init(true);

  // Route audio
  pedal.route_audio(pedal.instr_in, low_cutoff.input);
  pedal.route_audio(low_cutoff.output, compressor.input);
  pedal.route_audio(compressor.output, bass_delay.input);
  pedal.route_audio(bass_delay.output, bass_gain.input);
  pedal.route_audio(bass_gain.output, mix2.input_1);

  pedal.route_audio(pedal.instr_in, mix2.input_2);
  pedal.route_audio(mix2.output, pedal.amp_out);

  // Left foot switch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap delay length
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);
    
  pedal.run();

}

void loop() {
  // put your main code here, to run repeatedly:

  // If new delay time has been tapped in, use that
  if (pedal.new_tap_interval()) { 
    bass_delay.set_length_ms(pedal.get_tap_interval_ms());
  } 
  
  if (pedal.pot_left.has_changed()) {
    low_cutoff.set_freq(50 + pedal.pot_left.val*300.0);
  }

  if (pedal.pot_center.has_changed()) {
    float steps = round(pedal.pot_center.val * 8.0)/8.0;
    bass_delay.set_length_ms(500 + 2000 * steps);

    // When we change the delay lenght with a pot, also update the flashing light
    pedal.set_tap_blink_rate_ms(500 + 2000 * steps);
    
  }

  if (pedal.pot_right.has_changed()) {
    bass_gain.set_gain(1.0 + pedal.pot_right.val*4.0);
  }


   // Run pedal service to take care of stuff
  pedal.service();

}
