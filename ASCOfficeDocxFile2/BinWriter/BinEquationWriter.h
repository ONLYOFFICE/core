#ifndef BINEQUATIONWRITER_H
#define BINEQUATIONWRITER_H

#include "BinReaderWriterDefines.h"
#include "../../Common/DocxFormat/Source/MathEquation/OutputDev.h"
//#include "String.h"
#include <stack>

/*namespace BinDocxRW
{
	class BinaryCommonWriter;
}*/
namespace MathEquation
{
	class EquationRun
	{
		public:
			CString str;
			TMathFont* pFont;
			LONG nTextSize;

		public:
			void AddChar(CString sChar, TMathFont* pNewFont, LONG lSize)
			{
				str = sChar;
				pFont = pNewFont;
				nTextSize = lSize;
			}
			bool CompareFont(TMathFont* pNewFont)
			{
				if ( pNewFont->sName == pFont->sName && pNewFont->bBold == pFont->bBold && pNewFont->bItalic == pFont->bItalic )
					return true;
				else
					return false;
			}
			/*TMathFont* GetFont ()
			{
				return pFont;
			}*/

	};

	class RunManager
	{
		private:
			TMathFont* oCurFont;
			std::vector<EquationRun> arrRun;
		public:
			void Add(EquationRun oElem)
			{				
				arrRun.push_back(oElem);
				return;
			}			
			bool IsEmpty()
			{
				return arrRun.empty();
			}
			LONG GetSize()
			{
				return arrRun.size();
			}			
			bool GetElem (LONG lPos, EquationRun &oRun)
			{
				if (lPos <= arrRun.size() )
				{
					oRun = arrRun[lPos];
					return true;
				}
				return false;
			}
			void Clear()
			{
				arrRun.clear();
			}
			void RemoveElem (LONG lPos)
			{
				arrRun.erase(arrRun.begin()+lPos);
			}
	};

	class BinaryEquationWriter : public IOutputDev
	{
		public:
			NSBinPptxRW::CBinaryFileWriter &m_oStream;
			std::stack<int> m_aEquationStack;
			std::stack<int> m_aNArrayStack;
			std::stack<int> m_aRunStack;
			std::stack<int> m_aFractionStack;
			std::stack<int> m_aGroupChrStack; 
			std::stack<int> m_aRadicalStack;
			std::stack<int> m_aMatrixStack;
			std::stack<int> m_aLimitStack;
			std::stack<int> m_aLimitElemsStack;
			std::stack<int> m_aScriptStack;
			std::stack<int> m_aAccentStack;
			std::stack<int> m_aRowsCounter;
			std::stack<int> m_aRowsPosCounter;
			std::stack<int> m_aDelimiterStack;
			RunManager oRManager;
			
			LONG nTextSize;
			LONG nCtrlSize;
		public:
			BinaryEquationWriter(NSBinPptxRW::CBinaryFileWriter &oStream) : bEmbel(false), m_oStream(oStream)
			{				
			}

			void WriteRPR(TMathFont* pFont, LONG nSize, BOOL bIsOpen)
			{
				if (NULL != pFont)
				{
					int nCurPos;
					//if (bIsOpen)
						nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::RPr);
					//else
					//	nCurPos = WriteItemStart(BinDocxRW::c_oSerRunType::rPr);

					if (false != pFont->bBold)
					{
						m_oStream.WriteBYTE(BinDocxRW::c_oSerProp_rPrType::Bold);
						m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Byte);
						m_oStream.WriteBOOL(true);
					}
					if (false != pFont->bItalic)
					{
						m_oStream.WriteBYTE(BinDocxRW::c_oSerProp_rPrType::Italic);
						m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Byte);
						m_oStream.WriteBOOL(true);
					}
					CString sFontName;
					//sFontName.Format(_T("%S"), pFont->sName.c_str());
					sFontName.Insert(0, _T("Cambria Math"));
					if (sFontName)
					{
						m_oStream.WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontAscii);
						m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
						m_oStream.WriteStringW(sFontName);

						m_oStream.WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontHAnsi);
						m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
						m_oStream.WriteStringW(sFontName);
					}
					if (nTextSize)
					{
						m_oStream.WriteBYTE(BinDocxRW::c_oSerProp_rPrType::FontSize);
						m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
						m_oStream.WriteLONG(nSize);
					}
					WriteItemEnd(nCurPos);
				}
				return;
			}
			
			void WriteRun()
			{
				BOOL bIsOpen;
				if (!m_aCommandStack.empty())
					bIsOpen = m_aCommandStack.top()->IsOpenNode(); //if false write ctrlPrp
				else
					bIsOpen = true;

				CString sText;
				LONG lSize = oRManager.GetSize();
				if (lSize > 0)
				{	
					for (int i=0; i<lSize; i++)
					{
						EquationRun oRun;
						bool bRes = oRManager.GetElem( i, oRun);
						if (bRes)
						{
							TMathFont* pCurFont = oRun.pFont;
							int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::MRun);
							WriteRPR(oRun.pFont, oRun.nTextSize, bIsOpen);

							int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::MText);
							m_oStream.WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
							m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
							m_oStream.WriteStringW(oRun.str);
							WriteItemEnd(nCurPos1);
							WriteItemEnd(nCurPos);							
						}
					}
					oRManager.Clear();
				}
				return;
			}

			int WriteItemStart(BYTE type)
			{
				//type
				m_oStream.WriteBYTE(type);
				return WriteItemWithLengthStart();
			}
			void WriteItemEnd(int nStart)
			{
				WriteItemWithLengthEnd(nStart);
			}
			int WriteItemWithLengthStart()
			{
				//Запоминаем позицию чтобы в конце записать туда длину
				int nStartPos = m_oStream.GetPosition();
				m_oStream.Skip(4);	
				return nStartPos;
			}
			void WriteItemWithLengthEnd(int nStart)
			{
				//Length
				int nEnd = m_oStream.GetPosition();
				m_oStream.SetPosition(nStart);
				m_oStream.WriteLONG(nEnd - nStart - 4);
				m_oStream.SetPosition(nEnd);
			}

			virtual void BeginEquation()
			{
				int nCurPos = WriteItemStart(BinDocxRW::c_oSerParType::OMath);
				m_aEquationStack.push(nCurPos);
			}
			virtual void EndEquation()
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
			virtual void BeginBlock()
			{
				if (!m_aCommandStack.empty())
				{
					CBaseCommand* pCommand = TopCommand();
					pCommand->Next();
					pCommand->WriteBeginBlock(this);
				}
			}
			virtual void EndBlock()
			{
				if (!m_aCommandStack.empty())
				{
					CBaseCommand* pCommand = TopCommand();
					pCommand->WriteEndBlock(this);
				}
			}
			virtual void SetSize(uint16_t nSize)
			{
				nTextSize = nSize*2;				
			}
			virtual void BeginChar(Unicode_t uChar, uint8_t nTypeFace, bool bSpecialSymbol)
			{
				BOOL bIsOpen;
				if (!m_aCommandStack.empty())
					bIsOpen = m_aCommandStack.top()->IsOpenNode(); //if false write ctrlPrp
				else
					bIsOpen = true;
								
				TMathFont* pFont = GetFont(nTypeFace);
				if (!pFont)
					return;

				EquationRun oRun;
				if (uChar)
				{
					CString str;
					if (bSpecialSymbol)
					{
						switch(uChar)
						{
							case specialsymAlignment:  str.Insert(0,0x0089); break;
							case specialsymZeroSpace:  str.Insert(0,0x200B); break;
							case specialsymThinSpace:  str.Insert(0,0x2009); break; break;
							case specialsymThickSpace: str.Insert(0,0x2004); break;
							case specialsymLargeSpace: str.Insert(0,0x2005); break;
							case specialsymOnePtSpace: str.Insert(0,0x2003); break;
						}
					}
					else
						str.Insert(0,uChar);
					
					oRun.AddChar(str, pFont, nTextSize);
					oRManager.Add(oRun);
				}							
			}
			virtual void AddCharEmbel(MEMBELTYPE eType)
			{
				bEmbel = true;
				int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Acc);
				int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::AccPr);
				CString str;
				switch (eType)
				{
					case embelDot:       str.Insert(0, 0x0307); break;
					case embelDDot:      str.Insert(0, 0x0308); break;
					case embelDDDot:     str.Insert(0, 0x20DB); break;
					case embelPrime:     str.Insert(0, 0x2032); break;
					case embelDPrime:    str.Insert(0, 0x2033); break;
					case embelLPrime:    str.Insert(0, 0x0300); break;
					case embelTilde:     str.Insert(0, 0x007E); break;
					case embelHat:       str.Insert(0, 0x005E); break;
					case embelSlash:     str.Insert(0, 0x002F); break;
					case embelLArrow:    str.Insert(0, 0x2190); break;
					case embelRArrow:    str.Insert(0, 0x2192); break;
					case embelDArrow:    str.Insert(0, 0x2194); break;
					case embelLHarpoon:  str.Insert(0, 0x21BC); break;
					case embelRHarpoon:  str.Insert(0, 0x21C0); break;
					case embelStrikeout: str.Insert(0, 0x0336); break;
					case embelBar:       str.Insert(0, 0x0305); break;
					case embelTPrime:    str.Insert(0, 0x2034); break;
					case embelFrown:     str.Insert(0, 0x23DD); break;
					case embelSmile:     str.Insert(0, 0x23DC); break;
				}
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
				WriteItemEnd(nCurPos1);

				int nCurPos2 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
				m_aAccentStack.push(nCurPos2);
				m_aAccentStack.push(nCurPos);
			}
			virtual void EndChar()
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
			virtual void BeginMatrix(uint8_t nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, uint8_t nRows, uint8_t nCols, uint8_t* pVerBorders, uint8_t* pHorBorders)
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
					case matrixhoralignLeft:		horAlign = SimpleTypes::xalignLeft;
					case matrixhoralignCenter:		horAlign = SimpleTypes::xalignCenter;
					case matrixhoralignRight:		horAlign = SimpleTypes::xalignRight;
					case matrixhoralignEqualSign:	horAlign = SimpleTypes::xalignCenter;
					case matrixhoralignCommaSign:	horAlign = SimpleTypes::xalignCenter;
				}
				WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::McJc, horAlign);

				WriteItemEnd(nCurPos4);
				WriteItemEnd(nCurPos3);
				WriteItemEnd(nCurPos2);
				WriteItemEnd(nCurPos1);

			}
			virtual void EndMatrix()
			{
				PopCommand();

				if (!m_aMatrixStack.empty())
				{
					int nCurPos = m_aMatrixStack.top();
					WriteItemEnd(nCurPos);
					m_aMatrixStack.pop();
				}
			}
			virtual void StartPile(uint8_t nHAlign, uint8_t nVAlign)
			{
				if (!m_aCommandStack.empty())
					m_aCommandStack.top()->SetPile(true);
			}
			virtual void EndPile()
			{
				if (!m_aCommandStack.empty())
				{
					m_aCommandStack.top()->SetPile(false);
					CBaseCommand* pCommand = TopCommand();
					pCommand->WriteEndBlock(this);

					int nRows = m_aRowsCounter.top();
					int nPos = m_aRowsPosCounter.top();
					m_aRowsCounter.pop();
					m_aRowsPosCounter.pop();

					int nEnd = m_oStream.GetPosition();
					m_oStream.SetPosition(nPos);
					m_oStream.WriteLONG(nRows);
					m_oStream.SetPosition(nEnd);

					ECommandType type; 
					type = pCommand->GetCommand();
					if (type == commandIntegral || type == commandNArray)
						pCommand->Next();
				}
			}
			virtual void BeginBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose)
			{
				if (!oRManager.IsEmpty())
					WriteRun();

				PushCommand(commandBrackets);

				int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Delimiter);
				m_aDelimiterStack.push(nCurPos);

				int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::DelimiterPr);
				WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Column, 2);

				if (!bOpen)
					WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, _T(""));
				else
				{
					CString strOpen;
					switch(eType)
					{
						case bracketsAngle:				strOpen.Insert(0, 0x27E8); break;
						case bracketsRound:				strOpen.Insert(0, 0x0028); break;
						case bracketsCurve:				strOpen.Insert(0, 0x007B); break;
						case bracketsSquare:			strOpen.Insert(0, 0x005B); break;
						case bracketsLine:				strOpen.Insert(0, 0x007C); break;
						case bracketsDLine:				strOpen.Insert(0, 0x2016); break;
						case bracketsFloor:				strOpen.Insert(0, 0x23A3); break;
						case bracketsCeil:				strOpen.Insert(0, 0x23A1); break;
						case bracketsSquareOpenOpen:	strOpen.Insert(0, 0x005B); break;
						case bracketsSquareCloseClose:	strOpen.Insert(0, 0x005D); break;
						case bracketsSquareCloseOpen:	strOpen.Insert(0, 0x005D); break;
						case bracketsSquareRound:		strOpen.Insert(0, 0x005B); break;
						case bracketsRoundSquare:		strOpen.Insert(0, 0x0028); break;
					}
					WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, strOpen);
				}

				if (!bClose)
					WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, _T(""));
				else
				{
					CString strEnd;
					switch(eType)
					{
						case bracketsAngle:				strEnd.Insert(0, 0x27E9); break;
						case bracketsRound:				strEnd.Insert(0, 0x0029); break;
						case bracketsCurve:				strEnd.Insert(0, 0x007D); break;
						case bracketsSquare:			strEnd.Insert(0, 0x005D); break;
						case bracketsLine:				strEnd.Insert(0, 0x007C); break;
						case bracketsDLine:				strEnd.Insert(0, 0x2016); break;
						case bracketsFloor:				strEnd.Insert(0, 0x23A6); break;
						case bracketsCeil:				strEnd.Insert(0, 0x23A4); break;
						case bracketsSquareOpenOpen:	strEnd.Insert(0, 0x005B); break;
						case bracketsSquareCloseClose:	strEnd.Insert(0, 0x005D); break;
						case bracketsSquareCloseOpen:	strEnd.Insert(0, 0x005B); break;
						case bracketsSquareRound:		strEnd.Insert(0, 0x0029); break;
						case bracketsRoundSquare:		strEnd.Insert(0, 0x005D); break;
					}
					WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, strEnd);
				}

				WriteItemEnd(nCurPos1);
			}
			virtual void EndBrackets  (MBRACKETSTYPE eType, bool bOpen, bool bClose)
			{
				PopCommand();

				if (!m_aDelimiterStack.empty())
				{
					int nCurPos = m_aDelimiterStack.top();
					WriteItemEnd(nCurPos);
					m_aDelimiterStack.pop();
				}
			}
			virtual void BeginRoot(bool bDegree)
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
			virtual void EndRoot  ()
			{
				PopCommand();

				if (!m_aRadicalStack.empty())
				{
					int nCurPos = m_aRadicalStack.top();
					WriteItemEnd(nCurPos);
					m_aRadicalStack.pop();
				}
			}
			virtual void BeginFraction(MFRACTIONTYPES eType, bool bInline)
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
			virtual void EndFraction  ()
			{
				PopCommand();

				if (!m_aFractionStack.empty())
				{
					int nCurPos = m_aFractionStack.top();
					WriteItemEnd(nCurPos);
					m_aFractionStack.pop();
				}
			}
			virtual void BeginScript(MSCRIPTALIGN eAlign, bool bBase = false, bool bSup = false, bool bSub = false, bool bInline = true)
			{
				CScriptCommand* pCommand = (CScriptCommand*)PushCommand(commandScript);				
				pCommand->SetProps(bInline, bBase, bSup, bSub);

				int nCurPos;
				int nCurPos1;
				switch(eAlign)
				{
					case scriptalignRight:
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
					case scriptalignCenter: break;
					case scriptalignLeft:
					{
						nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SPre);
						nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::SPrePr);
					}
						break;
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
			virtual void EndScript  ()
			{
				PopCommand();

				if (!m_aScriptStack.empty())
				{
					int nCurPos = m_aScriptStack.top();
					WriteItemEnd(nCurPos);
					m_aScriptStack.pop();
				}
			}
			virtual void BeginBar(MBARTYPE eType, bool bTop)
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

				CString str;
				switch(eType)
				{
					case bartypeLine:			str.Insert(0,0x0305);break;
					case bartypeDoubleLine:		str.Insert(0,0x033F);break;
					case bartypeArrowLeft:		str.Insert(0,0x20D6);break;
					case bartypeArrowRight:		str.Insert(0,0x20D7);break;
					case bartypeArrowDouble:	str.Insert(0,0x20E1);break;
				}
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);

				WriteItemEnd(nCurPos1);

			}
			virtual void EndBar  ()
			{
				PopCommand();

				if (!m_aGroupChrStack.empty())
				{
					int nCurPos = m_aGroupChrStack.top();
					WriteItemEnd(nCurPos);
					m_aGroupChrStack.pop();
				}
			}
			virtual void BeginArrow(MARROWTYPE eType, bool bTop)
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

				CString str;
				switch(eType)
				{
					case arrowtypeLeft:			str.Insert(0,0x2190);break;
					case arrowtypeRight:		str.Insert(0,0x2192);break;
					case arrowtypeDouble:		str.Insert(0,0x2194);break;
				}
				WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);

				WriteItemEnd(nCurPos1);
			}
			virtual void EndArrow  ()
			{
				PopCommand();

				if (!m_aGroupChrStack.empty())
				{
					int nCurPos = m_aGroupChrStack.top();
					WriteItemEnd(nCurPos);
					m_aGroupChrStack.pop();
				}
			}
			virtual void BeginIntegral(MINTEGRALTYPE eType)
			{
				if (!oRManager.IsEmpty())
					WriteRun();

				PushCommand(commandIntegral);

				int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Nary);
				m_aNArrayStack.push(nCurPos);

				int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::NaryPr);
				CString str;
				switch(eType)
				{		
					case integraltypeSingle:
						str.Insert(0,0x222B);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case integraltypeSingleRSub:
						str.Insert(0,0x222B);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
						break;
					case integraltypeSingleRSubSup:
						str.Insert(0,0x222B);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						break;
					case integraltypeSingleOriented:
						str.Insert(0,0x222E);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case integraltypeSingleOrientedRSub:
						str.Insert(0,0x222E);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
						break;

					case integraltypeDouble:
						str.Insert(0,0x222C);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case integraltypeDoubleRSub:
						str.Insert(0,0x222C);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
						break;
					case integraltypeDoubleOriented:
						str.Insert(0,0x222F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case integraltypeDoubleOrientedRSub:
						str.Insert(0,0x222F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;

					case integraltypeTriple:
						str.Insert(0,0x222D);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case integraltypeTripleRSub:
						str.Insert(0,0x222D);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case integraltypeTripleOriented:
						str.Insert(0,0x2230);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case integraltypeTripleOrientedRSub:
						str.Insert(0,0x2230);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;

					case integraltypeSingleCSubSup:
						str.Insert(0,0x222B);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						break;
					case integraltypeSingleCSub:
						str.Insert(0,0x222B);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true);
						break;
					case integraltypeSingleOrientedCSub:
						str.Insert(0,0x222E);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;

					case integraltypeDoubleOrientedCSub:
						str.Insert(0,0x222F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case integraltypeDoubleCSub:
						str.Insert(0,0x222C);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;

					case integraltypeTripleOrientedCSub:
						str.Insert(0,0x2230);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case integraltypeTripleCSub:
						str.Insert(0,0x222D);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
				}

				
				WriteItemEnd(nCurPos1);
			}
			virtual void EndIntegral  ()
			{
				PopCommand();

				if (!m_aNArrayStack.empty())
				{
					int nCurPos = m_aNArrayStack.top();
					WriteItemEnd(nCurPos);
					m_aNArrayStack.pop();
				}
			}
			virtual void BeginVerticalBrace(bool bTop)
			{
				if (!oRManager.IsEmpty())
					WriteRun();

				BYTE pos, vertJc;
				CString chr;
				if (bTop)
				{
					chr.Insert(0,0x23DE);
					vertJc = SimpleTypes::verticaljcBottom;
					pos = SimpleTypes::tbTop;
					int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::LimUpp);
					m_aLimitStack.push(nCurPos);

					int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::LimUppPr);
					//+ctrlprp
					WriteItemEnd(nCurPos1);
				}
				else
				{
					chr.Insert(0,0x23DF);
					vertJc = SimpleTypes::verticaljcTop;
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
			virtual void EndVerticalBrace  ()
			{
				PopCommand();

				if (!m_aLimitStack.empty())
				{
					int nCurPos = m_aLimitStack.top();
					WriteItemEnd(nCurPos);
					m_aLimitStack.pop();
				}
			}
			virtual void BeingNArray(MNARRAYTYPE eType)
			{
				if (!oRManager.IsEmpty())
					WriteRun();

				CNArrayCommand* pCommand = (CNArrayCommand*)PushCommand(commandNArray);
				pCommand->SetType(eType);

				int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Nary);
				m_aNArrayStack.push(nCurPos);

				int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::NaryPr);
				CString str;
				switch(eType)
				{		
					case narySumCSub:
						str.Insert(0,0x2211);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case narySumCSubSup:
						str.Insert(0,0x2211);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr); 
						break;
					case narySum:
						str.Insert(0,0x2211);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case narySumRSub:
						str.Insert(0,0x2211);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case narySumRSubSup:
						str.Insert(0,0x2211);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						break;

					case naryProdCSub:
						str.Insert(0,0x220F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryProdCSubSup:
						str.Insert(0,0x220F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr); 
						break;
					case naryProd:
						str.Insert(0,0x220F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case naryProdRSub:
						str.Insert(0,0x220F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryProdRSubSup:
						str.Insert(0,0x220F);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						break;

					case naryCoProdCSub:
						str.Insert(0,0x2210);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryCoProdCSubSup:
						str.Insert(0,0x2210);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr); 
						break;
					case naryCoProd:
						str.Insert(0,0x2210);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case naryCoProdRSub:
						str.Insert(0,0x2210);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryCoProdRSubSup:
						str.Insert(0,0x2210);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						break;

					case naryUnionCSub:
						str.Insert(0,0x22C3);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryUnionCSubSup:
						str.Insert(0,0x22C3);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr); 
						break;
					case naryUnion:
						str.Insert(0,0x22C3);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case naryUnionRSub:
						str.Insert(0,0x22C3);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryUnionRSubSup:
						str.Insert(0,0x22C3);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						break;

					case naryIntersectCSub:
						str.Insert(0,0x22C2);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryIntersectCSubSup:
						str.Insert(0,0x22C2);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr); 
						break;
					case naryIntersect:
						str.Insert(0,0x22C2);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocUndOvr);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SubHide, true);
						break;
					case naryIntersectRSub:
						str.Insert(0,0x22C2);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::SupHide, true); 
						break;
					case naryIntersectRSubSup:
						str.Insert(0,0x22C2);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::Chr, str);
						WriteItemVal(BinDocxRW::c_oSer_OMathBottomNodesType::LimLoc, SimpleTypes::limLocSubSup);
						break;

				}

				
				WriteItemEnd(nCurPos1);
			}
			virtual void EndNArray  ()
			{
				CNArrayCommand* pCommand = (CNArrayCommand*)TopCommand();
				MNARRAYTYPE eType = pCommand->GetType();
				PopCommand();

				if (!m_aNArrayStack.empty())
				{
					int nCurPos = m_aNArrayStack.top();
					WriteItemEnd(nCurPos);
					m_aNArrayStack.pop();
				}
			}
			virtual void BeginLongDivision(MLONGDIVISION eType)
			{
				if (!oRManager.IsEmpty())
					WriteRun();
				//PushCommand(commandLongDivision);
				if (eType == longdivisionWithResult)
				{
					BeginFraction(fractionRegular, true);
				}
				else if (eType == longdivisionRegular)
				{
					BeginBrackets(bracketsLine, true, false);
				}
			}
			virtual void EndLongDivision  ()
			{
				ECommandType eType;
				CBaseCommand* pCommand = TopCommand();
				eType = pCommand->GetCommand();
				if (eType == commandFraction)
				{
					EndFraction();
				}
				else if (eType == commandBrackets)
				{
					MBRACKETSTYPE ebType;
					EndBrackets(ebType, false,false);
				}
			}
			virtual void BeginAngleBracketsWithSeparator(MANGLEBRACKETSWITHSEPARATORTYPE eType)
			{
				if (!oRManager.IsEmpty())
					WriteRun();

				CBracketsWithSeparatorCommand* pCommand = (CBracketsWithSeparatorCommand*)PushCommand(commandBracketsSep);
				pCommand->SetType(eType);

				int nCurPos = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Delimiter);
				m_aDelimiterStack.push(nCurPos);

				int nCurPos1 = WriteItemStart(BinDocxRW::c_oSer_OMathContentType::DelimiterPr);
				CString beg, sep, end;
				switch (eType)
				{
					case angbrwithsepBoth:	// < | >
						beg.Insert(0,0x27E8); sep.Insert(0, 0x007C); end.Insert(0, 0x27E9);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, beg);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::SepChr, sep);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, end);
						WriteItemValLong(BinDocxRW::c_oSer_OMathBottomNodesType::Column, 2);
						break;
					case angbrwithsepLeft:	// < |
						beg.Insert(0,0x27E8); end.Insert(0, 0x007C);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, beg);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, end);
						break;
					case angbrwithsepRight:	// | >
						beg.Insert(0,0x007C); end.Insert(0, 0x27E9);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::BegChr, beg);
						WriteItemValStr(BinDocxRW::c_oSer_OMathBottomNodesType::EndChr, end);
						break;
				}				
				WriteItemEnd(nCurPos1);

			}
			virtual void EndAngleBracketsWithSeparator  ()
			{
				PopCommand();

				if (!m_aDelimiterStack.empty())
				{
					int nCurPos = m_aDelimiterStack.top();
					WriteItemEnd(nCurPos);
					m_aDelimiterStack.pop();
				}
			}

			void AddFont(uint8_t nTypeFace, std::string sName, bool bBold, bool bItalic)
			{
				TMathFont aFont;
				aFont.sName   = sName;
				aFont.bBold   = bBold;
				aFont.bItalic = bItalic;
				m_mFonts[nTypeFace] = aFont;
			}
			void WriteItemVal(BYTE name, BYTE val)
			{
				int nCurPos = WriteItemStart(name);

				m_oStream.WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
				m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Byte);
				m_oStream.WriteBYTE(val);

				WriteItemEnd(nCurPos);
			}
			void WriteItemValLong(BYTE name, LONG val)
			{
				int nCurPos = WriteItemStart(name);

				m_oStream.WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
				m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);
				m_oStream.WriteLONG(val);

				WriteItemEnd(nCurPos);
			}
			void WriteItemVal(BYTE name, BOOL val)
			{
				int nCurPos = WriteItemStart(name);

				m_oStream.WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
				m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Byte);
				m_oStream.WriteBOOL(val);

				WriteItemEnd(nCurPos);
			}
			void WriteItemValStr(BYTE name, CString val)
			{
				int nCurPos = WriteItemStart(name);

				m_oStream.WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
				m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Variable);
				m_oStream.WriteStringW(val);

				WriteItemEnd(nCurPos);
			}

		private:
		enum ECommandType
		{
			commandMatrix			 = 0x00,
			commandBrackets			 = 0x01,
			commandRoot				 = 0x02,
			commandFraction			 = 0x03,
			commandScript			 = 0x04,
			commandBar				 = 0x05,
			commandArrow			 = 0x06,
			commandIntegral			 = 0x07,
			commandVerticalBrace	 = 0x08,
			commandNArray			 = 0x09,
			//commandLongDivision		 = 0x0a,
			commandBracketsSep		 = 0x0b,
			commandVerticalBraceLim	 = 0x0c
		};

		class CBaseCommand
		{
		public:

			CBaseCommand() : nBlockNum(-1), bPile(false), bEqArrayStart(false)
			{
			}

			virtual ~CBaseCommand() 
			{
			}

			void Next() 
			{
				BOOL bCurPile;
				bCurPile = GetPile();
				if (!bCurPile || (nBlockNum == -1))
					nBlockNum++;
			}

			BOOL IsOpenNode()
			{
				return bOpenNode;
			}
			void SetPile(BOOL bSetPile)
			{
				bPile = bSetPile;
				bEqArrayStart = !bSetPile;
			}
			bool GetPile()
			{
				return bPile;
			}

			int GetBlockNum()
			{
				return nBlockNum;
			}
			void WriteBeginNode(BinaryEquationWriter* pWriter, BYTE elem)
			{
				int nElemPos;
				if (bEqArrayStart)
				{
					nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
					nRows++;
				}
				else
					nElemPos = pWriter->WriteItemStart(elem);
				m_aBaseStack.push(nElemPos);

				if (bPile && !bEqArrayStart)
				{
					bEqArrayStart = true;
					nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::EqArr);
					m_aBaseStack.push(nElemPos);

					
					nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::EqArrPr);
					int nCurPos1 = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathBottomNodesType::Row);		
					pWriter->m_oStream.WriteBYTE(BinDocxRW::c_oSer_OMathBottomNodesValType::Val);
					pWriter->m_oStream.WriteBYTE(BinDocxRW::c_oSerPropLenType::Long);

					pWriter->m_aRowsPosCounter.push( pWriter->WriteItemWithLengthStart());
					
					pWriter->WriteItemEnd(nCurPos1);

					pWriter->WriteItemEnd(nElemPos);

					nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
					nRows = 1;
					m_aBaseStack.push(nElemPos);
				}
			}

			void WriteEndNode(BinaryEquationWriter* pWriter)
			{
				int nCurPos;
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
					pWriter->WriteItemEnd(nCurPos);
				else if (!bPile && !bEqArrayStart)
					pWriter->WriteItemEnd(nCurPos);
				else if (!bPile && bEqArrayStart)
				{					
					pWriter->m_aRowsCounter.push(nRows);
					bEqArrayStart = false;
					pWriter->WriteItemEnd(nCurPos);//eqArr

					if (!m_aBaseStack.empty())
					{
						nCurPos = m_aBaseStack.top();
						m_aBaseStack.pop();
					}
					pWriter->WriteItemEnd(nCurPos);
				}
			}

			virtual ECommandType GetCommand() = 0;
			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter) = 0;
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)   = 0;

		protected:
			int nRows;
			std::stack<int> m_aBaseStack;
			BOOL bOpenNode;
			BOOL bPile;
			BOOL bEqArrayStart;
			int nBlockNum;
		};

		class CMatrixCommand : public CBaseCommand
		{
		public:
			CMatrixCommand() : nRows(0), nCols(0) {}
			virtual ~CMatrixCommand() {}
			virtual ECommandType GetCommand()
			{
				return commandMatrix;
			}

			void SetProps(int nRows, int nCols)
			{
				this->nRows = nRows;
				this->nCols = nCols;
			}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{
				int nCurRow = nBlockNum / nCols;
				int nCurCol = nBlockNum % nCols;
				
				if (0 == nCurCol)
					nRowPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Mr);

				nColPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			}

			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				int nCurRow = nBlockNum / nCols;
				int nCurCol = nBlockNum % nCols;

				pWriter->WriteItemEnd(nColPos);

				if (nCols - 1 == nCurCol)
					pWriter->WriteItemEnd(nRowPos);
			}
		private:
			int nRowPos;
			int nColPos;
			int nRows;
			int nCols;
		};
		class CBracketsCommand : public CBaseCommand
		{
		public:
			CBracketsCommand() {}
			virtual ~CBracketsCommand() {}
			virtual ECommandType GetCommand(){return commandBrackets;}
			
			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}
			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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
		private:
			int nElemPos;
		};
		class CRootCommand : public CBaseCommand
		{
		public:
			CRootCommand() {}
			virtual ~CRootCommand() {}
			virtual ECommandType GetCommand(){return commandRoot;}
			
			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}
			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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
		private:
			int nElemPos;
			int nDegPos;
		};
		class CFractionCommand : public CBaseCommand
		{
		public:
			CFractionCommand() {}
			virtual ~CFractionCommand() {}
			virtual ECommandType GetCommand(){return commandFraction;}
			
			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}
			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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
		};
		class CScriptCommand : public CBaseCommand
		{
		public:
			CScriptCommand() {}
			virtual ~CScriptCommand() {}
			virtual ECommandType GetCommand(){return commandScript;}

			void SetProps(bool bInline, bool bBase, bool bSup, bool bSub)
			{
				this->bBase   = bBase;
				this->bInline = bInline;
				this->bSub    = bSub;
				this->bSup    = bSup;
			}
			
			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}
		private:

			void Write(BinaryEquationWriter* pWriter, bool bBegin)
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

		private:
			bool bBase;
			bool bSup;
			bool bSub;
			bool bInline;
		};
		class CBarCommand : public CBaseCommand
		{
		public:
			CBarCommand() {}
			virtual ~CBarCommand() {}
			virtual ECommandType GetCommand(){return commandBar;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{
				nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			}

			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				pWriter->WriteItemEnd(nElemPos);
			}
		private:
			int nElemPos;
		};
		class CArrowCommand : public CBaseCommand
		{
		public:
			CArrowCommand() {}
			virtual ~CArrowCommand() {}
			virtual ECommandType GetCommand(){return commandArrow;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{
				nElemPos = pWriter->WriteItemStart(BinDocxRW::c_oSer_OMathContentType::Element);
			}

			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				pWriter->WriteItemEnd(nElemPos);
			}
		private:
			int nElemPos;
		};
		class CIntegralCommand : public CBaseCommand
		{
		public:
			CIntegralCommand() {}
			virtual ~CIntegralCommand() {}
			virtual ECommandType GetCommand(){return commandIntegral;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}
		private:

			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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
		};
		class CVerticalBraceCommand : public CBaseCommand
		{
		public:
			CVerticalBraceCommand() {}
			virtual ~CVerticalBraceCommand() {}
			virtual ECommandType GetCommand(){return commandVerticalBrace;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}

			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
			{
				bOpenNode = bBeginNode;
				if (0 == nBlockNum)
				{
					if (bBeginNode)
						WriteBeginNode(pWriter, BinDocxRW::c_oSer_OMathContentType::Element);
					else
					{
						WriteEndNode(pWriter);
						pWriter->PopCommand();
						
						int nCurPos = pWriter->m_aLimitStack.top();
						pWriter->WriteItemEnd(nCurPos); //groupChr
						pWriter->m_aLimitStack.pop();

						
						nCurPos = pWriter->m_aLimitStack.top();
						pWriter->WriteItemEnd(nCurPos); //lim element
						pWriter->m_aLimitStack.pop();

						
						pWriter->PushCommand(commandVerticalBraceLim);
						CBaseCommand* pCommand = pWriter->TopCommand();
						pCommand->Next();
						pCommand->WriteBeginBlock(pWriter);
					}
				}
			}
		private:
			int nBasePos;
			int nBraceBasePos;
		};
		class CVerticalBraceLimCommand : public CBaseCommand
		{
		public:
			CVerticalBraceLimCommand() {}
			virtual ~CVerticalBraceLimCommand() {}
			virtual ECommandType GetCommand(){return commandVerticalBraceLim;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}

			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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
		private:
			int nBasePos;
			int nBraceBasePos;
		};
		class CNArrayCommand : public CBaseCommand
		{
		public:
			CNArrayCommand() {}
			virtual ~CNArrayCommand() {}
			virtual ECommandType GetCommand(){return commandNArray;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}

			void SetType(MNARRAYTYPE eType)
			{
				this->eType = eType;
			}

			MNARRAYTYPE GetType()
			{
				return eType;
			}
			
		private:

			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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

		private:			
			MNARRAYTYPE eType;
		};

		/*class CLongDivisionCommand : public CBaseCommand
		{
		public:
			CLongDivisionCommand() {}
			virtual ~CLongDivisionCommand() {}
			virtual ECommandType GetCommand(){return commandLongDivision;}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeBegin("base");
				else
					pWriter->WriteNodeBegin("result");
			}

			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeEnd("base");
				else
					pWriter->WriteNodeEnd("result");
			}
		private:
			BOOL bPile;
		};*/
		class CBracketsWithSeparatorCommand : public CBaseCommand
		{
		public:
			CBracketsWithSeparatorCommand() {}
			virtual ~CBracketsWithSeparatorCommand() {}
			virtual ECommandType GetCommand(){return commandBracketsSep;}
		
			void SetType(MANGLEBRACKETSWITHSEPARATORTYPE eType)
			{
				this->eType = eType;
			}

			virtual void WriteBeginBlock(BinaryEquationWriter* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(BinaryEquationWriter* pWriter)
			{
				Write(pWriter, false);
			}

		private:

			void Write(BinaryEquationWriter* pWriter, bool bBeginNode)
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

		private:
			MANGLEBRACKETSWITHSEPARATORTYPE eType;

		};
		CBaseCommand* PushCommand(ECommandType eType)
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
			//case commandLongDivision:		pCommand = new CLongDivisionCommand(); break;
			case commandBracketsSep:		pCommand = new CBracketsWithSeparatorCommand(); break;
			}

			m_aCommandStack.push(pCommand);

			return pCommand;
		}
		void PopCommand()
		{
			CBaseCommand* pCommand = m_aCommandStack.top();
			if (pCommand)
				delete pCommand;

			m_aCommandStack.pop();
		}

		CBaseCommand* TopCommand()
		{
			return m_aCommandStack.top();
		}

		private:

			CString rRet;

			bool bEmbel;
			std::stack<CBaseCommand*> m_aCommandStack;
	};
} 
#endif //BINEQUATIONWRITER_H