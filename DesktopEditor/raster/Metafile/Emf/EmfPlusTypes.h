#ifndef EMFPLUSTYPES_H
#define EMFPLUSTYPES_H

#include "EmfTypes.h"
#include "../Common/MetaFileObjects.h"

namespace MetaFile
{
	typedef enum
	{
		UnitTypeWorld      = 0x00,
		UnitTypeDisplay    = 0x01,
		UnitTypePixel      = 0x02,
		UnitTypePoint      = 0x03,
		UnitTypeInch       = 0x04,
		UnitTypeDocument   = 0x05,
		UnitTypeMillimeter = 0x06
	} EEmfPlusUnitType;

	struct TEmfPlusRectR
	{
		char chX;
		char chY;
		char chWidth;
		char chHeight;

		TEmfPlusRectR()
		{
			chX      = 0;
			chY      = 0;
			chWidth  = 0;
			chHeight = 0;
		}

		TEmfPlusRectR(const TEmfPlusRectR& oRect)
		{
			chX      = oRect.chX;
			chY      = oRect.chY;
			chWidth  = oRect.chWidth;
			chHeight = oRect.chHeight;
		}
	};

	struct TEmfPlusRect
	{
		short shX;
		short shY;
		short shWidth;
		short shHeight;

		TEmfPlusRect()
		{
			shX      = 0;
			shY      = 0;
			shWidth  = 1024;
			shHeight = 1024;
		}

		TEmfPlusRect(const TEmfPlusRect& oRect)
		{
			shX      = oRect.shX;
			shY      = oRect.shY;
			shWidth  = oRect.shWidth;
			shHeight = oRect.shHeight;
		}

		TRectL ToRectL()
		{
			TRectL oRect;

			oRect.Left   = (int)shX;
			oRect.Right  = (int)(shX + shWidth);
			oRect.Top    = (int)shY;
			oRect.Bottom = (int)(shY + shHeight);

			return oRect;
		}
	};

	struct TEmfPlusARGB
	{
		unsigned char chBlue;
		unsigned char chGreen;
		unsigned char chRed;
		unsigned char chAlpha;

		TEmfPlusARGB()
		{
			chBlue  = 0;
			chGreen = 0;
			chRed   = 0;
			chAlpha = 255;
		}

		TEmfPlusARGB(unsigned char chBlue, unsigned char chGreen, unsigned char chRed)
		{
			this->chBlue  = chBlue;
			this->chGreen = chGreen;
			this->chRed   = chRed;
		}

		TEmfPlusARGB(const TEmfPlusARGB& oARGB)
		{
			chBlue  = oARGB.chBlue;
			chGreen = oARGB.chGreen;
			chRed   = oARGB.chRed;
			chAlpha = oARGB.chAlpha;
		}

		TEmfPlusARGB& operator=(const TEmfPlusARGB& oARGB)
		{
			chBlue  = oARGB.chBlue;
			chGreen = oARGB.chGreen;
			chRed   = oARGB.chRed;
			chAlpha = oARGB.chAlpha;

			return *this;
		}
	};

	struct TGUID
	{
		int           nData1;
		short         shData2;
		short         shData3;
		long long int llnData4;

		TGUID()
		{
			nData1   = 0;
			shData2  = 0;
			shData3  = 0;
			llnData4 = 0;
		}

		TGUID(const TGUID& oTGUID)
		{
			nData1   = oTGUID.nData1;
			shData2  = oTGUID.shData2;
			shData3  = oTGUID.shData3;
			llnData4 = oTGUID.llnData4;
		}
	};

	struct TEmfPlusRectF
	{
		double dX;
		double dY;
		double dWidth;
		double dHeight;

		TEmfPlusRectF() : dX(0), dY(0), dWidth(0), dHeight(0){}

		TEmfPlusRectF(const TEmfPlusRectF& oRectangle)
		{
			dX      = oRectangle.dX;
			dY      = oRectangle.dY;
			dWidth  = oRectangle.dWidth;
			dHeight = oRectangle.dHeight;
		}

		TEmfPlusRectF(const TEmfPlusRect& oRectangle)
		{
			dX      = (double)oRectangle.shX;
			dY      = (double)oRectangle.shY;
			dWidth  = (double)oRectangle.shWidth;
			dHeight = (double)oRectangle.shHeight;
		}

		TRectD ToRectD() const
		{
			TRectD oRectangle;

			oRectangle.Left   = dX;
			oRectangle.Top    = dY;
			oRectangle.Right  = dX + dWidth;
			oRectangle.Bottom = dY + dHeight;

			return oRectangle;
		}

		TRectL ToRectL() const
		{
			TRectL oRectangle;

			oRectangle.Left   = dX;
			oRectangle.Top    = dY;
			oRectangle.Right  = dX + dWidth;
			oRectangle.Bottom = dY + dHeight;

			return oRectangle;
		}
	};

	#define TEmfPlusXForm TEmfXForm

	struct TEmfPlusPoint : public TPointS
	{
		TEmfPlusPoint() : TPointS(0, 0) {}
		TEmfPlusPoint(short shX, short shY) : TPointS(shX, shY) {}
		
		TPointL ToPointL() const
		{
			TPointL oPoint;

			oPoint.X = (int)X;
			oPoint.Y = (int)Y;

			return oPoint;
		}
	};
	
	struct TEmfPlusPointF : public TPointD
	{
		TEmfPlusPointF() : TPointD(0, 0) {}
		TEmfPlusPointF(double dX, double dY) : TPointD(dX, dY) {}
		
		void Set(const TEmfPlusPoint& oPointS)
		{
			X = (double)oPointS.X;
			Y = (double)oPointS.Y;
		}
		
		TPointL ToPointL() const
		{
			TPointL oPoint;

			oPoint.X = (int)X;
			oPoint.Y = (int)Y;

			return oPoint;
		}
	};

	typedef  enum
	{
		CombineModeReplace    = 0x00,
		CombineModeIntersect  = 0x01,
		CombineModeUnion      = 0x02,
		CombineModeXOR        = 0x03,
		CombineModeExclude    = 0x04,
		CombineModeComplement = 0x05
	} EEmfPlusCombineMode;

	typedef enum
	{
		CustomLineCapDataTypeDefault         = 0x00000000,
		CustomLineCapDataTypeAdjustableArrow = 0x00000001
	} CustomLineCapDataType;

	typedef enum
	{
		CustomLineCapDataFillPath = 0x00000001,
		CustomLineCapDataLinePath = 0x00000002
	} CustomLineCapDataFlags;

	class CLineCapData : public ILineCap
	{
	public:
		CLineCapData() {};
		virtual ~CLineCapData() {};

		virtual CustomLineCapDataType GetType() const = 0;
	};

	struct TEmfPlusCustomLineCapArrowData : public CLineCapData
	{
		double         dWidth;
		double         dHeight;
		double         dMiddleInset;
		bool           bFillState;
		unsigned int   unLineStartCap;
		unsigned int   unLineEndCap;
		unsigned int   unLineJoin;
		double         dLineMiterLimit;
		double         dWidthScale;
		TEmfPlusPointF oFillHotSpot;
		TEmfPlusPointF oLineHotSpot;

		virtual ~TEmfPlusCustomLineCapArrowData() {};

		CustomLineCapDataType GetType() const override
		{
			return CustomLineCapDataTypeAdjustableArrow;
		};
	};

	class CEmfPlusPath;

	struct TEmfPlusCustomLineCapData : public CLineCapData
	{
		unsigned int   unCustomLineCapDataFlags;
		unsigned int   unBaseCap;
		double         dBaseInset;
		unsigned int   unStrokeStartCap;
		unsigned int   unStrokeEndCap;
		unsigned int   unStrokeJoin;
		double         dStrokeMiterLimit;
		double         dWidthScale;
		TEmfPlusPointF oFillHotSpot;
		TEmfPlusPointF oStrokeHotSpot;
		CEmfPlusPath*  pPath;

		TEmfPlusCustomLineCapData() : pPath(NULL) {};
		virtual ~TEmfPlusCustomLineCapData();

		CustomLineCapDataType GetType() const override
		{
			return CustomLineCapDataTypeDefault;
		};
	};
}

#endif // EMFPLUSTYPES_H
