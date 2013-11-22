#pragma once

#include <ImageSerializeObjects.h>
#include <TransformConstants.h>
#include <ImageTransformsCore.h>
#include <ImageTransformsCoreConstants.h>
#include <ImageTransformsCoreWrappers.h>
#include <ImageTransformsCoreGdiplus.h>
#include <ImageTransformsCoreBGRA.h>
#include <ImageTransformsCoreCOM.h>
#include <ImagePaintCompose.h>

//#pragma message ("TODO : ImageStudio::Paint::Compose::Constants::c_nClock3AnglesReverse	    - данный эффект не правильно настроен")
//#pragma message ("TODO : ImageStudio::Paint::Compose::Constants::c_nClock3AnglesClockwise   - данный эффект не правильно настроен")
//#pragma message ("TODO : ImageStudio::Paint::Compose::Constants::c_nSlideSide*				- данный эффект не правильно настроен")

namespace ImageStudio
{
	namespace Paint
	{
		namespace Animations
		{
			namespace Constants
			{
				static const int c_nEffectAnimationEntranceType					=	0x00000001;
				static const int c_nEffectAnimationExitType						=	0x00000002;
				static const int c_nEffectAnimationEmphasisType					=	0x00000003;
				static const int c_nEffectAnimationMotionPathType				=	0x00000004;

				static const int c_nEffectAnimationCustom						=	0x00000000;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationAppear						=	0x00000001;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFlyIn						=	0x00000002;		//+ 
				static const int c_nEffectAnimationBlinds						=	0x00000003;		//- 
				static const int c_nEffectAnimationBox 							=	0x00000004;		//+ 
				static const int c_nEffectAnimationCheckBoard 					=	0x00000005;		//+ 
				static const int c_nEffectAnimationCircle 						=	0x00000006;		//+
				static const int c_nEffectAnimationCrawl 						=	0x00000007;		//  
				static const int c_nEffectAnimationDiamond 						=	0x00000008;		//+  
				static const int c_nEffectAnimationDissolve 					=	0x00000009;		//- The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFade 						=	0x0000000A;		//+ The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFlashOnce 					=	0x0000000B;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationPeek 						=	0x0000000C;		//  
				static const int c_nEffectAnimationPlus 						=	0x0000000D;		//+ 
				static const int c_nEffectAnimationRandomBars 					=	0x0000000E;		//+  
				static const int c_nEffectAnimationSpiral 						=	0x0000000F;		//+	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationSplit 						=	0x00000010;		//+ 
				static const int c_nEffectAnimationStretch 						=	0x00000011;		//  
				static const int c_nEffectAnimationStrips 						=	0x00000012;		//+  
				static const int c_nEffectAnimationSwivel 						=	0x00000013;		//  
				static const int c_nEffectAnimationWedge 						=	0x00000014;		//+	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationWheel 						=	0x00000015;		//+  
				static const int c_nEffectAnimationWipe 						=	0x00000016;		//+   
				static const int c_nEffectAnimationZoom 						=	0x00000017;		//+  
				static const int c_nEffectAnimationRandomEffects				=	0x00000018;		//	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationBoomerang 					=	0x00000019;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationBounce 						=	0x0000001A;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationColorReveal 					=	0x0000001B;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationCredits 						=	0x0000001C;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEaseIn 						=	0x0000001D;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFloat 						=	0x0000001E;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationGrowAndTurn 					=	0x0000001F;		//	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationLightSpeed 					=	0x00000022;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationPinWheel 					=	0x00000023;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationRiseUp						=	0x00000025;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationSwish 						=	0x00000026;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationThinLine 					=	0x00000027;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationUnfold 						=	0x00000028;		//	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationWhip 						=	0x00000029;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationAscend 						=	0x0000002A;		//	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationCenterRevolve				=	0x0000002B;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFadedSwivel 					=	0x0000002D;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationDescend 						=	0x0000002F;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationSling 						=	0x00000030;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationSpinner 						=	0x00000031;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationCompress 					=	0x00000032;		//	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationZip 							=	0x00000033;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationArcUp 						=	0x00000034;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFadedZoom 					=	0x00000035;		//+ The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationGlide 						=	0x00000036;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationExpand 						=	0x00000037;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFlip 						=	0x00000038;		//  The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationFold 						=	0x0000003A;		//  The corresponding effect direction MUST be ignored. 

				// эффекты MP формируются набором кривых и прямых
				static const int c_nEffectAnimationMPCustom 					=	0x00000000;  
				static const int c_nEffectAnimationMPCircle 					=	0x00000001;   
				static const int c_nEffectAnimationMPRightTriangle  			=	0x00000002;   
				static const int c_nEffectAnimationMPDiamond 					=	0x00000003;  
				static const int c_nEffectAnimationMPHexagon  					=	0x00000004;   
				static const int c_nEffectAnimationMPFivePointStar  			=	0x00000005;   
				static const int c_nEffectAnimationMPCrescentMoon  				=	0x00000006;   
				static const int c_nEffectAnimationMPSquare  					=	0x00000007;  
				static const int c_nEffectAnimationMPTrapezoid  				=	0x00000008;    
				static const int c_nEffectAnimationMPHeart  					=	0x00000009;  
				static const int c_nEffectAnimationMPOctagon  					=	0x0000000A;    
				static const int c_nEffectAnimationMPSixPointStar  				=	0x0000000B;   
				static const int c_nEffectAnimationMPFootball  					=	0x0000000C;  
				static const int c_nEffectAnimationMPEqualTriangle  			=	0x0000000D;   
				static const int c_nEffectAnimationMPParallelogram  			=	0x0000000E;   
				static const int c_nEffectAnimationMPPentagon  					=	0x0000000F;  
				static const int c_nEffectAnimationMPFourPointStar  			=	0x00000010;  
				static const int c_nEffectAnimationMPEightPointStar  			=	0x00000011;  
				static const int c_nEffectAnimationMPTeardrop  					=	0x00000012;  
				static const int c_nEffectAnimationMPPointyStar  				=	0x00000013;  
				static const int c_nEffectAnimationMPCurvedSquare  				=	0x00000014;    
				static const int c_nEffectAnimationMPCurvedX  					=	0x00000015;  
				static const int c_nEffectAnimationMPVerticalFigure8 			=	0x00000016;  
				static const int c_nEffectAnimationMPCurvyStar  				=	0x00000017;  
				static const int c_nEffectAnimationMPLoopDeLoop  				=	0x00000018;  
				static const int c_nEffectAnimationMPBuzzSaw  					=	0x00000019;    
				static const int c_nEffectAnimationMPHorizontalFigure8			=	0x0000001A;	   
				static const int c_nEffectAnimationMPPeanut  					=	0x0000001B;  
				static const int c_nEffectAnimationMPFigure8four  				=	0x0000001C;    
				static const int c_nEffectAnimationMPNeutron  					=	0x0000001D;  
				static const int c_nEffectAnimationMPSwoosh  					=	0x0000001E;  
				static const int c_nEffectAnimationMPBean  						=	0x0000001F;  
				static const int c_nEffectAnimationMPPlus  						=	0x00000020;  
				static const int c_nEffectAnimationMPInvertedTriangle			=	0x00000021;  
				static const int c_nEffectAnimationMPInvertedSquare  			=	0x00000022;  
				static const int c_nEffectAnimationMPLeft  						=	0x00000023;   
				static const int c_nEffectAnimationMPTurnRight  				=	0x00000024;   
				static const int c_nEffectAnimationMPArcDown  					=	0x00000025;   
				static const int c_nEffectAnimationMPZigzag  					=	0x00000026;  
				static const int c_nEffectAnimationMPSCurve2  					=	0x00000027;   
				static const int c_nEffectAnimationMPSineWave  					=	0x00000028;     
				static const int c_nEffectAnimationMPBounceLeft  				=	0x00000029;  
				static const int c_nEffectAnimationMPDown  						=	0x0000002A;    
				static const int c_nEffectAnimationMPTurnUp  					=	0x0000002B;  
				static const int c_nEffectAnimationMPArcUp  					=	0x0000002C;   
				static const int c_nEffectAnimationMPHeartBeat  				=	0x0000002D;    
				static const int c_nEffectAnimationMPSpiralRight  				=	0x0000002E;    
				static const int c_nEffectAnimationMPWave  						=	0x0000002F;  
				static const int c_nEffectAnimationMPCurvyLeft  				=	0x00000030;   
				static const int c_nEffectAnimationMPDiagonalDownRight 			=	0x00000031;   
				static const int c_nEffectAnimationMPTurnDown  					=	0x00000032;   
				static const int c_nEffectAnimationMPArcLeft  					=	0x00000033;   
				static const int c_nEffectAnimationMPFunnel  					=	0x00000034;  
				static const int c_nEffectAnimationMPSpring  					=	0x00000035;  
				static const int c_nEffectAnimationMPBounceRight  				=	0x00000036;    
				static const int c_nEffectAnimationMPSpiralLeft  				=	0x00000037;  
				static const int c_nEffectAnimationMPDiagonalUpRight  			=	0x00000038;   
				static const int c_nEffectAnimationMPTurnUpRight  				=	0x00000039;   
				static const int c_nEffectAnimationMPArcRight  					=	0x0000003A;  
				static const int c_nEffectAnimationMPScurve1  					=	0x0000003B;    
				static const int c_nEffectAnimationMPDecayingWave  				=	0x0000003C;    
				static const int c_nEffectAnimationMPCurvyRight  				=	0x0000003D;  
				static const int c_nEffectAnimationMPStairsDown  				=	0x0000003E;  
				static const int c_nEffectAnimationMPRight  					=	0x0000003F;   
				static const int c_nEffectAnimationMPUp  						=	0x00000040; 

				static const int c_nEffectAnimationEmphasisChangeFillColor		=	0x00000001;  //  
				static const int c_nEffectAnimationEmphasisChangeFont			=	0x00000002;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisChangeFillColor2		=	0x00000003;  //  
				static const int c_nEffectAnimationEmphasisChangeFontSize 		=	0x00000004;  //
				static const int c_nEffectAnimationEmphasisChangeFontStyle 		=	0x00000005;  //
				static const int c_nEffectAnimationEmphasisGrowAndShrink 		=	0x00000006;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisChangeFillColor3		=	0x00000007;  //  
				static const int c_nEffectAnimationEmphasisSpin 				=	0x00000008;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisTransparency 		=	0x00000009;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisBoldFlash 			=	0x0000000A;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisBlast 				=	0x0000000E;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisBoldReveal 			=	0x0000000F;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisBrushOnColor 		=	0x00000010;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisBrushOnUnderline 	=	0x00000012;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisColorBlend			=	0x00000013;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisColorWave 			=	0x00000014;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisComplementaryColor 	=	0x00000015;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisComplementaryColor2	=	0x00000016;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisContrastingColor 	=	0x00000017;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisDarken 				=	0x00000018;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisDesaturate 			=	0x00000019;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisFlashBulb			=	0x0000001A;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisFlicker 				=	0x0000001B;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisGrowWithColor 		=	0x0000001C;  // The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisLighten 				=	0x0000001E;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisStyleEmphasis 		=	0x0000001F;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisTeeter 				=	0x00000020;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisVerticalGrow 		=	0x00000021;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisWave 				=	0x00000022;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisBlink 				=	0x00000023;  //	The corresponding effect direction MUST be ignored. 
				static const int c_nEffectAnimationEmphasisShimmer 				=	0x00000024;  //	The corresponding effect direction MUST be ignored. 
			}
		}

		namespace Animations
		{
			// временно отключена анимация. если она и будет, то в AVSGraphics

			// эффекты с compose
			//class CComposeLinker 
			//{
			//public:

			//	CComposeLinker ()
			//	{
			//		m_nComposeEffect	=	0;
			//		m_nComposeTitleX	=	0;
			//		m_nComposeTitleY	=	0;

			//		m_bSwapSources		=	false;
			//		m_bNoneCompose		=	false;
			//	}

			//	virtual ~CComposeLinker ()
			//	{

			//	}

			//
			//	inline bool VerifyEffect ( int Effect, int EffectType, int EffectDirection, double Progress )
			//	{
			//		m_nEffectDirection	=	EffectDirection;
			//		m_nEffectType		=	EffectType;
			//	
			//		m_nProgress			=	Progress;
			//		
			//		//if ( 1.0 < m_nProgress )
			//		{
			//		//	m_nProgress		=	1.0;
			//		}
			//		
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationWheel			==	Effect )	{	OnEffectAnimationWheel		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationCircle		==	Effect )	{	OnEffectAnimationCircle		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationBox			==	Effect )	{	OnEffectAnimationBox		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationDiamond		==	Effect )	{	OnEffectAnimationDiamond	();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationPlus			==	Effect )	{	OnEffectAnimationPlus		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationWipe			==	Effect )	{	OnEffectAnimationWipe		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationSplit			==	Effect )	{	OnEffectAnimationSplit		();	return true;	}
			//		// if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationDissolve		==	Effect )	{	OnEffectAnimationDissolve	();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationRandomBars	==	Effect )	{	OnEffectAnimationRandomBars	();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationStrips		==	Effect )	{	OnEffectAnimationStrips		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationWedge			==	Effect )	{	OnEffectAnimationWedge		();	return true;	}
			//		//if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationBlinds		==	Effect )	{	OnEffectAnimationBlinds		();	return true;	}
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationCheckBoard	==	Effect )	{	OnEffectAnimationCheckBoard	();	return true;	}

			//		// TODO : все Compose Effects

			//		return false;
			//	}

			//	inline bool ApplyEffect ( Gdiplus::Bitmap* pBitmap, Gdiplus::Bitmap** ppBitmap = NULL )		//	указатель на картинку для компоуза картинок
			//	{
			//		if ( m_bNoneCompose )
			//			return true;

			//		Gdiplus::BitmapData oData;
			//		if ( Gdiplus::Ok == pBitmap->LockBits ( NULL, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &oData ) )
			//		{
			//			ProcessComposeEffect ( pBitmap->GetWidth(), pBitmap->GetHeight(), (BYTE*)oData.Scan0, ppBitmap );
			//			pBitmap->UnlockBits ( &oData );

			//			return true;
			//		}

			//		return false;
			//	}

			//	inline bool IsNoneEffect ()
			//	{
			//		return m_bNoneCompose;
			//	}
			//
			//protected:

			//	inline int ProcessComposeEffect ( int Width, int Height, BYTE* pSource, Gdiplus::Bitmap** ppBitmap = NULL )
			//	{
			//		if ( 0.0 == m_nProgress )
			//			return 1;

			//		BYTE* pCompose				=	new BYTE [ 4 * Width * Height ];
			//		if ( NULL == pCompose )
			//			return 0;

			//		memset ( pCompose, 0L,  4 * Width * Height );

			//		int ComposeSuccess			=	false;

			//		ImageStudio::Paint::Compose::CImageComposer	Composer;
			//		if ( NULL == ppBitmap )
			//		{
			//			if ( m_bSwapSources )
			//			{
			//				ComposeSuccess		=	Composer.Transform ( pCompose, pSource, pSource, Width, Height, m_nComposeTitleX, m_nComposeTitleY, m_nComposeEffect, m_nProgress );
			//			}
			//			else
			//			{
			//				ComposeSuccess		=	Composer.Transform ( pSource, pCompose, pSource, Width, Height, m_nComposeTitleX, m_nComposeTitleY, m_nComposeEffect, m_nProgress );
			//			}
			//		}
			//		else
			//		{
			//			Gdiplus::Bitmap* pBitmap =	new Gdiplus::Bitmap ( Width, Height, PixelFormat32bppARGB );
			//			if ( NULL == pBitmap )
			//			{
			//				delete [] pCompose;

			//				return false;
			//			}

			//			if ( m_bSwapSources )
			//			{
			//				ComposeSuccess		=	Composer.Transform ( pCompose, pSource, pCompose, Width, Height, m_nComposeTitleX, m_nComposeTitleY, m_nComposeEffect, m_nProgress );
			//			}
			//			else
			//			{
			//				ComposeSuccess		=	Composer.Transform ( pSource, pCompose, pCompose, Width, Height, m_nComposeTitleX, m_nComposeTitleY, m_nComposeEffect, m_nProgress );
			//			}

			//			BitmapData oBitmapData; 
			//			if ( Gdiplus::Ok == pBitmap->LockBits ( NULL, ImageLockModeWrite, PixelFormat32bppARGB, &oBitmapData ) )
			//			{
			//				memcpy ( oBitmapData.Scan0, pCompose, oBitmapData.Stride * Height );

			//				pBitmap->UnlockBits ( &oBitmapData );
			//			}

			//			*ppBitmap	=	pBitmap;
			//		}

			//		delete [] pCompose;

			//		return ComposeSuccess;
			//	}


			//	inline int OnEffectAnimationWheel ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000001 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClockBottomReverse;

			//			if ( 0x00000002 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock2AnglesReverse;

			//			if ( 0x00000003 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock3AnglesReverse;

			//			if ( 0x00000004 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock4AnglesReverse;

			//			if ( 0x00000008 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock8AnglesReverse;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000001 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClockBottomClockwise;

			//			if ( 0x00000002 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock2AnglesClockwise;

			//			if ( 0x00000003 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock3AnglesClockwise;

			//			if ( 0x00000004 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock4AnglesClockwise;

			//			if ( 0x00000008 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClock8AnglesClockwise;
			//		}

			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationCircle ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleIn;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleOut;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleOut;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCircleIn;
			//		}
			//		
			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationBox ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxIn;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxOut;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxOut;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterBoxIn;
			//		}
			//		
			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationDiamond ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondIn;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondOut;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondOut;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterDiamondIn;
			//		}
			//		
			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationPlus ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossIn;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossOut;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000010 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossOut;

			//			if ( 0x00000020 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCenterCrossIn;
			//		}
			//		
			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationWipe ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000001 == m_nEffectDirection )	//	UP
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealUp;

			//			if ( 0x00000002 == m_nEffectDirection )	//	RIGHT
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealLeft;
			//			
			//			if ( 0x00000004 == m_nEffectDirection )	//	DOWN
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealDown;
			//			
			//			if ( 0x00000008 == m_nEffectDirection )	//	LEFT
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealRight;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000001 == m_nEffectDirection )	//	UP
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealDown;

			//			if ( 0x00000002 == m_nEffectDirection )	//	RIGHT
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealRight;
			//			
			//			if ( 0x00000004 == m_nEffectDirection )	//	DOWN
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealUp;
			//			
			//			if ( 0x00000008 == m_nEffectDirection )	//	LEFT
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nRevealLeft;
			//		}
			//		
			//		m_nComposeTitleX		=	-2;
			//		m_nComposeTitleY		=	-2;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationSplit ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x0000001A == m_nEffectDirection )	//	Horizontal in
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitHorizontal;
			//				
			//				m_bSwapSources		=	true;
			//				m_nProgress			=	1.0 - m_nProgress;
			//				
			//				if ( 0.0 >= m_nProgress )
			//				{
			//					m_bNoneCompose	=	true;
			//				}
			//			}

			//			if ( 0x0000002A == m_nEffectDirection )	//	Horizontal out
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitHorizontal;
			//			}
			//			
			//			if ( 0x00000015 == m_nEffectDirection )	//	Vertical in
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitVertical;
			//			
			//				m_bSwapSources		=	true;
			//				m_nProgress			=	1.0 - m_nProgress;
			//				
			//				if ( 0.0 >= m_nProgress )
			//				{
			//					m_bNoneCompose	=	true;
			//				}
			//			}
			//			
			//			if ( 0x00000025 == m_nEffectDirection )	//	Vertical out
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitVertical;
			//			}
			//		}
			//		else
			//		{
			//			if ( 0x0000001A == m_nEffectDirection )	//	Horizontal in
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitHorizontal;
			//				m_nProgress			=	1.0 - m_nProgress;
			//			
			//				if ( 1.0 < m_nProgress )
			//				{
			//					m_nProgress		=	1.0;
			//				}
			//			}

			//			if ( 0x0000002A == m_nEffectDirection )	//	Horizontal out
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitHorizontal;
			//				m_bSwapSources		=	true;

			//				if ( 0.0 >= m_nProgress )
			//				{
			//					m_bNoneCompose	=	true;
			//				}
			//			}
			//			
			//			if ( 0x00000015 == m_nEffectDirection )	//	Vertical in
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitVertical;
			//				m_nProgress			=	1.0 - m_nProgress;
			//			}
			//			
			//			if ( 0x00000025 == m_nEffectDirection )	//	Vertical out
			//			{
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSplitVertical;
			//			
			//				m_bSwapSources		=	true;
			//				
			//				if ( 0.0 >= m_nProgress )
			//				{
			//					m_bNoneCompose	=	true;
			//				}
			//			}
			//		}
			//		
			//		m_nComposeTitleX		=	-2;
			//		m_nComposeTitleY		=	-2;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationDissolve ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			m_nProgress			=	1.0 - m_nProgress;
			//		}

			//		m_nComposeEffect		=	ImageStudio::Paint::Compose::Constants::c_nEnhancedColorNoise;
			//		
			//		m_nComposeTitleX		=	8;
			//		m_nComposeTitleY		=	8;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationRandomBars ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x0000000A == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nBarsHorizontal;

			//			if ( 0x00000005 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nBarsVertical;
			//			
			//			if ( 1.0 <= m_nProgress )
			//			{
			//				m_bNoneCompose	=	true;
			//			}
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x0000000A == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nBarsHorizontal;

			//			if ( 0x00000005 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nBarsVertical;
			//			
			//			if ( 0.0 == m_nProgress )
			//			{
			//				m_bNoneCompose	=	true;
			//			}
			//		}

			//		m_nComposeTitleX		=	-2;
			//		m_nComposeTitleY		=	-2;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationStrips ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x00000009 == m_nEffectDirection )	//   Up left.  
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicLeftTop;

			//			if ( 0x00000003 == m_nEffectDirection )	//   Up right.  
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicRightTop;
			//		
			//			if ( 0x00000006 == m_nEffectDirection )	//   Down right. 
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicRightBottom;
			//		
			//			if ( 0x0000000C == m_nEffectDirection )	//   Down left.  
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicLeftBottom;
			//			
			//			if ( 1.0 <= m_nProgress )
			//			{
			//				m_bNoneCompose	=	true;
			//			}
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x00000009 == m_nEffectDirection )	//   Up left.  
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicLeftTop;

			//			if ( 0x00000003 == m_nEffectDirection )	//   Up right.  
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicRightTop;
			//		
			//			if ( 0x00000006 == m_nEffectDirection )	//   Down right. 
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicRightBottom;
			//		
			//			if ( 0x0000000C == m_nEffectDirection )	//   Down left.  
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nMosaicLeftBottom;
			//			
			//			if ( 0.0 == m_nProgress )
			//			{
			//				m_bNoneCompose	=	true;
			//			}
			//		}

			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}
			//	inline int OnEffectAnimationWedge ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClockBottomOpposite;
			//		}
			//		else
			//		{
			//			m_nProgress			=	1.0 - m_nProgress;

			//			m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nClockTopOpposite;
			//		}
			//		
			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationBlinds ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x0000000A == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSlideSideTopToBottom;

			//			if ( 0x00000005 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSlideSideLeftToRight;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x0000000A == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSlideSideTopToBottom;

			//			if ( 0x00000005 == m_nEffectDirection )
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nSlideSideLeftToRight;
			//		}
			//
			//		m_nComposeTitleX		=	-1;
			//		m_nComposeTitleY		=	-1;

			//		return 1;
			//	}

			//	inline int OnEffectAnimationCheckBoard ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_nEffectType )
			//		{
			//			if ( 0x0000000A == m_nEffectDirection )	//	Across
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCheckerRightToLeft;

			//			if ( 0x00000005 == m_nEffectDirection )	//	Vertical
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCheckerTopToBottom;
			//		}
			//		else
			//		{
			//			m_nProgress				=	1.0 - m_nProgress;

			//			if ( 0x0000000A == m_nEffectDirection )	//	Across
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCheckerRightToLeft;

			//			if ( 0x00000005 == m_nEffectDirection )	//	Vertical
			//				m_nComposeEffect	=	ImageStudio::Paint::Compose::Constants::c_nWipeCheckerTopToBottom;
			//		}
			//
			//		m_nComposeTitleX		=	-4;
			//		m_nComposeTitleY		=	-4;

			//		return 1;
			//	}
		
			//	// TODO : все эффекты
		
			//protected:

			//	double					m_nProgress;

			//	int						m_nEffectType;
			//	int						m_nEffectDirection;

			//	int						m_nComposeEffect;
			//	int						m_nComposeTitleX;
			//	int						m_nComposeTitleY;

			//	// если транзишна нету прямого, а есть "обратный" ему, то используем swap images sources
			//	bool					m_bSwapSources;
			//	bool					m_bNoneCompose;
			//};

			//// эффекты с использованием трансформациий ( NO MOTION PATH )
			//class CBoundTransformer
			//{
			//public:

			//	CBoundTransformer ()
			//	{

			//	}

			//	virtual ~CBoundTransformer ()
			//	{

			//	}



			//	
			//	inline bool OnProcess ( ImageStudio::Serialize::Paint::Animations::EffectAnimation*	pEffect, double Progress, double RealWidth, double RealHeight, double OriginalX, double OriginalY )
			//	{																																						  
			//		m_pEffect		=	pEffect;
			//		m_nProgress		=	Progress;
			//		m_nRealWidth	=	RealWidth;
			//		m_nRealHeight	=	RealHeight;
			//		m_nOriginalX	=	OriginalX;
			//		m_nOriginalY	=	OriginalY;

			//		if ( m_pEffect )
			//		{
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationFlyIn			==	pEffect->m_nID )	{	OnEffectAnimationFlyIn		();	return true;	}
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationSpiral		==	pEffect->m_nID )	{	OnEffectAnimationSpiral		();	return true;	}
			//		}

			//		return false;
			//	}

			//	inline Gdiplus::Matrix& GetTransformEP ()
			//	{
			//		return m_mTransformEP;
			//	}

			//
			//private:

			//	inline int OnEffectAnimationFlyIn ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationEntranceType == m_pEffect->m_nType )
			//		{
			//			m_nProgress	=	1.0 - m_nProgress;
			//		}

			//		m_mTransformEP.Reset ();

			//		float TranslateX		=	(float)( m_pEffect->m_nPlaneWidth	+ m_nRealWidth	* 2.0f ) * 0.5f * (float)m_nProgress;
			//		float TranslateY		=	(float)( m_pEffect->m_nPlaneHeight	+ m_nRealHeight	* 2.0f ) * 0.5f * (float)m_nProgress;

			//		if ( 0x00000002 == m_pEffect->m_nDirection )	//	Right. 
			//		{
			//			m_mTransformEP.Translate ( TranslateX, 0.0f );
			//		}

			//		if ( 0x00000008 == m_pEffect->m_nDirection )	//	Left  
			//		{
			//			m_mTransformEP.Translate ( -TranslateX, 0.0f );
			//		}

			//		if ( 0x00000001 == m_pEffect->m_nDirection )	//	Top 
			//		{
			//			m_mTransformEP.Translate ( 0.0f, -TranslateY );
			//		}

			//		if ( 0x00000004 == m_pEffect->m_nDirection )	//	Bottom 
			//		{
			//			m_mTransformEP.Translate ( 0.0f, TranslateY );
			//		}

			//		if ( 0x00000009 == m_pEffect->m_nDirection )	//	Top left 
			//		{
			//			m_mTransformEP.Translate ( -TranslateX, -TranslateY );
			//		}

			//		if ( 0x00000003 == m_pEffect->m_nDirection )	//	Top right 
			//		{
			//			m_mTransformEP.Translate ( TranslateX, -TranslateY );
			//		}

			//		if ( 0x00000006 == m_pEffect->m_nDirection )	//	Bottom right 
			//		{
			//			m_mTransformEP.Translate ( TranslateX, TranslateY );
			//		}

			//		if ( 0x0000000C == m_pEffect->m_nDirection )	//	Bottom left 
			//		{
			//			m_mTransformEP.Translate ( -TranslateX, TranslateY );
			//		}
			//		
			//		return 1;
			//	}


			//	inline int OnEffectAnimationSpiral ()
			//	{
			//		float TranslateX	=	0.0f;
			//		float TranslateY	=	0.0f;

			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationEntranceType == m_pEffect->m_nType )
			//		{
			//			m_nProgress		=	1.0 - m_nProgress;

			//			double MPPI2	=	3.141592653589793f * 2.0 * m_nProgress;

			//			TranslateX		=	(float) ( ( cos ( -MPPI2 ) * ( -m_nOriginalX ) - sin ( -MPPI2 ) * (m_pEffect->m_nPlaneHeight - m_nOriginalY) ) * m_nProgress );
			//			TranslateY		=	(float) ( ( sin ( -MPPI2 ) * ( -m_nOriginalX ) + cos ( -MPPI2 ) * (m_pEffect->m_nPlaneHeight - m_nOriginalY) ) * m_nProgress );
			//		}
			//		else
			//		{
			//			double MPPI2	=	3.141592653589793f * 2.0 * m_nProgress;
			//			
			//			TranslateX		=	(float) ( ( cos ( MPPI2 ) * ( - m_nOriginalX ) - sin ( MPPI2 ) * ( m_pEffect->m_nPlaneHeight - m_nOriginalY ) ) * m_nProgress );
			//			TranslateY		=	(float) ( ( sin ( MPPI2 ) * ( - m_nOriginalX ) + cos ( MPPI2 ) * ( m_pEffect->m_nPlaneHeight - m_nOriginalY ) ) * m_nProgress );
			//		}

			//		m_mTransformEP.Reset ();
			//	
			//		Gdiplus::Matrix	mTranslate;			mTranslate.Translate ( TranslateX, TranslateY );
			//		m_mTransformEP.Multiply ( &mTranslate );
			//		
			//		Gdiplus::Matrix	mOrTranslate;		mOrTranslate.Translate ( (Gdiplus::REAL)( m_nOriginalX ), (Gdiplus::REAL) ( m_nOriginalY ) );
			//		m_mTransformEP.Multiply ( &mOrTranslate );
			//	
			//		Gdiplus::Matrix	mScale;				mScale.Scale ( (Gdiplus::REAL) ( 1.0f - m_nProgress + 0.00001f ), (Gdiplus::REAL) ( 1.0 - m_nProgress + 0.00001f ) );
			//		m_mTransformEP.Multiply ( &mScale );

			//		mOrTranslate.Reset ();				mOrTranslate.Translate ( -(Gdiplus::REAL)( m_nOriginalX ), -(Gdiplus::REAL) ( m_nOriginalY ) );

			//		m_mTransformEP.Multiply ( &mOrTranslate );

			//		return 1;
			//	}

			//	
			//	// TODO : все эффекты

			//protected:

			//	double					m_nProgress;

			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation*	m_pEffect;

			//	Gdiplus::Matrix			m_mTransform;

			//	double					m_nOriginalX;
			//	double					m_nOriginalY;

			//	double					m_nRealWidth;	
			//	double					m_nRealHeight;

			//	Gdiplus::Matrix			m_mTransformEP;				//	ENTRACE OR EXIT PATH TRASNFORM
			//};

			//class CTextDrawerEx
			//{
			//public:
			//	CTextDrawerEx ()
			//	{

			//	}

			//	virtual ~CTextDrawerEx()
			//	{

			//	}

			//protected:

			//};

			//class CImageDrawerEx
			//{
			//public:
			//	CImageDrawerEx ()
			//	{
			//		m_nMPOffSetX		=	0.0;
			//		m_nMPOffSetY		=	0.0;
			//		m_nAlpha			=	1.0;
			//		m_nEffectGoTimeF	=	0.0;
			//	}

			//	virtual ~CImageDrawerEx ()
			//	{

			//	}

			//	inline int OnProcess ( double Time, double dLeft, double dRight, double dTop, double dBottom, double dCompleteness, IUnknown* pCanvas, IUnknown* pSource, ImageStudio::Serialize::Paint::Animations::DrawImageFromFileAnimate* pAction )
			//	{	
			//		m_nEffectGoTimeF	=	1.000001;

			//		m_nTime				=	Time;
			//		m_pAction			=	pAction;

			//		m_dLeft				=	dLeft;
			//		m_dRight			=	dRight;
			//		m_dTop				=	dTop;
			//		m_dBottom			=	dBottom;
			//		
			//		m_pAction->m_Animation.GetGoEffect ( Time, m_GoEffect, m_nEffectGoTimeF );

			//		m_pMainEffect		=	NULL;
			//		m_pMPEffect			=	NULL;

			//		bool EffectBegin	=	false;
			//		bool ExitEffectEnd	=	false;

			//		/*IAVSGdiplusRenderer *pRenderer = NULL;
			//		::CoCreateInstance ( __uuidof(CAVSGdiplusRenderer), NULL, CLSCTX_INPROC, __uuidof(IAVSGdiplusRenderer), (void**)&pRenderer );
			//		if ( NULL != pRenderer )
			//		{
			//			float f1 = float(m_dLeft);
			//			float f2 = float(m_dRight);
			//			float f3 = float(m_dTop);
			//			float f4 = float(m_dBottom);

			//			((CAVSGdiplusRenderer*)pRenderer)->CalculatePoint ( f1, f2 );
			//			((CAVSGdiplusRenderer*)pRenderer)->CalculatePoint ( f3, f4 );

			//			m_dLeft		= f1;
			//			m_dRight	= f2;
			//			m_dTop		= f3;
			//			m_dBottom	= f4;
			//		}*/

			//		CGdiplusRenderer oRenderer;
			//		if ( TRUE )
			//		{
			//			float f1 = float(m_dLeft);
			//			float f2 = float(m_dRight);
			//			float f3 = float(m_dTop);
			//			float f4 = float(m_dBottom);

			//			oRenderer.CalculatePoint ( f1, f2 );
			//			oRenderer.CalculatePoint ( f3, f4 );

			//			m_dLeft		= f1;
			//			m_dRight	= f2;
			//			m_dTop		= f3;
			//			m_dBottom	= f4;
			//		}

			//		if ( m_pAction->m_Animation.GetEffect ( Time, &m_pMainEffect, &m_pMPEffect, m_nEffectGoTimeF, EffectBegin, ExitEffectEnd ) )
			//		{
			//			// вначале нету эффектов и первый эффект fade in
			//			if ( m_pAction->m_Animation.IsFirstEntrance () && EffectBegin || ExitEffectEnd )
			//			{
			//				return ImageStudio::Transforms::Core::c_nErrorNone;
			//			}

			//			if ( NULL != m_pMainEffect )
			//			{
			//				if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationEntranceType == m_pMainEffect->m_nType ||
			//					ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_pMainEffect->m_nType )
			//				{
			//					m_GoEffect	=	*m_pMainEffect;
			//				}
			//			}
			//		}

			//		OnProcessMotionPathEffect ();

			//		// эффекты, что не требует выполнения COMPOSE преобразование, имеет смысл оптимизоровать
			//		if ( NULL != m_pMainEffect )
			//		{
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationFadedZoom	== m_pMainEffect->m_nID )	OnEffectAnimationFadedZoom ();
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationZoom		== m_pMainEffect->m_nID )	OnEffectAnimationZoom ();
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationFade		== m_pMainEffect->m_nID )	OnEffectAnimationFade ();

			//			m_oBoundTransformer.OnProcess ( 
			//				m_pMainEffect,
			//				m_nEffectGoTimeF, 
			//				(double) abs ( m_dLeft	- m_dRight ),
			//				(double) abs ( m_dTop	- m_dBottom ),
			//				( m_dLeft	- m_dRight ) * 0.5,
			//				( m_dTop	- m_dBottom ) * 0.5 );
			//		}

			//		UpdateTransforms ();

			//		// пока только одна анимация  для объекта, TODO : максиум три эффекта над одним объектом одновременно может быть применено
			//		// fade in or fade out + motion path + emphasis
			//		// все использования compose effect's должны быть скрыты в этом классе для картинок

			//		int nResult		=	ImageStudio::Transforms::Core::c_nErrorNone;

			//		nResult	= DrawImageFromFile ( pCanvas, pSource, pAction->BackColor, pAction->nDrawImageScaleType, pAction->nDrawImageScaleBackColor, static_cast<int> ( m_nAlpha * pAction->Alpha ), dCompleteness, m_dLeft, m_dTop, m_dRight, m_dBottom, pAction->Angle, pAction->bAlign, pAction->nAlignType, pAction->nMarginX, pAction->nMarginY, pAction->nSizeType, pAction->nWidth, pAction->nHeight );

			//		return ( ImageStudio::Transforms::Core::c_nErrorNone == nResult );
			//	}

			//protected:

			//	inline void UpdateTransforms ()
			//	{
			//		m_mTransform.Reset ();

			//		Gdiplus::Matrix	mBoundTranslate;
			//		mBoundTranslate.Translate ( (Gdiplus::REAL)( m_dLeft + m_dRight ) * 0.5f, (Gdiplus::REAL) ( m_dTop + m_dBottom ) * 0.5f );

			//		m_mTransform.Multiply ( &mBoundTranslate );
			//		m_mTransform.Multiply ( &m_mTransformScale );
			//		m_mTransform.Multiply ( &m_mTransformRotate );

			//		mBoundTranslate.Reset ();
			//		mBoundTranslate.Translate ( -(Gdiplus::REAL)( m_dLeft + m_dRight ) * 0.5f, -(Gdiplus::REAL) ( m_dTop + m_dBottom ) * 0.5f );

			//		m_mTransform.Multiply ( &mBoundTranslate );

			//		m_mTransform.Multiply ( &m_mTransformMP );

			//		m_mTransform.Multiply ( &m_oBoundTransformer.GetTransformEP () );
			//	}


			//	inline int DrawImageFromFile ( IUnknown* pCanvas, IUnknown* pSource, int nRemoveColor, int nType, int nBackColor, int nAlpha, double dCompleteness, double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0, BOOL bAlign = FALSE, int nAlignType = 4, int nMarginX = 10, int nMarginY = 10, int nSizeType = 0, int nImWidth = 0, int nImHeight = 0 )
			//	{
			//		BYTE* pCanvasBGRA = NULL;
			//		int nCanvasWidth = 0;
			//		int nCanvasHeight = 0;

			//		// вытаскиваем из интерфейса указатель на память в формате BGRA
			//		int nStatus = Transforms::Core::COM::COM_GetBGRAImageFromInterface ( pCanvas, pCanvasBGRA, nCanvasWidth, nCanvasHeight);
			//		if (nStatus != ImageStudio::Transforms::Core::c_nErrorNone)
			//			return nStatus;


			//		BYTE* pSourceBGRA = NULL;
			//		int nSourceWidth = 0;
			//		int nSourceHeight = 0;

			//		// вытаскиваем из интерфейса указатель на память в формате BGRA
			//		int nStride = 0;
			//		nStatus = Transforms::Core::COM::COM_GetBGRAImageFromInterface(pSource, pSourceBGRA, nSourceWidth, nSourceHeight, &nStride);
			//		if (nStatus != ImageStudio::Transforms::Core::c_nErrorNone)
			//			return nStatus;

			//		CBuffer<BYTE> oBuffer;
			//		if( nStride < 0 )
			//		{
			//			oBuffer.Create( nSourceWidth * nSourceHeight * 4 );
			//			if( !oBuffer.IsEmpty() )
			//			{
			//				BYTE* pSrc = pSourceBGRA + (nSourceHeight - 1) * nSourceWidth * 4;
			//				BYTE* pDst = oBuffer.GetPtr();

			//				for( int nRow = 0; nRow < nSourceHeight; ++nRow, pSrc -= nSourceWidth * 4, pDst += nSourceWidth * 4 )
			//				{
			//					memcpy( pDst, pSrc, nSourceWidth * 4 );
			//				}

			//				pSourceBGRA = oBuffer.GetPtr();
			//			}
			//		}

			//		if( nRemoveColor >=0 )
			//		{
			//			ImageStudio::Transforms::Core::BGRA::BGRA_RemoveColorFromImage(pSourceBGRA, nSourceWidth, nSourceHeight, nRemoveColor);
			//		}

			//		if( dRight < dLeft || dBottom < dTop )
			//		{
			//			dRight = dLeft + nSourceWidth;
			//			dBottom = dTop + nSourceHeight;
			//		}

			//		switch ( nSizeType )
			//		{
			//		case 0: 
			//			break;
			//		case 1: 
			//			dRight  = dLeft + nImWidth;
			//			dBottom = dTop  + nImHeight;
			//			break;
			//		case 2:
			//			dRight  = dLeft + nImWidth;
			//			dBottom = dTop  + double(nImWidth * nSourceHeight) / double(nSourceWidth);
			//			break;
			//		case 3:
			//			dBottom = dTop  + nImHeight;
			//			dRight  = dLeft + double(nImHeight * nSourceWidth) / double(nSourceHeight);
			//			break;
			//		}

			//		if ( bAlign )
			//		{
			//			double dRectHeight = dBottom - dTop;
			//			double dRectWidth  = dRight - dLeft;

			//			int nMarginX_new = nMarginX;
			//			int nMarginY_new = nMarginY;


			//			if ( 0 != dAngle )
			//			{
			//				double dXOld = dRectWidth / 2;
			//				double dYOld = dRectHeight / 2;


			//				double dCurAngle = dAngle * 3.141592653589793 / 180;

			//				double dX1 = fabs( dXOld * cos(dCurAngle) - dYOld * sin(dCurAngle) );
			//				double dY1 = fabs( dXOld * sin(dCurAngle) + dYOld * cos(dCurAngle) );

			//				double dX2 = fabs( dXOld * cos(dCurAngle) + dYOld * sin(dCurAngle) );
			//				double dY2 = fabs( dXOld * sin(dCurAngle) - dYOld * cos(dCurAngle) );


			//				nMarginX_new += int(max( dX1, dX2 ) - dXOld);
			//				nMarginY_new += int(max( dY1, dY2 ) - dYOld);

			//			}


			//			switch (nAlignType)
			//			{
			//			case 0: dLeft = nMarginX_new;                                     dTop = nMarginY_new;                                       break;
			//			case 1: dLeft = double(nCanvasWidth) / 2  - dRectWidth / 2;       dTop = nMarginY_new;                                       break;
			//			case 2: dLeft = double(nCanvasWidth) - dRectWidth - nMarginX_new; dTop = nMarginY_new;                                       break;
			//			case 3: dLeft = nMarginX_new;                                     dTop = double(nCanvasHeight) / 2  - dRectHeight / 2;       break;
			//			case 4: dLeft = double(nCanvasWidth) / 2  - dRectWidth / 2;       dTop = double(nCanvasHeight) / 2  - dRectHeight / 2;       break;
			//			case 5: dLeft = double(nCanvasWidth) - dRectWidth - nMarginX_new; dTop = double(nCanvasHeight) / 2  - dRectHeight / 2;       break;
			//			case 6: dLeft = nMarginX_new;                                     dTop = double(nCanvasHeight) - dRectHeight - nMarginY_new; break;
			//			case 7: dLeft = double(nCanvasWidth) / 2  - dRectWidth / 2;       dTop = double(nCanvasHeight) - dRectHeight - nMarginY_new; break;
			//			case 8: dLeft = double(nCanvasWidth) - dRectWidth - nMarginX_new; dTop = double(nCanvasHeight) - dRectHeight - nMarginY_new; break;
			//			}

			//			dRight  = dLeft + dRectWidth;
			//			dBottom = dTop + dRectHeight;

			//		}

			//		return BGRA_DrawImageFromFile( pCanvasBGRA, nCanvasWidth, nCanvasHeight, pSourceBGRA, nSourceWidth, nSourceHeight, nType, nBackColor, nAlpha, dCompleteness, dLeft, dTop, dRight, dBottom, dAngle );
			//	}

			//	inline int BGRA_DrawImageFromFile ( BYTE* pCanvasBGRA, int nCanvasWidth, int nCanvasHeight, BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, int nType, int nBackColor, int nAlpha, double dCompleteness, double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0 )
			//	{
			//		if( NULL == pCanvasBGRA || NULL == pSourceBGRA )
			//			return ImageStudio::Transforms::Core::c_nErrorInvalidPointer;

			//		if( nCanvasWidth <= 0 || nCanvasHeight <= 0 || nSourceWidth <= 0 || nSourceHeight <= 0 )
			//			return ImageStudio::Transforms::Core::c_nErrorInvalidFormat;

			//		int alpha = int(max(0, min(nAlpha, 255)) * dCompleteness + 0.5);
			//		if( alpha <= 0 )
			//			return ImageStudio::Transforms::Core::c_nErrorNone;

			//		pCanvasBGRA += 4 * nCanvasWidth * (nCanvasHeight - 1);

			//		Gdiplus::Bitmap oCanvas( nCanvasWidth, nCanvasHeight, -4 * nCanvasWidth, PixelFormat32bppARGB, pCanvasBGRA );
			//		if( Gdiplus::Ok == oCanvas.GetLastStatus() )
			//		{
			//			Gdiplus::Graphics oRender( &oCanvas );
			//			if( Gdiplus::Ok == oRender.GetLastStatus() )
			//			{
			//				BYTE* pBuffer = NULL;

			//				if( alpha < 255 )
			//				{
			//					int nPixels = nSourceWidth * nSourceHeight;
			//					pBuffer = new BYTE[nPixels*4];
			//					if( pBuffer )
			//					{
			//						BYTE* pSrc = pSourceBGRA;
			//						BYTE* pDst = pBuffer;
			//						for( ; nPixels > 0; nPixels -= 1, pSrc += 4, pDst += 4 )
			//						{
			//							pDst[0] = pSrc[0];
			//							pDst[1] = pSrc[1];
			//							pDst[2] = pSrc[2];
			//							pDst[3] = pSrc[3] * alpha / 255;
			//						}

			//						pSourceBGRA = pBuffer;
			//					}
			//				}
			//				else
			//				{
			//					alpha = 255;
			//				}

			//				//pSourceBGRA += 4 * nSourceWidth * (nSourceHeight - 1);

			//				Gdiplus::Bitmap oImage( nSourceWidth, nSourceHeight, 4*nSourceWidth, PixelFormat32bppARGB, pSourceBGRA );
			//				if( Gdiplus::Ok == oImage.GetLastStatus() )
			//				{
			//					// compute output rectangle
			//					REAL rLeft   = REAL(min(dLeft, dRight));
			//					REAL rTop    = REAL(min(dTop, dBottom));
			//					REAL rRight  = REAL(max(dLeft, dRight));
			//					REAL rBottom = REAL(max(dTop, dBottom));

			//					REAL rNaturalLeft   = rLeft;
			//					REAL rNaturalTop    = rTop;
			//					REAL rNaturalWidth  = rRight - rLeft;
			//					REAL rNaturalHeight = rBottom - rTop;

			//					double dNewWidth  = abs(dRight - dLeft);
			//					double dNewHeight = abs(dBottom - dTop);
			//					double dWidth     = nSourceWidth;
			//					double dHeight    = nSourceHeight;
			//					double dScaleX    = dNewWidth / dWidth;
			//					double dScaleY    = dNewHeight / dHeight;
			//					double dZoom      = min(1.0, min(dScaleX, dScaleY));

			//					switch( nType )
			//					{
			//					case c_nResizeStretchNearest:
			//					case c_nResizeStretchBillinear:
			//					case c_nResizeStretchBicubic:
			//						break;
			//					case c_nResizeShrinkNearest:
			//					case c_nResizeShrinkBillinear:
			//					case c_nResizeShrinkBicubic:
			//						if (dScaleX > dScaleY)
			//						{
			//							// полосы слева и справа
			//							REAL rOffset   = REAL((dNewWidth - (dWidth * dNewHeight / dHeight)) / 2);
			//							rNaturalLeft  += rOffset;
			//							rNaturalWidth -= rOffset * 2;
			//						}
			//						else
			//						{
			//							// полосы сверху и снизу
			//							REAL rOffset    = REAL((dNewHeight - (dHeight * dNewWidth / dWidth)) / 2);
			//							rNaturalTop    += rOffset;
			//							rNaturalHeight -= rOffset * 2;
			//						}
			//						break;
			//					case c_nResizeCropNearest:
			//					case c_nResizeCropBillinear:
			//					case c_nResizeCropBicubic:
			//						rNaturalLeft   = 0; 
			//						rNaturalTop    = 0;
			//						rNaturalWidth  = REAL(nSourceWidth); 
			//						rNaturalHeight = REAL(nSourceHeight);

			//						if( dScaleX > dScaleY )
			//						{
			//							// ширина - на месте
			//							double dNewHeightReal = dNewHeight;
			//							dNewHeight    *= dHeight / dWidth;
			//							double dOffset        = (dNewHeight - dNewHeightReal) / 2;
			//							dOffset       *= dHeight / dNewHeight;

			//							REAL rOffset    = (REAL)dOffset;
			//							//REAL rOffset  = (REAL)(((dWidth * dNewHeight / dHeight) - dWidth) / 2);
			//							rNaturalTop    += rOffset;
			//							rNaturalHeight -= rOffset * 2;
			//						}
			//						else
			//						{
			//							// высота - на месте
			//							double dNewWidthReal = dNewWidth;
			//							dNewWidth    *= dWidth / dHeight;
			//							double dOffset       = (dNewWidth - dNewWidthReal) / 2;
			//							dOffset      *= dWidth / dNewWidth;

			//							REAL rOffset   = (REAL)dOffset;
			//							//REAL rOffset = (REAL)(((dHeight * dNewWidth / dWidth) - dHeight) / 2);
			//							rNaturalLeft  += rOffset;
			//							rNaturalWidth -= rOffset * 2;
			//						}

			//						Gdiplus::RectF oDstRect(rLeft, rTop, rRight - rLeft, rBottom - rTop);
			//						if( dAngle != 0 )
			//						{
			//							float dx = oDstRect.X + oDstRect.Width / 2;
			//							float dy = oDstRect.Y + oDstRect.Height / 2;

			//							oRender.TranslateTransform( -dx, -dy, Gdiplus::MatrixOrderAppend );
			//							oRender.RotateTransform( float(-dAngle), Gdiplus::MatrixOrderAppend );
			//							oRender.TranslateTransform( dx, dy, Gdiplus::MatrixOrderAppend );
			//						}

			//						oRender.DrawImage( &oImage, oDstRect, rNaturalLeft, rNaturalTop, rNaturalWidth, rNaturalHeight, UnitPixel);

			//						if( pBuffer )
			//							delete [] pBuffer;

			//						return ImageStudio::Transforms::Core::c_nErrorNone;
			//					}

			//					if( (rLeft < rNaturalLeft || rTop < rNaturalTop) && nBackColor >= 0 )
			//					{
			//						Gdiplus::SolidBrush brush( Gdiplus::Color( BYTE(alpha), GetRValue(nBackColor), GetGValue(nBackColor), GetBValue(nBackColor) ) );
			//						oRender.FillRectangle( &brush, Gdiplus::RectF(rLeft, rTop, rRight - rLeft, rBottom - rTop) );
			//					}

			//					Gdiplus::RectF oDstRect(rNaturalLeft, rNaturalTop, rNaturalWidth, rNaturalHeight);
			//					if( dAngle != 0 )
			//					{
			//						float dx = oDstRect.X + oDstRect.Width / 2;
			//						float dy = oDstRect.Y + oDstRect.Height / 2;

			//						oRender.TranslateTransform( -dx, -dy, Gdiplus::MatrixOrderAppend );
			//						oRender.RotateTransform( float(-dAngle), Gdiplus::MatrixOrderAppend );
			//						oRender.TranslateTransform( dx, dy, Gdiplus::MatrixOrderAppend );
			//					}

			//					oRender.SetTransform ( &m_mTransform );

			//					double EffectProgress	=	0.0;

			//					ImageStudio::Serialize::Paint::Animations::EffectAnimation	effect;
			//					m_pAction->m_Animation.GetGoEffect ( m_nTime, effect, EffectProgress );

			//					if ( m_oComposeLinker.VerifyEffect ( m_GoEffect.m_nID, m_GoEffect.m_nType, m_GoEffect.m_nDirection, m_nEffectGoTimeF ) )
			//					{	
			//						Gdiplus::Bitmap* pBitmap = NULL;
			//						if ( m_oComposeLinker.ApplyEffect ( &oImage, &pBitmap ) )
			//						{
			//							if ( false == m_oComposeLinker.IsNoneEffect () )
			//							{
			//								if ( NULL != pBitmap )
			//								{
			//									oRender.DrawImage( pBitmap, oDstRect, 0, 0, REAL(nSourceWidth), REAL(nSourceHeight), UnitPixel );
			//								}
			//								else
			//								{
			//									oRender.DrawImage ( &oImage, oDstRect, 0, 0, REAL(nSourceWidth), REAL(nSourceHeight), UnitPixel );
			//								}
			//							}
			//						}
			//						
			//						RELEASEOBJECT ( pBitmap )
			//					}		
			//					else
			//					{

			//						oRender.DrawImage ( &oImage, oDstRect, 0, 0, REAL(nSourceWidth), REAL(nSourceHeight), UnitPixel );
			//					}

			//					if( pBuffer )
			//						delete [] pBuffer;

			//					oRender.SetTransform ( &Gdiplus::Matrix() );
			//				}
			//			}
			//		}

			//		return ImageStudio::Transforms::Core::c_nErrorNone;
			//	}



			//protected:
			//
			//	inline int OnProcessMotionPathEffect ()
			//	{
			//		m_mTransformMP.Reset();

			//		double OffSetX			=	0.0;				
			//		double OffSetY			=	0.0;

			//		if ( m_pAction->m_Animation.GetMotionOffset ( m_nTime, OffSetX, OffSetY ) )
			//		{
			//			m_nMPOffSetX		=	OffSetX;
			//			m_nMPOffSetY		=	OffSetY;
			//		}

			//		m_mTransformMP.Translate ( (Gdiplus::REAL)m_nMPOffSetX, (Gdiplus::REAL)m_nMPOffSetY );

			//		return 1;
			//	}


			//	
			//	// Часть эффектов, по возможности лучше произвести без использования compose c прозрачной картинкой
			//	inline int OnEffectAnimationFadedZoom ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_GoEffect.m_nType )
			//		{
			//			m_nEffectGoTimeF	=	1.0 - m_nEffectGoTimeF;
			//		}

			//		m_nAlpha				=	m_nEffectGoTimeF;

			//		m_mTransformScale.Reset ();
			//		m_mTransformScale.Scale ( (Gdiplus::REAL)m_nEffectGoTimeF, (Gdiplus::REAL)m_nEffectGoTimeF );

			//		return 1;
			//	}

			//	inline int OnEffectAnimationZoom ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_GoEffect.m_nType )
			//		{
			//			m_nEffectGoTimeF	=	1.0 - m_nEffectGoTimeF;
			//		}

			//		m_mTransformScale.Reset ();
			//		m_mTransformScale.Scale ( (Gdiplus::REAL)m_nEffectGoTimeF, (Gdiplus::REAL)m_nEffectGoTimeF );

			//		return 1;
			//	}

			//	inline int OnEffectAnimationFade ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_GoEffect.m_nType )
			//		{
			//			m_nEffectGoTimeF	=	1.0 - m_nEffectGoTimeF;
			//		}

			//		m_nAlpha				=	m_nEffectGoTimeF;

			//		return 1;
			//	}

			//	// TODO : все эффекты 

			//protected:

			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation*				m_pMainEffect;
			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation*				m_pMPEffect;

			//	ImageStudio::Serialize::Paint::Animations::DrawImageFromFileAnimate*	m_pAction;
			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation				m_GoEffect;
			//
			//	double					m_nMPOffSetX;	
			//	double					m_nMPOffSetY;
			//
			//	double					m_nEffectGoTimeF;

			//	double					m_nTime;

			//	double					m_nAlpha;

			//	double 					m_dLeft;
			//	double 					m_dRight;
			//	double 					m_dTop;
			//	double 					m_dBottom;

			//	CComposeLinker			m_oComposeLinker;
			//	CBoundTransformer		m_oBoundTransformer;

			//	// TODO :  в отдельный класс для применения общей трансформации
			//	Gdiplus::Matrix			m_mTransform;

			//	Gdiplus::Matrix			m_mTransformScale;
			//	Gdiplus::Matrix			m_mTransformRotate;
			//	Gdiplus::Matrix			m_mEmphasisTransform;	
			//	Gdiplus::Matrix			m_mTransformTranslate;		
			//	Gdiplus::Matrix			m_mTransformMP;				//	MOTHION PATH TRASNFORM
			//};

			//class CGraphicsPathDrawerEx
			//{
			//public:
			//	CGraphicsPathDrawerEx ()
			//	{
			//		m_nMPOffSetX		=	0.0;				
			//		m_nMPOffSetY		=	0.0;
			//		m_nEffectGoTimeF	=	0.0;

			//		m_pMainEffect		=	NULL;
			//	}
			//	virtual ~CGraphicsPathDrawerEx ()
			//	{

			//	}

			//	inline int OnProcess ( double Time, IUnknown* pCanvas, ImageStudio::Paint::CPaintStateManager* pPaintSettings, ImageStudio::Serialize::Paint::Animations::DrawGraphicPathAnimate* pAction )
			//	{	
			//		m_nTime				=	Time;
			//		m_pAction			=	pAction;
			//		m_pPaintSettings	=	pPaintSettings;

			//		m_nEffectGoTimeF	=	1.000001;

			//		m_pAction->m_Animation.GetGoEffect ( Time, m_GoEffect, m_nEffectGoTimeF );

			//		m_pMainEffect		=	NULL;
			//		m_pMPEffect			=	NULL;

			//		bool EffectBegin	=	false;
			//		bool ExitEffectEnd	=	false;
			//		
			//		m_arParts			=	m_pAction->m_arParts;

			//		if ( m_pAction->m_Animation.GetEffect ( Time, &m_pMainEffect, &m_pMPEffect, m_nEffectGoTimeF, EffectBegin, ExitEffectEnd ) )
			//		{
			//			// вначале нету эффектов и первый эффект fade in
			//			if ( m_pAction->m_Animation.IsFirstEntrance () && EffectBegin || ExitEffectEnd )
			//			{
			//				return ImageStudio::Transforms::Core::c_nErrorNone;
			//			}

			//			if ( NULL != m_pMainEffect )
			//			{
			//				if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationEntranceType == m_pMainEffect->m_nType ||
			//					ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_pMainEffect->m_nType )
			//				{
			//					m_GoEffect	=	*m_pMainEffect;
			//				}
			//			}
			//		}

			//		PrepareFigure ();

			//		OnProcessMotionPathEffect ();

			//		m_pPaintSettings->SaveState	( );
			//		
			//		Painter::CPen oPen_;
			//		oPen_.FromXmlString(pAction->Pen.m_oPen.ToXmlString());
			//		Painter::CBrush oBrush_;
			//		oBrush_.FromXmlString(pAction->Brush.m_oBrush.ToXmlString());
			//		
			//		m_pPaintSettings->SetPen	( oPen_ );
			//		m_pPaintSettings->SetBrush	( oBrush_, FALSE );

			//		Painter::CPen& oPen		=	m_pPaintSettings->GetPen();
			//		Painter::CBrush& oBrush	=	m_pPaintSettings->GetBrush();

			//		int nResult = ImageStudio::Transforms::Core::c_nErrorNone;

			//		// пока только одна анимация  для объекта, TODO : максиум три эффекта над одним объектом одновременно может быть применено
			//		// fade in or fade out + motion path + emphasis
			//		// все использования compose effect's должны быть скрыты в этом классе для картинок

			//		// эффекты, что не требует выполнения COMPOSE преобразование, имеет смысл оптимизоровать

			//		if ( NULL != m_pMainEffect )
			//		{
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationFadedZoom	== m_pMainEffect->m_nID )	OnEffectAnimationFadedZoom ();
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationZoom		== m_pMainEffect->m_nID )	OnEffectAnimationZoom ();
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationFade		== m_pMainEffect->m_nID )	OnEffectAnimationFade ();

			//			m_oBoundTransformer.OnProcess ( 
			//				m_pMainEffect, 
			//				m_nEffectGoTimeF, 
			//				(double) m_rcFigureBounds.Width, 
			//				(double) m_rcFigureBounds.Height,
			//				(double)m_rcFigureBounds.X + (double) m_rcFigureBounds.Width  * 0.5, 
			//				(double)m_rcFigureBounds.Y + (double) m_rcFigureBounds.Height * 0.5 );
			//		}

			//		// здесь по-идее нужно пробежаться 
			//		// по всем точкам-настройкам и умножить их на метрику...
			//		// пока - пофигу

			//		BYTE* pBGRA = NULL;
			//		int nWidth = 0;
			//		int nHeight = 0;

			//		// вытаскиваем из интерфейса указатель на память в формате BGRA
			//		int nStatus = ImageStudio::Transforms::Core::COM::COM_GetBGRAImageFromInterface ( pCanvas, pBGRA, nWidth, nHeight );
			//		if ( nStatus != ImageStudio::Transforms::Core::c_nErrorNone )
			//			return nStatus;

			//		nResult = ImageStudio::Transforms::Core::c_nErrorGdiplusFailure;

			//		// переворачиваем изображение для GDI+
			//		
			//		pBGRA += 4 * nWidth * (nHeight - 1);

			//		Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRA );
			//		if( Gdiplus::Ok == oImage.GetLastStatus() )
			//		{
			//			Gdiplus::Graphics oRender ( &oImage );
			//			if ( Gdiplus::Ok == oRender.GetLastStatus() )
			//			{
			//				// сообственно отрисовка
			//				Gdiplus::Pen* pPen		=	m_pPaintSettings->GetPen().GetPen();
			//				Gdiplus::Brush* pBrush	=	m_pPaintSettings->GetBrush().GetBrush ( &m_pPaintSettings->GetTextureManager(), &m_pPaintSettings->GetBrush().GetRect() );

			//				m_pGraphics				=	&oRender;

			//				bool ComposeEffect = false;
			//				if ( NULL != m_pMainEffect )
			//				{
			//					ComposeEffect	=	m_oComposeLinker.VerifyEffect ( m_pMainEffect->m_nID, m_pMainEffect->m_nType, m_pMainEffect->m_nDirection, m_nEffectGoTimeF );
			//				}

			//				if ( ComposeEffect )
			//				{
			//					DrawFigureOnComposePlane ();
			//				}
			//				else
			//				{
			//					UpdateTransforms ();

			//					oRender.SetTransform ( &m_mTransform );

			//					DrawFigure ( &oRender, pPen, pBrush );

			//					oRender.SetTransform ( &Gdiplus::Matrix() );
			//				}

			//				nResult = ImageStudio::Transforms::Core::c_nErrorNone;
			//			}
			//		}
			//		
			//		nResult = ImageStudio::Transforms::Core::c_nErrorInvalidInterface;
			//		

			//		////IAVSGdiplusRenderer *pRenderer = NULL;
			//		////::CoCreateInstance ( __uuidof(CAVSGdiplusRenderer), NULL, CLSCTX_INPROC, __uuidof(IAVSGdiplusRenderer), (void**)&pRenderer );
			//		////if ( NULL == pRenderer )
			//		////	return nResult;

			//		////pRenderer->SetWidth((float)pAction->m_dWidthMM);
			//		////pRenderer->SetHeight((float)pAction->m_dHeightMM);
			//		////pRenderer->CreateFromMediaData ( pCanvas );

			//		////Gdiplus::Graphics* pGraphics = ((CAVSGdiplusRenderer*)pRenderer)->GetGraphics ();
			//		////if ( Gdiplus::Ok == pGraphics->GetLastStatus() )
			//		////{
			//		////	m_pGraphics				=	pGraphics;

			//		////	bool ComposeEffect = false;
			//		////	if ( NULL != m_pMainEffect )
			//		////	{
			//		////		ComposeEffect	=	m_oComposeLinker.VerifyEffect ( m_pMainEffect->m_nID, m_pMainEffect->m_nType, m_pMainEffect->m_nDirection, m_nEffectGoTimeF );
			//		////	}

			//		////	if ( ComposeEffect )
			//		////	{
			//		////		DrawFigureOnComposePlane ( ((CAVSGdiplusRenderer*)pRenderer) );
			//		////	}
			//		////	else
			//		////	{
			//		////		UpdateTransforms ( pRenderer );

			//		////		pGraphics->SetTransform ( &m_mTransform );
			//		////
			//		////		Draw ( pRenderer );

			//		////		pGraphics->SetTransform ( &Gdiplus::Matrix() );
			//		////	}

			//		////	nResult = ImageStudio::Transforms::Core::c_nErrorNone;
			//		////}

			//		////RELEASEINTERFACE(pRenderer);
			//		////
			//		////m_pPaintSettings->RestoreState();

			//		m_pAction->m_arParts.RemoveAll ();
			//		m_pAction->m_arParts = 	m_arParts;

			//		return ( ImageStudio::Transforms::Core::c_nErrorNone );
			//	}

			//protected:

			//	inline bool PrepareFigure ()
			//	{
			//		Gdiplus::PointF SetPoint	( 0.0f, 0.0f );
			//		Gdiplus::PointF MovePoint	( 0.0f, 0.0f );
			//		
			//		/*IAVSGdiplusRenderer *pRenderer = NULL;
			//		::CoCreateInstance ( __uuidof(CAVSGdiplusRenderer), NULL, CLSCTX_INPROC, __uuidof(IAVSGdiplusRenderer), (void**)&pRenderer );
			//		if ( NULL == pRenderer )
			//			return false;*/

			//		CGdiplusRenderer oRenderer;

			//		for ( int nIndex = 0; nIndex < m_pAction->m_arParts.GetSize(); ++nIndex )
			//		{
			//			for ( int i = 0; i < m_pAction->m_arParts[nIndex].m_arPoints.GetSize (); ++i )
			//			{
			//				float fX = m_pAction->m_arParts[nIndex].m_arPoints[i].X;
			//				float fY = m_pAction->m_arParts[nIndex].m_arPoints[i].Y;
			//				
			//				//((CAVSGdiplusRenderer*)pRenderer)->CalculatePoint ( fX, fY );
			//				oRenderer.CalculatePoint( fX, fY );
			//				
			//				m_pAction->m_arParts[nIndex].m_arPoints[i].X = fX;
			//				m_pAction->m_arParts[nIndex].m_arPoints[i].Y = fY;
			//			}

			//			m_pAction->m_arParts[nIndex].DrawOnGraphics ( &m_gpFigure, SetPoint, MovePoint );
			//		}

			//		int PointCount				=	m_gpFigure.GetPointCount();
			//		Gdiplus::PointF* pPoints	=	new PointF [ PointCount ];

			//		if ( NULL != pPoints )
			//		{
			//			if ( Gdiplus::Ok == m_gpFigure.GetPathPoints ( pPoints, PointCount ) )
			//			{
			//				if ( Gdiplus::Ok == m_gpFigure.GetBounds ( &m_rcFigureBounds ) )
			//				{
			//					delete [] pPoints;
			//					return true;
			//				}
			//			}
			//		}

			//		return false;
			//	}

			//	inline void UpdateTransforms ()
			//	{
			//		m_mTransform.Reset ();

			//		Gdiplus::Matrix	mBoundTranslate;
			//		
			//		float fX = ( m_rcFigureBounds.X + m_rcFigureBounds.Width * 0.5f );
			//		float fY = ( m_rcFigureBounds.Y + m_rcFigureBounds.Height * 0.5f );

			//		mBoundTranslate.Translate ( fX, fY );

			//		m_mTransform.Multiply ( &m_mBoundTranslate );
			//		m_mTransform.Multiply ( &mBoundTranslate );
			//		m_mTransform.Multiply ( &m_mTransformScale );
			//		m_mTransform.Multiply ( &m_mTransformRotate );

			//		mBoundTranslate.Reset ();
			//		mBoundTranslate.Translate ( -fX, -fY );

			//		m_mTransform.Multiply ( &mBoundTranslate );

			//		m_mTransform.Multiply ( &m_mTransformMP );
			//	
			//		m_mTransform.Multiply ( &m_oBoundTransformer.GetTransformEP () );
			//	}

			//	inline void DrawFigure ( Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pPen, Gdiplus::Brush* pBrush )
			//	{
			//		if ( m_pAction->m_bStroke )
			//		{
			//			pGraphics->SetSmoothingMode ( SmoothingModeAntiAlias );
			//		}
			//		else
			//		{
			//			pGraphics->SetSmoothingMode ( SmoothingModeNone );
			//		}

			//		pGraphics->DrawPath ( pPen, &m_gpFigure );

			//		if ( m_pAction->m_bFill )
			//		{
			//			pGraphics->FillPath ( pBrush, &m_gpFigure );
			//		}
			//	}

			//	inline void DrawFigureOnComposePlane ()
			//	{
			//		Gdiplus::Bitmap oBitmap ( static_cast<int> ( m_rcFigureBounds.Width ), static_cast<int> ( m_rcFigureBounds.Height ), PixelFormat32bppARGB );
			//		if ( Gdiplus::Ok == oBitmap.GetLastStatus () )
			//		{
			//			Gdiplus::Graphics oGraphics ( &oBitmap );
			//			if ( Gdiplus::Ok == oGraphics.GetLastStatus () )
			//			{
			//				if ( Gdiplus::Ok == oGraphics.Clear ( Gdiplus::Color ( 0, 0, 0, 0 ) ) )
			//				{
			//					Gdiplus::Matrix	mBoundTranslate;
			//					mBoundTranslate.Translate ( ( -m_rcFigureBounds.X ), ( -m_rcFigureBounds.Y ) );
			//					oGraphics.SetTransform ( &mBoundTranslate );

			//					// сообственно отрисовка
			//					Gdiplus::Pen* pPen		=	m_pPaintSettings->GetPen().GetPen();
			//					Gdiplus::Brush* pBrush	=	m_pPaintSettings->GetBrush().GetBrush ( &m_pPaintSettings->GetTextureManager(), &m_pPaintSettings->GetBrush().GetRect() );

			//					DrawFigure ( &oGraphics, pPen, pBrush );

			//					oGraphics.SetTransform ( &Gdiplus::Matrix() );

			//					if ( m_oComposeLinker.ApplyEffect ( &oBitmap ) )
			//					{
			//						if ( false == m_oComposeLinker.IsNoneEffect () )
			//						{
			//							m_mBoundTranslate.Reset ();
			//							m_mBoundTranslate.Translate ( m_rcFigureBounds.X, m_rcFigureBounds.Y );

			//							UpdateTransforms ( );

			//							m_pGraphics->SetTransform ( &m_mTransform );

			//							m_pGraphics->DrawImage ( &oBitmap, 0, 0, oBitmap.GetWidth(), oBitmap.GetHeight () );

			//							m_pGraphics->SetTransform ( &Gdiplus::Matrix() );
			//						}
			//					}
			//				}
			//			}
			//		}
			//	}

			//
			//protected:

			//	inline int OnProcessMotionPathEffect ()
			//	{
			//		m_mTransformMP.Reset();

			//		double OffSetX		=	0.0;				
			//		double OffSetY		=	0.0;

			//		if ( m_pMPEffect )
			//		{
			//			if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationMotionPathType == m_pMPEffect->m_nType )
			//			{
			//				if ( static_cast <ImageStudio::Serialize::Paint::Animations::CMotionPath*>(m_pMPEffect)->GetValue ( m_nTime, OffSetX, OffSetY ) )
			//				{
			//					m_nMPOffSetX	=	OffSetX;
			//					m_nMPOffSetY	=	OffSetY;
			//				}
			//			}
			//		}

			//		m_mTransformMP.Translate ( (Gdiplus::REAL)m_nMPOffSetX, (Gdiplus::REAL)m_nMPOffSetY );

			//		return 1;
			//	}

			//	inline int OnEffectAnimationFadedZoom ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_GoEffect.m_nType )
			//		{
			//			m_nEffectGoTimeF	=	1.0 - m_nEffectGoTimeF;
			//		}

			//		m_pPaintSettings->GetPen().ScaleAlpha			( m_nEffectGoTimeF );

			//		m_pPaintSettings->GetBrush().ScaleAlpha1		( m_nEffectGoTimeF );
			//		m_pPaintSettings->GetBrush().ScaleAlpha2		( m_nEffectGoTimeF );
			//		m_pPaintSettings->GetBrush().ScaleTextureAlpha	( m_nEffectGoTimeF );

			//		m_mTransformScale.Reset ();
			//		m_mTransformScale.Scale ( (Gdiplus::REAL)m_nEffectGoTimeF, (Gdiplus::REAL)m_nEffectGoTimeF );

			//		return 1;
			//	}

			//	inline int OnEffectAnimationZoom ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_GoEffect.m_nType )
			//		{
			//			m_nEffectGoTimeF	=	1.0 - m_nEffectGoTimeF;
			//		}

			//		m_mTransformScale.Reset ();
			//		m_mTransformScale.Scale ( (Gdiplus::REAL)m_nEffectGoTimeF, (Gdiplus::REAL)m_nEffectGoTimeF );

			//		return 1;
			//	}

			//	inline int OnEffectAnimationFade ()
			//	{
			//		if ( ImageStudio::Paint::Animations::Constants::c_nEffectAnimationExitType == m_GoEffect.m_nType )
			//		{
			//			m_nEffectGoTimeF	=	1.0 - m_nEffectGoTimeF;
			//		}

			//		m_pPaintSettings->GetPen().ScaleAlpha			( m_nEffectGoTimeF );

			//		m_pPaintSettings->GetBrush().ScaleAlpha1		( m_nEffectGoTimeF );
			//		m_pPaintSettings->GetBrush().ScaleAlpha2		( m_nEffectGoTimeF );
			//		m_pPaintSettings->GetBrush().ScaleTextureAlpha	( m_nEffectGoTimeF );

			//		return 1;
			//	}


			//	// TODO : все эффекты 

			//protected:

			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation*			m_pMainEffect;
			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation*			m_pMPEffect;

			//	ImageStudio::Serialize::Paint::Animations::DrawGraphicPathAnimate*	m_pAction;
			//	ImageStudio::Paint::CPaintStateManager*								m_pPaintSettings;
			//	ImageStudio::Serialize::Paint::Animations::EffectAnimation			m_GoEffect;

			//	double					m_nTime;

			//	Gdiplus::GraphicsPath	m_gpFigure;
			//	Gdiplus::RectF			m_rcFigureBounds;

			//	Gdiplus::Graphics*		m_pGraphics;

			//	double					m_nMPOffSetX;	
			//	double					m_nMPOffSetY;

			//	double					m_nEffectGoTimeF;

			//	CComposeLinker			m_oComposeLinker;
			//	CBoundTransformer		m_oBoundTransformer;

			//	// TODO :  в отдельный класс для применения общей трансформации
			//	Gdiplus::Matrix			m_mTransform;

			//	Gdiplus::Matrix			m_mTransformScale;
			//	Gdiplus::Matrix			m_mTransformRotate;
			//	Gdiplus::Matrix			m_mEmphasisTransform;	
			//	Gdiplus::Matrix			m_mTransformTranslate;		
			//	Gdiplus::Matrix			m_mTransformMP;				//	 MOTHION PATH TRTASNFORM
			//	Gdiplus::Matrix			m_mBoundTranslate;			//	

			//	CSimpleArray<ImageStudio::Serialize::Paint::Common::CDrawGraphicPath::CPart>		m_arParts;
			//};
		}
	}
}