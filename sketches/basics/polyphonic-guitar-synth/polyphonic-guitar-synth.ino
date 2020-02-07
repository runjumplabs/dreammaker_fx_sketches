/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Polyphonic guitar synth pedal
Effect description:  This is a polyphonic guitar synth meaning that it tracks
multiple strings.  It uses an FM synth engine along with ADSR envelope generator
and an output filter.  The pedal is configured by default to use a triangle
(OSC_TRIANGLE) wave that is modulated with a sine wave (OSC_SINE).  However, lots
of interesting sounds can be created by swapping these out with out types of oscillators
(e.g. OSC_SQUARE, OSC_RAMP_POS, OSC_RAMP_NEG, OSC_RANDOM, etc).

Left pot label: FM mod depth
Left pot function: The depth of the FM mod in the synthesizer

Center pot label: Attack time
Center pot function: The attack time of the synth sound (i.e. how long it takes 
to ramp to full volume when you play a new string)

Right pot label: Modulation frequency
Right pot function: The frequency of the FM modulator ranging from 0.25 x the note played
to 4.25x.

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label:
Right footswitch function: describe right footswitch function

Youtube Url: 
Soundcloud Url: https://soundcloud.com/dreammakerfx/polyphonic-synth-guitar-effect-on-dreammaker-fx

Created by: DreamMaker
DreamMakerFx package version: 1.5.1
Version: 1.0
*/
#include <dreammakerfx.h>


// Add your fx module declarations here

fx_instrument_synth synth(OSC_TRIANGLE,     // Primary oscillator
                         OSC_RAMP_NEG,          // FM mod oscillator
                         0.1,               // FM mod depth
                         1.0,               // Freq ratio (ratio of instrument frequency to synth frequency (1.0 = same)
                         2.0,               // FM mod frequency ratio (1.0 = same frequency as primary oscillator)
                         250.0,             // Attack time in milliseconds
                         3.0,               // Filter resonance
                         0.6);              // Filter response (0.0 to 1.0)
                         


void setup() {
  // put your setup code here, to run once:
   
  // Initialize the pedal!
  pedal.init();

  // Route audio through effects from pedal.instr_in to pedal.amp_out
  pedal.route_audio(synth.output, pedal.amp_out);  

  // Optionally use a bypass switch FOOTSWITCH_LEFT or FOOTSWITCH_RIGHT
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  if (true) {
      pedal.print_instance_stack();
      pedal.print_routing_table();
      pedal.print_param_tables();
    }

  // Optionally use a tap tempo / delay switch
  // Can also be FOOTSWITCH_LEFT or FOOTSWITCH_RIGHT but different than
  // the bypass switch
  // pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  // Run this effect
  pedal.run();

}


void loop() {
  // put your main code here, to run repeatedly:

  // Left pot control
  if (pedal.pot_left.has_changed()) { 
    // Left pot value is: pedal.pot_left.val
    synth.set_fm_mod_depth(pedal.pot_left.val);
  } 

  // Center pot control
  if (pedal.pot_center.has_changed()) { 
    // Center pot value is: pedal.pot_center.val
    synth.set_attack_ms(25.0 + pedal.pot_center.val*975.0);
  } 

  // Right pot set control
  if (pedal.pot_right.has_changed()) {
    // Right pot value is: pedal.pot_right.val
    float x = 0.25 + round(pedal.pot_right.val * 16.0)*0.25;
    synth.set_fm_mod_ratio(x);
    
  }

  // Run pedal service to take care of stuff
  pedal.service();  

}
