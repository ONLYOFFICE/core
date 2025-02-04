#ifndef _METAFILE_EMF_EMFPLUSOBJECTS_H
#define _METAFILE_EMF_EMFPLUSOBJECTS_H

#include "EmfObjects.h"
#include "EmfPlusTypes.h"
#include "../../../common/File.h"
#include "../Common/MetaFileObjects.h"
#include "../Common/CPath.h"
#include "../Common/CClip.h"
#include <stdint.h>

#ifndef MININT32
#define MAXUINT32   ((uint32_t)~((uint32_t)0))
#define MAXINT32    ((int32_t)(MAXUINT32 >> 1))
#define MININT32    ((int32_t)~MAXINT32)
#endif

namespace MetaFile
{
	typedef enum
	{
		ObjectTypeInvalid         = 0x00,
		ObjectTypeBrush           = 0x01,
		ObjectTypePen             = 0x02,
		ObjectTypePath            = 0x03,
		ObjectTypeRegion          = 0x04,
		ObjectTypeImage           = 0x05,
		ObjectTypeFont            = 0x06,
		ObjectTypeStringFormat    = 0x07,
		ObjectTypeImageAttributes = 0x08,
		ObjectTypeCustomLineCap   = 0x09,
		ObjectTypeBuffer          = 0xff
	} EEmfPlusObjectType;

    #define CEmfPlusObjectBase      CEmfObjectBase
    #define CEmfPlusDC              CEmfDC

	typedef enum
	{
		BrushTypeSolidColor     = 0x00,
		BrushTypeHatchFill      = 0x01,
		BrushTypeTextureFill	= 0x02,
		BrushTypePathGradient	= 0x03,
		BrushTypeLinearGradient = 0x04
	} EEmfPlusBrushType;

    #define PEN_DATA_TRANSFORM        0x00000001
    #define PEN_DATA_STARTCAP         0x00000002
    #define PEN_DATA_ENDCAP           0x00000004
    #define PEN_DATA_JOIN             0x00000008
    #define PEN_DATA_MITERLIMIT       0x00000010
    #define PEN_DATA_LINESTYLE        0x00000020
    #define PEN_DATA_DASHEDLINECAP    0x00000040
    #define PEN_DATA_DASHEDLINEOFFSET 0x00000080
    #define PEN_DATA_DASHEDLINE       0x00000100
    #define PEN_DATA_NONCENTER        0x00000200
    #define PEN_DATA_COMPOUNDLINE     0x00000400
    #define PEN_DATA_CUSTOMSTARTCAP   0x00000800
    #define PEN_DATA_CUSTOMENDCAP     0x00001000

    #define PS_STARTCAP_ROUND  0x00000040
    #define PS_STARTCAP_SQUARE 0x00000050
    #define PS_STARTCAP_FLAT   0x00000060
    #define PS_STARTCAP_MASK   0x000000F0

	typedef enum
	{
		ImageDataTypeUnknown  = 0x00,
		ImageDataTypeBitmap   = 0x01,
		ImageDataTypeMetafile = 0x02
	} EEmfPlusImageDataType;

	typedef enum
	{
		BitmapDataTypePixel      = 0x00,
		BitmapDataTypeCompressed = 0x01,
		BitmapDataTypeUnknow     = 0x02
	} EEmfPlusBitmapDataType;

	typedef enum
	{
		MetafileDataTypeUnknown      = 0x00,
		MetafileDataTypeWmf          = 0x01,
		MetafileDataTypeWmfPlaceable = 0x02,
		MetafileDataTypeEmf          = 0x03,
		MetafileDataTypeEmfPlusOnly  = 0x04,
		MetafileDataTypeEmfPlusDual  = 0x05
	} EEmfPlusMetafileDataType;

	class CEmfPlusObject : public CEmfPlusObjectBase
	{
	public:
		CEmfPlusObject(){};
		virtual ~CEmfPlusObject(){};

		virtual EEmfPlusObjectType GetObjectType() const
		{
			return ObjectTypeInvalid;
		}
	};

	typedef enum
	{
		HatchStyleHorizontal             = 0x00000000,
		HatchStyleVertical               = 0x00000001,
		HatchStyleForwardDiagonal        = 0x00000002,
		HatchStyleBackwardDiagonal       = 0x00000003,
		HatchStyleLargeGrid              = 0x00000004,
		HatchStyleDiagonalCross          = 0x00000005,
		HatchStyle05Percent              = 0x00000006,
		HatchStyle10Percent              = 0x00000007,
		HatchStyle20Percent              = 0x00000008,
		HatchStyle25Percent              = 0x00000009,
		HatchStyle30Percent              = 0x0000000A,
		HatchStyle40Percent              = 0x0000000B,
		HatchStyle50Percent              = 0x0000000C,
		HatchStyle60Percent              = 0x0000000D,
		HatchStyle70Percent              = 0x0000000E,
		HatchStyle75Percent              = 0x0000000F,
		HatchStyle80Percent              = 0x00000010,
		HatchStyle90Percent              = 0x00000011,
		HatchStyleLightDownwardDiagonal  = 0x00000012,
		HatchStyleLightUpwardDiagonal    = 0x00000013,
		HatchStyleDarkDownwardDiagonal   = 0x00000014,
		HatchStyleDarkUpwardDiagonal     = 0x00000015,
		HatchStyleWideDownwardDiagonal   = 0x00000016,
		HatchStyleWideUpwardDiagonal     = 0x00000017,
		HatchStyleLightVertical          = 0x00000018,
		HatchStyleLightHorizontal        = 0x00000019,
		HatchStyleNarrowVertical         = 0x0000001A,
		HatchStyleNarrowHorizontal       = 0x0000001B,
		HatchStyleDarkVertical           = 0x0000001C,
		HatchStyleDarkHorizontal         = 0x0000001D,
		HatchStyleDashedDownwardDiagonal = 0x0000001E,
		HatchStyleDashedUpwardDiagonal   = 0x0000001F,
		HatchStyleDashedHorizontal       = 0x00000020,
		HatchStyleDashedVertical         = 0x00000021,
		HatchStyleSmallConfetti          = 0x00000022,
		HatchStyleLargeConfetti          = 0x00000023,
		HatchStyleZigZag                 = 0x00000024,
		HatchStyleWave                   = 0x00000025,
		HatchStyleDiagonalBrick          = 0x00000026,
		HatchStyleHorizontalBrick        = 0x00000027,
		HatchStyleWeave                  = 0x00000028,
		HatchStylePlaid                  = 0x00000029,
		HatchStyleDivot                  = 0x0000002A,
		HatchStyleDottedGrid             = 0x0000002B,
		HatchStyleDottedDiamond          = 0x0000002C,
		HatchStyleShingle                = 0x0000002D,
		HatchStyleTrellis                = 0x0000002E,
		HatchStyleSphere                 = 0x0000002F,
		HatchStyleSmallGrid              = 0x00000030,
		HatchStyleSmallCheckerBoard      = 0x00000031,
		HatchStyleLargeCheckerBoard      = 0x00000032,
		HatchStyleOutlinedDiamond        = 0x00000033,
		HatchStyleSolidDiamond           = 0x00000034
	} EEmfPlusHatchStyle;

	typedef enum
	{
		BrushDataPath             = 0x00000001,
		BrushDataTransform        = 0x00000002,
		BrushDataPresetColors     = 0x00000004,
		BrushDataBlendFactorsH    = 0x00000008,
		BrushDataBlendFactorsV    = 0x00000010,
		BrushDataFocusScales      = 0x00000040,
		BrushDataIsGammaCorrected = 0x00000080,
		BrushDataDoNotTransform   = 0x00000100
	} EEmfPlusBrushDataFlags;

	class CEmfPlusBrush : public CEmfPlusObject, public IBrush
	{
	public:
		CEmfPlusBrush();
		virtual ~CEmfPlusBrush();
		virtual EEmfObjectType GetType()           const override;
		virtual EEmfPlusObjectType GetObjectType() const override;

		// IBrush
		int          GetColor()         const override;
		int          GetColor2()        const override;
		unsigned int GetStyle()         const override;
		unsigned int GetStyleEx()       const override;
		unsigned int GetHatch()         const override;
		unsigned int GetAlpha()         const override;
		unsigned int GetAlpha2()        const override;
		std::wstring GetDibPatterPath() const override;
		void         GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight) const override;
		void         GetCenterPoint(double& dX, double& dY) const override;
		void         GetBounds(double& left, double& top, double& width, double& height) const override;

		void GetGradientColors(std::vector<long>& arColors, std::vector<double>& arPositions) const override;
	public:
		TEmfPlusARGB   oColor;
		TEmfPlusARGB   oColorBack;
		unsigned int   unStyle;
		unsigned int   unHatch;
		TEmfPlusRectF  oRectF;
		TEmfPlusPointF oCenterPoint;
		unsigned int   unAngle;
		std::wstring   wsDibPatternPath;

		std::vector<std::pair<TEmfPlusARGB, double>> arGradientColors;
	};

	class CEmfPlusPen: public CEmfPlusObject, public IPen
	{
	public:
		CEmfPlusPen();
		virtual ~CEmfPlusPen();
		virtual EEmfObjectType GetType()           const override;
		virtual EEmfPlusObjectType GetObjectType() const override;

		// IPen
		int             GetColor()        const override;
		unsigned int    GetStyle()        const override;
		double          GetWidth()        const override;
		unsigned int    GetAlpha()        const override;
		double          GetMiterLimit()   const override;
		double          GetDashOffset()   const override;
		void            GetDashData(double*& arDatas, unsigned int& unSize) const override;
		const ILineCap* GetStartLineCap() const override;
		const ILineCap* GetEndLineCap()   const override;
	public:
		unsigned int   unStyle;
		double         dWidth;
		TEmfPlusARGB   oColor;
		CEmfPlusBrush *pBrush;
		double         dMiterLimit;
		double         dDashOffset;
		double*        pDataDash;
		unsigned int   unSizeDash;

		CLineCapData  *pLineStartCapData;
		CLineCapData  *pLineEndCapData;
	};

	class CEmfPlusFont : public CEmfPlusObject, public IFont
	{
	public:
		CEmfPlusFont();
		virtual ~CEmfPlusFont();
		virtual EEmfObjectType GetType()           const override;
		virtual EEmfPlusObjectType GetObjectType() const override;

		// IFont
		double       GetHeight()      const override;
		std::wstring GetFaceName()    const override;
		int          GetWeight()      const override;
		bool         IsItalic()       const override;
		bool         IsStrikeOut()    const override;
		bool         IsUnderline()    const override;
		int          GetEscapement()  const override;
		int          GetCharSet()     const override;
		int          GetOrientation() const override;

	public:
		double       m_dEmSize;
		unsigned int m_unSizeUnit;
		bool         m_bBold;
		bool         m_bItalic;
		bool         m_bUnderline;
		bool         m_bStrikeout;
		std::wstring m_wsFamilyName;
	};

	typedef  enum
	{
		PathPointTypeStart  = 0x00,
		PathPointTypeLine   = 0x01,
		PathPointTypeBezier = 0x03
	} EEmfPlusPathPointType;

	class CEmfPlusBuffer : public CEmfPlusObject
	{
	public:
		CEmfPlusBuffer();
		virtual ~CEmfPlusBuffer();
		virtual EEmfPlusObjectType GetObjectType() const override;

		void         SetSize(unsigned int unSize);
		unsigned int GetSize() const;
		void         AddData(BYTE *pData, unsigned int unSize);
		unsigned int GetUnreadSize() const;
		void         GetData(BYTE*& pBuffer, unsigned int& unSize) const;
	private:
		BYTE* m_pBuffer;
		ULONG m_ulPosition;
		ULONG m_ulFullSize;

		friend class CEmfPlusImage;
	};

	class CEmfPlusPath : public CEmfPlusObject, public CPath
	{
	public:
		CEmfPlusPath();
		CEmfPlusPath(CEmfPlusPath* pPath);
		virtual ~CEmfPlusPath();
		virtual EEmfPlusObjectType GetObjectType() const override;

		TRectD GetBounds() const;
	};

	typedef  enum
	{
		WrapModeTile       = 0x00,
		WrapModeTileFlipX  = 0x01,
		WrapModeTileFlipY  = 0x02,
		WrapModeTileFlipXY = 0x03,
		WrapModeClamp      = 0x04
	} EEmfPlusWrapMode;

	class CEmfPlusImageAttributes : public CEmfPlusObject
	{
	public:
		CEmfPlusImageAttributes();
		virtual EEmfPlusObjectType GetObjectType() const override;

	public:
		EEmfPlusWrapMode eWrapMode;
		TEmfPlusARGB     oClampColor;
		int              nObjectClamp;
	};

//		typedef  enum
//		 {
//		   BitmapDataTypePixel = 0x00000000,
//		   BitmapDataTypeCompressed = 0x00000001
//		 } BitmapDataType;

	class CEmfPlusImage : public CEmfPlusBuffer
	{
	public:
		CEmfPlusImage();
		virtual EEmfPlusObjectType GetObjectType() const override;

		void                     SetImageDataType(unsigned int unImageDataType);
		EEmfPlusImageDataType    GetImageDataType() const;
		bool                     SetMetafileType(unsigned int unType);
		EEmfPlusMetafileDataType GetMetafileType()  const;

		void SetImageSize(unsigned int unWidth, unsigned int unHeight);
		void GetImageSize(unsigned int &unWidth, unsigned int &unHeight);
	private:

		EEmfPlusImageDataType    m_eImageDataType;
		EEmfPlusMetafileDataType m_eMetafileDataType;

		unsigned int m_unWidth;
		unsigned int m_unHeight;
	};

	typedef  enum
	{
		RegionNodeDataTypeAnd        = 0x00000001,
		RegionNodeDataTypeOr         = 0x00000002,
		RegionNodeDataTypeXor        = 0x00000003,
		RegionNodeDataTypeExclude    = 0x00000004,
		RegionNodeDataTypeComplement = 0x00000005,
		RegionNodeDataTypeRect       = 0x10000000,
		RegionNodeDataTypePath       = 0x10000001,
		RegionNodeDataTypeEmpty      = 0x10000002,
		RegionNodeDataTypeInfinite   = 0x10000003
	} EEmfPlusRegionNodeDataType;

	typedef enum
	{
		EmfPLusRegionNodeTypeEmpty = 0,
		EmfPLusRegionNodeTypePath  = 1,
		EmfPLusRegionNodeTypeRectF = 2,
		EmfPLusRegionNodeTypeChild = 3
	} EEmfPLusRegionNodeType;

	class CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNode();
		virtual ~CEmfPlusRegionNode();

		virtual EEmfPlusRegionNodeDataType GetType()     const;
		virtual EEmfPLusRegionNodeType     GetNodeType() const;

	public:
		EEmfPlusRegionNodeDataType eType;
	};

	class CEmfPlusRegionNodePath : public CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNodePath();
		virtual ~CEmfPlusRegionNodePath();

		EEmfPLusRegionNodeType GetNodeType() const override;

		bool          Empty() const;
		CEmfPlusPath* GetPath() const;

	public:
		CEmfPlusPath *pPath;
	};

	class CEmfPlusRegionNodeRectF : public CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNodeRectF();
		virtual ~CEmfPlusRegionNodeRectF();
		
		EEmfPLusRegionNodeType GetNodeType() const override;

		bool           Empty()   const;
		TEmfPlusRectF* GetRect() const;

	public:
		TEmfPlusRectF* pRect;
	};

	class CEmfPlusRegionNodeChild : public CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNodeChild();
		virtual ~CEmfPlusRegionNodeChild();

		EEmfPLusRegionNodeType GetNodeType() const override;
		
		void DrawOnClip(CClip& oClip, const TXForm& oTransform, const TRectL* pOutRect = NULL);
	public:
		CEmfPlusRegionNode *pLeft, *pRigth;
	};

	class CEmfPlusRegion : public CEmfPlusObject
	{
	public:
		CEmfPlusRegion();
		virtual ~CEmfPlusRegion();

		virtual EEmfPlusObjectType GetObjectType() const override;

	public:
		std::vector<CEmfPlusRegionNode*> arNodes;
	};

	typedef  enum
	{
		FontStyleBold      = 0x00000001,
		FontStyleItalic    = 0x00000002,
		FontStyleUnderline = 0x00000004,
		FontStyleStrikeout = 0x00000008
	} EEmfPlusFontStyle;

	typedef  enum
	{
		StringAlignmentNear   = 0x00000000,
		StringAlignmentCenter = 0x00000001,
		StringAlignmentFar    = 0x00000002
	} EmfPlusStringAlignment;

	typedef  enum
	{
		StringTrimmingNone              = 0x00000000,
		StringTrimmingCharacter         = 0x00000001,
		StringTrimmingWord              = 0x00000002,
		StringTrimmingEllipsisCharacter = 0x00000003,
		StringTrimmingEllipsisWord      = 0x00000004,
		StringTrimmingEllipsisPath      = 0x00000005
	} EmfPlusStringTrimming;

	class CEmfPlusStringFormat : public CEmfPlusObject
	{
	public:
		CEmfPlusStringFormat();
		virtual EEmfPlusObjectType GetObjectType() const override;

		unsigned int unStringFormatFlags;
		unsigned int unStringAlignment;
		unsigned int unLineAlign;
		double       dFirstTabOffset;
		double       dLeadingMargin;
		double       dTrailingMargin;
		double       dTracking;
		unsigned int unTrimming;
	};
}

#endif // _METAFILE_EMF_EMFPLUSOBJECTS_H
