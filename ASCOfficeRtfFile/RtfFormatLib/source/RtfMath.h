#pragma once
#include "Basic.h"

#include "RtfProperty.h"
#include "RtfParagraph.h"
#include "RtfShape.h"

class RtfMath : public IDocumentElement, public ItemContainer<IDocumentElementPtr>
{
public: 
	CString				m_sRtfName;
	CString				m_sOOXName;

	bool				m_bHeader; //чтобы отличать заголовок от вложенных групп

	bool				m_bIsVal;
	bool				m_bIsBool;
	
	RtfParagraph		m_oVal;
	RtfShapePtr			m_oPicture;

	RtfMath( )
	{
		m_bHeader = true;
		m_bIsVal = m_bIsBool = false;
	}
	RtfMath(const RtfMath& oMath )
	{
		(*this) = oMath;
	}
	RtfMath& operator=(const RtfMath& oMath )
	{
		m_bHeader	= oMath.m_bHeader;
		m_bIsVal	= oMath.m_bIsVal;
		m_bIsBool	= oMath.m_bIsBool;

		m_sRtfName	= oMath.m_sRtfName;
		m_sOOXName	= oMath.m_sOOXName;
	
		m_aArray.clear();
		m_aArray	= oMath.m_aArray;

		return (*this);
	}
	bool IsEmpty()
	{
		return m_sRtfName.IsEmpty();
	}
	bool IsValid()
	{
		return (false == m_sRtfName.IsEmpty());
	}
	bool IsRtfControlPropertyBool	( CString sControlW );
	bool IsRtfControlProperty		( CString sControlW );
	bool IsRtfControlWord			( CString sControlW );

	void SetRtfName(CString sName)
	{
		m_sRtfName = sName;
		
		m_sOOXName = sName;
		m_sOOXName.Insert(1, _T(":"));
	}

	void	SetOOXType		(int type);
	CString RenderToRtf		(RenderParameter oRenderParameter);
	CString RenderToOOX		(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfMath> RtfMathPtr;
