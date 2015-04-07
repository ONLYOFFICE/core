#include "SlidePersist.h"

#include "../Records/TextFullSettings.h"

SSlidePersist::SSlidePersist() : m_arTextAttrs()
{
}

SSlidePersist::SSlidePersist(const SSlidePersist& oSrc)
{
	*this = oSrc;
}

CString SSlidePersist::ToString()
{
	CString str = _T("");
	str.Format(_T("<SlideInfo PsrRef='%d' SlideID='%d' />"), m_nPsrRef, m_nSlideID);
	return str;
}

SSlidePersist& SSlidePersist::operator =(const SSlidePersist& oSrc)
{
	m_nPsrRef	= oSrc.m_nPsrRef;
	m_nSlideID = oSrc.m_nSlideID;

	m_arTextAttrs = oSrc.m_arTextAttrs;
	return (*this);
}