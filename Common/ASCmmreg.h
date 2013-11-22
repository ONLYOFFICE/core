#ifdef _MSC_VER
	#pragma once		// for MSVS
#endif	// #ifdef _MSC_VER

#ifndef _AVSMMREG_H_	// For Borland compiler
#define _AVSMMREG_H_


#define WAVE_FORMAT_AAC				0xA106
#define WAVE_FORMAT_AAC_LATM		0xA107
#define WAVE_FORMAT_OGG_RAW			0x674c
#define WAVE_FORMAT_OGG				0x674f
#define WAVE_FORMAT_MLP				0x674e
#define WAVE_FORMAT_TRUEHD			0x675a
#define WAVE_FORMAT_AC3COREHD		0x675b
#define WAVE_FORMAT_LPCM			0x674d
#define	WAVE_APPLE_LOSSLESS			0x999a
#define WAVE_FORMAT_FLAC			0x5555
#define WAVE_APPLE_ANY				0x0FA1
#define WAVE_FORMAT_TRANSFORM		0x9999
#define WAVE_FORMAT_MPC				0x0888
#define WAVE_FORMAT_AAC				0xA106
#define WAVE_FORMAT_ACELP			0x0136
#define WAVE_FORMAT_IMA_ADPCM_AMV	0x07E1
#define WAVE_FORMAT_IMA_ADPCM_QT	0x0BC9
#define WAVE_FORMAT_IMA_ADPCM_FLV	0x0FB1
#define WAVE_FORMAT_SPG726			0x0045
#define WAVE_FORMAT_AMR_AMV			0x0136
#define WAVE_FORMAT_G729			0x0777
#define WAVE_FORMAT_DTS_DS			0x0190
#define WAVE_FORMAT_FLAC			0x5555
#define WAVE_FORMAT_BPCM			0x2752

#define WAVE_FORMAT_IMA_ADPCM_QT	0x0BC9	//WAVE_FORMAT_IMA_ADPCM + 3000

#define WAVE_FORMAT_RA_1			0x3431	//RealAudio 1.0
#define WAVE_FORMAT_RA_2			0x3832	//RealAudio 2.0
#define WAVE_FORMAT_RA_COOK			0x6F63	//RealAudio COOK
#define WAVE_FORMAT_RA_SIPRO		0x6973	//RealAudio Sipro
#define WAVE_FORMAT_RA_ATRAC3		0x7461	//RealAudio ATRAC3
#define WAVE_FORMAT_RA_AC3			0x6E64	//RealAudio AC3
#define WAVE_FORMAT_RA_AAC			0x6172	//RealAudio AAC

#endif	// #ifndef _AVSMMREG_H_