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

#include "OutputDev.h"
#include "String.h"
#include <stack>

#ifndef APPEND
    #define APPEND(str,ascii) str += ascii
#endif

#define OUTPUTCHARS

namespace MathEquation
{

	class CStringOutput : public IOutputDev
	{
	public:
		CStringOutput() : bEmbel(false)
		{
		}

		virtual void BeginEquation()
		{
			WriteNodeBegin("mathequation", false);
		}
		virtual void EndEquation()
		{
			WriteNodeEnd("mathequation");
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

        virtual void BeginChar(unsigned short uChar, uint8_t nTypeFace, bool bSpecialSymbol)
		{
			WriteNodeBegin("char", true);

			if (bSpecialSymbol)
			{
				switch(uChar)
				{
				case specialsymAlignment:  WriteStringAttribute("value", "Alignment symbol"); break;
				case specialsymZeroSpace:  WriteStringAttribute("value", "ZeroSpace"); break;
				case specialsymThinSpace:  WriteStringAttribute("value", "ThinSpace"); break;
				case specialsymThickSpace: WriteStringAttribute("value", "ThickSpace"); break;
				case specialsymLargeSpace: WriteStringAttribute("value", "LargeSpace"); break;
				case specialsymOnePtSpace: WriteStringAttribute("value", "OnePtSpace"); break;
				}
			}
			else
#ifdef OUTPUTCHARS
				WriteStringAttribute("value", uChar);
#else
				WriteIntAttribute("value", uChar);
#endif

			TMathFont* pFont = GetFont(nTypeFace);

			if (NULL != pFont)
			{
				WriteStringAttribute("fontname", pFont->sName);
				WriteBoolAttribute("bold", pFont->bBold);
				WriteBoolAttribute("italic", pFont->bItalic);
			}

			bEmbel = false;
		}
		virtual void AddCharEmbel(MEMBELTYPE eType)
		{
			if (!bEmbel)
			{
				APPEND(rRet," embel=\"");
				bEmbel = true;
			}

			switch (eType)
			{
			case embelDot:       APPEND(rRet," dot "); break;
			case embelDDot:      APPEND(rRet," ddot "); break;
			case embelDDDot:     APPEND(rRet," dddot "); break;
			case embelPrime:     APPEND(rRet," prime "); break;
			case embelDPrime:    APPEND(rRet," double prime "); break;
			case embelLPrime:    APPEND(rRet," left prime "); break;
			case embelTilde:     APPEND(rRet," tilde "); break;
			case embelHat:       APPEND(rRet," hat "); break;
			case embelSlash:     APPEND(rRet," slash "); break;
			case embelLArrow:    APPEND(rRet," left arrow "); break;
			case embelRArrow:    APPEND(rRet," right arrow "); break;
			case embelDArrow:    APPEND(rRet," double arrow "); break;
			case embelLHarpoon:  APPEND(rRet," left harpoon "); break;
			case embelRHarpoon:  APPEND(rRet," right harpoon "); break;
			case embelStrikeout: APPEND(rRet," strikeout "); break;
			case embelBar:       APPEND(rRet," bar "); break;
			case embelTPrime:    APPEND(rRet," triple prime "); break;
			case embelFrown:     APPEND(rRet," frown "); break;
			case embelSmile:     APPEND(rRet," smiple "); break;
			}
		}
		virtual void EndChar()
		{
			if (bEmbel)
				APPEND(rRet, "\"");
		
			WriteNodeEnd(true);
		}
		virtual void SetSize(uint16_t nSize)
		{
			WriteNodeBegin("size", true);
			WriteIntAttribute("value", nSize);
			WriteNodeEnd(true);
		}
		virtual void BeginMatrix(uint8_t nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, uint8_t nRows, uint8_t nCols, uint8_t* pVerBorders, uint8_t* pHorBorders)
		{
			CMatrixCommand* pCommand = (CMatrixCommand*)PushCommand(commandMatrix);
			pCommand->SetProps(nRows, nCols);

			WriteNodeBegin("matrix", true);
			WriteIntAttribute("rows", nRows);
			WriteIntAttribute("cols", nCols);

			switch(eHorAlign)
			{
			case matrixhoralignLeft:      WriteStringAttribute("horalign", "left"); break;
			case matrixhoralignCenter:    WriteStringAttribute("horalign", "center"); break;
			case matrixhoralignRight:     WriteStringAttribute("horalign", "right"); break;
			case matrixhoralignEqualSign: WriteStringAttribute("horalign", "equal"); break;
			case matrixhoralignCommaSign: WriteStringAttribute("horalign", "comma"); break;
			}

			switch(eVerAlign)
			{
			case matrixveralignTop:      WriteStringAttribute("veralign", "top"); break;
			case matrixveralignBaseLine: WriteStringAttribute("veralign", "baseline"); break;
			case matrixveralginBottom:   WriteStringAttribute("veralign", "bottom"); break;
			}

			WriteBoolAttribute("equalrows", bEqualRows);
			WriteBoolAttribute("equalcols", bEqualCols);

			std::string sVerBorders;
			for (int nVerIndex = 0; nVerIndex <= nRows; nVerIndex++)
			{
				MMATRIXBORDERTYPE eType = (MMATRIXBORDERTYPE)pVerBorders[nVerIndex];
				switch(eType)
				{
				case matrixborderNone:  sVerBorders.append(" none "); break;
				case matrixborderSolid: sVerBorders.append(" solid "); break;
				case matrixborderDash:  sVerBorders.append(" dash "); break;
				case matrixborderDot:   sVerBorders.append(" dot "); break;
				}
			}

			std::string sHorBorders;
			for (int nHorIndex = 0; nHorIndex <= nCols; nHorIndex++)
			{
				MMATRIXBORDERTYPE eType = (MMATRIXBORDERTYPE)pHorBorders[nHorIndex];
				switch(eType)
				{
				case matrixborderNone:  sHorBorders.append(" none "); break;
				case matrixborderSolid: sHorBorders.append(" solid "); break;
				case matrixborderDash:  sHorBorders.append(" dash "); break;
				case matrixborderDot:   sHorBorders.append(" dot "); break;
				}
			}
			WriteStringAttribute("verborders", sVerBorders);
			WriteStringAttribute("horborders", sHorBorders);
			WriteNodeEnd(false);
		}
		virtual void EndMatrix()
		{
			PopCommand();
			WriteNodeEnd("matrix");
		}		
		virtual void StartPile(uint8_t nHAlign, uint8_t nVAlign)
		{
			//switch(nHAlign)
			//{
			//case 1:
			//default:
			//	APPEND(rRet,"alignl {");
			//	break;
			//case 2:
			//	APPEND(rRet,"alignc {");
			//	break;
			//case 3:
			//	APPEND(rRet,"alignr {");
			//	break;
			//}
			//switch(nVAlign)
			//{
			//}

			WriteNodeBegin("pile", false);
		}
		virtual void EndPile()
		{
			WriteNodeEnd("pile");
		}
		virtual void BeginBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose)
		{
			PushCommand(commandBrackets);

			WriteNodeBegin("brackets", true);

			switch(eType)
			{
			case bracketsAngle:            WriteStringAttribute("type", "angle"); break;
			case bracketsRound:            WriteStringAttribute("type", "round"); break;
			case bracketsCurve:            WriteStringAttribute("type", "curve"); break;
			case bracketsSquare:           WriteStringAttribute("type", "square"); break;
			case bracketsLine:             WriteStringAttribute("type", "line"); break;
			case bracketsDLine:            WriteStringAttribute("type", "double line"); break;
			case bracketsFloor:            WriteStringAttribute("type", "floor"); break;
			case bracketsCeil:             WriteStringAttribute("type", "ceil");  break;
			case bracketsSquareOpenOpen:   WriteStringAttribute("type", "square open open");  break;
			case bracketsSquareRound:      WriteStringAttribute("type", "square-round");  break;
			case bracketsSquareCloseClose: WriteStringAttribute("type", "square close close");  break;
			case bracketsSquareCloseOpen:  WriteStringAttribute("type", "square close open");  break;
			case bracketsRoundSquare:      WriteStringAttribute("type", "round-square");  break;
			}

			WriteBoolAttribute("open", bOpen);
			WriteBoolAttribute("close", bClose);
			WriteNodeEnd(false);
		}
		virtual void EndBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose)
		{
			PopCommand();
			WriteNodeEnd("brackets");
		}

		virtual void BeginRoot(bool bDegree)
		{
			PushCommand(commandRoot);
			
			WriteNodeBegin("root", true);
			WriteBoolAttribute("nroot", bDegree);
			WriteNodeEnd(false);
		}
		virtual void EndRoot()
		{
			PopCommand();
			WriteNodeEnd("root");
		}

		virtual void BeginFraction(MFRACTIONTYPES eType, bool bInline)
		{
			PushCommand(commandFraction);

			WriteNodeBegin("fraction", true);
			WriteBoolAttribute("inline", bInline);
			WriteNodeEnd(false);
		}
		virtual void EndFraction()
		{
			PopCommand();
			WriteNodeEnd("fraction");
		}

		virtual void BeginScript(MSCRIPTALIGN eAlign, bool bBase = false, bool bSup = false, bool bSub = false, bool bInline = true)
		{
			CScriptCommand* pCommand = (CScriptCommand*)PushCommand(commandScript);
			pCommand->SetProps(bInline, bBase, bSup, bSub);

			WriteNodeBegin("script", true);

			switch(eAlign)
			{
			case scriptalignRight:  WriteStringAttribute("align", "right"); break;
			case scriptalignCenter: WriteStringAttribute("align", "center"); break;
			case scriptalignLeft:   WriteStringAttribute("align", "left"); break;
			}

			WriteBoolAttribute("base", bBase);
			WriteBoolAttribute("sup", bSup);
			WriteBoolAttribute("sub", bSub);
			WriteBoolAttribute("inline", bInline);

			WriteNodeEnd(false);
		}		
		virtual void EndScript()
		{
			PopCommand();

			WriteNodeEnd("script");
		}


		virtual void BeginBar(MBARTYPE eType, bool bTop)
		{
			PushCommand(commandBar);
			WriteNodeBegin("bar", true);
			
			switch(eType)
			{
			case bartypeLine:        WriteStringAttribute("type", "line"); break;
			case bartypeDoubleLine:  WriteStringAttribute("type", "doubleline"); break;
			case bartypeArrowLeft:   WriteStringAttribute("type", "arrow-left"); break;
			case bartypeArrowRight:  WriteStringAttribute("type", "arrow-right"); break;
			case bartypeArrowDouble: WriteStringAttribute("type", "arrow-double"); break;
			}

			WriteBoolAttribute("top", bTop);

			WriteNodeEnd(false);
		}
		virtual void EndBar  ()
		{
			PopCommand();
			WriteNodeEnd("bar");
		}
		virtual void BeginArrow(MARROWTYPE eType, bool bTop)
		{
			PushCommand(commandArrow);
			WriteNodeBegin("arrow", true);
			switch(eType)
			{
			case arrowtypeLeft:   WriteStringAttribute("type", "left"); break;
			case arrowtypeRight:  WriteStringAttribute("type", "right"); break;
			case arrowtypeDouble: WriteStringAttribute("type", "double"); break;
			}

			WriteBoolAttribute("top", bTop);
			WriteNodeEnd(false);
		}
		virtual void EndArrow()
		{
			PopCommand();
			WriteNodeEnd("arrow");
		}
		virtual void BeginIntegral(MINTEGRALTYPE eType)
		{
			PushCommand(commandIntegral);

			WriteNodeBegin("integral", true);

			switch(eType)
			{
			case integraltypeSingle:             WriteStringAttribute("type", "Single"); break;
			case integraltypeSingleRSub:         WriteStringAttribute("type", "SingleRSub"); break;
			case integraltypeSingleRSubSup:      WriteStringAttribute("type", "SingleRSubSup"); break;
			case integraltypeSingleOriented:     WriteStringAttribute("type", "SingleOriented"); break;
			case integraltypeSingleOrientedRSub: WriteStringAttribute("type", "SingleOrientedRSub"); break;

			case integraltypeDouble:             WriteStringAttribute("type", "Double"); break;
			case integraltypeDoubleRSub:         WriteStringAttribute("type", "DoubleRSub"); break;
			case integraltypeDoubleOriented:     WriteStringAttribute("type", "DoubleOriented"); break;
			case integraltypeDoubleOrientedRSub: WriteStringAttribute("type", "DoubleOrientedRSub"); break;

			case integraltypeTriple:             WriteStringAttribute("type", "Triple"); break;
			case integraltypeTripleRSub:         WriteStringAttribute("type", "TripleRSub"); break;
			case integraltypeTripleOriented:     WriteStringAttribute("type", "TripleOriented"); break;
			case integraltypeTripleOrientedRSub: WriteStringAttribute("type", "TripleOrientedRSub"); break;

			case integraltypeSingleCSubSup:      WriteStringAttribute("type", "SingleCSubSup"); break;
			case integraltypeSingleCSub:         WriteStringAttribute("type", "SingleCSub"); break;
			case integraltypeSingleOrientedCSub: WriteStringAttribute("type", "SingleOrientedCSub"); break;

			case integraltypeDoubleOrientedCSub: WriteStringAttribute("type", "DoubleOrientedCSub"); break;
			case integraltypeDoubleCSub:         WriteStringAttribute("type", "DoubleCSub"); break;

			case integraltypeTripleOrientedCSub: WriteStringAttribute("type", "TripleOrientedCSub"); break;
			case integraltypeTripleCSub:         WriteStringAttribute("type", "TripleCSub"); break;
			}

			WriteNodeEnd(false);
		}
		virtual void EndIntegral()
		{
			PopCommand();
			WriteNodeEnd("integral");
		}
		virtual void BeginVerticalBrace(bool bTop)
		{
			PushCommand(commandVerticalBrace);
			WriteNodeBegin("verticalbrace", true);
			WriteBoolAttribute("top", bTop);
			WriteNodeEnd(false);
		}
		virtual void EndVerticalBrace()
		{
			PopCommand();
			WriteNodeEnd("verticalbrace");
		}
		virtual void BeingNArray(MNARRAYTYPE eType)
		{
			CNArrayCommand* pCommand = (CNArrayCommand*)PushCommand(commandNArray);
			pCommand->SetType(eType);
			
			switch(eType)
			{		
			case narySumCSub:      WriteNodeBegin("sum", true); WriteStringAttribute("type", "CSub"); break;
			case narySumCSubSup:   WriteNodeBegin("sum", true); WriteStringAttribute("type", "CSubSup"); break;
			case narySum:          WriteNodeBegin("sum", true); WriteStringAttribute("type", "regular"); break;
			case narySumRSub:      WriteNodeBegin("sum", true); WriteStringAttribute("type", "RSub"); break;
			case narySumRSubSup:   WriteNodeBegin("sum", true); WriteStringAttribute("type", "RSubSup"); break;

			case naryProdCSub:     WriteNodeBegin("prod", true); WriteStringAttribute("type", "CSub"); break;
			case naryProdCSubSup:  WriteNodeBegin("prod", true); WriteStringAttribute("type", "CSubSup"); break;
			case naryProd:         WriteNodeBegin("prod", true); WriteStringAttribute("type", "regular"); break;
			case naryProdRSub:     WriteNodeBegin("prod", true); WriteStringAttribute("type", "RSub"); break;
			case naryProdRSubSup:  WriteNodeBegin("prod", true); WriteStringAttribute("type", "RSubSup"); break;

			case naryCoProdCSub:     WriteNodeBegin("coprod", true); WriteStringAttribute("type", "CSub"); break;
			case naryCoProdCSubSup:  WriteNodeBegin("coprod", true); WriteStringAttribute("type", "CSubSup"); break;
			case naryCoProd:         WriteNodeBegin("coprod", true); WriteStringAttribute("type", "regular"); break;
			case naryCoProdRSub:     WriteNodeBegin("coprod", true); WriteStringAttribute("type", "RSub"); break;
			case naryCoProdRSubSup:  WriteNodeBegin("coprod", true); WriteStringAttribute("type", "RSubSup"); break;

			case naryUnionCSub:     WriteNodeBegin("union", true); WriteStringAttribute("type", "CSub"); break;
			case naryUnionCSubSup:  WriteNodeBegin("union", true); WriteStringAttribute("type", "CSubSup"); break;
			case naryUnion:         WriteNodeBegin("union", true); WriteStringAttribute("type", "regular"); break;
			case naryUnionRSub:     WriteNodeBegin("union", true); WriteStringAttribute("type", "RSub"); break;
			case naryUnionRSubSup:  WriteNodeBegin("union", true); WriteStringAttribute("type", "RSubSup"); break;

			case naryIntersectCSub:     WriteNodeBegin("intersect", true); WriteStringAttribute("type", "CSub"); break;
			case naryIntersectCSubSup:  WriteNodeBegin("intersect", true); WriteStringAttribute("type", "CSubSup"); break;
			case naryIntersect:         WriteNodeBegin("intersect", true); WriteStringAttribute("type", "regular"); break;
			case naryIntersectRSub:     WriteNodeBegin("intersect", true); WriteStringAttribute("type", "RSub"); break;
			case naryIntersectRSubSup:  WriteNodeBegin("intersect", true); WriteStringAttribute("type", "RSubSup"); break;
			}
		
			WriteNodeEnd(false);
		}
		virtual void EndNArray()
		{
			CNArrayCommand* pCommand = (CNArrayCommand*)TopCommand();
			MNARRAYTYPE eType = pCommand->GetType();

			PopCommand();

			switch(eType)
			{		
			case narySumCSub:
			case narySumCSubSup:
			case narySum:
			case narySumRSub:
			case narySumRSubSup:
				WriteNodeEnd("sum"); 
				break;
			case naryProdCSub:
			case naryProdCSubSup:
			case naryProd:
			case naryProdRSub: 
			case naryProdRSubSup:
				WriteNodeEnd("prod");
				break;
			case naryCoProdCSub:   
			case naryCoProdCSubSup:
			case naryCoProd:       
			case naryCoProdRSub:   
			case naryCoProdRSubSup:
				WriteNodeEnd("coprod");
				break;
			case naryUnionCSub:   
			case naryUnionCSubSup:
			case naryUnion:        
			case naryUnionRSub:    
			case naryUnionRSubSup: 
				WriteNodeEnd("union");
				break;
			case naryIntersectCSub:
			case naryIntersectCSubSup:
			case naryIntersect:       
			case naryIntersectRSub:   
			case naryIntersectRSubSup:
				WriteNodeEnd("intersect");
				break;
			}
		}
		virtual void BeginLongDivision(MLONGDIVISION eType)
		{
			PushCommand(commandLongDivision);
			WriteNodeBegin("longdivision");
		}
		virtual void EndLongDivision()
		{
			PopCommand();
			WriteNodeEnd("longdivision");
		}
		virtual void BeginAngleBracketsWithSeparator(MANGLEBRACKETSWITHSEPARATORTYPE eType)
		{
			CBracketsWithSeparatorCommand* pCommand = (CBracketsWithSeparatorCommand*)PushCommand(commandBracketsSep);
			pCommand->SetType(eType);

			WriteNodeBegin("brackets-separator");
		}
		virtual void EndAngleBracketsWithSeparator()
		{
			PopCommand();
			WriteNodeEnd("brackets-separator");
		}

	private:
		enum ECommandType
		{
			commandMatrix        = 0x00,
			commandBrackets      = 0x01,
			commandRoot          = 0x02,
			commandFraction      = 0x03,
			commandScript        = 0x04,
			commandBar           = 0x05,
			commandArrow         = 0x06,
			commandIntegral      = 0x07,
			commandVerticalBrace = 0x08,
			commandNArray        = 0x09,
			commandLongDivision  = 0x0a,
			commandBracketsSep   = 0x0b
		};

		class CBaseCommand
		{
		public:

			CBaseCommand() : nBlockNum(-1)
			{
			}

			virtual ~CBaseCommand() 
			{
			}

			void Next() 
			{
				nBlockNum++;
			}

			int GetBlockNum()
			{
				return nBlockNum;
			}

			virtual ECommandType GetCommand() = 0;
			virtual void WriteBeginBlock(CStringOutput* pWriter) = 0;
			virtual void WriteEndBlock(CStringOutput* pWriter)   = 0;

		protected:
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

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				int nCurRow = nBlockNum / nCols;
				int nCurCol = nBlockNum % nCols;
				
				if (0 == nCurCol)
					pWriter->WriteNodeBegin("row");

				pWriter->WriteNodeBegin("cell");
			}

			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				int nCurRow = nBlockNum / nCols;
				int nCurCol = nBlockNum % nCols;

				pWriter->WriteNodeEnd("cell");

				if (nCols - 1 == nCurCol)
					pWriter->WriteNodeEnd("row");
			}
		private:
			int nRows;
			int nCols;
		};
		class CBracketsCommand : public CBaseCommand
		{
		public:
			CBracketsCommand() {}
			virtual ~CBracketsCommand() {}
			virtual ECommandType GetCommand(){return commandBrackets;}

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				pWriter->WriteNodeBegin("e");
			}

			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				pWriter->WriteNodeEnd("e");
			}
		};
		class CRootCommand : public CBaseCommand
		{
		public:
			CRootCommand() {}
			virtual ~CRootCommand() {}
			virtual ECommandType GetCommand(){return commandRoot;}
			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeBegin("base", false);
				else if (1 == nBlockNum)
					pWriter->WriteNodeBegin("degree", false);	
			}
			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeEnd("base");
				else if (1 == nBlockNum)
					pWriter->WriteNodeEnd("degree");	
			}
		};
		class CFractionCommand : public CBaseCommand
		{
		public:
			CFractionCommand() {}
			virtual ~CFractionCommand() {}
			virtual ECommandType GetCommand(){return commandFraction;}
			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeBegin("numerator", false);		
				else if (1 == nBlockNum)
					pWriter->WriteNodeBegin("denominator", false);		
			}
			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeEnd("numerator");
				else if (1 == nBlockNum)
					pWriter->WriteNodeEnd("denominator");
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

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				Write(pWriter, false);
			}

		private:

			void Write(CStringOutput* pWriter, bool bBegin)
			{
				if (bInline)
				{
					if (0 == nBlockNum)
					{
						if (bBase)
							WriteNode("base", pWriter, bBegin);
						else if (bSub)
							WriteNode("sub", pWriter, bBegin);
						else if (bSup)
							WriteNode("sup", pWriter, bBegin);
					}
					else if (1 == nBlockNum)
					{
						if (bBase)
						{
							if (bSub)
								WriteNode("sub", pWriter, bBegin);
							else if (bSup)
								WriteNode("sup", pWriter, bBegin);
						}
						else if (bSub && bSup)
						{
							WriteNode("sup", pWriter, bBegin);
						}
					}
					else if (2 == nBlockNum)
					{
						if (bBase && bSub && bSup)
							WriteNode("sup", pWriter, bBegin);
					}
				}
				else
				{
					if (0 == nBlockNum)
					{
						if (bSub)
							WriteNode("sub", pWriter, bBegin);
						else if (bSup)
							WriteNode("sup", pWriter, bBegin);
						else if (bBase)
							WriteNode("base", pWriter, bBegin);
					}
					else if (1 == nBlockNum)
					{
						if (bSub)
						{
							if (bSup)
								WriteNode("sup", pWriter, bBegin);
							else if (bBase)
								WriteNode("base", pWriter, bBegin);
						}
						else if (bSup && bBase)
						{
							WriteNode("base", pWriter, bBegin);
						}
					}
					else if (2 == nBlockNum)
					{
						if (bBase && bSub && bSup)
							WriteNode("base", pWriter, bBegin);
					}
				}
			}

			void WriteNode(const char* sNodeName, CStringOutput* pWriter, bool bNodeBegin)
			{
				if (bNodeBegin)
					pWriter->WriteNodeBegin(sNodeName);
				else
					pWriter->WriteNodeEnd(sNodeName);
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

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				pWriter->WriteNodeBegin("e");
			}

			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				pWriter->WriteNodeEnd("e");
			}
		};
		class CArrowCommand : public CBaseCommand
		{
		public:
			CArrowCommand() {}
			virtual ~CArrowCommand() {}
			virtual ECommandType GetCommand(){return commandArrow;}

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				pWriter->WriteNodeBegin("e");
			}

			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				pWriter->WriteNodeEnd("e");
			}
		};
		class CIntegralCommand : public CBaseCommand
		{
		public:
			CIntegralCommand() {}
			virtual ~CIntegralCommand() {}
			virtual ECommandType GetCommand(){return commandIntegral;}

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				Write(pWriter, false);
			}

		private:

			void Write(CStringOutput* pWriter, bool bBeginNode)
			{
				if (0 == nBlockNum)
				{
					WriteNode("base", pWriter, bBeginNode);
				}
				else if (1 == nBlockNum)
				{
					WriteNode("sub", pWriter, bBeginNode);
				}
				else if (2 == nBlockNum)
				{
					WriteNode("sup", pWriter, bBeginNode);
				}
			}

			void WriteNode(const char* sNodeName, CStringOutput* pWriter, bool bNodeBegin)
			{
				if (bNodeBegin)
					pWriter->WriteNodeBegin(sNodeName);
				else
					pWriter->WriteNodeEnd(sNodeName);
			}
		};
		class CVerticalBraceCommand : public CBaseCommand
		{
		public:
			CVerticalBraceCommand() {}
			virtual ~CVerticalBraceCommand() {}
			virtual ECommandType GetCommand(){return commandVerticalBrace;}

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeBegin("base");
				else
					pWriter->WriteNodeBegin("brace-base");
			}

			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeEnd("base");
				else
					pWriter->WriteNodeEnd("brace-base");
			}
		};
		class CNArrayCommand : public CBaseCommand
		{
		public:
			CNArrayCommand() {}
			virtual ~CNArrayCommand() {}
			virtual ECommandType GetCommand(){return commandNArray;}

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(CStringOutput* pWriter)
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

			void Write(CStringOutput* pWriter, bool bBeginNode)
			{
				if (0 == nBlockNum)
				{
					WriteNode("base", pWriter, bBeginNode);
				}
				else if (1 == nBlockNum)
				{
					WriteNode("sub", pWriter, bBeginNode);
				}
				else if (2 == nBlockNum)
				{
					WriteNode("sup", pWriter, bBeginNode);
				}
			}

			void WriteNode(const char* sNodeName, CStringOutput* pWriter, bool bNodeBegin)
			{
				if (bNodeBegin)
					pWriter->WriteNodeBegin(sNodeName);
				else
					pWriter->WriteNodeEnd(sNodeName);
			}

		private:

			MNARRAYTYPE eType;
		};

		class CLongDivisionCommand : public CBaseCommand
		{
		public:
			CLongDivisionCommand() {}
			virtual ~CLongDivisionCommand() {}
			virtual ECommandType GetCommand(){return commandLongDivision;}

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeBegin("base");
				else
					pWriter->WriteNodeBegin("result");
			}

			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				if (0 == nBlockNum)
					pWriter->WriteNodeEnd("base");
				else
					pWriter->WriteNodeEnd("result");
			}
		};
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

			virtual void WriteBeginBlock(CStringOutput* pWriter)
			{									
				Write(pWriter, true);
			}
			virtual void WriteEndBlock(CStringOutput* pWriter)
			{
				Write(pWriter, false);
			}

		private:

			void Write(CStringOutput* pWriter, bool bBeginNode)
			{
				if (0 == nBlockNum)
				{
					if (angbrwithsepBoth == eType || angbrwithsepLeft == eType)
						WriteNode("left", pWriter, bBeginNode);
					else
						WriteNode("right", pWriter, bBeginNode);
				}
				else if (1 == nBlockNum)
				{
					WriteNode("right", pWriter, bBeginNode);
				}
			}

			void WriteNode(const char* sNodeName, CStringOutput* pWriter, bool bNodeBegin)
			{
				if (bNodeBegin)
					pWriter->WriteNodeBegin(sNodeName);
				else
					pWriter->WriteNodeEnd(sNodeName);
			}
		private:

			MANGLEBRACKETSWITHSEPARATORTYPE eType;

		};
		CBaseCommand* PushCommand(ECommandType eType)
		{
			CBaseCommand* pCommand = NULL;
			switch(eType)
			{
			case commandMatrix:        pCommand = new CMatrixCommand(); break;
			case commandBrackets:      pCommand = new CBracketsCommand(); break;
			case commandRoot:          pCommand = new CRootCommand(); break;
			case commandFraction:      pCommand = new CFractionCommand(); break;
			case commandScript:        pCommand = new CScriptCommand(); break;
			case commandBar:           pCommand = new CBarCommand(); break;
			case commandArrow:         pCommand = new CArrowCommand(); break;
			case commandIntegral:      pCommand = new CIntegralCommand(); break;
			case commandVerticalBrace: pCommand = new CVerticalBraceCommand(); break;
			case commandNArray:        pCommand = new CNArrayCommand(); break;
			case commandLongDivision:  pCommand = new CLongDivisionCommand(); break;
			case commandBracketsSep:   pCommand = new CBracketsWithSeparatorCommand(); break;
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
		void WriteNodeBegin(const char* sName, bool bAttributes = false)
		{
			rRet += "<";
			rRet += sName;

			if (!bAttributes)
				rRet += ">";
		}
		void WriteNodeEnd(bool bEmpty = false)
		{
			if (bEmpty)
				rRet += "/>";
			else
				rRet += ">";
		}
		void WriteNodeEnd(const char* sName)
		{
			rRet += "</";
			rRet += sName;
			rRet += ">";
		}
		void WriteBoolAttribute(const char* sName, bool bValue)
		{
			rRet += " ";
			rRet += sName;
			rRet += "=\"";
			if (bValue)
				rRet += "true";
			else
				rRet += "false";
			rRet += "\"";
		}

		void WriteStringAttribute(const char* sName, std::string sValue)
		{
			rRet += " ";
			rRet += sName;
			rRet += "=\"";
			rRet += sValue;
			rRet += "\"";
		}
        void WriteStringAttribute(const char* sName, unsigned short uChar)
		{
			rRet += " ";
			rRet += sName;
			rRet += "=\"";
			rRet += uChar;
			rRet += "\"";
		}
		void WriteIntAttribute(const char* sName, int nValue)
		{
			rRet += " ";
			rRet += sName;
			rRet += "=\"";
			rRet += String::CreateFromInt32(nValue);
			rRet += "\"";
		}
	private:

		String rRet;

		bool bEmbel;
		std::stack<CBaseCommand*> m_aCommandStack;
	};

}

