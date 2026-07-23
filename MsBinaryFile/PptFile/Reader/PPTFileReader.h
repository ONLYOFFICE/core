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

#include "PPTDocumentInfo.h"

namespace PPT
{
class CPPTFileReader
{
public: 
    CPPTFileReader(POLE::Storage *pStorage, const std::wstring &strTempPath);
	~CPPTFileReader();
    bool IsPowerPoint();
    bool IsEncrypted();
	bool ReadPersists();
	void ReadDocument();
protected: 

	XLS::CFStreamPtr GetDocumentStream();
	XLS::CFStreamPtr GetPictureStream();
	XLS::CFStreamPtr GetEncryptedSummaryStream();
	XLS::CFStreamPtr GetDocumentSummaryStream();
	XLS::CFStreamPtr GetSummaryStream();
	
	XLS::CFStreamPtr GetStreamByName(const std::wstring & name);

	bool ReadCurrentUser(POLE::Stream *pStm);
	void ReadEncryptedSummary();
	void ReadDocumentSummary();
    void ReadPictures();
 
private: 
	bool						m_bDualStorage;
	CRecordCurrentUserAtom		m_oCurrentUser; 

	XLS::CFStreamPtr			m_pDocumentStream; 
	XLS::CFStreamPtr			m_pPictureStream; 
	XLS::CFStreamPtr			m_pDocumentSummaryStream;
	XLS::CFStreamPtr			m_pEncryptedSummaryStream;

	bool						m_bIsPPTFile;

public:	
	int							m_nPresentationCodePage;
	POLE::Storage*				m_pStorage; 
    std::vector<bool>			m_arLoadImageFlags;
	CPPTDocumentInfo			m_oDocumentInfo;
	_commonInfo					m_oCommonInfo;
};
}
