/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
            int                     m_nUnicodeClean; // количество символов игнорируемых за юникодом
            RtfCharProperty         m_oCharProp;
            RtfParagraphProperty    m_oParagraphProp;
            RtfRowProperty          m_oRowProperty;
            RtfCellProperty         m_oCellProperty;
            RtfOldList              m_oCurOldList;
			ReaderStatePtr			m_pSaveState;
            std::string             m_sCurText;
			bool					m_bControlPresent;
			
			ReaderState();
	};

    ReaderStatePtr      m_oState;
    RtfSectionProperty  m_oCurSectionProp;
    RtfLex              m_oLex;
    int                 m_nFootnote; //толко для симовола chftn. основано на том что вложенных footnote быть не может
    int                 m_nDefFont;
	int					m_nDefLang;
	int					m_nDefLangAsian;
	std::wstring		m_sTempFolder;

	RtfReader(RtfDocument& oDocument, std::wstring sFilename );
	~RtfReader();

	void PushState();
	void PopState();
	bool Load();
	long GetProgress();
	void Stop();

private:
	RtfDocument& m_oDocument;
	std::wstring m_sFilename;
};

class RtfAbstractReader
{
public:
	NFileWriter::CBufferedFileWriter* m_oFileWriter;

	RtfAbstractReader();

	bool Parse(RtfDocument& oDocument, RtfReader& oReader);

	virtual void PushState(RtfReader& oReader);
	virtual void PopState(RtfDocument& oDocument, RtfReader& oReader);

	bool StartSubReader( RtfAbstractReader& poNewReader, RtfDocument& oDocument, RtfReader& oReader  );
	void Skip( RtfDocument& oDocument, RtfReader& oReader );

	virtual bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar );

	virtual void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText );
	virtual void ExitReader( RtfDocument& oDocument, RtfReader& oReader );
	virtual void ExitReader2( RtfDocument& oDocument, RtfReader& oReader );

	std::wstring ExecuteTextInternal(RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, bool bHasPar, int nPar, int& nSkipChars);
	virtual void ExecuteTextInternal2(RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars);

	void ExecuteTextInternalSkipChars(std::wstring & sResult, RtfReader& oReader, std::string & sKey, int& nSkipChars);
	void ExecuteTextInternalSkipChars(std::string & sResult, RtfReader& oReader, std::string & sKey, int& nSkipChars);
	std::wstring ExecuteTextInternalCodePage( std::string & sCharString, RtfDocument & oDocument, RtfReader & oReader);
	
	bool		m_bUseGlobalCodepage;
	bool		m_bStopReader;

private:
	RtfToken	m_oTok;
	bool		m_bCanStartNewReader;

	int			m_nSkipChars;
	bool		m_bSkip;

protected: 
	int			m_nCurGroups;
};

