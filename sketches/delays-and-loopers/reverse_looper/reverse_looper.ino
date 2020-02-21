/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*

Effect name: Reverse looper
Effect description:  This is a standard looper pedal.  However, when the right
footswitch is pressed, the playback of the loop is momentarily reversed.

Left pot label: 
Left pot function: Not used

Center pot label: 
Center pot function: Not used

Right pot label: 
Right pot function: Not used

Left footswitch label: Loop direction
Left footswitch function: When pressed, the loop is reversed

Right footswitch label: Record / reset
Right footswitch function: Hold down to record a loop. Once loop is recorded, tap again
to clear the loop.  Now you're ready to record again.

Youtube Url: 
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.3
Sketch version: 1.0
*/
#include <dreammakerfx.h>

fx_looper looper1(1.0, 1.0, 10, false);

void setup() {
  // put your setup code here, to run once:
  pedal.init();  

  pedal.route_audio(pedal.instr_in, looper1.input);
  pedal.route_audio(looper1.output, pedal.amp_out);
  
  // Run this effect
  pedal.run();
}

bool looper_active = false;

void loop() {
  // put your main code here, to run repeatedly:

  if (pedal.button_pressed(FOOTSWITCH_LEFT, true)) {
    looper1.set_playback_rate(-1.0);
  }
  if (pedal.button_released(FOOTSWITCH_LEFT, true)) {
    looper1.set_playback_rate(1.0);
  }

  // When left foot switch held down, record a loop (if one not playing)
  // or stop the loop that is currently playing and get ready for another 
  // recording
  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
    if (!looper_active) {
      looper1.start_loop_recording();
      looper_active = true;
    } else {
      looper1.stop_loop_playback();
      looper_active = false;
    }
  }
  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
    if (looper_active) {
      looper1.stop_loop_recording();      
    }
  }
    // Service 
  pedal.service();

}
