/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
            int                     m_nUnicodeClean; // number of characters ignored after unicode
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
    int                 m_nFootnote; //only for chftn symbol. based on the fact that nested footnotes cannot exist
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

