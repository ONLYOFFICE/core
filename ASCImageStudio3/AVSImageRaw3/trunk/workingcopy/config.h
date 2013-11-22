#pragma once

// dcraw config


#define OPT_MAX_ADJUST_BRIGHTNESS	5.0
#define OPT_MIN_ADJUST_BRIGHTNESS	1.0/OPT_MAX_ADJUST_BRIGHTNESS;


#define	OPT_MAX_CORRECT_CHROMATIC_ABERRATION	(1.001+0.0001*7)
#define OPT_MIN_CORRECT_CHROMATIC_ABERRATION	(0.999-0.0001*7)

#define OPT_MAX_MULTIPLIER	9.0
#define OPT_MIN_MULTIPLIER	0.0

#define OPT_MAX_GREEN	2.5
#define OPT_MIN_GREEN	0.2

#define OPT_MAX_TEMPERATURE	15000.0
#define OPT_MIN_TEMPERATURE	2000.0


#define OPT_MIN_DENOISING	0.0
#define OPT_MAX_DENOISING	1000.0

namespace ImageRaw3{
enum EHighlightModes {
		clip = 0
		,unclip
		,blend,
		reconstruct0,
		reconstruct1,
		reconstruct2,
		reconstruct3,
		reconstruct4,
		reconstruct5,
		reconstruct6,
		highlightmodecount
	};
}

#define OPT_MAX_MEDIANFILTER_PASSES 10

