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

#include "BinEquationWriter.h"
#include "../../Presentation/BinaryFileReaderWriter.h"
#include "../../../Common/SimpleTypes_OMath.h"

namespace MathEquation
{
	EquationRun::EquationRun()
	{
		bNormal = false;
	}
	void EquationRun::AddChar(std::wstring sChar, TMathFont* pNewFont, LONG lSize)
	{
		str = sChar;
		pFont = pNewFont;
		nTextSize = lSize;
		bAccent = false;
	}
	bool EquationRun::CompareFont(TMathFont* pNewFont)
	{
		if ( pNewFont->sName == pFont->sName && pNewFont->bBold == pFont->bBold && pNewFont->bItalic == pFont->bItalic )
			return true;
		else
			return false;
	}
	void EquationRun::SetAccent(bool bAcc, MEMBELTYPE type)
	{
		bAccent = bAcc;
		eType = type;
	}
	TMathFont* EquationRun::GetFont()
	{
		return pFont;
	}
	LONG EquationRun::GetSize()
	{
		return nTextSize;
	}

	RunManager::RunManager()
	{
		bAlligment = false;
	}
	void RunManager::SetAlligment(bool bPos)
	{
		bAlligment = bPos;
		return;
	}
	bool RunManager::GetAlligment()
	{
		return bAlligment;
	}
	void RunManager::Add(EquationRun oElem)
	{
		arrRun.push_back(oElem);
		return;
	}
	bool RunManager::IsEmpty()
	{
		return arrRun.empty();
	}
	LONG RunManager::GetSize()
	{
		return (LONG)arrRun.size();
	}
	bool RunManager::GetElem (LONG lPos, EquationRun &oRun)
	{
		if (lPos < (LONG)arrRun.size() )
		{
			oRun = arrRun[lPos];
			return true;
		}
		return false;
	}
	void RunManager::Clear()
	{
		arrRun.clear();
		return;
	}
	void RunManager::RemoveElem (LONG lPos)
	{
		arrRun.erase(arrRun.begin()+lPos);
		return;
	}
	void RunManager::Delete(LONG lStart, LONG lCount)
	{
		arrRun.erase(arrRun.begin()+lStart, arrRun.begin()+lCount);
		return;
	}
	void RunManager::AddAccent (MEMBELTYPE eType)
	{
		LONG lPos = GetSize() - 1;

		if (lPos < 0) return;

		EquationRun oRun;
		oRun = arrRun[lPos];
		RemoveElem(lPos);

		oRun.bAccent = true;
		oRun.eType = eType;
		Add(oRun);
	}

    BinaryEquationWriter::BinaryEquationWriter(NSBinPptxRW::CBinaryFileWriter *pStream) : bEmbel(false), m_pStream(pStream)
	{
	}
	void BinaryEquationWriter::WriteRPR(TMathFont* pFont, LONG nSize, bool bIsOpen)
	{
		if (NULL != pFont)
		{
			int nCurPos;
			//if (bIsOpen)
				nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::RPr);
			//else
			//	nCurPos = WriteItemStart(BinDocxRW::c_oSerRunType::rPr);

			std::wstring sFontName = L"Cambria Math";
			{
                m_pStream->WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontAscii);
                m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
                m_pStream->WriteStringW(sFontName);

                m_pStream->WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontHAnsi);
                m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
                m_pStream->WriteStringW(sFontName);

                m_pStream->WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontAE);
                m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
                m_pStream->WriteStringW(sFontName);
			}
			if (nTextSize)
			{
                m_pStream->WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontSize);
                m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
                m_pStream->WriteLONG(nSize);

                /*m_pStream->WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontSizeCS);
                m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
                m_pStream->WriteLONG(nSize);*/
			}
			WriteItemEnd(nCurPos);
		}
		return;
	}
	void BinaryEquationWriter::WriteMRPR(EquationRun oRun)
	{

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::MRPr);
		BYTE eStyle = SimpleTypes::stylePlain;
		if (oRun.pFont && (oRun.pFont->bBold || oRun.pFont->bItalic))
		{
			if (oRun.pFont->bBold)
			{
				if (oRun.pFont->bItalic)
					eStyle = SimpleTypes::styleBoldItalic;
				else
					eStyle = SimpleTypes::styleBold;
			}
			else if ( oRun.pFont->bItalic )
				eStyle = SimpleTypes::styleItalic;
		}
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Sty, eStyle );

		if (oRun.bNormal)
			WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Nor, oRun.bNormal);
		WriteItemEnd(nCurPos);
		return;
	}
	void BinaryEquationWriter::WriteRunContent(EquationRun oRun, bool bIsOpen)
	{
		TMathFont* pCurFont = oRun.pFont;
		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::MRun);
		WriteRPR(oRun.pFont, oRun.nTextSize, bIsOpen);
		WriteMRPR(oRun);
		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::MText);
        m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
        m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
        m_pStream->WriteStringW(oRun.str);
		WriteItemEnd(nCurPos1);
		WriteItemEnd(nCurPos);
	}
	void BinaryEquationWriter::CutRun(int nCount)
	{
		LONG lSize = oRManager.GetSize();
		if (nCount < lSize+1)
		{
			for (int i=0; i<nCount; i++)
				oRManager.Delete(0,1);
		}
	}
	void BinaryEquationWriter::WriteRun(bool bLast)
	{
		bool bIsOpen;
		if (!m_aCommandStack.empty())
			bIsOpen = m_aCommandStack.top()->IsOpenNode(); //if false write ctrlPrp
		else
			bIsOpen = true;

		std::wstring sText;
		LONG lSize = oRManager.GetSize();

		if (bLast)
			lSize--;
		if (lSize > 0)
		{
			for (int i=0; i<lSize; i++)
			{
				EquationRun oRun;
				bool bRes = oRManager.GetElem( i, oRun);
				if (bRes)
				{
					if (!oRun.bAccent)
					{
						WriteRunContent(oRun, bIsOpen);
					}
					else
					{
						if (embelPrime == oRun.eType || embelDPrime == oRun.eType || embelTPrime == oRun.eType)
						{
							int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSup);
							int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSupPr);
							std::wstring str;
							switch (oRun.eType)
							{
								case embelPrime:     str =  0x2032; break;
								case embelDPrime:    str =  0x2033; break;
								case embelTPrime:    str =  0x2034; break;
							}
							WriteItemEnd(nCurPos1);

							nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Sup);
							EquationRun oSupRun;
							oSupRun.AddChar(str, oRun.GetFont(), oRun.GetSize());
							WriteRunContent(oSupRun, bIsOpen);
							WriteItemEnd(nCurPos1);

							nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
							WriteRunContent(oRun, bIsOpen);
							WriteItemEnd(nCurPos1);
							WriteItemEnd(nCurPos);
						}
						else if (embelLPrime == oRun.eType)
						{
							int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SPre);
							int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SPrePr);
							std::wstring str;
							str =  0x0060;
							WriteItemEnd(nCurPos1);

							nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Sup);
							EquationRun oSupRun;
							oSupRun.AddChar(str, oRun.GetFont(), oRun.GetSize());
							WriteRunContent(oSupRun, bIsOpen);
							WriteItemEnd(nCurPos1);

							nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
							WriteRunContent(oRun, bIsOpen);
							WriteItemEnd(nCurPos1);
							WriteItemEnd(nCurPos);
						}
						else
						{
							int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Acc);
							int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::AccPr);
							std::wstring str;
							switch (oRun.eType)
							{
								case embelDot:       str =  0x0307; break;
								case embelDDot:      str =  0x0308; break;
								case embelDDDot:     str =  0x20DB; break;
								case embelTilde:     str =  0x0303; break;
								case embelHat:       str =  0x0302; break;
								case embelSlash:     str =  0x002F; break;
								case embelLArrow:    str =  0x20D6; break;
								case embelRArrow:    str =  0x20D7; break;
								case embelDArrow:    str =  0x20E1; break;
								case embelLHarpoon:  str =  0x20D0; break;
								case embelRHarpoon:  str =  0x20D1; break;
								case embelStrikeout: str =  0x0336; break;
								case embelBar:       str =  0x0305; break;
								case embelFrown:     str =  0x0311; break;
								case embelSmile:     str =  0x0306; break;
							}
							WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
							WriteItemEnd(nCurPos1);

							int nCurPos2 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);

							WriteRunContent(oRun, bIsOpen);

							WriteItemEnd(nCurPos2);
							WriteItemEnd(nCurPos);
						}
					}
				}
			}
			oRManager.Delete(0,lSize);
		}
		oRManager.SetAlligment(false);
		return;
	}
	int BinaryEquationWriter::WriteItemStart(BYTE type)
	{
		//type
        m_pStream->WriteBYTE(type);
		return WriteItemWithLengthStart();
	}
	void BinaryEquationWriter::WriteItemEnd(int nStart)
	{
		WriteItemWithLengthEnd(nStart);
	}
	int BinaryEquationWriter::WriteItemWithLengthStart()
	{
		//Запоминаем позицию чтобы в конце записать туда длину
        int nStartPos = m_pStream->GetPosition();
        m_pStream->Skip(4);
		return nStartPos;
	}
	void BinaryEquationWriter::WriteItemWithLengthEnd(int nStart)
	{
		//Length
        int nEnd = m_pStream->GetPosition();
        m_pStream->SetPosition(nStart);
        m_pStream->WriteLONG(nEnd - nStart - 4);
        m_pStream->SetPosition(nEnd);
	}
	void BinaryEquationWriter::BeginEquation()
	{
		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::OMath);
		m_aEquationStack.push(nCurPos);
	}
	void BinaryEquationWriter::EndEquation()
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		if (!m_aEquationStack.empty())
		{
			int nCurPos = m_aEquationStack.top();
			WriteItemEnd(nCurPos);
			m_aEquationStack.pop();
		}
	}
	void BinaryEquationWriter::BeginBlock()
	{
		if (!m_aCommandStack.empty())
		{
			CBaseCommand* pCommand = TopCommand();
			pCommand->Next();
			pCommand->WriteBeginBlock(this);
		}
	}
	void BinaryEquationWriter::EndBlock()
	{
		if (!m_aCommandStack.empty())
		{
			CBaseCommand* pCommand = TopCommand();
			pCommand->WriteEndBlock(this);
		}
	}
	void BinaryEquationWriter::SetSize(uint16_t nSize)
	{
		nTextSize = nSize*2;
	}
	void BinaryEquationWriter::BeginChar(unsigned short uChar, unsigned char nTypeFace, bool bSpecialSymbol)
	{
		bool bIsOpen;
		if (!m_aCommandStack.empty())
			bIsOpen = m_aCommandStack.top()->IsOpenNode(); //if false write ctrlPrp
		else
			bIsOpen = true;

		TMathFont* pFont = GetFont(nTypeFace);
		if (!pFont && !bIsOpen)
			return;

		EquationRun oRun;
		if (uChar)
		{
			std::wstring str;
			if (bSpecialSymbol)
			{
				switch(uChar)
				{
					case specialsymAlignment:
					{
						str = 0x200A;
						if (oRManager.GetAlligment())
							oRun.bNormal = true;
						else
							oRManager.SetAlligment(true);
					}
					break;
					case specialsymZeroSpace:  str = 0x200B; break;
					case specialsymThinSpace:  str = 0x2009; break;
					case specialsymThickSpace: str = 0x2005; break;
					case specialsymLargeSpace: str = 0x2003; break;
					case specialsymOnePtSpace: str = 0x200A; break;
				}
			}
			else
			{
				if (uChar == 0x0026)
				{
					if (oRManager.GetAlligment())
						oRun.bNormal = true;
					else
						oRManager.SetAlligment(true);
				}
				else if (uChar == 0x03C6)
					uChar = 0x03D5;
				else if (uChar == 0x03D5)
					uChar = 0x03C6;
				str =  uChar;
			}

			oRun.AddChar(str, pFont, nTextSize);
			oRManager.Add(oRun);
		}
	}
	void BinaryEquationWriter::AddCharEmbel(MEMBELTYPE eType)
	{
		oRManager.AddAccent(eType);
	}
	void BinaryEquationWriter::EndChar()
	{
		/*if (!m_aRunStack.empty())
		{
			int nCurPos = m_aRunStack.top();
			WriteItemEnd(nCurPos);
			m_aRunStack.pop();

			if (bEmbel)
			{
				int nCurPos;
				if (!m_aAccentStack.empty())
				{
					nCurPos = m_aAccentStack.top();
					m_aAccentStack.pop();
					WriteItemEnd(nCurPos);
				}
				if (!m_aAccentStack.empty())
				{
					nCurPos = m_aAccentStack.top();
					m_aAccentStack.pop();
					WriteItemEnd(nCurPos);
				}
				bEmbel = false;
			}
		}*/
	}
	void BinaryEquationWriter::BeginMatrix(unsigned char nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, unsigned char nRows, unsigned char nCols, unsigned char* pVerBorders, unsigned char* pHorBorders)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		CMatrixCommand* pCommand = (CMatrixCommand*)PushCommand(commandMatrix);
		pCommand->SetProps(nRows, nCols);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Matrix);
		m_aMatrixStack.push(nCurPos);
		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::MPr);
		WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Row, nRows);

		int nCurPos2 = WriteItemStart(BinDocxRW::c_oSer_OMathBottomNodesType::Mcs);
		int nCurPos3 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Mc);
		int nCurPos4 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::McPr);

		WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Count, nCols);

		BYTE horAlign;
		switch(eHorAlign)
		{
			case matrixhoralignLeft:		horAlign = SimpleTypes::xalignLeft;		break;
			case matrixhoralignCenter:		horAlign = SimpleTypes::xalignCenter;	break;
			case matrixhoralignRight:		horAlign = SimpleTypes::xalignRight;	break;
			case matrixhoralignEqualSign:	horAlign = SimpleTypes::xalignCenter;	break;
			case matrixhoralignCommaSign:	horAlign = SimpleTypes::xalignCenter;	break;
		}
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::McJc, horAlign);

		WriteItemEnd(nCurPos4);
		WriteItemEnd(nCurPos3);
		WriteItemEnd(nCurPos2);
		WriteItemEnd(nCurPos1);

	}
	void BinaryEquationWriter::EndMatrix()
	{
		PopCommand();

		if (!m_aMatrixStack.empty())
		{
			int nCurPos = m_aMatrixStack.top();
			WriteItemEnd(nCurPos);
			m_aMatrixStack.pop();
		}
	}
	void BinaryEquationWriter::StartPile(unsigned char nHAlign, unsigned char nVAlign)
	{
		switch (nHAlign)
		{
		case 1:
		case 4: nHAlignPile = SimpleTypes::xalignLeft; break;
		case 2: nHAlignPile = SimpleTypes::xalignCenter; break;
		case 3:
		case 5: nHAlignPile = SimpleTypes::xalignRight; break;
		default: nHAlignPile = SimpleTypes::xalignLeft; break;
		}

		if (m_aCommandStack.empty())
		{
			PushCommand(commandEqArray);
		}
		m_aCommandStack.top()->SetPile(true, nHAlignPile);
	}
	void BinaryEquationWriter::EndPile()
	{
		if (!m_aCommandStack.empty())
		{
			m_aCommandStack.top()->SetPile(false, SimpleTypes::xalignLeft);
			CBaseCommand* pCommand = TopCommand();
			pCommand->WriteEndBlock(this);

			int nRows = 0;
			if (!m_aRowsCounter.empty())
			{
				nRows = m_aRowsCounter.top();
				m_aRowsCounter.pop();
			}
			int nPos = -1;
			if (!m_aRowsPosCounter.empty())
			{
				nPos = m_aRowsPosCounter.top();
				m_aRowsPosCounter.pop();
			}
            int nEnd = m_pStream->GetPosition();
			if (nPos >= 0)
			{
                m_pStream->SetPosition(nPos);
                m_pStream->WriteLONG(nRows);
			}
            m_pStream->SetPosition(nEnd);

			ECommandType type;
			type = pCommand->GetCommand();
			if (type == commandIntegral || type == commandNArray)
				pCommand->Next();
		}
		nHAlignPile = SimpleTypes::xalignLeft;
	}
	void BinaryEquationWriter::BeginBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		PushCommand(commandBrackets);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Delimiter);
		m_aDelimiterStack.push(nCurPos);
		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::DelimiterPr);

		int nCurPos2 = WriteItemStart(BinDocxRW::c_oSer_OMathBottomNodesType::Column);

        m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
        m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);

        int nStartPos = m_pStream->GetPosition();
        m_pStream->Skip(4);
		m_aDelimiterCounter.push(nStartPos);

		WriteItemEnd(nCurPos2);

		if (!bOpen)
			WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, _T(""));
		else
		{
			std::wstring strOpen;
			switch(eType)
			{
				case bracketsAngle:				strOpen =  0x27E8; break;
				case bracketsRound:				strOpen =  0x0028; break;
				case bracketsCurve:				strOpen =  0x007B; break;
				case bracketsSquare:			strOpen =  0x005B; break;
				case bracketsLine:				strOpen =  0x007C; break;
				case bracketsDLine:				strOpen =  0x2016; break;
				case bracketsFloor:				strOpen =  0x230A; break;
				case bracketsCeil:				strOpen =  0x2308; break;
				case bracketsSquareOpenOpen:	strOpen =  0x005B; break;
				case bracketsSquareCloseClose:	strOpen =  0x005D; break;
				case bracketsSquareCloseOpen:	strOpen =  0x005D; break;
				case bracketsSquareRound:		strOpen =  0x005B; break;
				case bracketsRoundSquare:		strOpen =  0x0028; break;
			}
			WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, strOpen);
		}

		if (!bClose)
			WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, _T(""));
		else
		{
			std::wstring strEnd;
			switch(eType)
			{
				case bracketsAngle:				strEnd =  0x27E9; break;
				case bracketsRound:				strEnd =  0x0029; break;
				case bracketsCurve:				strEnd =  0x007D; break;
				case bracketsSquare:			strEnd =  0x005D; break;
				case bracketsLine:				strEnd =  0x007C; break;
				case bracketsDLine:				strEnd =  0x2016; break;
				case bracketsFloor:				strEnd =  0x230B; break;
				case bracketsCeil:				strEnd =  0x2309; break;
				case bracketsSquareOpenOpen:	strEnd =  0x005B; break;
				case bracketsSquareCloseClose:	strEnd =  0x005D; break;
				case bracketsSquareCloseOpen:	strEnd =  0x005B; break;
				case bracketsSquareRound:		strEnd =  0x0029; break;
				case bracketsRoundSquare:		strEnd =  0x005D; break;
			}
			WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, strEnd);
		}

		WriteItemEnd(nCurPos1);
	}
	void BinaryEquationWriter::EndBrackets  (MBRACKETSTYPE eType, bool bOpen, bool bClose)
	{
		if (!m_aDelimiterCounter.empty())
		{
			int nCount = m_aCommandStack.top()->GetCount();
			int nPos = m_aDelimiterCounter.top();
            int nEnd = m_pStream->GetPosition();
            m_pStream->SetPosition(nPos);
            m_pStream->WriteLONG(nCount);
            m_pStream->SetPosition(nEnd);
			m_aDelimiterCounter.pop();
		}

		PopCommand();
		if (!m_aDelimiterStack.empty())
		{
			int nCurPos = m_aDelimiterStack.top();
			WriteItemEnd(nCurPos);
			m_aDelimiterStack.pop();
		}
	}
	void BinaryEquationWriter::BeginRoot(bool bDegree)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		PushCommand(commandRoot);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Rad);
		m_aRadicalStack.push(nCurPos);

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::RadPr);
		if (!bDegree)
			WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::DegHide, true);
		WriteItemEnd(nCurPos1);
	}
	void BinaryEquationWriter::EndRoot  ()
	{
		PopCommand();

		if (!m_aRadicalStack.empty())
		{
			int nCurPos = m_aRadicalStack.top();
			WriteItemEnd(nCurPos);
			m_aRadicalStack.pop();
		}
	}
	void BinaryEquationWriter::BeginFraction(MFRACTIONTYPES eType, bool bInline)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		PushCommand(commandFraction);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Fraction);
		m_aFractionStack.push(nCurPos);

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::FPr);
		BYTE fracType;
		switch(eType)
		{
			case fractionRegular:
				fracType = SimpleTypes::fTypeBar;
				break;
			case fractionSlanted:
				if (bInline)
					fracType = SimpleTypes::fTypeLin;
				else
					fracType = SimpleTypes::fTypeSkw;
				break;
		}
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Type, fracType);

		WriteItemEnd(nCurPos1);
	}
	void BinaryEquationWriter::EndFraction  ()
	{
		PopCommand();

		if (!m_aFractionStack.empty())
		{
			int nCurPos = m_aFractionStack.top();
			WriteItemEnd(nCurPos);
			m_aFractionStack.pop();
		}
	}
	void BinaryEquationWriter::BeginScript(MSCRIPTALIGN eAlign, bool bBase, bool bSup, bool bSub, bool bInline)
	{
		bool bSaveLastRun = false;
		if (!bBase && !oRManager.IsEmpty())
			bSaveLastRun = true;

		WriteRun(bSaveLastRun);

		CScriptCommand* pCommand = (CScriptCommand*)PushCommand(commandScript);
		pCommand->SetProps(bInline, bBase, bSup, bSub);

		int nCurPos;
		int nCurPos1;
		switch(eAlign)
		{
			case scriptalignRight:
			case scriptalignCenter:
			case scriptalignLeft:
				if (bSub)
				{
					if (bSup)
					{
						nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSubSup);
						nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSubSupPr);
					}
					else
					{
						nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSub);
						nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSubPr);
					}
				}
				else if (bSup)
				{
					nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSup);
					nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SSupPr);
				}
				break;
			/*case scriptalignLeft: можно сделать так, но выглядеть будет плохо
			{
				nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SPre);
				nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SPrePr);
			}
			break;*/
		}
		if (bInline)
			WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::AlnScr, true);
		WriteItemEnd(nCurPos1);
		m_aScriptStack.push(nCurPos);

		if (!bBase && !oRManager.IsEmpty())
		{
			int nCurPos2 = nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			WriteRun();
			WriteItemEnd(nCurPos2);
		}
	}
	void BinaryEquationWriter::EndScript  ()
	{
		PopCommand();

		if (!m_aScriptStack.empty())
		{
			int nCurPos = m_aScriptStack.top();
			WriteItemEnd(nCurPos);
			m_aScriptStack.pop();
		}
	}
	void BinaryEquationWriter::BeginBar(MBARTYPE eType, bool bTop)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		PushCommand(commandBar);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::GroupChr);
		m_aGroupChrStack.push(nCurPos);

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::GroupChrPr);

		BYTE pos, vertJc;
		if (bTop)
		{
			pos		= SimpleTypes::tbTop;
			vertJc	= SimpleTypes::tbBot;
		}
		else
		{
			pos		= SimpleTypes::tbBot;
			vertJc	= SimpleTypes::tbTop;
		}

		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Pos, pos);
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::VertJc, vertJc);

		std::wstring str;
		switch(eType)
		{
			case bartypeLine:			str = 0x0305;break;
			case bartypeDoubleLine:		str = 0x033F;break;
			case bartypeArrowLeft:		str = 0x20D6;break;
			case bartypeArrowRight:		str = 0x20D7;break;
			case bartypeArrowDouble:	str = 0x20E1;break;
		}
		WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);

		WriteItemEnd(nCurPos1);

	}
	void BinaryEquationWriter::EndBar  ()
	{
		PopCommand();

		if (!m_aGroupChrStack.empty())
		{
			int nCurPos = m_aGroupChrStack.top();
			WriteItemEnd(nCurPos);
			m_aGroupChrStack.pop();
		}
	}
	void BinaryEquationWriter::BeginArrow(MARROWTYPE eType, bool bTop)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		PushCommand(commandArrow);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::GroupChr);
		m_aGroupChrStack.push(nCurPos);

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::GroupChrPr);

		BYTE pos, vertJc;
		if (bTop)
		{
			pos		= SimpleTypes::tbBot;
			vertJc	= SimpleTypes::tbBot;
		}
		else
		{
			pos		= SimpleTypes::tbTop;
			vertJc	= SimpleTypes::tbTop;
		}

		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Pos, pos);
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::VertJc, vertJc);

		std::wstring str;
		switch(eType)
		{
			case arrowtypeLeft:			str = 0x2190;break;
			case arrowtypeRight:		str = 0x2192;break;
			case arrowtypeDouble:		str = 0x2194;break;
		}
		WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);

		WriteItemEnd(nCurPos1);
	}
	void BinaryEquationWriter::EndArrow  ()
	{
		PopCommand();

		if (!m_aGroupChrStack.empty())
		{
			int nCurPos = m_aGroupChrStack.top();
			WriteItemEnd(nCurPos);
			m_aGroupChrStack.pop();
		}
	}
	void BinaryEquationWriter::BeginIntegral(MINTEGRALTYPE eType)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		PushCommand(commandIntegral);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Nary);
		m_aNArrayStack.push(nCurPos);
		int lCount = 0;

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::NaryPr);
		std::wstring str;
		switch(eType)
		{
		//todooo проверить запись .. (BYTE)SimpleTypes::ххх
			case integraltypeSingle:
				str = 0x222B;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				lCount = 1;
				break;
			case integraltypeSingleRSub:
				str = 0x222B;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 1;
				break;
			case integraltypeSingleRSubSup:
				str = 0x222B;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				lCount = 1;
				break;
			case integraltypeSingleOriented:
				str = 0x222E;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				lCount = 1;
				break;
			case integraltypeSingleOrientedRSub:
				str = 0x222E;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 1;
				break;

			case integraltypeDouble:
				str = 0x222C;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				lCount = 2;
				break;
			case integraltypeDoubleRSub:
				str = 0x222C;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 2;
				break;
			case integraltypeDoubleOriented:
				str = 0x222F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				lCount = 2;
				break;
			case integraltypeDoubleOrientedRSub:
				str = 0x222F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 2;
				break;

			case integraltypeTriple:
				str = 0x222D;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				lCount = 3;
				break;
			case integraltypeTripleRSub:
				str = 0x222D;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 3;
				break;
			case integraltypeTripleOriented:
				str = 0x2230;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				lCount = 3;
				break;
			case integraltypeTripleOrientedRSub:
				str = 0x2230;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 3;
				break;

			case integraltypeSingleCSubSup:
				str = 0x222B;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				lCount = 1;
				break;
			case integraltypeSingleCSub:
				str = 0x222B;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 1;
				break;
			case integraltypeSingleOrientedCSub:
				str = 0x222E;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 1;
				break;

			case integraltypeDoubleOrientedCSub:
				str = 0x222F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 2;
				break;
			case integraltypeDoubleCSub:
				str = 0x222C;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 2;
				break;

			case integraltypeTripleOrientedCSub:
				str = 0x2230;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 3;
				break;
			case integraltypeTripleCSub:
				str = 0x222D;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				lCount = 3;
				break;
		}
		m_aNArrayCutStack.push(lCount);


		WriteItemEnd(nCurPos1);
	}
	void BinaryEquationWriter::EndIntegral  ()
	{
		PopCommand();

		if (!m_aNArrayCutStack.empty())
		{
			int lCount = m_aNArrayCutStack.top();
			if (!oRManager.IsEmpty())
				CutRun(lCount);
			m_aNArrayCutStack.pop();
		}

		if (!m_aNArrayStack.empty())
		{
			int nCurPos = m_aNArrayStack.top();
			WriteItemEnd(nCurPos);
			m_aNArrayStack.pop();
		}
	}
	void BinaryEquationWriter::BeginVerticalBrace(bool bTop)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		BYTE pos, vertJc;
		std::wstring chr;
		if (bTop)
		{
			chr = 0x23DE;
			vertJc = SimpleTypes::verticaljcTop;
			pos = SimpleTypes::tbTop;
			int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::LimUpp);
			m_aLimitStack.push(nCurPos);

			int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::LimUppPr);
			//+ctrlprp
			WriteItemEnd(nCurPos1);
		}
		else
		{
			chr = 0x23DF;
			vertJc = SimpleTypes::verticaljcBottom;
			pos = SimpleTypes::tbBot;
			int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::LimLow);
			m_aLimitStack.push(nCurPos);

			int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::LimLowPr);
			//+ctrlprp
			WriteItemEnd(nCurPos1);
		}


		int nCurPos2 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
		m_aLimitStack.push(nCurPos2);
		int nCurPos3 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::GroupChr);
		m_aLimitStack.push(nCurPos3);

		int nCurPos4 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::GroupChrPr);
		WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, chr);
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Pos, pos);
		WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::VertJc, vertJc);
		WriteItemEnd(nCurPos4);

		PushCommand(commandVerticalBrace);

	}
	void BinaryEquationWriter::EndVerticalBrace  ()
	{
		PopCommand();

		if (!m_aLimitStack.empty())
		{
			int nCurPos = m_aLimitStack.top();
			WriteItemEnd(nCurPos);
			m_aLimitStack.pop();
		}
	}
	void BinaryEquationWriter::BeingNArray(MNARRAYTYPE eType)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		CNArrayCommand* pCommand = (CNArrayCommand*)PushCommand(commandNArray);
		pCommand->SetType(eType);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Nary);
		m_aNArrayStack.push(nCurPos);

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::NaryPr);
		std::wstring str;
		switch(eType)
		{
			case narySumCSub:
				str = 0x2211;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case narySumCSubSup:
				str = 0x2211;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				break;
			case narySum:
				str = 0x2211;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				break;
			case narySumRSub:
				str = 0x2211;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case narySumRSubSup:
				str = 0x2211;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				break;

			case naryProdCSub:
				str = 0x220F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryProdCSubSup:
				str = 0x220F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				break;
			case naryProd:
				str = 0x220F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				break;
			case naryProdRSub:
				str = 0x220F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryProdRSubSup:
				str = 0x220F;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				break;

			case naryCoProdCSub:
				str = 0x2210;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryCoProdCSubSup:
				str = 0x2210;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				break;
			case naryCoProd:
				str = 0x2210;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				break;
			case naryCoProdRSub:
				str = 0x2210;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryCoProdRSubSup:
				str = 0x2210;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				break;

			case naryUnionCSub:
				str = 0x22C3;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryUnionCSubSup:
				str = 0x22C3;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				break;
			case naryUnion:
				str = 0x22C3;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				break;
			case naryUnionRSub:
				str = 0x22C3;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryUnionRSubSup:
				str = 0x22C3;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				break;

			case naryIntersectCSub:
				str = 0x22C2;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryIntersectCSubSup:
				str = 0x22C2;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				break;
			case naryIntersect:
				str = 0x22C2;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocUndOvr);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
				break;
			case naryIntersectRSub:
				str = 0x22C2;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
				break;
			case naryIntersectRSubSup:
				str = 0x22C2;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, (BYTE)SimpleTypes::limLocSubSup);
				break;

		}
		m_aNArrayCutStack.push(1);

		WriteItemEnd(nCurPos1);
	}
	void BinaryEquationWriter::EndNArray  ()
	{
		CNArrayCommand* pCommand = (CNArrayCommand*)TopCommand();
		MNARRAYTYPE eType = pCommand->GetType();
		PopCommand();

		if (!m_aNArrayCutStack.empty())
		{
			int lCount = m_aNArrayCutStack.top();
			if (!oRManager.IsEmpty())
				CutRun(lCount);
			m_aNArrayCutStack.pop();
		}

		if (!m_aNArrayStack.empty())
		{
			int nCurPos = m_aNArrayStack.top();
			WriteItemEnd(nCurPos);
			m_aNArrayStack.pop();
		}
	}
	void BinaryEquationWriter::BeginLongDivision(MLONGDIVISION eType)
	{
		if (!oRManager.IsEmpty())
			WriteRun();
		if (eType == longdivisionWithResult)
		{
			PushCommand(commandLongDivision);

			int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Fraction);
			m_aFractionStack.push(nCurPos);

			int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::FPr);
			BYTE fType = SimpleTypes::fTypeBar;
			WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::Type, fType);

			WriteItemEnd(nCurPos1);
		}
		else if (eType == longdivisionRegular)
		{
			BeginBrackets(bracketsLine, true, false);
		}
	}
	void BinaryEquationWriter::EndLongDivision  ()
	{

		ECommandType eType;
		CBaseCommand* pCommand = TopCommand();
		eType = pCommand->GetCommand();
		if (eType == commandLongDivision)
		{
			EndFraction();
		}
		else if (eType == commandBrackets)
		{
			MBRACKETSTYPE ebType = MBRACKETSTYPE::bracketsAngle; //???
			EndBrackets(ebType, false, false);
		}
	}
	void BinaryEquationWriter::BeginAngleBracketsWithSeparator(MANGLEBRACKETSWITHSEPARATORTYPE eType)
	{
		if (!oRManager.IsEmpty())
			WriteRun();

		CBracketsWithSeparatorCommand* pCommand = (CBracketsWithSeparatorCommand*)PushCommand(commandBracketsSep);
		pCommand->SetType(eType);

		int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Delimiter);
		m_aDelimiterStack.push(nCurPos);

		int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::DelimiterPr);
		std::wstring beg, sep, end;
		switch (eType)
		{
			case angbrwithsepBoth:	// < | >
				beg = 0x27E8; sep =  0x007C; end =  0x27E9;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, beg);
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::SepChr, sep);
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, end);
				WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Column, 2);
				break;
			case angbrwithsepLeft:	// < |
				beg = 0x27E8; end =  0x007C;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, beg);
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, end);
				WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Column, 1);
				break;
			case angbrwithsepRight:	// | >
				beg = 0x007C; end =  0x27E9;
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, beg);
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, end);
				WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Column, 1);
				break;
		}
		WriteItemEnd(nCurPos1);

	}
	void BinaryEquationWriter::EndAngleBracketsWithSeparator  ()
	{
		PopCommand();

		if (!m_aDelimiterStack.empty())
		{
			int nCurPos = m_aDelimiterStack.top();
			WriteItemEnd(nCurPos);
			m_aDelimiterStack.pop();
		}
	}
	void BinaryEquationWriter::AddFont(unsigned char nTypeFace, std::string sName, bool bBold, bool bItalic)
	{
		TMathFont aFont;
		aFont.sName   = sName;
		aFont.bBold   = bBold;
		aFont.bItalic = bItalic;
		m_mFonts[nTypeFace] = aFont;
	}
	void BinaryEquationWriter::WriteItemVal(BYTE name, BYTE val)
	{
		int nCurPos = WriteItemStart(name);

        m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
        m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Byte);
        m_pStream->WriteBYTE(val);

		WriteItemEnd(nCurPos);
	}
	void BinaryEquationWriter::WriteItemValLong(BYTE name, LONG val)
	{
		int nCurPos = WriteItemStart(name);

        m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
        m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
        m_pStream->WriteLONG(val);

		WriteItemEnd(nCurPos);
	}
	void BinaryEquationWriter::WriteItemVal(BYTE name, bool val)
	{
		int nCurPos = WriteItemStart(name);

        m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
        m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Byte);
        m_pStream->WriteBOOL(val);

		WriteItemEnd(nCurPos);
	}
	void BinaryEquationWriter::WriteItemValStr(BYTE name, std::wstring val)
	{
		int nCurPos = WriteItemStart(name);

        m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
        m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
        m_pStream->WriteStringW(val);

		WriteItemEnd(nCurPos);
	}

	BinaryEquationWriter::CBaseCommand::CBaseCommand() : nBlockNum(-1), bPile(false), bEqArrayStart(false), nCount(0), nHAlignPile(2)
	{
	}
	BinaryEquationWriter::CBaseCommand::~CBaseCommand()
	{
	}
	void BinaryEquationWriter::CBaseCommand::Next()
	{
		bool bCurPile;
		bCurPile = GetPile();
		if (!bCurPile || (nBlockNum == -1))
			nBlockNum++;
	}
	bool BinaryEquationWriter::CBaseCommand::IsOpenNode()
	{
		return bOpenNode;
	}
	int BinaryEquationWriter::CBaseCommand::GetCount()
	{
		return nCount;
	}
	void BinaryEquationWriter::CBaseCommand::SetPile(bool bSetPile, BYTE nHPile)
	{
		nHAlignPile = nHPile;
		bPile = bSetPile;
		bEqArrayStart = !bSetPile;
	}
	bool BinaryEquationWriter::CBaseCommand::GetPile()
	{
		return bPile;
	}
	int BinaryEquationWriter::CBaseCommand::GetBlockNum()
	{
		return nBlockNum;
	}
	void BinaryEquationWriter::CBaseCommand::WriteBeginNode(BinaryEquationWriter* pWriter, BYTE elem)
	{
		int nElemPos;
		if (bEqArrayStart)
		{
			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			nRows++;
		}
		else
		{
			nElemPos = pWriter->WriteItemStart(elem);
			nCount++;
		}
		m_aBaseStack.push(nElemPos);

		if (bPile && !bEqArrayStart)
		{
			bEqArrayStart = true;
			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::EqArr);
			m_aBaseStack.push(nElemPos);


			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::EqArrPr);

			pWriter->WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::McJc, nHAlignPile);

			int nCurPos1 = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathBottomNodesType::Row);
            pWriter->m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
            pWriter->m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
			pWriter->m_aRowsPosCounter.push( pWriter->WriteItemWithLengthStart());
			pWriter->WriteItemEnd(nCurPos1);



			pWriter->WriteItemEnd(nElemPos);

			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			nRows = 1;
			m_aBaseStack.push(nElemPos);
		}
	}
	void BinaryEquationWriter::CBaseCommand::WriteBeginNode(BinaryEquationWriter* pWriter)
	{
		int nElemPos;
		if (bEqArrayStart)
		{
			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			nRows++;
			m_aBaseStack.push(nElemPos);
		}
		if (bPile && !bEqArrayStart)
		{
			bEqArrayStart = true;
			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::EqArr);
			m_aBaseStack.push(nElemPos);


			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::EqArrPr);

			pWriter->WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::McJc, nHAlignPile);

			int nCurPos1 = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathBottomNodesType::Row);
            pWriter->m_pStream->WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
            pWriter->m_pStream->WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
			pWriter->m_aRowsPosCounter.push(pWriter->WriteItemWithLengthStart());
			pWriter->WriteItemEnd(nCurPos1);



			pWriter->WriteItemEnd(nElemPos);

			nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			nRows = 1;
			m_aBaseStack.push(nElemPos);
		}
	}
	void BinaryEquationWriter::CBaseCommand::WriteEndNode(BinaryEquationWriter* pWriter)
			{
				int nCurPos = -1;
				if (!m_aBaseStack.empty())
				{
					nCurPos = m_aBaseStack.top();
					m_aBaseStack.pop();
				}
				if (!pWriter->oRManager.IsEmpty())
				{
					pWriter->WriteRun();
				}

				if (bPile && bEqArrayStart)
				{
					if (nCurPos > 0)
						pWriter->WriteItemEnd(nCurPos);
				}
				else if (!bPile && !bEqArrayStart)
				{
					if (nCurPos > 0)
						pWriter->WriteItemEnd(nCurPos);
				}
				else if (!bPile && bEqArrayStart)
				{
					pWriter->m_aRowsCounter.push(nRows);
					bEqArrayStart = false;

					if (nCurPos > 0)
						pWriter->WriteItemEnd(nCurPos);//eqArr

					if (!m_aBaseStack.empty())
					{
						nCurPos = m_aBaseStack.top();
						m_aBaseStack.pop();
						pWriter->WriteItemEnd(nCurPos);
					}
				}
			}

	BinaryEquationWriter::CMatrixCommand::CMatrixCommand() : nRows(0), nCols(0) {}
	BinaryEquationWriter::CMatrixCommand::~CMatrixCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CMatrixCommand::GetCommand()
	{
		return commandMatrix;
	}
	void BinaryEquationWriter::CMatrixCommand::SetProps(int nRows, int nCols)
	{
		this->nRows = nRows;
		this->nCols = nCols;
	}
	void BinaryEquationWriter::CMatrixCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		int nCurRow = nBlockNum / nCols;
		int nCurCol = nBlockNum % nCols;

		if (0 == nCurCol)
			nRowPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Mr);

		nColPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
	}
	void BinaryEquationWriter::CMatrixCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				int nCurRow = nBlockNum / nCols;
				int nCurCol = nBlockNum % nCols;

				if (!pWriter->oRManager.IsEmpty())
				{
					pWriter->WriteRun();
				}
				pWriter->WriteItemEnd(nColPos);

				if (nCols - 1 == nCurCol)
					pWriter->WriteItemEnd(nRowPos);
			}

	BinaryEquationWriter::CBracketsCommand::CBracketsCommand() {}
	BinaryEquationWriter::CBracketsCommand::~CBracketsCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CBracketsCommand::GetCommand(){return commandBrackets;}
	void BinaryEquationWriter::CBracketsCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CBracketsCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CBracketsCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CRootCommand::CRootCommand() {}
	BinaryEquationWriter::CRootCommand::~CRootCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CRootCommand::GetCommand(){return commandRoot;}
	void BinaryEquationWriter::CRootCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CRootCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CRootCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
						WriteEndNode(pWriter);
				}
				else if (1 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Deg);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CFractionCommand::CFractionCommand() {}
	BinaryEquationWriter::CFractionCommand::~CFractionCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CFractionCommand::GetCommand(){return commandFraction;}
	void BinaryEquationWriter::CFractionCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CFractionCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CFractionCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Num);
					else
						WriteEndNode(pWriter);
				}
				else if (1 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Den);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CScriptCommand::CScriptCommand() {}
	BinaryEquationWriter::CScriptCommand::~CScriptCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CScriptCommand::GetCommand(){return commandScript;}
	void BinaryEquationWriter::CScriptCommand::SetProps(bool bInline, bool bBase, bool bSup, bool bSub)
	{
		this->bBase   = bBase;
		this->bInline = bInline;
		this->bSub    = bSub;
		this->bSup    = bSup;
	}
	void BinaryEquationWriter::CScriptCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CScriptCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CScriptCommand::Write(BinaryEquationWriter* pWriter, bool bBegin)
			{
				bOpenNode = bBegin;
				if (bInline)
				{
					if (0 == nBlockNum)
					{
						if (bBase)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
							else
								WriteEndNode(pWriter);
						}
						else if (bSub)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sub);
							else
								WriteEndNode(pWriter);
						}
						else if (bSup)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
							else
								WriteEndNode(pWriter);
						}
					}
					else if (1 == nBlockNum)
					{
						if (bBase)
						{
							if (bSub)
							{
								if (bBegin)
									WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sub);
								else
									WriteEndNode(pWriter);
							}
							else if (bSup)
							{
								if (bBegin)
									WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
								else
									WriteEndNode(pWriter);
							}
						}
						else if (bSub && bSup)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
							else
								WriteEndNode(pWriter);
						}
					}
					else if (2 == nBlockNum)
					{
						if (bSup)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
							else
								WriteEndNode(pWriter);
						}
					}
				}
				else
				{
					if (0 == nBlockNum) //sSubSup
					{
						if (bSub)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sub);
							else
								WriteEndNode(pWriter);
						}
						else if (bSup)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
							else
								WriteEndNode(pWriter);
						}
						else if (bBase)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
							else
								WriteEndNode(pWriter);
						}
					}
					else if (1 == nBlockNum)
					{
						if (bSub)
						{
							if (bSup)
							{
								if (bBegin)
									WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
								else
									WriteEndNode(pWriter);
							}
							else if (bBase)
							{
								if (bBegin)
									WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
								else
									WriteEndNode(pWriter);
							}
						}
						else if (bSup && bBase)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
							else
								WriteEndNode(pWriter);
						}
					}
					else if (2 == nBlockNum)
					{
						if (bBase && bSub && bSup)
						{
							if (bBegin)
								WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
							else
								WriteEndNode(pWriter);
						}
					}
				}
			}

	BinaryEquationWriter::CBarCommand::CBarCommand() {}
	BinaryEquationWriter::CBarCommand::~CBarCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CBarCommand::GetCommand(){return commandBar;}
	void BinaryEquationWriter::CBarCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CBarCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CBarCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (bBeginNode)
					WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
				else
					WriteEndNode(pWriter);
			}

	BinaryEquationWriter::CArrowCommand::CArrowCommand() {}
	BinaryEquationWriter::CArrowCommand::~CArrowCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CArrowCommand::GetCommand(){return commandArrow;}
	void BinaryEquationWriter::CArrowCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
	}
	void BinaryEquationWriter::CArrowCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		pWriter->WriteItemEnd(nElemPos);
	}

	BinaryEquationWriter::CEqArrayCommand::CEqArrayCommand() {}
	BinaryEquationWriter::CEqArrayCommand::~CEqArrayCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CEqArrayCommand::GetCommand(){ return commandEqArray; }
	void BinaryEquationWriter::CEqArrayCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CEqArrayCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CEqArrayCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (bBeginNode)
					WriteBeginNode(pWriter);
				else
					WriteEndNode(pWriter);
			}

	BinaryEquationWriter::CIntegralCommand::CIntegralCommand() {}
	BinaryEquationWriter::CIntegralCommand::~CIntegralCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CIntegralCommand::GetCommand(){return commandIntegral;}
	void BinaryEquationWriter::CIntegralCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CIntegralCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CIntegralCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
						WriteEndNode(pWriter);
				}
				else if (1 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sub);
					else
						WriteEndNode(pWriter);
				}
				else if (2 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CVerticalBraceCommand::CVerticalBraceCommand() {}
	BinaryEquationWriter::CVerticalBraceCommand::~CVerticalBraceCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CVerticalBraceCommand::GetCommand(){return commandVerticalBrace;}
	void BinaryEquationWriter::CVerticalBraceCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CVerticalBraceCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CVerticalBraceCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
					{
						WriteEndNode(pWriter);

						if (!bPile)
						{
							pWriter->PopCommand();

							int nCurPos = pWriter->m_aLimitStack.top();
							pWriter->WriteItemEnd(nCurPos); //groupChr
							pWriter->m_aLimitStack.pop();

							nCurPos = pWriter->m_aLimitStack.top();
							pWriter->WriteItemEnd(nCurPos); //lim element
							pWriter->m_aLimitStack.pop();

							pWriter->PushCommand(commandVerticalBraceLim);
							CBaseCommand* pCommand = pWriter->TopCommand();
							pCommand->WriteBeginBlock(pWriter);
						}
					}
				}
			}

	BinaryEquationWriter::CVerticalBraceLimCommand::CVerticalBraceLimCommand() {}
	BinaryEquationWriter::CVerticalBraceLimCommand::~CVerticalBraceLimCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CVerticalBraceLimCommand::GetCommand(){return commandVerticalBraceLim;}
	void BinaryEquationWriter::CVerticalBraceLimCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CVerticalBraceLimCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CVerticalBraceLimCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Lim);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CNArrayCommand::CNArrayCommand() {}
	BinaryEquationWriter::CNArrayCommand::~CNArrayCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CNArrayCommand::GetCommand(){return commandNArray;}
	void BinaryEquationWriter::CNArrayCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CNArrayCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CNArrayCommand::SetType(MNARRAYTYPE eType)
	{
		this->eType = eType;
	}
	MNARRAYTYPE BinaryEquationWriter::CNArrayCommand::GetType()
	{
		return eType;
	}
	void BinaryEquationWriter::CNArrayCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
						WriteEndNode(pWriter);
				}
				else if (1 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sub);
					else
						WriteEndNode(pWriter);
				}
				else if (2 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Sup);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CLongDivisionCommand::CLongDivisionCommand() {}
	BinaryEquationWriter::CLongDivisionCommand::~CLongDivisionCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CLongDivisionCommand::GetCommand(){ return commandLongDivision; }
	void BinaryEquationWriter::CLongDivisionCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CLongDivisionCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CLongDivisionCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (1 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Num);
					else
						WriteEndNode(pWriter);
				}
				else if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Den);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CBracketsWithSeparatorCommand::CBracketsWithSeparatorCommand() {}
	BinaryEquationWriter::CBracketsWithSeparatorCommand::~CBracketsWithSeparatorCommand() {}
	BinaryEquationWriter::ECommandType BinaryEquationWriter::CBracketsWithSeparatorCommand::GetCommand(){return commandBracketsSep;}
	void BinaryEquationWriter::CBracketsWithSeparatorCommand::SetType(MANGLEBRACKETSWITHSEPARATORTYPE eType)
	{
		this->eType = eType;
	}
	void BinaryEquationWriter::CBracketsWithSeparatorCommand::WriteBeginBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, true);
	}
	void BinaryEquationWriter::CBracketsWithSeparatorCommand::WriteEndBlock(BinaryEquationWriter* pWriter)
	{
		Write(pWriter, false);
	}
	void BinaryEquationWriter::CBracketsWithSeparatorCommand::Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
						WriteEndNode(pWriter);
				}
				else if (1 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
						WriteEndNode(pWriter);
				}
			}

	BinaryEquationWriter::CBaseCommand* BinaryEquationWriter::PushCommand(ECommandType eType)
	{
		CBaseCommand* pCommand = NULL;
		switch(eType)
		{
		case commandMatrix:				pCommand = new CMatrixCommand(); break;
		case commandBrackets:			pCommand = new CBracketsCommand(); break;
		case commandRoot:				pCommand = new CRootCommand(); break;
		case commandFraction:			pCommand = new CFractionCommand(); break;
		case commandScript:				pCommand = new CScriptCommand(); break;
		case commandBar:				pCommand = new CBarCommand(); break;
		case commandArrow:				pCommand = new CArrowCommand(); break;
		case commandIntegral:			pCommand = new CIntegralCommand(); break;
		case commandVerticalBrace:		pCommand = new CVerticalBraceCommand(); break;
		case commandVerticalBraceLim:	pCommand = new CVerticalBraceLimCommand(); break;
		case commandNArray:				pCommand = new CNArrayCommand(); break;
		case commandLongDivision:		pCommand = new CLongDivisionCommand(); break;
		case commandBracketsSep:		pCommand = new CBracketsWithSeparatorCommand(); break;
		case commandEqArray:			pCommand = new CEqArrayCommand(); break;
		}

		m_aCommandStack.push(pCommand);

		return pCommand;
	}
	void BinaryEquationWriter::PopCommand()
	{
		CBaseCommand* pCommand = m_aCommandStack.top();
		if (pCommand)
			delete pCommand;

		m_aCommandStack.pop();
	}
	BinaryEquationWriter::CBaseCommand* BinaryEquationWriter::TopCommand()
	{
		return m_aCommandStack.top();
	}
}
