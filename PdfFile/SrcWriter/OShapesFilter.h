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
#ifndef _PDF_WRITER_SRC_OSHAPES_FILTER_H
#define _PDF_WRITER_SRC_OSHAPES_FILTER_H

#include <algorithm>
#include <cstdlib>
#include <set>
#include <string>
#include <vector>
#include <utility>

// Removes editor shape blocks from a decoded page content stream:
//   /OShapes << /IDF <id> /MCID n >> BDC ... EMC
// A block is removed when its IDF equals sIDF and its MCID is in arrMCID.
// Everything else is copied byte for byte. Nested marked content inside the block goes with it.
// The scan is a PDF content tokenizer, so strings, comments and inline images that contain
// "BDC" or "EMC" bytes are not mistaken for operators.
namespace PdfWriter
{
namespace OShapesFilter
{
	enum class ETokenType
	{
		Name, String, HexString, DictStart, DictEnd, ArrayStart, ArrayEnd, Regular, End
	};
	struct CToken
	{
		ETokenType eType;
		size_t nStart;
		size_t nEnd;
	};

	inline bool IsWhite(unsigned char c)
	{
		return c == 0 || c == 9 || c == 10 || c == 12 || c == 13 || c == 32;
	}
	inline bool IsDelim(unsigned char c)
	{
		return c == '(' || c == ')' || c == '<' || c == '>' || c == '[' || c == ']' || c == '{' || c == '}' || c == '/' || c == '%';
	}

	class CTokenizer
	{
	public:
		CTokenizer(const std::string& sData) : m_sData(sData), m_nPos(0) {}

		CToken Next()
		{
			const size_t nLen = m_sData.size();
			for (;;)
			{
				while (m_nPos < nLen && IsWhite(m_sData[m_nPos]))
					++m_nPos;
				if (m_nPos < nLen && m_sData[m_nPos] == '%')
				{
					while (m_nPos < nLen && m_sData[m_nPos] != 10 && m_sData[m_nPos] != 13)
						++m_nPos;
					continue;
				}
				break;
			}
			if (m_nPos >= nLen)
				return { ETokenType::End, nLen, nLen };

			const size_t nStart = m_nPos;
			const unsigned char c = m_sData[m_nPos];
			if (c == '(')
			{
				int nDepth = 0;
				while (m_nPos < nLen)
				{
					const unsigned char ch = m_sData[m_nPos++];
					if (ch == '\\')
						++m_nPos;
					else if (ch == '(')
						++nDepth;
					else if (ch == ')' && --nDepth == 0)
						break;
				}
				return { ETokenType::String, nStart, std::min(m_nPos, nLen) };
			}
			if (c == '<')
			{
				if (m_nPos + 1 < nLen && m_sData[m_nPos + 1] == '<')
				{
					m_nPos += 2;
					return { ETokenType::DictStart, nStart, m_nPos };
				}
				while (m_nPos < nLen && m_sData[m_nPos] != '>')
					++m_nPos;
				if (m_nPos < nLen)
					++m_nPos;
				return { ETokenType::HexString, nStart, m_nPos };
			}
			if (c == '>')
			{
				m_nPos += (m_nPos + 1 < nLen && m_sData[m_nPos + 1] == '>') ? 2 : 1;
				return { ETokenType::DictEnd, nStart, m_nPos };
			}
			if (c == '[' || c == '{')
			{
				++m_nPos;
				return { ETokenType::ArrayStart, nStart, m_nPos };
			}
			if (c == ']' || c == '}')
			{
				++m_nPos;
				return { ETokenType::ArrayEnd, nStart, m_nPos };
			}
			if (c == '/')
			{
				++m_nPos;
				while (m_nPos < nLen && !IsWhite(m_sData[m_nPos]) && !IsDelim(m_sData[m_nPos]))
					++m_nPos;
				return { ETokenType::Name, nStart, m_nPos };
			}
			if (c == ')')
			{
				++m_nPos;
				return { ETokenType::Regular, nStart, m_nPos };
			}
			while (m_nPos < nLen && !IsWhite(m_sData[m_nPos]) && !IsDelim(m_sData[m_nPos]))
				++m_nPos;
			return { ETokenType::Regular, nStart, m_nPos };
		}

		// After the ID operator: skip the image data up to and including EI.
		void SkipInlineImage()
		{
			const size_t nLen = m_sData.size();
			if (m_nPos < nLen)
				++m_nPos; // single white space after ID
			while (m_nPos + 1 < nLen)
			{
				if (m_sData[m_nPos] == 'E' && m_sData[m_nPos + 1] == 'I' &&
					(m_nPos == 0 || IsWhite(m_sData[m_nPos - 1])) &&
					(m_nPos + 2 >= nLen || IsWhite(m_sData[m_nPos + 2]) || IsDelim(m_sData[m_nPos + 2])))
				{
					m_nPos += 2;
					return;
				}
				++m_nPos;
			}
			m_nPos = nLen;
		}

		std::string Text(const CToken& t) const { return m_sData.substr(t.nStart, t.nEnd - t.nStart); }

	private:
		const std::string& m_sData;
		size_t m_nPos;
	};

	inline int HexValue(unsigned char c)
	{
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		return -1;
	}
	// Bytes of a string token, literal (...) or hex <...>.
	inline std::string DecodeString(const std::string& sTok, ETokenType eType)
	{
		std::string sRes;
		if (eType == ETokenType::HexString)
		{
			int nHi = -1;
			for (size_t i = 1; i + 1 <= sTok.size(); ++i)
			{
				int v = HexValue(sTok[i]);
				if (v < 0)
					continue;
				if (nHi < 0)
					nHi = v;
				else
				{
					sRes.push_back((char)((nHi << 4) | v));
					nHi = -1;
				}
			}
			if (nHi >= 0)
				sRes.push_back((char)(nHi << 4));
			return sRes;
		}
		for (size_t i = 1; i + 1 < sTok.size(); ++i)
		{
			unsigned char c = sTok[i];
			if (c != '\\')
			{
				sRes.push_back((char)c);
				continue;
			}
			if (++i >= sTok.size() - 1)
				break;
			c = sTok[i];
			switch (c)
			{
			case 'n': sRes.push_back('\n'); break;
			case 'r': sRes.push_back('\r'); break;
			case 't': sRes.push_back('\t'); break;
			case 'b': sRes.push_back('\b'); break;
			case 'f': sRes.push_back('\f'); break;
			case '\r': if (i + 1 < sTok.size() - 1 && sTok[i + 1] == '\n') ++i; break;
			case '\n': break;
			default:
				if (c >= '0' && c <= '7')
				{
					int v = c - '0';
					for (int k = 0; k < 2 && i + 1 < sTok.size() - 1 && sTok[i + 1] >= '0' && sTok[i + 1] <= '7'; ++k)
						v = v * 8 + (sTok[++i] - '0');
					sRes.push_back((char)(v & 0xFF));
				}
				else
					sRes.push_back((char)c);
			}
		}
		return sRes;
	}

	// Returns true and fills sOut when at least one block was removed.
	inline bool RemoveBlocks(const std::string& sIn, const std::string& sIDF, const std::set<int>& arrMCID, std::string& sOut)
	{
		CTokenizer oTok(sIn);
		std::vector<CToken> arrOperands;
		std::vector<std::pair<size_t, size_t>> arrCut;
		int nNesting = 0;     // [ ] and << >> depth of the current operands
		int nRemoveDepth = 0; // marked content depth inside a block being removed
		size_t nRemoveStart = 0;

		for (;;)
		{
			CToken t = oTok.Next();
			if (t.eType == ETokenType::End)
				break;
			if (t.eType == ETokenType::DictStart || t.eType == ETokenType::ArrayStart)
				++nNesting;
			else if ((t.eType == ETokenType::DictEnd || t.eType == ETokenType::ArrayEnd) && nNesting > 0)
				--nNesting;

			bool bOperator = t.eType == ETokenType::Regular && nNesting == 0;
			if (bOperator)
			{
				std::string sOp = oTok.Text(t);
				char c0 = sOp.empty() ? 0 : sOp[0];
				if ((c0 >= '0' && c0 <= '9') || c0 == '-' || c0 == '+' || c0 == '.' || sOp == "true" || sOp == "false" || sOp == "null")
					bOperator = false;
			}
			if (!bOperator)
			{
				arrOperands.push_back(t);
				continue;
			}

			const std::string sOp = oTok.Text(t);
			if (sOp == "ID")
				oTok.SkipInlineImage();

			if (nRemoveDepth > 0)
			{
				if (sOp == "BDC" || sOp == "BMC")
					++nRemoveDepth;
				else if (sOp == "EMC" && --nRemoveDepth == 0)
					arrCut.push_back(std::make_pair(nRemoveStart, t.nEnd));
			}
			else if (sOp == "BDC" && arrOperands.size() >= 3 && arrOperands[0].eType == ETokenType::Name &&
					 oTok.Text(arrOperands[0]) == "/OShapes" && arrOperands[1].eType == ETokenType::DictStart)
			{
				std::string sBlockIDF;
				int nMCID = -1;
				for (size_t i = 2; i + 1 < arrOperands.size(); ++i)
				{
					if (arrOperands[i].eType != ETokenType::Name)
						continue;
					const std::string sKey = oTok.Text(arrOperands[i]);
					const CToken& v = arrOperands[i + 1];
					if (sKey == "/IDF" && (v.eType == ETokenType::String || v.eType == ETokenType::HexString))
						sBlockIDF = DecodeString(oTok.Text(v), v.eType);
					else if (sKey == "/MCID" && v.eType == ETokenType::Regular)
						nMCID = std::atoi(oTok.Text(v).c_str());
				}
				if (sBlockIDF == sIDF && arrMCID.count(nMCID))
				{
					nRemoveDepth = 1;
					nRemoveStart = arrOperands[0].nStart;
				}
			}
			arrOperands.clear();
		}

		if (arrCut.empty())
			return false;

		sOut.clear();
		sOut.reserve(sIn.size());
		size_t nPos = 0;
		for (const std::pair<size_t, size_t>& oCut : arrCut)
		{
			sOut.append(sIn, nPos, oCut.first - nPos);
			sOut.push_back('\n');
			nPos = oCut.second;
		}
		sOut.append(sIn, nPos, std::string::npos);
		return true;
	}
}
}

#endif // _PDF_WRITER_SRC_OSHAPES_FILTER_H
