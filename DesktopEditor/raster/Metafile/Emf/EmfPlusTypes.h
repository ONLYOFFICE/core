#ifndef EMFPLUSTYPES_H
#define EMFPLUSTYPES_H

#include "EmfTypes.h"

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

		TEmfRectL GetRectL()
		{
			TEmfRectL oRect;

			oRect.lLeft   = (int)shX;
			oRect.lRight  = (int)(shX + shWidth);
			oRect.lTop    = (int)shY;
			oRect.lBottom = (int)(shY + shHeight);

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
			chAlpha = 0;
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
	};


	struct TEmfPlusPointR
	{
		//TODO: реализовать
	};

	struct TGUID
	{
		int nData1;
		short shData2;
		short shData3;
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

		TRectD GetRectD() const
		{
			TRectD oRectangle;

			oRectangle.dLeft   = dX;
			oRectangle.dTop    = dY;
			oRectangle.dRight  = dX + dWidth;
			oRectangle.dBottom = dY + dHeight;

			return oRectangle;
		}

		TEmfRectL GetRectL() const
		{
			TEmfRectL oRectangle;

			oRectangle.lLeft   = dX;
			oRectangle.lTop    = dY;
			oRectangle.lRight  = dX + dWidth;
			oRectangle.lBottom = dY + dHeight;

			return oRectangle;
		}
	};

    #define TEmfPlusPoint TEmfPointS
    #define TEmfPlusXForm TEmfXForm

	struct TEmfPlusPointF
	{
		double X;
		double Y;

		TEmfPlusPointF(){}

		TEmfPlusPointF(const double& dX, const double& dY) : X(dX), Y(dY) {};

		TEmfPointL ToPointL() const
		{
			TEmfPointL oPoint;

			oPoint.x = (int)X;
			oPoint.y = (int)Y;

			return oPoint;
		}

		TEmfPlusPointF& operator=(const TEmfPlusPoint& oPoint)
		{
			X = (double)oPoint.x;
			Y = (double)oPoint.y;

			return *this;
		}

		TEmfPlusPointF& operator=(const TEmfPlusPointR& oPoint)
		{
			//TODO: реализовать
			return *this;
		}

		TEmfPlusPointF& operator+(const TEmfPlusPointR& oPoint)
		{
			//TODO: реализовать
			return *this;
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

	class CLineCapData
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
		virtual ~TEmfPlusCustomLineCapData() { RELEASEOBJECT(pPath) };

		CustomLineCapDataType GetType() const override
		{
			return CustomLineCapDataTypeDefault;
		};
	};
}

#endif // EMFPLUSTYPES_H
