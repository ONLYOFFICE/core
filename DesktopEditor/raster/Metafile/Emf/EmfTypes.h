#ifndef _EMF_TYPES_H
#define _EMF_TYPES_H

namespace Metafile
{
	struct TEmfColor
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a; //Reserved Must be 0x00

		void Init()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}

		void Copy(TEmfColor* pOther)
		{
			r = pOther->r;
			g = pOther->g;
			b = pOther->b;
			a = pOther->a;
		}
	};

	struct TEmfRect
	{
		short shLeft;
		short shTop;
		short shRight;
		short shBottom;
	};

	struct TEmfRectL
	{
		long lLeft;
		long lTop;
		long lRight;
		long lBottom;
	};

	struct TEmfSizeL
	{
		unsigned long ulX;
		unsigned long ulY;
	};

	struct TEmfPointL
	{
		long x;
		long y;
	};

	struct TEmfHeader
	{
		TEmfRectL      oBounds;
		TEmfRectL      oFrame;
		unsigned long  ulSignature;
		unsigned long  ulVersion;
		unsigned long  ulSize;
		unsigned long  ulRecords;
		unsigned short ushObjects;
		unsigned short ushReserved;
		unsigned long  ulSizeDescription;
		unsigned long  ulOffsetDescription;
		unsigned long  ulPalEntries;
		TEmfSizeL      oDevice;
		TEmfSizeL      oMillimeters;
	};

	struct TEmfStretchDIBITS
	{
		TEmfRectL     Bounds;
		long          xDest;
		long          yDest;
		long          xSrc;
		long          ySrc;
		long          cxSrc;
		long          cySrc;
		unsigned long offBmiSrc;
		unsigned long cbBmiSrc;
		unsigned long offBitsSrc;
		unsigned long cbBitsSrc;
		unsigned long UsageSrc;
		unsigned long BitBltRasterOperation;
		long          cxDest;
		long          cyDest;
	};

#define MWT_IDENTITY	  0x01
#define MWT_LEFTMULTIPLY  0x02
#define MWT_RIGHTMULTIPLY 0x03
#define MWT_SET           0x04

	struct TEmfXForm
	{
		double M11;
		double M12;
		double M21;
		double M22;
		double Dx;
		double Dy;

		void Init()
		{
			M11 = 1;
			M12 = 0;
			M21 = 0;
			M22 = 1;
			Dx  = 0;
			Dy  = 0;
		}

		void Copy(TEmfXForm* pOther)
		{
			M11 = pOther->M11;
			M12	= pOther->M12;
			M21	= pOther->M21;
			M22	= pOther->M22;
			Dx	= pOther->Dx;
			Dy	= pOther->Dy;
		}

		void Multiply(TEmfXForm &oOther, unsigned long ulMode)
		{
			if (MWT_IDENTITY == ulMode)
				Init();
			else if (MWT_LEFTMULTIPLY == ulMode)
			{
				// oOther слева, текущая матрица справа
				double dM11 = oOther.M11 * M11 + oOther.M12 * M21;
				double dM12 = oOther.M11 * M21 + oOther.M12 * M22;
				double dM21 = oOther.M21 * M11 + oOther.M22 * M21;
				double dM22 = oOther.M21 * M21 + oOther.M22 * M22;

				double dDx = oOther.Dx * M11 + oOther.Dy * M21 + Dx;
				double dDy = oOther.Dx * M21 + oOther.Dy * M22 + Dy;

				M11 = dM11;
				M12	= dM12;
				M21	= dM21;
				M22	= dM22;
				Dx	= dDx;
				Dy	= dDy;
			}
			else if (MWT_RIGHTMULTIPLY == ulMode)
			{
				// oOther справа, текущая матрица слева
				double dM11 = M11 * oOther.M11 + M12 * oOther.M21;
				double dM12 = M11 * oOther.M21 + M12 * oOther.M22;
				double dM21 = M21 * oOther.M11 + M22 * oOther.M21;
				double dM22 = M21 * oOther.M21 + M22 * oOther.M22;

				double dDx = Dx * oOther.M11 + Dy * oOther.M21 + oOther.Dx;
				double dDy = Dx * oOther.M21 + Dy * oOther.M22 + oOther.Dy;

				M11 = dM11;
				M12	= dM12;
				M21	= dM21;
				M22	= dM22;
				Dx	= dDx;
				Dy	= dDy;
			}
			else //if (MWT_SET == ulMode)
			{
				Copy(&oOther);
			}
		}
	};
};
#endif //_EMF_TYPES_H