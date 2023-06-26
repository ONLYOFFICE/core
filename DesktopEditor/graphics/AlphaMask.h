#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"
#include "./config.h"

namespace Aggplus
{
	class CAlphaMask_private;
	class GRAPHICS_DECL CAlphaMask : public IGrObject
	{
	public:
		CAlphaMask();
		~CAlphaMask();

		void Clear();

		Status CrateImageBuffer(UINT unWidth, UINT unHeight);
		Status CrateAlphaBuffer(UINT unWidth, UINT unHeight);

		Status LoadFromAlphaBuffer(BYTE* pBuffer, UINT unWidth, UINT unHeight, bool bExternalBuffer = true);
		Status LoadFromImageBuffer(BYTE* pBuffer, UINT unWidth, UINT unHeight, bool bExternalBuffer = true);

		Status LoadFromFile(const std::wstring& wsFilePath);
		Status LoadFromImage(IGrObject* pGrObject, bool bCopy = true);

		BYTE* GetMask();

		CAlphaMask& operator=(const CAlphaMask& oAlphaMask);
	public:
		CAlphaMask_private *m_internal;
	};
}

#endif // _BUILD_ALPHAMASK_H_
