#pragma once
#include "RtfChar.h"

class RtfBookmarkStart : public IDocumentElement
{
public: int nFirstColumn;
public: int nLastColumn;
public: CString m_sName;
public:  RtfBookmarkStart()
		{
			nFirstColumn = PROP_DEF;
			nLastColumn = PROP_DEF;
		}
public: int GetType()
		{
			return TYPE_RTF_BOOKMARKSTART;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter);
public: CString RenderToOOX(RenderParameter oRenderParameter);
};
class RtfBookmarkEnd : public IDocumentElement
{
public: CString m_sName;
public: RtfBookmarkEnd()
		{
		}
public: int GetType()
		{
			return TYPE_RTF_BOOKMARKEND;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter);
public: CString RenderToOOX(RenderParameter oRenderParameter);
};
class RtfFootnote : public IDocumentElement
{
public: bool m_bEndNote;
public: TextItemContainerPtr m_oContent;
public: RtfCharProperty m_oCharProp;
public: RtfFootnote()
		{
			m_bEndNote = false;
			m_oContent = TextItemContainerPtr( new TextItemContainer() );
		}
public: int GetType()
		{
			return TYPE_RTF_FOOTNOTE;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter);
public: CString RenderToOOX(RenderParameter oRenderParameter);
};

typedef boost::shared_ptr<RtfFootnote> RtfFootnotePtr;
typedef boost::shared_ptr<RtfBookmarkEnd> RtfBookmarkEndPtr;
typedef boost::shared_ptr<RtfBookmarkStart> RtfBookmarkStartPtr;