/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Echo looper 
Effect description:  This is a basic looper pedal that uses the route_control() 
function to pass the tapped loop length along to an echo effect. The echo effect 
is set to 1/4 the lenght of the loop so each time a new loop is set, the echo time 
is updated to.  The looper output also runs through a low-pass filter with a bit 
of resonance so you can turn the recorded loop into a cool background droning 
pattern.

Left pot label: Echo feedback
Left pot function: Feedback amount for echoes (how long it takes them to disappear)

Center pot label: Looper mix
Center pot function: Amount of looper in the output mix

Right pot label: Looper filter
Right pot function: Low pass filter with some gentle resonance on the output of the looper

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: Record / reset
Right footswitch function: Hold down to record a loop. Once loop is recorded, tap again
to clear the loop.  Now you're ready to record again.

Youtube Url: 
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.2
Sketch version: 1.0
*/
#include <dreammakerfx.h>
/*

                         Loop duration sets delay length
                    +-----------------------------------------+
                    |                                         |
                +---+----+    +--------+   +--------+   +-----v-----+
                |        |    |        |   |        |   |           |
                |        |    |        |   |        |   |           |
Instr In +--+-->+ Looper +--->+ Filter +-->+        +-->+   Delay   +-----> Amp Out
            |   |        |    |        |   |        |   |           |
            |   |        |    |        |   |        |   |           |
            |   +--------+    +--------+   |  Mix 2 |   +-----------+
            |                              |        |
            +----------------------------->+        |
                                           |        |
                                           |        |
                                           +--------+

 Created with http://asciiflow.com/
 
 */

fx_looper   loopy( 0.0,   // No dry mix
                   0.8,   // Looped audio mix
                   12,    // Max loop length in seconds
                   false); // Disable FX processing as audio enters the loop

fx_delay    echo(8000,    // Max lenght of 8 seconds
                 0.7 );   // Feedback of 0.7

fx_biquad_filter    looper_filter(300.0,                 // 300 Hz starting frequency
                                   2.0,                  // A tiny bit of resonance to provide some color
                                   0.0,                  // Filter gain: 0db
                                   BIQUAD_TYPE_LPF,      // Type is low-pass filter
                                   TRANS_VERY_SLOW,      // Transition speed very slow since controlling with pots
                                   BIQUAD_ORDER_4);      // Type is bandpass     
fx_mixer_2          mix2;

void setup() {
  
  // put your setup code here, to run once:
  pedal.init();

  pedal.route_audio(pedal.instr_in, loopy.input);
  pedal.route_audio(loopy.output, looper_filter.input);
  pedal.route_audio(looper_filter.output, mix2.input_1);
  pedal.route_audio(pedal.instr_in, mix2.input_2);
  pedal.route_audio(mix2.output, echo.input);
  pedal.route_audio(echo.output, pedal.amp_out);

  // Connect the tapped loop length of our looper to the delay size and divide by 1/4 and convert to milliseconds (so four echoes per loop)
  pedal.route_control(loopy.loop_length_seconds, 
                      echo.length_ms, 
                      250.0,        // Scale by 1000 * 1/4 (convert to ms and then divide by 4 so we get four echoes)
                      0);           // Offset = 0


  // Left footswitch bypasses effect
  pedal.add_bypass_button(FOOTSWITCH_LEFT);
  
  // Run this effect
  pedal.run();

}

bool looper_active = false;
  
void loop() {

  // When left foot switch held down, record a loop (if one not playing)
  // or stop the loop that is currently playing and get ready for another 
  // recording
  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
    if (!looper_active) {
      loopy.start_loop_recording();
      looper_active = true;
    } else {
      loopy.stop_loop_playback();
      looper_active = false;
    }
  }
  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
    if (looper_active) {
      loopy.stop_loop_recording();      
    }
  }

  // Left pot changes the feedback of the delay (determining how long the echoes last)
  if (pedal.pot_left.has_changed()) {
    echo.set_feedback(pedal.pot_left.val);
  }

  // Center pot sets amount of looper in final mix
  if (pedal.pot_center.has_changed()) {
    loopy.set_loop_mix(pedal.pot_center.val*2.0);
  }  

  // Right pot set low-pass filter on loop
  if (pedal.pot_right.has_changed()) {
    // The .val_log_inv provides an inverse log curve that's good for setting frequencies
    looper_filter.set_freq(200.0 + 3000.0 * pedal.pot_right.val_log_inv);
  }

  // Service 
  pedal.service();
}
