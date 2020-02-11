/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*

Effect name: Double looper
Effect description:   This effect uses two independent loopers.  The duration 
of the second looper is synchronized to the first.  Hold down the left footswitch 
to record the first loop.  Then hold down the right footswitch to record a second layer.
Tapping either of the footswitches after the record will clear that loop.
The left pot controls loop 1 volume, the right pot controls loop 2 volume and the 
center pot controls a gentle low-pass filter over the loops.


Left pot label: Primary loop mix
Left pot function: Sets volume of primary loop controlled with left switch

Center pot label: Filter
Center pot function: Sets cutoff of low-pass filter that loops run through

Right pot label: Secondary loop mix
Right pot function: Sets the volume of the secondary loop controlled with right swtich

Left footswitch label: Record primary
Left footswitch function: Records the primary loop

Right footswitch label: Record secondary
Right footswitch function: Records the secondary loop

Youtube Url: 
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.2
Sketch version: 1.0
*/

/**

                    +-----------+
                    |           |
              +---->+ looper1   +---------+
              |     |           |         |
              |     +----+------+         v
              |          |           +----+-----+    +-----------+
              |          |           |          |    |           |
Instr In +----+          |sync       | mixer 2  +--->+ loop_filt +->Amp Out
              |          |           |          |    |           |
              |          v           +----+-----+    +-----------+
              |     +----+------+         ^
              |     |           |         |
              +---->+ looper2   +---------+
                    |           |
                    +-----------+


Created with http://asciiflow.com/

 */

#include <dreammakerfx.h>

// First looper set to max length of 15 seconds
fx_looper looper1(1.0,        // Dry mix of clean guitar
                  0.6,        // Loop mix
                  15,         // Maximum loop length in seconds
                  false);     // Disable FX send / receive 

// Second looper set to max length of 15 seconds
fx_looper looper2(0.0,        // Dry mix is 0.0 on this one since the other looper is passing guitar 
                  0.6,        // Loop mix
                  15,         // Maximum loop length in seconds 
                  false);     // Disable FX send / receive 

// Output low-pass filter
fx_biquad_filter  loop_filt(400.0,            // Initial cutoff is 400Hz
                            1.0,              // Resonance is 1.0 (standard)
                            BIQUAD_TYPE_LPF); // Filter type is lowpass

// Two channel mixer to mix loop outputs
fx_mixer_2 mixer_2ch;

void setup() {
  
  // Initialize the pedal
  pedal.init();  

  // Route instrument in to first looper
  pedal.route_audio(pedal.instr_in, looper1.input);
  pedal.route_audio(looper1.output, mixer_2ch.input_1);

  // Route instrument in to second looper
  pedal.route_audio(pedal.instr_in, looper2.input);
  pedal.route_audio(looper2.output, mixer_2ch.input_2);

  // Mix looper outputs, send through filter and then to amp out
  pedal.route_audio(mixer_2ch.output, loop_filt.input);
  pedal.route_audio(loop_filt.output, pedal.amp_out);

  // Link the length of the first loop to the lenght of the second loop
  pedal.route_control(looper1.loop_length_seconds, looper2.loop_length_seconds_set);
  
  // Run this effect
  pedal.run();
}

bool looper_1_active = false;
bool looper_2_active = false;

void loop() {

  // When left foot switch held down, record a loop (if one not playing)
  // or stop the loop that is currently playing and get ready for another 
  // recording
  if (pedal.button_pressed(FOOTSWITCH_LEFT, true)) {
    if (!looper_1_active) {
      looper1.start_loop_recording();
      looper_1_active = true;
    } else {
      looper1.stop_loop_playback();
      looper_1_active = false;
    }
  }
  if (pedal.button_released(FOOTSWITCH_LEFT, true)) {
    if (looper_1_active) {
      looper1.stop_loop_recording();      
    }
  }

  if (pedal.button_pressed(FOOTSWITCH_RIGHT, true)) {
   if (!looper_2_active) {
      looper2.start_loop_recording();
      looper_2_active = true;
    } else {
      looper2.stop_loop_playback();
      looper_2_active = false;
    }
  }

  if (pedal.button_released(FOOTSWITCH_RIGHT, true)) {
    if (looper_2_active) {
      looper2.stop_loop_recording();      
    }
  }  

  // Left pot changes the volume of the first loop
  if (pedal.pot_left.has_changed()) {
    looper1.set_loop_mix(pedal.pot_left.val);
  }
  // Right pot changes the volume of the second loop
  if (pedal.pot_right.has_changed()) {
    looper2.set_loop_mix(pedal.pot_right.val);
  }  
  
  // Center pot changes low-pass filter from 200 to 2000 Hz
  if (pedal.pot_center.has_changed()) {
    loop_filt.set_freq(200.0 + pedal.pot_center.val*1800.0);
  }    
  
    // Service 
  pedal.service();

}
