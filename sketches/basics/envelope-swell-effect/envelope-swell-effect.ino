/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Attack Decay Swell 

Effect description:  This is an example that shows how to use the fx_adsr_envelope
effect to control the attack and decay of a played note.  The ADSR envelope standads 
for attack-decay-sustain-release and controls how quickly a note fades and out.  
The sketch also demonstrates how to route the new note pedal event to the ADSR envelope 
so each time you play a new note, the ADSR envelope is applied.  By turning attack and release
way down, each note sounds like a quick pluck.  And increase attack creates a swell 
effect like an ebow.

Left pot label: Attack time
Left pot function: Lenght of attack from 50ms to 1000ms

Center pot label: Gain
Center pot function: Volume of output.  For longer swells, a bit more gain is needed.

Right pot label: Release time
Right pot function: Lenght of release from 50ms to 1000ms

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Nothing
Right footswitch function: Nothing

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.1
Sketch version: 1.0
*/ 
#include <dreammakerfx.h>

// Add your fx module declarations here
fx_adsr_envelope  env(250.0,  // Attack is 250ms
                      10.0,   // Decay is 10ms
                      10.0,   // Sustain is 10ms
                      500.0,  // Release is 500ms 
                      1.0,    // Sustain ratio
                      1.0);   // Full volume

void setup() {
  // put your setup code here, to run once:
  
  // Initialize the pedal!
  pedal.init(true, true, true);

  // Route audio through effects from pedal.instr_in to pedal.amp_out
  pedal.route_audio(pedal.instr_in, env.input);
  pedal.route_audio(env.output, pedal.amp_out);  

  pedal.route_control(pedal.new_note, env.start);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}

void loop() {
  // put your main code here, to run repeatedly:

  // Left pot control
  if (pedal.pot_left.has_changed()) { 
    // Left pot value is: pedal.pot_left.val
    env.set_attack_ms(50 + pedal.pot_left.val * 2000.0);
  } 

  // Center pot controls output volume
  if (pedal.pot_center.has_changed()) { 
    env.set_output_gain(1.0 + pedal.pot_center.val * 8.0);
  } 

  // Right pot set control
  if (pedal.pot_right.has_changed()) {
    // Right pot value is: pedal.pot_right.val
    env.set_release_ms(50 + pedal.pot_right.val * 1000.0);

  }

  // Run pedal service to take care of stuff
  pedal.service(); 
  
}
