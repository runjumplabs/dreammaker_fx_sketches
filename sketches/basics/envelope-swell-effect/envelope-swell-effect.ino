/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Swell 

Effect description:  This is an example that shows how to use the fx_adsr_envelope
effect to create a swell-like effects.  The ADSR envelope standads for attack-decay-
sustain-release and controls how quickly a note fades and out.  The sketch also
demonstrates how to route the new note pedal event to the ADSR envelope so each time 
you play a new note, the ADSR envelope is applied.

Left pot label: Nothing
Left pot function: Nothing

Center pot label: Nothing
Center pot function: Nothing

Right pot label: Nothing
Right pot function: Nothing

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Nothing
Right footswitch function: Nothing

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.4.2
Sketch version: 1.0
*/ 
#include <dreammakerfx.h>

// Add your fx module declarations here
fx_adsr_envelope  env(250.0,  // Attack is 250ms
                      10.0,   // Decay is 10ms
                      10.0,   // Sustain is 10ms
                      500.0,  // Release is 500ms 
                      1.0);   // Full volume

void setup() {
  // put your setup code here, to run once:
  
  // Initialize the pedal!
  pedal.init();

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
  } 

  // Center pot control
  if (pedal.pot_center.has_changed()) { 
    // Center pot value is: pedal.pot_center.val
  } 

  // Right pot set control
  if (pedal.pot_right.has_changed()) {
    // Right pot value is: pedal.pot_right.val
  }

  // Run pedal service to take care of stuff
  pedal.service(); 
  
}
