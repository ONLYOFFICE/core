#ifndef _METAFILE_EMF_EMFPLUSOBJECTS_H
#define _METAFILE_EMF_EMFPLUSOBJECTS_H

#include "EmfPath.h"
#include "EmfObjects.h"
#include "EmfPlusTypes.h"
#include "../../../common/File.h"
#include "../Common/MetaFileObjects.h"

namespace MetaFile
{
        typedef  enum
        {
                ObjectTypeInvalid           = 0x00,
                ObjectTypeBrush             = 0x01,
                ObjectTypePen               = 0x02,
                ObjectTypePath              = 0x03,
                ObjectTypeRegion            = 0x04,
                ObjectTypeImage             = 0x05,
                ObjectTypeFont              = 0x06,
                ObjectTypeStringFormat      = 0x07,
                ObjectTypeImageAttributes   = 0x08,
                ObjectTypeCustomLineCap     = 0x09
        } EEmfPlusObjectType;

        #define CEmfPlusObjectBase      CEmfObjectBase
        #define CEmfPlusDC              CEmfDC

	typedef  enum
	{
		BrushTypeSolidColor	= 0x00,
		BrushTypeHatchFill	= 0x01,
		BrushTypeTextureFill	= 0x02,
		BrushTypePathGradient	= 0x03,
		BrushTypeLinearGradient = 0x04
	} EEmfPlusBrushType;

	#define PEN_DATA_TRANSFORM	    0x00000001
	#define PEN_DATA_STARTCAP	    0x00000002
	#define PEN_DATA_ENDCAP		    0x00000004
	#define PEN_DATA_JOIN		    0x00000008
	#define PEN_DATA_MITERLIMIT	    0x00000010
	#define PEN_DATA_LINESTYLE	    0x00000020
	#define PEN_DATA_DASHEDLINECAP	    0x00000040
	#define PEN_DATA_DASHEDLINEOFFSET   0x00000080
	#define PEN_DATA_DASHEDLINE	    0x00000100
	#define PEN_DATA_NONCENTER	    0x00000200
	#define PEN_DATA_COMPOUNDLINE	    0x00000400
	#define PEN_DATA_CUSTOMSTARTCAP	    0x00000800
	#define PEN_DATA_CUSTOMENDCAP	    0x00001000

	#define PS_STARTCAP_ROUND     0x00000040
	#define PS_STARTCAP_SQUARE    0x00000050
	#define PS_STARTCAP_FLAT      0x00000060
	#define PS_STARTCAP_MASK      0x000000F0

	typedef  enum
	{
		ImageDataTypeUnknown	= 0x00,
		ImageDataTypeBitmap	= 0x01,
		ImageDataTypeMetafile	= 0x02
	} EEmfPlusImageDataType;

        typedef  enum
        {
                BitmapDataTypePixel       = 0x00,
                BitmapDataTypeCompressed  = 0x01,
                BitmapDataTypeUnknow      = 0x02
        } EEmfPlusBitmapDataType;

        typedef  enum
        {
                MetafileDataTypeUnknown         = 0x00,
                MetafileDataTypeWmf             = 0x01,
                MetafileDataTypeWmfPlaceable    = 0x02,
                MetafileDataTypeEmf             = 0x03,
                MetafileDataTypeEmfPlusOnly     = 0x04,
                MetafileDataTypeEmfPlusDual     = 0x05
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
			return std::wstring();
		}

		void GetBounds(double& left, double& top, double& width, double& height)
		{
			left	= RectF.dX;
			top	= RectF.dY;
			width	= RectF.dWidth;
			height	= RectF.dHeight;
		};

		TEmfPlusARGB		Color;
		TEmfPlusARGB		ColorBack;
		unsigned int		Style;
		unsigned int		Hatch;
		TEmfPlusRectF		RectF;
		unsigned int		Angle;
	};

	class CEmfPlusPen: public CEmfPlusObject, public IPen
	{
	    public:
		CEmfPlusPen() : Style(PS_SOLID), Width(1), Color(0, 0, 0), Brush(NULL), MiterLimit(0), DashOffset(0), DataDash(NULL), SizeDash(0) {}
		virtual ~CEmfPlusPen() { RELEASEOBJECT(Brush)
					 RELEASEARRAYOBJECTS(DataDash)}
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
			return 255;
//			return (unsigned int)Color.chAlpha;
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

		unsigned int	Style;
		double		Width;
		TEmfPlusARGB	Color;
		CEmfPlusBrush	*Brush;
		double		MiterLimit;
		double		DashOffset;
		double*		DataDash;
		unsigned int	SizeDash;
	};

        typedef  enum
        {
                FontStyleBold           = 0x00000001,
                FontStyleItalic         = 0x00000002,
                FontStyleUnderline      = 0x00000004,
                FontStyleStrikeout      = 0x00000008
        } EEmfPlusFontStyle;

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

		int          GetWeight()
		{
			return (m_bBold) ? 700 : 400;
		}

		bool         IsItalic()
		{
			return m_bItalic;
		}

		bool         IsStrikeOut()
		{
			return m_bStrikeout;
		}
		bool         IsUnderline()
		{
			return m_bUnderline;
		}
		int          GetEscapement()
		{
			return 0;
		}
		int          GetCharSet()
		{
			return 0;
		}
		int          GetOrientation()
		{
			return 0;
		}

		double		m_dEmSize;
		unsigned int	m_unSizeUnit;
		bool		m_bBold;
		bool		m_bItalic;
		bool		m_bUnderline;
		bool		m_bStrikeout;
		std::wstring	m_wsFamilyName;
	};

        typedef  enum
        {
                PathPointTypeStart      = 0x00,
                PathPointTypeLine       = 0x01,
                PathPointTypeBezier     = 0x03
        } EEmfPlusPathPointType;

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

                        oRect.dRight = oRect.dBottom = 0;

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
                WrapModeTile        = 0x00,
                WrapModeTileFlipX   = 0x01,
                WrapModeTileFlipY   = 0x02,
                WrapModeTileFlipXY  = 0x03,
                WrapModeClamp       = 0x04
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

        class CEmfPlusImage : public CEmfPlusObject
        {
            public:
                CEmfPlusImage() : CEmfPlusObject(), m_pImageBuffer(NULL), m_ulPosition(0),
                                  m_ulFullSize(0), m_eImageDataType(ImageDataTypeUnknown),
                                  m_eMetafileDataType(MetafileDataTypeUnknown){};
                virtual ~CEmfPlusImage()
                {
                        if (NULL != m_pImageBuffer)
                                delete [] m_pImageBuffer;
                };

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

                void SetSizeData(unsigned int unSize)
                {
                        if (NULL != m_pImageBuffer)
                                delete [] m_pImageBuffer;

                        m_pImageBuffer = new BYTE[unSize];
                        m_ulFullSize = unSize;
                }

                unsigned int GetUnreadSize() const
                {
                        return (m_ulFullSize - m_ulPosition);
                }

                void AddData(BYTE *pData, unsigned int unSize)
                {
                        if (NULL == m_pImageBuffer && 0 == m_ulFullSize && 0 < unSize)
                        {
                               SetSizeData(unSize);
                        }
                        else if (0 == m_ulFullSize)
                                return;

                        if (unSize + m_ulPosition > m_ulFullSize)
                                unSize = m_ulFullSize - m_ulPosition;

                        memcpy(m_pImageBuffer + m_ulPosition * sizeof (BYTE), pData, unSize);

                        m_ulPosition += unSize;
                }

                void GetData(BYTE*& pBuffer, unsigned int& unSize) const
                {
                        pBuffer = m_pImageBuffer;
                        unSize = m_ulPosition;
                }

            private:
                BYTE* m_pImageBuffer;
                ULONG m_ulPosition;
                ULONG m_ulFullSize;

                EEmfPlusImageDataType    m_eImageDataType;
                EEmfPlusMetafileDataType m_eMetafileDataType;
        };

        typedef  enum
        {
          RegionNodeDataTypeAnd         = 0x00000001,
          RegionNodeDataTypeOr          = 0x00000002,
          RegionNodeDataTypeXor         = 0x00000003,
          RegionNodeDataTypeExclude     = 0x00000004,
          RegionNodeDataTypeComplement  = 0x00000005,
          RegionNodeDataTypeRect        = 0x10000000,
          RegionNodeDataTypePath        = 0x10000001,
          RegionNodeDataTypeEmpty       = 0x10000002,
          RegionNodeDataTypeInfinite    = 0x10000003
        } EEmfPlusRegionNodeDataType;

        class CEmfPlusRegionNode
        {
            public:
                CEmfPlusRegionNode() : pRect(NULL){};
                virtual ~CEmfPlusRegionNode()
                {
                        RELEASEOBJECT(pRect);
                };

                EEmfPlusRegionNodeDataType GetType() const
                {
                        return eType;
                }

                TEmfPlusRectF* GetRect() const
                {
                        return pRect;
                }

                EEmfPlusRegionNodeDataType      eType;
                TEmfPlusRectF*                  pRect;
        };

        class CEmfPlusRegion : public CEmfPlusObject
        {
            public:
                CEmfPlusRegion() : CEmfPlusObject() {};
                virtual ~CEmfPlusRegion(){};

                virtual EEmfPlusObjectType GetObjectType() override
                {
                        return ObjectTypeRegion;
                }

                std::vector<CEmfPlusRegionNode> arNodes;
        };
}

#endif // _METAFILE_EMF_EMFPLUSOBJECTS_H
