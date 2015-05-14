#pragma once
#include "RtfLex.h"
#include "RtfProperty.h"
#include "RtfDocument.h"

class RtfConvertationManager;

class RtfReader
{
public: 
	class ReaderState;
	typedef boost::shared_ptr<ReaderState> ReaderStatePtr;

    RtfConvertationManager *m_convertationManager;
	
	class ReaderState
	{
		public: 
            int                     m_nUD; // количество символов игнорируемых за юникодом
            RtfCharProperty         m_oCharProp;
            RtfParagraphProperty    m_oParagraphProp;
            RtfRowProperty          m_oRowProperty;
            RtfCellProperty         m_oCellProperty;
            RtfOldList              m_oCurOldList;
		//		RtfSectionProperty m_oSectionProp;
			ReaderStatePtr psave;
            std::string             m_sCurText;
			
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
    ReaderStatePtr      m_oState;
    RtfSectionProperty  m_oCurSectionProp;
    RtfLex              m_oLex;
    int                 m_nFootnote; //толко для симовола chftn. основано на том что вложенных footnote быть не может
    int                 m_nDefFont;
    CString             m_sTempFolder;

	RtfReader(RtfDocument& oDocument, std::wstring sFilename );
	~RtfReader()
	{
	}
	void PushState();
	void PopState();
	bool Load();
	long GetProgress()
	{
		return (long)( g_cdMaxPercent * m_oLex.GetProgress());
	}
	void Stop()
	{
		m_oLex.CloseSource();
	}
private:
	RtfDocument& m_oDocument;
	std::wstring m_sFilename;
};

class RtfAbstractReader
{
public:
	NFileWriter::CBufferedFileWriter* m_oFileWriter;

	RtfAbstractReader()
	{
		m_bCanStartNewReader = false;
		m_bSkip = false;
		m_nSkipChars = 0;
		m_nCurGroups = 1;
		m_oFileWriter = NULL;
		m_bStopReader = false;

		m_bUseGlobalCodepage = false;
	}
	bool Parse(RtfDocument& oDocument, RtfReader& oReader )
	{
		NFileWriter::CBufferedFileWriter* poOldWriter = oReader.m_oLex.m_oFileWriter;
		oReader.m_oLex.m_oFileWriter = m_oFileWriter;

		int res = 0;
		m_oTok = oReader.m_oLex.NextCurToken();
		if( m_oTok.Type == m_oTok.None )
			m_oTok = oReader.m_oLex.NextToken();
	
		while (m_oTok.Type != RtfToken::Eof && false == m_bStopReader)
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
                        if( m_oTok.Key ==  "u")
						{
							ExecuteText( oDocument, oReader, ExecuteTextInternal( oDocument, oReader, m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter, m_nSkipChars) );
							break;
						}
						else
						{
							if( true == m_bSkip )
							{
								if( false ==  ExecuteCommand( oDocument, oReader, CString(m_oTok.Key.c_str()), m_oTok.HasParameter, m_oTok.Parameter ) )
									Skip( oDocument, oReader );
								m_bSkip = false;
							}
							else
								ExecuteCommand( oDocument, oReader, CString(m_oTok.Key.c_str()), m_oTok.HasParameter, m_oTok.Parameter );
						}
						if( true == m_bCanStartNewReader )
							m_bCanStartNewReader = false;
						break;
				case RtfToken::Control:
                        if( m_oTok.Key == "42" )
							m_bSkip = true;
                        if( m_oTok.Key == "39" && true == m_oTok.HasParameter )
                            oReader.m_oState->m_sCurText += m_oTok.Parameter ;
						break;
				case RtfToken::Text:
                        oReader.m_oState->m_sCurText += m_oTok.Key;
						break;
			}
			if( false == m_bStopReader)
				m_oTok = oReader.m_oLex.NextToken();
		}

		oReader.m_oLex.m_oFileWriter = poOldWriter;

		return true;
	}
	virtual void PushState(RtfReader& oReader)
	{
		oReader.PushState();
		m_nCurGroups++;
		m_bCanStartNewReader = true;
	}
	virtual void PopState(RtfDocument& oDocument, RtfReader& oReader)
	{
		if( m_nCurGroups > 0 )
			m_nCurGroups--;
		else
			;//ATLASSERT(false);
		if( m_nCurGroups == 0 )
		{
			m_bStopReader = true;
			ExitReader( oDocument, oReader );
		}
		oReader.PopState();
		if( m_nCurGroups == 0 )
			ExitReader2( oDocument, oReader );
	}
	bool StartSubReader( RtfAbstractReader& poNewReader, RtfDocument& oDocument, RtfReader& oReader  )
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
	void Skip( RtfDocument& oDocument, RtfReader& oReader )
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
	virtual bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
	{
		return true;
	}
	virtual void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString oText )
	{
	}
	virtual void ExitReader( RtfDocument& oDocument, RtfReader& oReade )
	{
	}
	virtual void ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
	{
	}
    /*static */CString ExecuteTextInternal( RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, bool bHasPar, int nPar, int& nSkipChars)
	{
		CString sResult;

        if( "u" == sKey )
		{
			if( true == bHasPar )
				sResult.AppendChar( nPar );
		}
		else
		{
            std::string sCharString;
            if( "39" == sKey )
			{
				if( true == bHasPar )
                    sCharString += (char)nPar ;
			}
			else
				sCharString = sKey;

			sResult = ExecuteTextInternalCodePage(sCharString, oDocument, oReader);
		}
		ExecuteTextInternalSkipChars(sResult, oReader, sKey, nSkipChars);
		return sResult;
	}

    void ExecuteTextInternal2( RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars)
	{
        if(oReader.m_oState->m_sCurText.length() > 0)
		{
			CString sResult = ExecuteTextInternalCodePage(oReader.m_oState->m_sCurText, oDocument, oReader);
            oReader.m_oState->m_sCurText.erase();
			if(sResult.GetLength() > 0)
			{
                std::string str;
                ExecuteTextInternalSkipChars(sResult, oReader, str, nSkipChars);
				ExecuteText( oDocument, oReader, sResult);
			}
		}
	}
    /*static */void ExecuteTextInternalSkipChars(CString & sResult, RtfReader& oReader, std::string & sKey, int& nSkipChars)
	{
		//удаляем символы вслед за юникодом
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
        if( "u" == sKey )
		{
			//надо правильно установить m_nSkipChars по значению \ucN
			nSkipChars = oReader.m_oState->m_nUD;
		}
	}
    /*static */CString ExecuteTextInternalCodePage( std::string & sCharString, RtfDocument & oDocument, RtfReader & oReader);
	
	bool		m_bUseGlobalCodepage;

private:
	RtfToken	m_oTok;
	bool		m_bCanStartNewReader;

	int			m_nSkipChars;
	bool		m_bSkip;
	bool		m_bStopReader;

protected: 
	int			m_nCurGroups;

};

