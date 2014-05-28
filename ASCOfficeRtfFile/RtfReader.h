#pragma once
#include "RtfLex.h"
#include "RtfProperty.h"
#include "RtfDocument.h"
#include "../Common/OfficeFileTemplate.h"

[ event_source(native), event_receiver(native)]
class RtfReader
{
public: __event void OnCompleteItem();

public: class ReaderState;
typedef boost::shared_ptr<ReaderState> ReaderStatePtr;
public: class ReaderState
{
	public: int m_nUD; // количество символов игнорируемых за юникодом
	public: RtfCharProperty m_oCharProp;
	public: RtfParagraphProperty m_oParagraphProp;
	public: RtfRowProperty m_oRowProperty;
	public: RtfCellProperty m_oCellProperty;
	public: RtfOldList m_oCurOldList;
	//public: RtfSectionProperty m_oSectionProp;
	public: ReaderStatePtr psave;
	public: CStringA m_sCurText;
	ReaderState()
	{
		m_nUD = 1;
		m_oCharProp.SetDefaultRtf();
		m_oParagraphProp.SetDefaultRtf();
		m_oRowProperty.SetDefaultRtf();
		m_oCellProperty.SetDefaultRtf();
		m_oCurOldList.SetDefault();
		//m_oSectionProp.SetDefaultOOX();
	}
};

//---------------------------------TextParser----------------------------------
public: ReaderStatePtr m_oState;
public: RtfSectionProperty m_oCurSectionProp;
public: RtfLex m_oLex;
private: RtfDocument& m_oDocument;
private: CString m_sFilename;
public: int m_nFootnote; //толко дл€ симовола chftn. основано на том что вложенных footnote быть не может
public: int m_nDefFont;
public: CString m_sTempFolder;
public: short m_shCancel;
public: RtfReader(RtfDocument& oDocument, CString sFilename );
public: ~RtfReader()
		{
		}
public: void PushState();
public: void PopState();
public: bool Load();
public: long GetProgress()
		{
			return (long)( g_cdMaxPercent * m_oLex.GetProgress());
		}
public: void Stop()
		{
			m_oLex.CloseSource();
		}
};
class RtfAbstractReader
{
private: RtfToken m_oTok;
private: bool m_bCanStartNewReader;
private: bool m_bStop;
private: int m_nSkipChars;
protected: int m_nCurGroups;
private: bool m_bSkip;
public: NFileWriter::CBufferedFileWriter* m_oFileWriter;

public: RtfAbstractReader()
		{
			m_bCanStartNewReader = false;
			m_bStop = false;
			m_bSkip = false;
			m_nSkipChars = 0;
			m_nCurGroups = 1;
			m_oFileWriter = NULL;
		}
public: bool Parse(RtfDocument& oDocument, RtfReader& oReader )
    {
		NFileWriter::CBufferedFileWriter* poOldWriter = oReader.m_oLex.m_oFileWriter;
		oReader.m_oLex.m_oFileWriter = m_oFileWriter;// ДuДГД|Дy Д~Дu NULL, ДДДА ДДДuД{ДГДД ДБДyДКДuД} ДГДВДpДxДЕ Дr ДЖДpДzД|

        int res = 0;
		m_oTok = oReader.m_oLex.NextCurToken();
		if( m_oTok.Type == m_oTok.None )
			m_oTok = oReader.m_oLex.NextToken();
		//bool bSkip = false;
		while (m_oTok.Type != RtfToken::Eof && false == m_bStop && c_shProgressCancel != oReader.m_shCancel)
		{
            switch (m_oTok.Type)
            {
				case RtfToken::GroupStart:
						ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
						PushState(oReader);
						break;
				case RtfToken::GroupEnd:
						ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
						PopState(oDocument, oReader);
						break;
				case RtfToken::Keyword:
						ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
						if( _T("u") == m_oTok.Key )
						{
							ExecuteText( oDocument, oReader, ExecuteTextInternal( oDocument, oReader, m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter, m_nSkipChars) );
							break;
						}
						else
						{
							if( true == m_bSkip )
							{
								if( false ==  ExecuteCommand( oDocument, oReader, (CStringW)m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter ) )
									Skip( oDocument, oReader );
								m_bSkip = false;
							}
							else
								ExecuteCommand( oDocument, oReader, (CStringW)m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter );
						}
						if( true == m_bCanStartNewReader )
							m_bCanStartNewReader = false;
						break;
				case RtfToken::Control:
						if( m_oTok.Key == _T("42") )
							m_bSkip = true;
						if( m_oTok.Key == _T("39") && true == m_oTok.HasParameter )
							oReader.m_oState->m_sCurText.AppendChar( m_oTok.Parameter );
						break;
				case RtfToken::Text:
						oReader.m_oState->m_sCurText.Append( m_oTok.Key );
						break;
			}
			if( false == m_bStop )
				m_oTok = oReader.m_oLex.NextToken();
        }

		oReader.m_oLex.m_oFileWriter = poOldWriter;

        return true;
    }
public: virtual void PushState(RtfReader& oReader)
		{
			oReader.PushState();
			m_nCurGroups++;
			m_bCanStartNewReader = true;
		}
public: virtual void PopState(RtfDocument& oDocument, RtfReader& oReader)
		{
			if( m_nCurGroups > 0 )
				m_nCurGroups--;
			else
				;//ATLASSERT(false);
			if( m_nCurGroups == 0 )
			{
				m_bStop = true;
				ExitReader( oDocument, oReader );
			}
			oReader.PopState();
			if( m_nCurGroups == 0 )
				ExitReader2( oDocument, oReader );
		}
public: bool StartSubReader( RtfAbstractReader& poNewReader, RtfDocument& oDocument, RtfReader& oReader  )
		{
			if( true == m_bCanStartNewReader )
			{
				m_bCanStartNewReader = false;
				m_nCurGroups--;
				//poNewReader.PushState( oReader );
				poNewReader.m_bSkip = m_bSkip;
				return poNewReader.Parse(oDocument, oReader);
			}
			return false;
		}
public: void Skip( RtfDocument& oDocument, RtfReader& oReader )
		{
			int cGroup = 1;
			while( cGroup >= 1 )
			{
				m_oTok = oReader.m_oLex.NextToken();
				if(m_oTok.Type == RtfToken::GroupStart)
					cGroup++;
				else if(m_oTok.Type == RtfToken::GroupEnd)
					cGroup--;
				else if(m_oTok.Type == RtfToken::Eof)
					break;
			}
			PopState( oDocument, oReader );
		}
public: virtual bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
		{
			return true;
		}
public: virtual void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString oText )
		{
		}
public: virtual void ExitReader( RtfDocument& oDocument, RtfReader& oReade )
		{
		}
public: virtual void ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
		{
		}
public: static CString ExecuteTextInternal( RtfDocument& oDocument, RtfReader& oReader, CStringA& sKey, bool bHasPar, int nPar, int& nSkipChars)
		{
			CString sResult;

			if( _T("u") == sKey )
			{
				if( true == bHasPar )
					sResult.AppendChar( nPar );
			}
			else
			{
				CStringA sCharString;
				if( _T("39") == sKey )
				{
					if( true == bHasPar )
						sCharString.AppendChar( nPar );
				}
				else
					sCharString = sKey;

				sResult = ExecuteTextInternalCodePage(sCharString, oDocument, oReader);
			}
			ExecuteTextInternalSkipChars(sResult, oReader, sKey, nSkipChars);
			return sResult;
		}

public: void ExecuteTextInternal2( RtfDocument& oDocument, RtfReader& oReader, CStringA& sKey, int& nSkipChars)
		{
			if(oReader.m_oState->m_sCurText.GetLength() > 0)
			{
				CString sResult = ExecuteTextInternalCodePage(oReader.m_oState->m_sCurText, oDocument, oReader);
				oReader.m_oState->m_sCurText.Empty();
				if(sResult.GetLength() > 0)
				{
					ExecuteTextInternalSkipChars(sResult, oReader, CStringA(""), nSkipChars);
					ExecuteText( oDocument, oReader, sResult);
				}
			}
		}
public: static void ExecuteTextInternalSkipChars(CString & sResult, RtfReader& oReader, CStringA& sKey, int& nSkipChars)
		{
			//удал€ем символы вслед за юникодом
			if( nSkipChars > 0 )
			{
				int nLength = sResult.GetLength();
				if( nSkipChars >= nLength )
				{
					nSkipChars -= nLength;
					sResult.Empty();
				}
				else
				{
					sResult = sResult.Right( nLength - nSkipChars );
					nSkipChars = 0;
				}
			}
			if( _T("u") == sKey )
			{
				//надо правильно установить m_nSkipChars по значению \ucN
				nSkipChars = oReader.m_oState->m_nUD;
			}
		}
public: static CString ExecuteTextInternalCodePage( CStringA& sCharString, RtfDocument& oDocument, RtfReader& oReader)
		{
			CString sResult;
			if( false == sCharString.IsEmpty() )
			{
				int nCodepage = -1;
				//примен€ем параметры codepage от текущего шрифта todo associated fonts.
				RtfFont oFont;
				if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) )
				{
					if( PROP_DEF != oFont.m_nCharset )
						nCodepage = RtfUtility::CharsetToCodepage( oFont.m_nCharset );
					else if( PROP_DEF != oFont.m_nCodePage )
						nCodepage = oFont.m_nCodePage;
				}
				//от настроек документа
				if( -1 == nCodepage && RtfDocumentProperty::cp_none != oDocument.m_oProperty.m_eCodePage )
				{
					switch ( oDocument.m_oProperty.m_eCodePage )
					{
					case RtfDocumentProperty::cp_ansi:
						{
							if( PROP_DEF != oDocument.m_oProperty.m_nAnsiCodePage )
								nCodepage = oDocument.m_oProperty.m_nAnsiCodePage;
							else
								nCodepage = CP_ACP;
							break;
						}
					case RtfDocumentProperty::cp_mac: nCodepage = CP_MACCP;break;
					case RtfDocumentProperty::cp_pc: nCodepage = 437;break;
					case RtfDocumentProperty::cp_pca: nCodepage = 850;break;
					}
				}
				//если ничего нет ставим ANSI
				if( -1 == nCodepage )
					nCodepage = CP_ACP;

				int nLengthW ;
				nLengthW = MultiByteToWideChar(nCodepage, 0, sCharString, -1, NULL, NULL);
				MultiByteToWideChar(nCodepage, 0, sCharString, -1, sResult.GetBuffer( nLengthW ), nLengthW);
				sResult.ReleaseBuffer();
				//sResult = sKey;
			}
			return sResult;
		}
};

