#pragma once

namespace NSPresentationEditor
{
	class CEffect
	{
	public:
		CString m_strEffect;

	public:
		CEffect() : m_strEffect(_T(""))
		{
		}
		CEffect& operator=(const CEffect& oSrc)
		{
			m_strEffect	= oSrc.m_strEffect;
			return *this;
		}
		CEffect(const CEffect& oSrc)
		{
			*this = oSrc;
		}
		~CEffect()
		{
		}
	};

	class CEffects
	{
	public:
		CAtlArray<CEffect> m_arEffects;

	public:
		CEffects() : m_arEffects()
		{
		}
		CEffects& operator=(const CEffects& oSrc)
		{
			m_arEffects.Copy(oSrc.m_arEffects);
			return *this;
		}
		CEffects(const CEffects& oSrc)
		{
			*this = oSrc;
		}
		~CEffects()
		{
		}
	};
}