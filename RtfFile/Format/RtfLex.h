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

#include "RtfToken.h"
#include "Utils.h"
#include "Basic.h"

class StringStream
{
private: 
	LONG64 m_nSizeAbs;//размер файла
	LONG64 m_nPosAbs;//позиция в файле

	unsigned char* m_aBuffer;

public:
	StringStream();
	~StringStream();

	void Clear();

	bool SetSource( std::wstring sPath  );
	void getBytes( int nCount, BYTE** pbData );
	int getc();
	void ungetc();
	void putString( std::string sText );

	LONG64 getCurPosition();
	LONG64 getSize();
};

class RtfLex
{
private: 
	StringStream	m_oStream;
	RtfToken		m_oCurToken;

public: 
	NFileWriter::CBufferedFileWriter* m_oFileWriter;
	char* m_caReadBuffer;
	int m_nReadBufSize;

	RtfLex();
	~RtfLex();

	double GetProgress();
	bool SetSource( std::wstring sPath );
	void CloseSource();
	RtfToken NextCurToken();
	void ReadBytes( int nCount, BYTE** pbData );
	RtfToken NextToken();
	void putString( std::string sText );

private: 
	void parseKeyword(RtfToken& token);
	void parseText(int car, RtfToken& token);

	bool GetNextChar( int& nChar );
	void parseTextFile(int car, RtfToken& token);
};
