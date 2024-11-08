#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"
#include "./config.h"

namespace agg
{
template<class AlphaMask>
class scanline_u8_am;
template<unsigned R, unsigned G, unsigned B>
struct rgb_to_gray_mask_u8;
struct one_component_mask_u8;
template<unsigned Step, unsigned Offset, class MaskF>
class alpha_mask_u8;
}

namespace Aggplus
{
	enum class EMaskDataType
	{
		ImageBuffer,
		AlphaBuffer
	};

	class GRAPHICS_DECL CAlphaMask : public IGrObject
	{
	public:
		CAlphaMask();
		CAlphaMask(BYTE* pBuffer, EMaskDataType eDataType, bool bExternalBuffer = true);
		virtual ~CAlphaMask();

		BYTE*         GetBuffer();
		EMaskDataType GetDataType() const;
		UINT          GetStep() const;

		Status Create(UINT unWidth, UINT unHeight, EMaskDataType eDataType);
		Status LoadFromBuffer(BYTE* pBuffer, EMaskDataType eDataType, bool bExternalBuffer = true);
	private:
		BYTE         *m_pBuffer;
		EMaskDataType m_enDataType;
		bool          m_bExternalBuffer;
	};

	enum class ESoftMaskType
	{
		RGBGrayBuffer,
		BGRGrayBuffer,
		Alpha4Buffer
	};

	class CSoftMask_private;
	class GRAPHICS_DECL CSoftMask : public IGrObject
	{
	public:
		CSoftMask(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bFlip, bool bRGB, bool bAlpha);
		~CSoftMask();

		unsigned int GetStep() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		BYTE* GetBuffer();
		ESoftMaskType GetDataType();

		agg::scanline_u8_am<agg::alpha_mask_u8<4, 0, agg::rgb_to_gray_mask_u8<2, 1, 0> > >& GetScanlineBGRGray();
		agg::scanline_u8_am<agg::alpha_mask_u8<4, 0, agg::rgb_to_gray_mask_u8<0, 1, 2> > >& GetScanlineRGBGray();
		agg::scanline_u8_am<agg::alpha_mask_u8<4, 3, agg::one_component_mask_u8> >& GetScanlineAlpha4();

	private:
		CSoftMask_private* m_pInternal;
	};
}

#endif // _BUILD_ALPHAMASK_H_
