#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"
#include "./config.h"

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
}

#endif // _BUILD_ALPHAMASK_H_
