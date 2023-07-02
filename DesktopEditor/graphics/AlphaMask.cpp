#include "AlphaMask.h"
#include "AlphaMask_private.h"

namespace Aggplus
{
	CAlphaMask::CAlphaMask()
		: m_internal(new CAlphaMask_private())
	{}

	CAlphaMask::~CAlphaMask()
	{
		RELEASEOBJECT(m_internal)
	}

	void CAlphaMask::Clear()
	{
		m_internal->Clear();
	}

	StatusAlphaMask CAlphaMask::GetStatus() const
	{
		return m_internal->GetStatus();
	}
	AMaskDataType CAlphaMask::GetDataType() const
	{
		return m_internal->GetDataType();
	}

	Status CAlphaMask::CreateImageBuffer(UINT unWidth, UINT unHeight)
	{
		return m_internal->Create(unWidth, unHeight, ImageBuffer);
	}

	Status CAlphaMask::CreateAlphaBuffer(UINT unWidth, UINT unHeight)
	{
		return m_internal->Create(unWidth, unHeight, AlphaBuffer);
	}

	Status CAlphaMask::LoadFromAlphaBuffer(BYTE *pBuffer, UINT unWidth, UINT unHeight, bool bExternalBuffer)
	{
		return m_internal->LoadFromBuffer(pBuffer, unWidth, unHeight, AlphaBuffer, bExternalBuffer);
	}

	Status CAlphaMask::LoadFromImageBuffer(BYTE *pBuffer, UINT unWidth, UINT unHeight, bool bExternalBuffer)
	{
		return m_internal->LoadFromBuffer(pBuffer, unWidth, unHeight, ImageBuffer, bExternalBuffer);
	}

	Status CAlphaMask::LoadFromFile(const std::wstring &wsFilePath)
	{
		return m_internal->LoadFromFile(wsFilePath);
	}

	Status CAlphaMask::LoadFromImage(IGrObject *pGrObject, bool bCopy)
	{
		return m_internal->LoadFromImage(pGrObject, bCopy);
	}

	BYTE *CAlphaMask::GetMask()
	{
		return m_internal->GetMask();
	}

	CAlphaMask &CAlphaMask::operator=(const CAlphaMask &oAlphaMask)
	{
		*m_internal = *oAlphaMask.m_internal;
		return *this;
	}
}
