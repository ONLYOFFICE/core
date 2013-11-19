#pragma once
#include "RtfParagraph.h"
#include "RtfShape.h"

class RtfOle : public IDocumentElement
{
public: typedef enum{ ot_none, ot_emb, ot_link } OleType;
public: OleType m_eOleType;
public: int m_nShapeId;
public: int m_nWidth;
public: int m_nHeight;
public: CString m_sOleClass;
private: IStorage* m_piStorage;
private: CString m_sOleFilename;

public: RtfShapePtr m_oResultPic;

public: RtfOle()
		{
			m_piStorage = NULL;
			SetDefault();
		}
public: ~RtfOle()
		{
			SetDefault();
		}
public: int GetType( )
		{
			return TYPE_RTF_OLE;
		}
public: bool IsValid()
		{
			return PROP_DEF != m_nWidth && PROP_DEF != m_nHeight
					&& ::GetFileAttributes( m_sOleFilename ) != DWORD( -1 ) && _T("") != m_sOleFilename;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			if( false == IsValid() )
				return _T("");
			CString sResult;
			sResult.Append( _T("{\\object") );
			if( PROP_DEF != m_eOleType )
			{
				switch( m_eOleType )
				{
					case ot_emb:  sResult.Append( _T("\\objemb") );break;
					case ot_link:  sResult.Append( _T("\\objlink") );break;
				}
			}
			RENDER_RTF_INT( m_nWidth, sResult, _T("objw") );
			RENDER_RTF_INT( m_nHeight, sResult, _T("objh") );
			if( _T("") != m_sOleClass )
				sResult.AppendFormat( _T("{\\*\\objclass %s}"), m_sOleClass );
			if( _T("") != m_sOleFilename )
				sResult.AppendFormat( _T("{\\*\\objdata %s}"), RtfInternalEncoder::Encode( m_sOleFilename ) );
			if( NULL != m_oResultPic )
			{
				sResult.AppendFormat( _T("{\\result \\pard\\plain%s}"), m_oResultPic->RenderToRtf( oRenderParameter ) );
			}
			sResult.Append( _T("}") );
			return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter);
private: CString RenderToOOXOnlyOle(RenderParameter oRenderParameter);
public: void SetFilename( CString sFilename )
		{
			m_sOleFilename = sFilename;
		}
public: void SetOle( IStorage* piOle )
		{
			if( NULL != piOle )
				piOle->QueryInterface( __uuidof(IStorage), (void**)&m_piStorage );
		}
public: void SetDefault()
		{
			m_eOleType = ot_none;
			m_nShapeId = PROP_DEF;
			m_nWidth = PROP_DEF;
			m_nHeight = PROP_DEF;
			RELEASEINTERFACE( m_piStorage ); //вместе с этим удаляется и файл
			Utils::RemoveDirOrFile( m_sOleFilename );
			m_sOleFilename = _T("");
		}
};
typedef boost::shared_ptr<RtfOle> RtfOlePtr;

struct RtfOle1ToOle2Stream : OLESTREAM
{
	BYTE* pBuffer;
	long nBufferSize;
	long nCurPos;
};
struct RtfOle2ToOle1Stream : OLESTREAM
{
	CAtlArray<BYTE> aBuffer;
};
DWORD CALLBACK OleGet1(LPOLESTREAM oStream, void FAR* pTarget, DWORD dwRead);;
DWORD CALLBACK OlePut1(LPOLESTREAM, const void FAR*, DWORD);
DWORD CALLBACK OleGet2(LPOLESTREAM oStream, void FAR* pTarget, DWORD dwRead);;
DWORD CALLBACK OlePut2(LPOLESTREAM, const void FAR*, DWORD);