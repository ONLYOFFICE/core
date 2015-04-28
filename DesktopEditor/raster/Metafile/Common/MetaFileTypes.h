#ifndef _METAFILE_COMMON_METAFILETYPES_H
#define _METAFILE_COMMON_METAFILETYPES_H

namespace MetaFile
{
	struct TEmfPointL;
	struct TWmfPointS;
	struct TWmfRect;

	struct TRect
	{
		int  nLeft;
		int  nTop;
		int  nRight;
		int  nBottom;

		TRect();
		TRect& operator=(TWmfRect& oRect);
	};

	struct TRectD
	{
		double dLeft;
		double dTop;
		double dRight;
		double dBottom;

		TRectD()
		{
			dLeft   = 0;
			dTop    = 0;
			dRight  = 1024;
			dBottom = 1024;
		}
		TRectD(TRect& oRect)
		{
			dLeft   = (double)oRect.nLeft;
			dTop    = (double)oRect.nTop;
			dRight  = (double)oRect.nRight;
			dBottom = (double)oRect.nBottom;
		}
		TRectD& operator=(TRect& oRect)
		{
			dLeft   = (double)oRect.nLeft;
			dTop    = (double)oRect.nTop;
			dRight  = (double)oRect.nRight;
			dBottom = (double)oRect.nBottom;
			return *this;
		}
		TRectD& operator*=(double& dValue)
		{
			dLeft   *= dValue;
			dTop    *= dValue;
			dRight  *= dValue;
			dBottom *= dValue;
			return *this;
		}
	};

	struct TPointL
	{
		int x;
		int y;

		TPointL();
		TPointL(TWmfPointS& oPoint);
		TPointL(TEmfPointL& oPoint);
		TPointL& operator=(TWmfPointS& oPoint);
		TPointL& operator=(TEmfPointL& oPoint);
	};

	struct TPointD
	{
		double x;
		double y;
	};

	struct TColor
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

#define MWT_IDENTITY	  0x01
#define MWT_LEFTMULTIPLY  0x02
#define MWT_RIGHTMULTIPLY 0x03
#define MWT_SET           0x04

	struct TXForm
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

		void Copy(TXForm* pOther)
		{
			M11 = pOther->M11;
			M12	= pOther->M12;
			M21	= pOther->M21;
			M22	= pOther->M22;
			Dx	= pOther->Dx;
			Dy	= pOther->Dy;
		}

		void Multiply(TXForm &oOther, unsigned int ulMode)
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
		void Apply(double& dX, double& dY)
		{
			double _dX = dX;
			double _dY = dY;

			dX = _dX * M11 + _dY * M21 + Dx;
			dY = _dX * M12 + _dY * M22 + Dy;
		}
	};
}

#endif //_METAFILE_COMMON_METAFILETYPES_H