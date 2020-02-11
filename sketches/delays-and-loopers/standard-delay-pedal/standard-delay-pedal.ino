/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Standard Digital Delay 

Effect description:  This is an example that shows how to use the fx_delay
effect to create a standard digital delay pedal.  This pedal is designed to use 
a delay buffer of 3 seconds but since the DreamMaker FX platform has several
minutes of delay RAM, much longer and more interesting delays can be created.

Left pot label: Feedback 
Left pot function: Basically how long the echo lasts before dying out

Center pot label: Delay time
Center pot function: How far apart the echos are (0.1 to 3 seconds)

Right pot label: Wet/dry mix 
Right pot function: The mix of clean audio and the echo effect.  All the way left 
is no effect at all and all the way right is effect only with none of the original 
dry signal.  Right in the middle (12:00) is a nice mix of the original dry signal
and the echo signal

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Tap delay
Right footswitch function: Tap it a few times at a set interval and the delay will lock on

Youtube Url:
Soundcloud Url:

Created by: DreamMaker
DreamMakerFx package version: 1.5.2
Sketch version: 1.0
*/    
#include <dreammakerfx.h>

// Add your fx module declarations here
fx_delay    my_delay(3000.0,  // 3000 ms / 3 seconds
                     0.6);    // 0.6 feedback ratio

void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init();

  // Route audio from instrument -> my_delay -> amp
  pedal.route_audio(pedal.instr_in, my_delay.input);
  pedal.route_audio(my_delay.output, pedal.amp_out);

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap delay length
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);

  pedal.run();
}

void loop() {

  // If new delay time has been tapped in, use that
  if (pedal.new_tap_interval()) { 
    my_delay.set_length_ms(pedal.get_tap_interval_ms());
  } 

  // Left pot changes the feedback of the delay (determining how long the echoes last)
  if (pedal.pot_left.has_changed()) {
    my_delay.set_feedback(pedal.pot_left.val);
  }
  
  // Right pot changes the wet / dry mix
  if (pedal.pot_right.has_changed()) {
    my_delay.set_dry_mix(1.0 - pedal.pot_right.val);
    my_delay.set_wet_mix(pedal.pot_right.val);
  }  
  
  // Center pot can also be used to change the delay length 
  // from 100ms to 3000ms
  if (pedal.pot_center.has_changed()) {
    float new_length_ms = 100.0 + pedal.pot_center.val*2900.0;
    my_delay.set_length_ms(new_length_ms);

    // When we change the delay lenght with a pot, also update the flashing light
    pedal.set_tap_blink_rate_ms(new_length_ms);
  }    
  
  // Service pedal
  pedal.service();
}
