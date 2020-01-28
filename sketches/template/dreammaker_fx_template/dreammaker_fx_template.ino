/*****************************************************************************
 * Add a description of your effect
 * 
 *****************************************************************************
 * Left pot: describe left pot function
 * Center pot: describe center pot function
 * Right pot: describe right pot function
 * 
 * Left footswitch: describe left footswitch function
 * Right footswitch: describe right footswitch function
 * 
 * Created by: your alias
 * DreamMakerFx package: DreamMakerFx package version used x.y.z (e.g. 1.4.2)
 * Version: version of your sketch (e.g. 1.0)
 * 
 * www.dreammakerfx.com
 * 
 ********************************************************************************/
#include <dreammakerfx.h>

// Add your fx module declarations here




void setup() {
  // put your setup code here, to run once:

    
  // put your setup code here, to run once:
  pedal.init();

  // Route audio from pedal.instr_in to pedal.amp_out
  pedal.route_audio(pedal.instr_in, );
  pedal.route_audio(vibrato.output, pedal.amp_out);  

  // Optionally use a bypass switch FOOTSWITCH_LEFT or FOOTSWITCH_RIGHT
  // pedal.add_bypass_button(FOOTSWITCH_LEFT);

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
