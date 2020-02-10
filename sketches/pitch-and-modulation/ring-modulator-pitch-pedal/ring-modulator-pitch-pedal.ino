/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*

Effect name: Typical Ring Modulator
Effect description:  The ring modulator effect moves all of the frequencies around
creating a very interesting and sometimes unwieldy sound.  

Left pot label: Frequency 
Left pot function: Sets the modulation frequency between 20 and 1000Hz

Center pot label: Depth
Center pot function: Sets the depth of modulation

Right pot label:
Right pot function: Not used

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label:
Right footswitch function: Not used

Youtube Url: 
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.1
Sketch version: 1.0
*/
#include <dreammakerfx.h>

fx_ring_mod ring_mod(220.0,   // Freq
                      1.0,    // Depth        
                      true);  // Enable filter

void setup() {
  
  // put your setup code here, to run once:
  pedal.init();

  // Route audio
  pedal.route_audio(pedal.instr_in, ring_mod.input);
  pedal.route_audio(ring_mod.output, pedal.amp_out);  

  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Run this effect
  pedal.run();

}


void loop() {


  // Left pot controls depth of the effect
  if (pedal.pot_left.has_changed()) { 
    ring_mod.set_freq(20 + 980*pedal.pot_left.val);
    
  }
    

  // Center pot filter gain from -12dB to 12dB
  if (pedal.pot_center.has_changed()) { 
      ring_mod.set_depth(pedal.pot_center.val);
  } 

  // Right pot controls filter width
  if (pedal.pot_right.has_changed()) {
  }

  // Run pedal service to take care of stuff
  pedal.service();  

}
