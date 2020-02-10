/******************************************************************************
 * DreamMaker FX / www.dreammakerfx.com
 *****************************************************************************/
/*
Effect name: Slicer Distortion
Effect description:  Uses the slicer effect to distort the incoming signal, chop 
it up and send through different filters

Left pot label: Filter resonance
Left pot function: Sets the resonance for the filters 

Center pot label: Drive
Center pot function: Sets the drive of the distortion 

Right pot label: Speed
Right pot function: Sets the speed of the slicer

Left footswitch label: Bypass
Left footswitch function: Bypasses the effect

Right footswitch label: None
Right footswitch function: None

Youtube Url: 
Soundcloud Url: 

Created by: DreamMaker
DreamMakerFx package version: 1.5.1
Sketch version: 1.0
*/ 
#include <dreammakerfx.h>

fx_slicer       slice4(750.0, 4);     

fx_mixer_4      mix4;
fx_destructor   tube_clip(0.1,           // Clipping level (from 0 to 1.0) - lower is heavier distortion
                          16.0,           // Input drive 
                          SMOOTH_CLIP);  // Distortion function = fuzz

fx_biquad_filter  filt1(200, 3.0, BIQUAD_TYPE_LPF);
fx_biquad_filter  filt2(800, 3.0, BIQUAD_TYPE_LPF);
fx_biquad_filter  filt3(1100, 3.0, BIQUAD_TYPE_LPF);
fx_biquad_filter  filt4(1800, 3.0, BIQUAD_TYPE_LPF);


void setup() {
  // put your setup code here, to run once:

  // Initialize the pedal!
  pedal.init();
   
  pedal.route_audio(pedal.instr_in, tube_clip.input);
  pedal.route_audio(tube_clip.output, slice4.input);
 
  pedal.route_audio(slice4.output_1, filt1.input);
  pedal.route_audio(slice4.output_2, filt2.input);
  pedal.route_audio(slice4.output_3, filt3.input);
  pedal.route_audio(slice4.output_4, filt4.input);

  pedal.route_audio(filt1.output, mix4.input_1);
  pedal.route_audio(filt2.output, mix4.input_2);
  pedal.route_audio(filt3.output, mix4.input_3);
  pedal.route_audio(filt4.output, mix4.input_4);
  
  pedal.route_audio(mix4.output, pedal.amp_out);

  // Reset the slicer position when a new note is played
  pedal.route_control(pedal.new_note, slice4.start);
  
  // left footswitch is bypass
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

   // Run this effect
  pedal.run();
}

void loop() {

  // Left pot changes dry signal gain
  if (pedal.pot_left.has_changed()) {
    filt1.set_resonance(1.0 + pedal.pot_left.val * 5.0);
    filt2.set_resonance(1.0 + pedal.pot_left.val * 5.0);
    filt3.set_resonance(1.0 + pedal.pot_left.val * 5.0);
    filt4.set_resonance(1.0 + pedal.pot_left.val * 5.0);
  }
  
  // Center pot changes pitch shift amount
  if (pedal.pot_center.has_changed()) {
    tube_clip.set_input_drive(2.0 + pedal.pot_center.val * 32);
  }  
  
  // Right pot changes mix gain
  if (pedal.pot_right.has_changed()) {
    slice4.set_period_ms(500.0 + 1000 * (1.0 - pedal.pot_right.val));
  }    
  
  // Service pedal
  pedal.service();
}
