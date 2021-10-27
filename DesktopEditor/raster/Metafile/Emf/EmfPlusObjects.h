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

        class CEmfPlusBitmapDataBase
        {
            public:
                CEmfPlusBitmapDataBase(unsigned int unSize, bool ThereIsContinuation) : Data(NULL), unRemainderRecordSize(unSize), bThereIsContinuation(ThereIsContinuation) {};
                virtual ~CEmfPlusBitmapDataBase() { if (NULL != Data) delete Data; };
                virtual EEmfPlusBitmapDataType GetType() const
                {
                        return BitmapDataTypeUnknow;
                }

               virtual unsigned int GetRemainderRecordSize() const
                {
                        return unRemainderRecordSize;
                }

                bool ThereIsContinuation() const
                {
                        return bThereIsContinuation;
                }

                BYTE*   Data;
            private:
                unsigned int    unRemainderRecordSize;
                bool            bThereIsContinuation;
        };

        class CEmfPlusBitmapData : public CEmfPlusBitmapDataBase
        {
            public:
                CEmfPlusBitmapData(unsigned int unSize, bool ThereIsContinuation) : CEmfPlusBitmapDataBase(unSize, ThereIsContinuation) {};
                virtual ~CEmfPlusBitmapData() {};
                virtual EEmfPlusBitmapDataType GetType() const
                {
                        return BitmapDataTypePixel;
                }
        };

        class CEmfPlusCompressedImage : public CEmfPlusBitmapDataBase
        {
            public:
                CEmfPlusCompressedImage(unsigned int unSize, bool ThereIsContinuation) : CEmfPlusBitmapDataBase(unSize, ThereIsContinuation) {};
                virtual ~CEmfPlusCompressedImage() {};
                virtual EEmfPlusBitmapDataType GetType() const
                {
                        return BitmapDataTypeCompressed;
                }
        };

        class CImageDataBase
        {
            public:
                CImageDataBase(unsigned int unSize, bool ThereIsContinuation) : unRemainderRecordSize(unSize), bThereIsContinuation(ThereIsContinuation) {};
                virtual ~CImageDataBase(){};
                virtual EEmfPlusImageDataType GetType() const
                {
                        return ImageDataTypeUnknown;
                };

                virtual unsigned int GetRemainderRecordSize() const
                {
                        return unRemainderRecordSize;
                }

            private:
                unsigned int    unRemainderRecordSize;
                bool            bThereIsContinuation;

                friend class CImageDataBitmap;
                friend class CImageDataMetafile;
        };

        class CImageDataBitmap : public CImageDataBase
        {
            public:
                CImageDataBitmap(unsigned int unSize, bool ThereIsContinuation) : CImageDataBase(unSize, ThereIsContinuation), pData(NULL) {};
                virtual ~CImageDataBitmap() { if (NULL != pData) delete pData; };
                virtual EEmfPlusImageDataType GetType() const
                {
                        return ImageDataTypeBitmap;
                };

                bool InitData(unsigned int unType)
                {
                        if (NULL != pData)
                                delete pData;

                        if (unType == BitmapDataTypeUnknow)
                                return false;
                        else if (unType == BitmapDataTypePixel)
                                pData = new CEmfPlusBitmapData(unRemainderRecordSize - 20, bThereIsContinuation);
                        else if (unType == BitmapDataTypeCompressed)
                                pData = new CEmfPlusCompressedImage(unRemainderRecordSize - 20, bThereIsContinuation);
                        else
                                return false;

                        return true;
                }

                virtual unsigned int GetRemainderRecordSize() const
                {
                        if (NULL == pData)
                                return CImageDataBase::GetRemainderRecordSize();

                        return pData->GetRemainderRecordSize();
                }

                int nWidth;
                int nHeight;
                int nStride;
                int nPixelFormat;

                CEmfPlusBitmapDataBase *pData;
        };

        class CImageDataMetafile : public CImageDataBase
        {
            public:
                CImageDataMetafile(unsigned int unSize, bool ThereIsContinuation) : CImageDataBase(unSize, ThereIsContinuation) {};
                virtual ~CImageDataMetafile(){};
                virtual EEmfPlusImageDataType GetType() const
                {
                        return ImageDataTypeMetafile;
                };

                void SetRemainderRecordSize(unsigned int unSize)
                {
                        unRemainderRecordSize = unSize;
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
                unsigned int    unMetafileSize;
        };

        class CEmfPlusImage
        {
            public:
                CEmfPlusImage(unsigned int unSize) : pData(NULL), unRemainderRecordSize(unSize), bThereIsContinuation(false) {};
                CEmfPlusImage(unsigned int unSize, bool ThereIsContinuation) : pData(NULL), unRemainderRecordSize(unSize), bThereIsContinuation(ThereIsContinuation) {};
                virtual ~CEmfPlusImage(){ if (NULL != pData) delete pData; };

                bool InitData(unsigned int unType)
                {
                        if (NULL != pData)
                                delete pData;

                        if (unType == ImageDataTypeUnknown)
                                return false;
                        else if (unType == ImageDataTypeBitmap)
                                pData = new CImageDataBitmap(unRemainderRecordSize - 8, bThereIsContinuation);
                        else if (unType == ImageDataTypeMetafile)
                                pData = new CImageDataMetafile(unRemainderRecordSize - 8, bThereIsContinuation);
                        else
                                return false;

                        return true;
                }

                unsigned int GetRemainderRecordSize()
                {
                        if (NULL != pData)
                                return pData->GetRemainderRecordSize();

                        return 0;
                }

                bool ValidData() const
                {
                        return (NULL != pData);
                }

                CImageDataBase *pData;
            private:
                unsigned int    unRemainderRecordSize;
                bool            bThereIsContinuation;
        };

        class CEmfPlusContineudObjectRecord
        {
            public:
                CEmfPlusContineudObjectRecord() : pImage(NULL) { unPosition = 0; };
                ~CEmfPlusContineudObjectRecord() {};

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

                        if (NULL != pImage && NULL != pImage->pData && pImage->pData->GetType() == ImageDataTypeMetafile)
                        {
                               SetDataSize(((CImageDataMetafile*)pImage->pData)->GetMetafileSize());
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
