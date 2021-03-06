typedef struct {
  byte cc;
  byte min;
  byte max;
} Param;
const char DCO1_OCTAVE_S[] PROGMEM = "DCO1OCT";
const Param DCO1_OCTAVE{0, 1, 3};
const char DCO1_WAVEFORM_S[] PROGMEM = "DCO1WAV";
const Param DCO1_WAVEFORM{1, 1, 2};
const char DCO1_HARMONIC_SELECTOR_S[] PROGMEM = "DCO1HS";
const Param DCO1_HARMONIC_SELECTOR{2, 1, 16};
const char DCO1_VOLUME_S[] PROGMEM = "DCO1VOL";
const Param DCO1_VOLUME{6, 0, 31};
const char DCO_SINGLE_DOUBLE_MODE_S[] PROGMEM = "DCOMODE";
const Param DCO_SINGLE_DOUBLE_MODE{7, 1, 2};
const char DCO2_OCTAVE_S[] PROGMEM = "DCO2OCT";
const Param DCO2_OCTAVE{8, 1, 3};
const char DCO2_WAVEFORM_S[] PROGMEM = "DCO2WAV";
const Param DCO2_WAVEFORM{9, 1, 2};
const char DCO2_HARMONIC_SELECTOR_S[] PROGMEM = "DCO2HS";
const Param DCO2_HARMONIC_SELECTOR{10, 1, 16};
const char DCO2_VOLUME_S[] PROGMEM = "DCO2VOL";
const Param DCO2_VOLUME{14, 0, 31};
const char DCO2_INTERVAL_S[] PROGMEM = "DCO2INT";
const Param DCO2_INTERVAL{16, 0, 11};
const char DCO2_DETUNE_S[] PROGMEM = "DCO2DET";
const Param DCO2_DETUNE{17, 0, 3};
const char NOISE_LEVEL_S[] PROGMEM = "NOISE";
const Param NOISE_LEVEL{18, 0, 15};
const char VCF_CUT_OFF_SET_POINT_S[] PROGMEM = "VCFCUT";
const Param VCF_CUT_OFF_SET_POINT{24, 0, 99};
const char VCF_KEYBOARD_TRACK_S[] PROGMEM = "VCFKB";
const Param VCF_KEYBOARD_TRACK{25, 0, 2};
const char VCF_EG3_POLARITY_S[] PROGMEM = "VCFPOL";
const Param VCF_EG3_POLARITY{26, 1, 2};
const char VCF_EG3_DEPTH_S[] PROGMEM = "VCFDEP";
const Param VCF_EG3_DEPTH{27, 0, 15};
const char CHORUS_MK1_AND_EX800_MODES_ONLY_S[] PROGMEM = "CHOR";
const Param CHORUS_MK1_AND_EX800_MODES_ONLY{31, 0, 1};
const char EG1_ATTACK_S[] PROGMEM = "EG1A";
const Param EG1_ATTACK{32, 0, 31};
const char EG1_DECAY_S[] PROGMEM = "EG1D";
const Param EG1_DECAY{33, 0, 31};
const char EG1_BREAK_POINT_S[] PROGMEM = "EG1B";
const Param EG1_BREAK_POINT{34, 0, 31};
const char EG1_SLOPE_S[] PROGMEM = "EG1SL";
const Param EG1_SLOPE{35, 0, 31};
const char EG1_SUSTAIN_S[] PROGMEM = "EG1SU";
const Param EG1_SUSTAIN{36, 0, 31};
const char EG1_RELEASE_S[] PROGMEM = "EG1R";
const Param EG1_RELEASE{37, 0, 31};
const char EG2_ATTACK_S[] PROGMEM = "EG2A";
const Param EG2_ATTACK{40, 0, 31};
const char EG2_DECAY_S[] PROGMEM = "EG2D";
const Param EG2_DECAY{41, 0, 31};
const char EG2_BREAK_POINT_S[] PROGMEM = "EG2B";
const Param EG2_BREAK_POINT{42, 0, 31};
const char EG2_SLOPE_S[] PROGMEM = "EG2SL";
const Param EG2_SLOPE{43, 0, 31};
const char EG2_SUSTAIN_S[] PROGMEM = "EG2SU";
const Param EG2_SUSTAIN{44, 0, 31};
const char EG2_RELEASE_S[] PROGMEM = "EG3R";
const Param EG2_RELEASE{45, 0, 31};
const char EG3_ATTACK_S[] PROGMEM = "EG3A";
const Param EG3_ATTACK{48, 0, 31};
const char EG3_DECAY_S[] PROGMEM = "EG3D";
const Param EG3_DECAY{49, 0, 31};
const char EG3_BREAK_POINT_S[] PROGMEM = "EG3B";
const Param EG3_BREAK_POINT{50, 0, 31};
const char EG3_SLOPE_S[] PROGMEM = "EG3SL";
const Param EG3_SLOPE{51, 0, 31};
const char EG3_SUSTAIN_S[] PROGMEM = "EG3SU";
const Param EG3_SUSTAIN{52, 0, 31};
const char EG3_RELEASE_S[] PROGMEM = "EG3R";
const Param EG3_RELEASE{53, 0, 31};
const char PORTAMENTO_RATE_S[] PROGMEM = "PORT";
const Param PORTAMENTO_RATE{125, 1, 16};
const char POLY_MODE_S[] PROGMEM = "POLY";
const Param POLY_MODE{127, 1, 5};
const int NUM_PARAMS = 37;
const Param ALL_PARAMS[NUM_PARAMS] PROGMEM = {DCO1_OCTAVE, DCO1_WAVEFORM, DCO1_HARMONIC_SELECTOR, DCO1_VOLUME, DCO_SINGLE_DOUBLE_MODE, DCO2_OCTAVE, DCO2_WAVEFORM, DCO2_HARMONIC_SELECTOR, DCO2_VOLUME, DCO2_INTERVAL, DCO2_DETUNE, NOISE_LEVEL, VCF_CUT_OFF_SET_POINT, VCF_KEYBOARD_TRACK, VCF_EG3_POLARITY, VCF_EG3_DEPTH, CHORUS_MK1_AND_EX800_MODES_ONLY, EG1_ATTACK, EG1_DECAY, EG1_BREAK_POINT, EG1_SLOPE, EG1_SUSTAIN, EG1_RELEASE, EG2_ATTACK, EG2_DECAY, EG2_BREAK_POINT, EG2_SLOPE, EG2_SUSTAIN, EG2_RELEASE, EG3_ATTACK, EG3_DECAY, EG3_BREAK_POINT, EG3_SLOPE, EG3_SUSTAIN, EG3_RELEASE, PORTAMENTO_RATE, POLY_MODE};
const char* const PARAM_NAMES[NUM_PARAMS] PROGMEM = {DCO1_OCTAVE_S, DCO1_WAVEFORM_S, DCO1_HARMONIC_SELECTOR_S, DCO1_VOLUME_S, DCO_SINGLE_DOUBLE_MODE_S, DCO2_OCTAVE_S, DCO2_WAVEFORM_S, DCO2_HARMONIC_SELECTOR_S, DCO2_VOLUME_S, DCO2_INTERVAL_S, DCO2_DETUNE_S, NOISE_LEVEL_S, VCF_CUT_OFF_SET_POINT_S, VCF_KEYBOARD_TRACK_S, VCF_EG3_POLARITY_S, VCF_EG3_DEPTH_S, CHORUS_MK1_AND_EX800_MODES_ONLY_S, EG1_ATTACK_S, EG1_DECAY_S, EG1_BREAK_POINT_S, EG1_SLOPE_S, EG1_SUSTAIN_S, EG1_RELEASE_S, EG2_ATTACK_S, EG2_DECAY_S, EG2_BREAK_POINT_S, EG2_SLOPE_S, EG2_SUSTAIN_S, EG2_RELEASE_S, EG3_ATTACK_S, EG3_DECAY_S, EG3_BREAK_POINT_S, EG3_SLOPE_S, EG3_SUSTAIN_S, EG3_RELEASE_S, PORTAMENTO_RATE_S, POLY_MODE_S};
