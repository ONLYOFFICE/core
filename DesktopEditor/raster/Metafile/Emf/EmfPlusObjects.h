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

	class CEmfPlusBrush : public CEmfPlusObjectBase, public IBrush
	{
	    public:
		CEmfPlusBrush() : Style(BS_SOLID) {};
		virtual ~CEmfPlusBrush() {};
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_BRUSH;
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
			return 0;
		}

		unsigned int GetHatch()
		{
			return Hatch;
		}

		unsigned int GetAlpha()
		{
			return 0;
		}

		unsigned int GetAlpha2()
		{
			return 0xff;
		}

		std::wstring GetDibPatterPath()
		{
			return std::wstring();
		}

		void GetBounds(double& left, double& top, double& width, double& height) {};

		TEmfPlusARGB		Color;
		TEmfPlusARGB		ColorBack;
		unsigned int		Style;
		unsigned int		Hatch;
	};

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

	class CEmfPlusPen: public CEmfPlusObjectBase, public IPen
	{
	    public:
		CEmfPlusPen() : Style(PS_SOLID), Width(1), Color(0, 0, 0), Brush(NULL) {}
		virtual ~CEmfPlusPen() { if (NULL != Brush) delete Brush; }
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_PEN;
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

		unsigned int GetWidth()
		{
			if (Width < 0)
				return 1;

			return (unsigned int)Width;
		}

		unsigned int	Style;
		double		Width;
		TEmfPlusARGB	Color;
		CEmfPlusBrush	*Brush;
	};

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
                MetafileDataTypeWmf             = 0x01,
                MetafileDataTypeWmfPlaceable    = 0x02,
                MetafileDataTypeEmf             = 0x03,
                MetafileDataTypeEmfPlusOnly     = 0x04,
                MetafileDataTypeEmfPlusDual     = 0x05
        } EEmfPlusMetafileDataType;

        class CEmfPlusObject
        {
            public:
                CEmfPlusObject(){};

                virtual EEmfPlusObjectType GetObjectType()
                {
                        return ObjectTypeInvalid;
                }
        };

        class CEmfPlusPath : public CEmfPlusObject, public CEmfPath
        {
            public:
                CEmfPlusPath() : CEmfPlusObject(), CEmfPath() {};
                CEmfPlusPath(CEmfPlusPath* pPath) : CEmfPlusObject(), CEmfPath(pPath) {};

                virtual EEmfPlusObjectType GetObjectType() override
                {
                        return ObjectTypeInvalid;
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
                                  m_ulFullSize(0), m_eImageDataType(ImageDataTypeUnknown){};
                ~CEmfPlusImage()
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
                        if (m_ulFullSize == 0)
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

                EEmfPlusImageDataType m_eImageDataType;
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
