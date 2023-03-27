#ifndef _METAFILE_EMF_EMFPLUSOBJECTS_H
#define _METAFILE_EMF_EMFPLUSOBJECTS_H

#include "EmfPath.h"
#include "EmfObjects.h"
#include "EmfPlusTypes.h"
#include "../../../common/File.h"
#include "../Common/MetaFileObjects.h"

#include "../../../../OOXML/Base/Types_32.h"

#ifndef MININT32
#define MAXUINT32   ((_UINT32)~((_UINT32)0))
#define MAXINT32    ((_INT32)(MAXUINT32 >> 1))
#define MININT32    ((_INT32)~MAXINT32)
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

		virtual EEmfPlusObjectType GetObjectType()
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
		CEmfPlusBrush() : CEmfPlusObject(), Style(BS_SOLID), Hatch(0), Angle(0) {};
		virtual ~CEmfPlusBrush() {};
		virtual EEmfObjectType GetType() override
		{
			return EMF_OBJECT_BRUSH;
		}

		virtual EEmfPlusObjectType GetObjectType() override
		{
				return ObjectTypeBrush;
		}

		int GetColor()
		{
			return METAFILE_RGBA(Color.chRed, Color.chGreen, Color.chBlue);
		}

		int GetColor2()
		{
			return METAFILE_RGBA(ColorBack.chRed, ColorBack.chGreen, ColorBack.chBlue);
		}

		unsigned int GetStyle()
		{
			return Style;
		}

		unsigned int GetStyleEx()
		{
			return Angle;
		}

		unsigned int GetHatch()
		{
			return Hatch;
		}

		unsigned int GetAlpha()
		{
			return Color.chAlpha;
		}

		unsigned int GetAlpha2()
		{
			return ColorBack.chAlpha;
		}

		std::wstring GetDibPatterPath()
		{
			return DibPatternPath;
		}

		void GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight)
		{

		}

		void GetCenterPoint(double& dX, double& dY)
		{
			dX = CenterPoint.X;
			dY = CenterPoint.Y;
		}

		void GetBounds(double& left, double& top, double& width, double& height)
		{
			left   = RectF.dX;
			top    = RectF.dY;
			width  = RectF.dWidth;
			height = RectF.dHeight;
		};

		TEmfPlusARGB   Color;
		TEmfPlusARGB   ColorBack;
		unsigned int   Style;
		unsigned int   Hatch;
		TEmfPlusRectF  RectF;
		TEmfPlusPointF CenterPoint;
		unsigned int   Angle;
		std::wstring   DibPatternPath;
	};

	class CEmfPlusPen: public CEmfPlusObject, public IPen
	{
	public:
		CEmfPlusPen() : Style(PS_SOLID), Width(1), Color(0, 0, 0),
		                Brush(NULL), MiterLimit(0), DashOffset(0),
		                DataDash(NULL), SizeDash(0), LineStartCapData(NULL), LineEndCapData(NULL) {}
		virtual ~CEmfPlusPen() { RELEASEOBJECT(Brush)
			                     RELEASEARRAYOBJECTS(DataDash)
			                     RELEASEOBJECT(LineStartCapData)
			                     RELEASEOBJECT(LineEndCapData)}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_PEN;
		}

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypePen;
		}

		int GetColor()
		{
			if (NULL != Brush)
				return Brush->GetColor();

			return METAFILE_RGBA(Color.chRed, Color.chGreen, Color.chBlue);
		};

		unsigned int GetStyle()
		{
			return Style;
		}

		double GetWidth()
		{
			if (Width < 0)
				return 1;

			return Width;
		}

		unsigned int GetAlpha()
		{
			if (NULL != Brush)
				return Brush->Color.chAlpha;

			return 255;
		}

		double GetMiterLimit()
		{
			return MiterLimit;
		}

		double GetDashOffset()
		{
			return DashOffset;
		}

		void GetDashData(double*& arDatas, unsigned int& unSize)
		{
			arDatas = DataDash;
			unSize  = SizeDash;
		}

		unsigned int  Style;
		double        Width;
		TEmfPlusARGB  Color;
		CEmfPlusBrush *Brush;
		double        MiterLimit;
		double        DashOffset;
		double*       DataDash;
		unsigned int  SizeDash;

		CLineCapData  *LineStartCapData;
		CLineCapData  *LineEndCapData;
	};

	class CEmfPlusFont : public CEmfPlusObject, public IFont
	{
	public:
		CEmfPlusFont() : m_dEmSize(18), m_unSizeUnit(0), m_bBold(false),
		                 m_bItalic(false), m_bUnderline(false), m_bStrikeout(false),
		                 m_wsFamilyName(L""){}
		virtual ~CEmfPlusFont() {}

		virtual EEmfObjectType GetType() override
		{
			return EMF_OBJECT_FONT;
		}

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypeFont;
		}

		double	GetHeight()
		{
			return m_dEmSize;
		}

		std::wstring GetFaceName()
		{
			return m_wsFamilyName;
		}

		int GetWeight()
		{
			return (m_bBold) ? 700 : 400;
		}

		bool IsItalic()
		{
			return m_bItalic;
		}

		bool IsStrikeOut()
		{
			return m_bStrikeout;
		}
		bool IsUnderline()
		{
			return m_bUnderline;
		}
		int GetEscapement()
		{
			return 0;
		}
		int GetCharSet()
		{
			return 0;
		}
		int GetOrientation()
		{
			return 0;
		}

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
		CEmfPlusBuffer() : m_pBuffer(NULL), m_ulPosition(0), m_ulFullSize(0)
		{}
		~CEmfPlusBuffer()
		{
			if (NULL != m_pBuffer)
				delete m_pBuffer;
		}

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypeBuffer;
		}

		void SetSize(unsigned int unSize)
		{
			if (NULL != m_pBuffer)
				delete [] m_pBuffer;

			m_pBuffer = new BYTE[unSize];
			m_ulFullSize   = unSize;
		}

		unsigned int GetSize() const
		{
			return m_ulFullSize;
		}

		void AddData(BYTE *pData, unsigned int unSize)
		{
			if (NULL == m_pBuffer && 0 == m_ulFullSize && 0 < unSize)
				SetSize(unSize);
			else if (0 == m_ulFullSize)
				    return;

			if (unSize + m_ulPosition > m_ulFullSize)
				unSize = m_ulFullSize - m_ulPosition;

			memcpy(m_pBuffer + m_ulPosition * sizeof (BYTE), pData, unSize);

			m_ulPosition += unSize;
		}

		unsigned int GetUnreadSize() const
		{
			return (m_ulFullSize - m_ulPosition);
		}

		void GetData(BYTE*& pBuffer, unsigned int& unSize) const
		{
			pBuffer = m_pBuffer;
			unSize = m_ulPosition;
		}
	private:
		BYTE* m_pBuffer;
		ULONG m_ulPosition;
		ULONG m_ulFullSize;

		friend class CEmfPlusImage;
	};

	class CEmfPlusPath : public CEmfPlusObject, public CEmfPath
	{
	public:
		CEmfPlusPath() : CEmfPlusObject(), CEmfPath() {};
		CEmfPlusPath(CEmfPlusPath* pPath) : CEmfPlusObject(), CEmfPath(pPath) {};

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypePath;
		}

		void Clear()
		{
			CEmfPath::Clear();
		}

		TRectD ConvertToRect() const
		{
			TRectD oRect;

			oRect.dRight = oRect.dBottom = MININT32;
			oRect.dLeft  = oRect.dTop    = MAXINT32;

			for (unsigned int ulIndex = 0; ulIndex < m_pCommands.size(); ulIndex++)
			{
				CEmfPathCommandBase* pCommand = m_pCommands.at(ulIndex);
				switch (pCommand->GetType())
				{
					case EMF_PATHCOMMAND_MOVETO:
					{
						CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;

						oRect.dLeft   = std::min(oRect.dLeft,	pMoveTo->x);
						oRect.dTop    = std::min(oRect.dTop,    pMoveTo->y);
						oRect.dRight  = std::max(oRect.dRight,  pMoveTo->x);
						oRect.dBottom = std::max(oRect.dBottom, pMoveTo->y);

						break;
					}
					case EMF_PATHCOMMAND_LINETO:
					{
						CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;

						oRect.dLeft   = std::min(oRect.dLeft,	pLineTo->x);
						oRect.dTop    = std::min(oRect.dTop,    pLineTo->y);
						oRect.dRight  = std::max(oRect.dRight,  pLineTo->x);
						oRect.dBottom = std::max(oRect.dBottom, pLineTo->y);

						break;
					}
					case EMF_PATHCOMMAND_CLOSE: return oRect;
				}
			}
			return oRect;
		}
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
		CEmfPlusImageAttributes() : CEmfPlusObject() {};

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypeImageAttributes;
		}

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
		CEmfPlusImage() : m_eImageDataType(ImageDataTypeUnknown),
		                  m_eMetafileDataType(MetafileDataTypeUnknown),
		                  m_unWidth(0), m_unHeight(0){};

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypeImage;
		}

		void SetImageDataType(unsigned int unImageDataType)
		{
			if (ImageDataTypeBitmap == unImageDataType)
				m_eImageDataType = ImageDataTypeBitmap;
			else if (ImageDataTypeMetafile == unImageDataType)
				m_eImageDataType = ImageDataTypeMetafile;
		}

		EEmfPlusImageDataType GetImageDataType() const
		{
			return m_eImageDataType;
		}

		bool SetMetafileType(unsigned int unType)
		{
			switch (unType)
			{
				case 1: m_eMetafileDataType = MetafileDataTypeWmf;              return true;
				case 2: m_eMetafileDataType = MetafileDataTypeWmfPlaceable;     return true;
				case 3: m_eMetafileDataType = MetafileDataTypeEmf;              return true;
				case 4: m_eMetafileDataType = MetafileDataTypeEmfPlusOnly;      return true;
				case 5: m_eMetafileDataType = MetafileDataTypeEmfPlusDual;      return true;
				default:m_eMetafileDataType = MetafileDataTypeUnknown;          return false;
			}
		}

		EEmfPlusMetafileDataType GetMetafileType() const
		{
			return m_eMetafileDataType;
		}

		void SetImageSize(unsigned int unWidth, unsigned int unHeight)
		{
			m_unWidth  = unWidth;
			m_unHeight = unHeight;
		}

		void GetImageSize(unsigned int &unWidth, unsigned int &unHeight)
		{
			unWidth  = m_unWidth;
			unHeight = m_unHeight;
		}
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
		CEmfPlusRegionNode() {};
		virtual ~CEmfPlusRegionNode() {};

		EEmfPlusRegionNodeDataType GetType() const
		{
			return eType;
		}

		EEmfPLusRegionNodeType GetNodeType() const
		{
			return EmfPLusRegionNodeTypeEmpty;
		}

		EEmfPlusRegionNodeDataType eType;
	};

	class CEmfPlusRegionNodePath : public CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNodePath() : pPath(NULL) {};
		virtual ~CEmfPlusRegionNodePath()
		{
			RELEASEOBJECT(pPath);
		}

		EEmfPLusRegionNodeType GetNodeType()
		{
			return EmfPLusRegionNodeTypePath;
		}

		bool Empty() const
		{
			return (NULL == pPath);
		}

		CEmfPlusPath* GetPath() const
		{
			return pPath;
		}

		CEmfPlusPath *pPath;
	};

	class CEmfPlusRegionNodeRectF : public CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNodeRectF() : pRect(NULL) {}
		virtual ~CEmfPlusRegionNodeRectF()
		{
			RELEASEOBJECT(pRect);
		}

		EEmfPLusRegionNodeType GetNodeType()
		{
			return EmfPLusRegionNodeTypeRectF;
		}

		bool Empty() const
		{
			return NULL == pRect;
		}

		TEmfPlusRectF* GetRect() const
		{
			return pRect;
		}

		TEmfPlusRectF* pRect;
	};

	class CEmfPlusRegionNodeChild : public CEmfPlusRegionNode
	{
	public:
		CEmfPlusRegionNodeChild() : pLeft(NULL), pRigth(NULL) {};
		virtual ~CEmfPlusRegionNodeChild()
		{
			RELEASEOBJECT(pLeft);
			RELEASEOBJECT(pRigth);
		}

		EEmfPLusRegionNodeType GetNodeType()
		{
			return EmfPLusRegionNodeTypeChild;
		}

		void ClipRegionOnRenderer(IOutputDevice* pOutput, TRect* pOutRect = NULL)
		{
			if (NULL == pOutput)
				return;

			unsigned int unType;

			switch (eType)
			{
				case RegionNodeDataTypeAnd:
				case RegionNodeDataTypeExclude:
				case RegionNodeDataTypeComplement:
				{
					unType = RGN_AND;
					break;
				}
				case RegionNodeDataTypeOr:
				{
					unType = RGN_OR;
					break;
				}
				case RegionNodeDataTypeXor:
				{
					unType = RGN_XOR;
					break;
				}
				default: return;
			}

			pOutput->StartClipPath(unType, ALTERNATE);

			for (CEmfPlusRegionNode *pNode : std::vector<CEmfPlusRegionNode*>{pLeft, pRigth})
			{
				switch (pNode->GetNodeType())
				{
					case EmfPLusRegionNodeTypeEmpty:
					{
						if (NULL != pOutRect)
						{
							pOutput->MoveTo(pOutRect->nLeft,  pOutRect->nTop);
							pOutput->LineTo(pOutRect->nRight, pOutRect->nTop);
							pOutput->LineTo(pOutRect->nRight, pOutRect->nBottom);
							pOutput->LineTo(pOutRect->nLeft,  pOutRect->nBottom);
							pOutput->ClosePath();
						}
						break;
					}
					case EmfPLusRegionNodeTypePath:
					{
						CEmfPlusRegionNodePath *pRegionNodePath = (CEmfPlusRegionNodePath*)pNode;

						if (!pRegionNodePath->Empty())
						{
							for (unsigned int ulIndex = 0; ulIndex < pRegionNodePath->GetPath()->m_pCommands.size(); ulIndex++)
							{
								CEmfPathCommandBase* pCommand = pRegionNodePath->GetPath()->m_pCommands.at(ulIndex);
								switch (pCommand->GetType())
								{
									case EMF_PATHCOMMAND_MOVETO:
									{
										CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;
										pOutput->MoveTo(pMoveTo->x, pMoveTo->y);
										break;
									}
									case EMF_PATHCOMMAND_LINETO:
									{
										CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;
										pOutput->LineTo(pLineTo->x, pLineTo->y);
										break;
									}
									case EMF_PATHCOMMAND_CURVETO:
									{
										CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;
										pOutput->CurveTo(pCurveTo->x1, pCurveTo->y1, pCurveTo->x2, pCurveTo->y2, pCurveTo->xE, pCurveTo->yE);
										break;
									}
									case EMF_PATHCOMMAND_ARCTO:
									{
										CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;
										pOutput->ArcTo(pArcTo->left, pArcTo->top, pArcTo->right, pArcTo->bottom, pArcTo->start, pArcTo->sweep);
										break;
									}
									case EMF_PATHCOMMAND_CLOSE:
									{
										pOutput->ClosePath();
										break;
									}
								}
							}
						}
					}
					case EmfPLusRegionNodeTypeRectF:
					{
						CEmfPlusRegionNodeRectF *pRegionNodeRectF = (CEmfPlusRegionNodeRectF*)pNode;

						if (!pRegionNodeRectF->Empty())
						{
							TRectD oRect = pRegionNodeRectF->GetRect()->GetRectD();

							pOutput->MoveTo(oRect.dLeft,  oRect.dTop);
							pOutput->LineTo(oRect.dRight, oRect.dTop);
							pOutput->LineTo(oRect.dRight, oRect.dBottom);
							pOutput->LineTo(oRect.dLeft,  oRect.dBottom);
							pOutput->ClosePath();
						}
					}
					default: break;
				}
			}

			pOutput->EndClipPath(unType);
		}

		CEmfPlusRegionNode *pLeft, *pRigth;
	};

	class CEmfPlusRegion : public CEmfPlusObject
	{
	public:
		CEmfPlusRegion() : CEmfPlusObject() {};
		virtual ~CEmfPlusRegion()
		{
			for (CEmfPlusRegionNode* pNode : arNodes)
				delete pNode;

			arNodes.clear();
		};

		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypeRegion;
		}

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
		CEmfPlusStringFormat() : CEmfPlusObject() {};
		virtual EEmfPlusObjectType GetObjectType() override
		{
			return ObjectTypeStringFormat;
		}

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
