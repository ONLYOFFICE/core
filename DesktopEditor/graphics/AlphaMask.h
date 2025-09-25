#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

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

	private:
		CSoftMask_private* m_pInternal;

		friend class CGraphics;
	};
}

#endif // _BUILD_ALPHAMASK_H_
