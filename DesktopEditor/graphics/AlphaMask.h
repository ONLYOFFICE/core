#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"
#include "./config.h"

#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"

namespace Aggplus
{
	enum class EMaskDataType
	{
		ImageBuffer,
		AlphaBuffer,
		Alpha4Buffer
	};
	typedef agg::scanline_u8_am<agg::alpha_mask_rgba32a> ScanlineRGBA32A;

	class GRAPHICS_DECL CAlphaMask : public IGrObject
	{
	public:
		CAlphaMask();
		CAlphaMask(BYTE* pBuffer, EMaskDataType eDataType, bool bExternalBuffer = true);
		virtual ~CAlphaMask();

		BYTE*         GetBuffer();
		EMaskDataType GetDataType() const;
		UINT          GetStep() const;

		void SetDataType(EMaskDataType oType);
		Status Create(UINT unWidth, UINT unHeight, EMaskDataType eDataType);
		Status LoadFromBuffer(BYTE* pBuffer, EMaskDataType eDataType, bool bExternalBuffer = true);

		ScanlineRGBA32A GetScanline();
	private:
		BYTE         *m_pBuffer;
		EMaskDataType m_enDataType;
		bool          m_bExternalBuffer;
	};
}

#endif // _BUILD_ALPHAMASK_H_
