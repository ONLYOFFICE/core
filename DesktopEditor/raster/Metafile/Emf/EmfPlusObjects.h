#ifndef _METAFILE_EMF_EMFPLUSOBJECTS_H
#define _METAFILE_EMF_EMFPLUSOBJECTS_H

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

        #define CEmfPlusObjectBase CEmfObjectBase

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

	class CEmfPlusRegion : public CEmfPlusObjectBase
	{

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
                CEmfPlusObject(unsigned int unImageSize, bool bIsContineudObject)
                        : unSize(unImageSize), bIsContineudObject(bIsContineudObject){};

                virtual unsigned int GetSize() const
                {
                        return unSize;
                }

                virtual void SetSize(unsigned int unSize)
                {
                        this->unSize = unSize;
                }

                virtual bool IsContineudObject() const
                {
                        return bIsContineudObject;
                }

            private:

                friend class CEmfPlusImage;
                friend class CEmfPlusBitmap;
                friend class CEmfPlusBitmapData;
                friend class CEmfPlusCompressedImage;
                friend class CEmfPlusMetafile;

                bool bIsContineudObject;
                unsigned int unSize;
        };

        class CBitmapDataBase : public CEmfPlusObject
        {
            public:
                CBitmapDataBase(unsigned int unImageSize, bool bIsContineudObject)
                        : CEmfPlusObject(unImageSize, bIsContineudObject){};

                virtual EEmfPlusBitmapDataType GetType() const
                {
                        return BitmapDataTypeUnknow;
                };
        };

        class CEmfPlusBitmapData : public CBitmapDataBase
        {
            public:
                CEmfPlusBitmapData(bool bIsContineudObject)
                        : CBitmapDataBase(0, bIsContineudObject){};

                virtual EEmfPlusBitmapDataType GetType()
                {
                        return BitmapDataTypePixel;
                };
        };

        class CEmfPlusCompressedImage : public CBitmapDataBase
        {
            public:
                CEmfPlusCompressedImage(bool bIsContineudObject)
                        : CBitmapDataBase(0, bIsContineudObject){};

                virtual EEmfPlusBitmapDataType GetType()
                {
                        return BitmapDataTypeCompressed;
                };
        };

        class CImageDataBase : public CEmfPlusObject
        {
            public:
                CImageDataBase(unsigned int unImageSize, bool bIsContineudObject)
                        : CEmfPlusObject(unImageSize, bIsContineudObject){};

                virtual EEmfPlusImageDataType GetType() const
                {
                        return ImageDataTypeUnknown;
                }
        };

        class CEmfPlusBitmap : public CImageDataBase
        {
            public:
                CEmfPlusBitmap(bool bIsContineudObject)
                        : pBitmapData(NULL), CImageDataBase(20, bIsContineudObject){};

                virtual ~CEmfPlusBitmap(){ RELEASEOBJECT(pBitmapData); };

                virtual EEmfPlusImageDataType GetType() const
                {
                        return ImageDataTypeBitmap;
                }

                virtual unsigned int GetSize() const
                {
                        if (NULL == pBitmapData)
                                return unSize;

                        return unSize + pBitmapData->GetSize();
                }

                bool InitData(unsigned int unType)
                {
                        RELEASEOBJECT(pBitmapData);

                        if (unType == BitmapDataTypePixel)
                        {
                               pBitmapData = new CEmfPlusBitmapData(bIsContineudObject);
                               return pBitmapData;
                        }
                        else if (unType == BitmapDataTypeCompressed)
                        {
                                pBitmapData = new CEmfPlusCompressedImage(bIsContineudObject);
                                return pBitmapData;
                        }

                        return false;
                }

                int nWidth;
                int nHeight;
                int nStride;
                int nPixelFormat;

                CBitmapDataBase *pBitmapData;
        };

        class CEmfPlusMetafile : public CImageDataBase
        {
            public:
                CEmfPlusMetafile(bool bIsContineudObject)
                        : pMetafileData(NULL), unLastDataSize(0),
                          CImageDataBase(8, bIsContineudObject){};

                virtual ~CEmfPlusMetafile() { RELEASEOBJECT(pMetafileData); };

                virtual EEmfPlusImageDataType GetType() const
                {
                        return ImageDataTypeMetafile;
                }

                BYTE * GetData() const
                {
                        return pMetafileData;
                }

                void SetData(BYTE* pData, unsigned int unSizeData)
                {
                        RELEASEOBJECT(pMetafileData);

                        unSize -= unLastDataSize;
                        unLastDataSize = unSizeData;
                        unSize += unLastDataSize;

                        pMetafileData = pData;
                }

                void SetMetafileSize(unsigned int unSize)
                {
                        unMetafileSize = unSize;
                }

                unsigned int GetMetafileSize() const
                {
                        return unMetafileSize;
                }

            private:

                BYTE *pMetafileData;
                unsigned int unLastDataSize;
                unsigned int unMetafileSize;
        };

        class CEmfPlusImage : public CEmfPlusObject
        {
            public:
                CEmfPlusImage(bool bIsContineudObject)
                        : pImageData(NULL), CEmfPlusObject(8, bIsContineudObject){};

                virtual ~CEmfPlusImage() { RELEASEOBJECT(pImageData); };

                bool InitData(unsigned int unType)
                {
                        RELEASEOBJECT(pImageData)

                        if (unType == ImageDataTypeBitmap)
                        {
                                pImageData = new CEmfPlusBitmap(bIsContineudObject);
                                return pImageData;
                        }
                        else if (unType == ImageDataTypeMetafile)
                        {
                                pImageData = new CEmfPlusMetafile(bIsContineudObject);
                                return pImageData;
                        }

                        return false;
                }

                virtual unsigned int GetSize() const
                {
                        if (NULL == pImageData)
                                return unSize;

                        return unSize + pImageData->GetSize();
                }

                BYTE *GetMetafileData() const
                {
                        if (NULL == pImageData || pImageData->GetType() != ImageDataTypeMetafile)
                                return NULL;

                        ((CEmfPlusMetafile*)pImageData)->GetData();
                }

                unsigned int GetMetafileSize() const
                {
                        if (NULL == pImageData || pImageData->GetType() != ImageDataTypeMetafile)
                                return 0;

                        ((CEmfPlusMetafile*)pImageData)->GetMetafileSize();
                }

                CImageDataBase *pImageData;
        };

        class CEmfPlusContineudObjectRecord
        {
            public:
                CEmfPlusContineudObjectRecord() : pImage(NULL), unPosition(0) {};
                virtual ~CEmfPlusContineudObjectRecord() {};

                void SetDataSize(unsigned int unSize)
                {
                        arData.reserve(unSize);
                        unDataSize = unSize;
                }

                void SetNeedReadSize(unsigned int unSize)
                {
                        unNeedRead = unSize;
                }

                void AddData(BYTE *pData, unsigned int unSize)
                {
                        std::vector<BYTE> arNewData(pData, pData + unSize);
                        arData.insert(arData.begin() + unPosition, arNewData.begin(), arNewData.end());
                        unPosition += arNewData.size();
                }

                BYTE* GetData() const
                {
                        return (BYTE*)arData.data();
                }

                unsigned int GetNeedReadSize() const
                {
                        return unNeedRead;
                }

                bool IsFirstReading() const
                {
                        return (unPosition == 0);
                }

                bool IsLastReading()
                {
                        return ((unPosition + unNeedRead) >= unDataSize);
                }

                void SetEmfPlusImage(CEmfPlusImage* pImage)
                {
                        this->pImage = pImage;

                        if (NULL != pImage)
                               SetDataSize(pImage->GetMetafileSize());
                }

                void UpdateImage()
                {
                        if (NULL == pImage)
                                return;

                        if (NULL != pImage && NULL != pImage->pImageData && pImage->pImageData->GetType() == ImageDataTypeMetafile)
                        {
                                unsigned int unSizeData = arData.size();
                                BYTE* pBuffer = new BYTE[unSizeData];
                                memcpy(pBuffer, arData.data(), unSizeData);
                                ((CEmfPlusMetafile*)pImage->pImageData)->SetData(pBuffer, unSizeData);
                        }
                }

                CEmfPlusImage* GetEmfPlusImage() const
                {
                        return pImage;
                }

                unsigned int GetSize() const
                {
                        return arData.size();
                }

                unsigned int GetMaxSize() const
                {
                        return unDataSize;
                }

            private:
                std::vector<BYTE>       arData;
                unsigned int            unPosition;
                unsigned int            unNeedRead;
                unsigned int            unDataSize;

                CEmfPlusImage*          pImage;
        };
}

#endif // _METAFILE_EMF_EMFPLUSOBJECTS_H
