#pragma once
#include "RtfChar.h"

class RtfBookmarkStart : public IDocumentElement
{
public: 
	int nFirstColumn;
	int nLastColumn;
	CString m_sName;
	RtfBookmarkStart()
	{
		nFirstColumn = PROP_DEF;
		nLastColumn = PROP_DEF;
	}
	int GetType()
	{
		return TYPE_RTF_BOOKMARKSTART;
	}
	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter);
};
class RtfBookmarkEnd : public IDocumentElement
{
public: 
	CString m_sName;
	RtfBookmarkEnd()
	{
	}
	int GetType()
	{
		return TYPE_RTF_BOOKMARKEND;
	}
	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter);
};
class RtfFootnote : public IDocumentElement
{
public: 
	bool m_bEndNote;
	TextItemContainerPtr m_oContent;
	RtfCharProperty m_oCharProp;
	RtfFootnote()
	{
		m_bEndNote = false;
		m_oContent = TextItemContainerPtr( new TextItemContainer() );
	}
	int GetType()
	{
		return TYPE_RTF_FOOTNOTE;
	}
	CString RenderToRtf(RenderParameter oRenderParameter);
	CString RenderToOOX(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfFootnote>		RtfFootnotePtr;
typedef boost::shared_ptr<RtfBookmarkEnd>	RtfBookmarkEndPtr;
typedef boost::shared_ptr<RtfBookmarkStart> RtfBookmarkStartPtr;