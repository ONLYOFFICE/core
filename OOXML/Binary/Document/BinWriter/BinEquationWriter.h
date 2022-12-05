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

#include "BinReaderWriterDefines.h"

#include "../../MathEquation/OutputDev.h"
#include "../../../Common/SimpleTypes_Shared.h"
#include "../../../Common/SimpleTypes_Word.h"

#include <stack>
#include <vector>

namespace NSBinPptxRW
{
    class CBinaryFileWriter;
}

namespace MathEquation
{
	class EquationRun
	{
		public:
            std::wstring str;
			TMathFont* pFont;
			LONG nTextSize;
			bool bAccent;
			MEMBELTYPE eType;
			bool bNormal;

			EquationRun();

			void AddChar(std::wstring sChar, TMathFont* pNewFont, LONG lSize);
			bool CompareFont(TMathFont* pNewFont);
			void SetAccent(bool bAcc, MEMBELTYPE type);
			TMathFont* GetFont();
			LONG GetSize();

	};

	class RunManager
	{
		private:
			TMathFont* oCurFont;
			std::vector<EquationRun> arrRun;			

		public:
			RunManager();

			void SetAlligment(bool bPos);
			bool GetAlligment();

			void Add(EquationRun oElem);
			bool IsEmpty();

			LONG GetSize();
			bool GetElem (LONG lPos, EquationRun &oRun);

			void Clear();
			void RemoveElem (LONG lPos);
			void Delete(LONG lStart, LONG lCount);

			void AddAccent (MEMBELTYPE eType);

			bool bAlligment;
	};

	class BinaryEquationWriter : public IOutputDev
	{
		public:
            NSBinPptxRW::CBinaryFileWriter *m_pStream;
			std::stack<int> m_aEquationStack;
			std::stack<int> m_aNArrayStack;
			std::stack<int> m_aNArrayCutStack;
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
			std::stack<int> m_aDelimiterCounter;
			RunManager oRManager;
			
			LONG nTextSize;
			LONG nCtrlSize;
			BYTE nHAlignPile;

            BinaryEquationWriter(NSBinPptxRW::CBinaryFileWriter *pStream);

			void WriteRPR(TMathFont* pFont, LONG nSize, bool bIsOpen);
			void WriteMRPR(EquationRun oRun);
			void WriteRunContent(EquationRun oRun, bool bIsOpen);

			void CutRun(int nCount);
			void WriteRun(bool bLast = false);

			int WriteItemStart(BYTE type);
			void WriteItemEnd(int nStart);

			int WriteItemWithLengthStart();
			void WriteItemWithLengthEnd(int nStart);

			virtual void BeginEquation();
			virtual void EndEquation();

			virtual void BeginBlock();
			virtual void EndBlock();

			virtual void SetSize(uint16_t nSize);
			virtual void BeginChar(unsigned short uChar, unsigned char nTypeFace, bool bSpecialSymbol);

			virtual void AddCharEmbel(MEMBELTYPE eType);
			virtual void EndChar();

			virtual void BeginMatrix(unsigned char nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, unsigned char nRows, unsigned char nCols, unsigned char* pVerBorders, unsigned char* pHorBorders);
			virtual void EndMatrix();

			virtual void StartPile(unsigned char nHAlign, unsigned char nVAlign);
			virtual void EndPile();

			virtual void BeginBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose);
			virtual void EndBrackets  (MBRACKETSTYPE eType, bool bOpen, bool bClose);

			virtual void BeginRoot(bool bDegree);
			virtual void EndRoot();

			virtual void BeginFraction(MFRACTIONTYPES eType, bool bInline);
			virtual void EndFraction();

			virtual void BeginScript(MSCRIPTALIGN eAlign, bool bBase = false, bool bSup = false, bool bSub = false, bool bInline = true);
			virtual void EndScript();

			virtual void BeginBar(MBARTYPE eType, bool bTop);
			virtual void EndBar();

			virtual void BeginArrow(MARROWTYPE eType, bool bTop);
			virtual void EndArrow();

			virtual void BeginIntegral(MINTEGRALTYPE eType);
			virtual void EndIntegral();

			virtual void BeginVerticalBrace(bool bTop);
			virtual void EndVerticalBrace();

			virtual void BeingNArray(MNARRAYTYPE eType);
			virtual void EndNArray();

			virtual void BeginLongDivision(MLONGDIVISION eType);
			virtual void EndLongDivision();

			virtual void BeginAngleBracketsWithSeparator(MANGLEBRACKETSWITHSEPARATORTYPE eType);
			virtual void EndAngleBracketsWithSeparator();

			void AddFont(unsigned char nTypeFace, std::string sName, bool bBold, bool bItalic);

			void WriteItemVal(BYTE name, BYTE val);
			void WriteItemValLong(BYTE name, LONG val);
			void WriteItemVal(BYTE name, bool val);
			void WriteItemValStr(BYTE name, std::wstring val);

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
                commandLongDivision		 = 0x0a,
                commandBracketsSep		 = 0x0b,
                commandVerticalBraceLim	 = 0x0c,
                commandEqArray			 = 0x0d
            };

            class CBaseCommand
            {
            public:
                CBaseCommand();

                virtual ~CBaseCommand();

                void Next();
                bool IsOpenNode();

                int GetCount();

                void SetPile(bool bSetPile, BYTE nHPile);
                bool GetPile();

                int GetBlockNum();

                void WriteBeginNode(BinaryEquationWriter* pWriter, BYTE elem);
                void WriteBeginNode(BinaryEquationWriter* pWriter);
                void WriteEndNode(BinaryEquationWriter* pWriter);

                virtual ECommandType GetCommand() = 0;
                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter) = 0;
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter)   = 0;

            protected:
                int nCount;
                int nRows;
                std::stack<int> m_aBaseStack;
                bool bOpenNode;
                bool bPile;
                bool bEqArrayStart;
                int nBlockNum;
                BYTE nHAlignPile;
            };

            class CMatrixCommand : public CBaseCommand
            {
            public:
                CMatrixCommand();
                virtual ~CMatrixCommand();
                virtual ECommandType GetCommand();

                void SetProps(int nRows, int nCols);

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

            private:
                int nRowPos;
                int nColPos;
                int nRows;
                int nCols;
            };
            class CBracketsCommand : public CBaseCommand
            {
            public:
                CBracketsCommand();
                virtual ~CBracketsCommand();
                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                int nElemPos;
            };
            class CRootCommand : public CBaseCommand
            {
            public:
                CRootCommand();
                virtual ~CRootCommand();
                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                int nElemPos;
                int nDegPos;
            };
            class CFractionCommand : public CBaseCommand
            {
            public:
                CFractionCommand();
                virtual ~CFractionCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);
            };
            class CScriptCommand : public CBaseCommand
            {
            public:
                CScriptCommand();
                virtual ~CScriptCommand();

                virtual ECommandType GetCommand();
                void SetProps(bool bInline, bool bBase, bool bSup, bool bSub);

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

            private:
                void Write(BinaryEquationWriter* pWriter, bool bBegin);

            private:
                bool bBase;
                bool bSup;
                bool bSub;
                bool bInline;
            };
            class CBarCommand : public CBaseCommand
            {
            public:
                CBarCommand();
                virtual ~CBarCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                int nElemPos;
            };
            class CArrowCommand : public CBaseCommand
            {
            public:
                CArrowCommand();
                virtual ~CArrowCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

            private:
                int nElemPos;
            };
            class CEqArrayCommand : public CBaseCommand
            {
            public:
                CEqArrayCommand();
                virtual ~CEqArrayCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

            private:
                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                int nElemPos;
            };
            class CIntegralCommand : public CBaseCommand
            {
            public:
                CIntegralCommand();
                virtual ~CIntegralCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

            private:
                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);
            };
            class CVerticalBraceCommand : public CBaseCommand
            {
            public:
                CVerticalBraceCommand();
                virtual ~CVerticalBraceCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                int nBasePos;
                int nBraceBasePos;
            };
            class CVerticalBraceLimCommand : public CBaseCommand
            {
            public:
                CVerticalBraceLimCommand();
                virtual ~CVerticalBraceLimCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                int nBasePos;
                int nBraceBasePos;
            };
            class CNArrayCommand : public CBaseCommand
            {
            public:
                CNArrayCommand();
                virtual ~CNArrayCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void SetType(MNARRAYTYPE eType);

                MNARRAYTYPE GetType();

            private:
                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                MNARRAYTYPE eType;
            };

            class CLongDivisionCommand : public CBaseCommand
            {
            public:
                CLongDivisionCommand();
                virtual ~CLongDivisionCommand();

                virtual ECommandType GetCommand();

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);
            };

            class CBracketsWithSeparatorCommand : public CBaseCommand
            {
            public:
                CBracketsWithSeparatorCommand();
                virtual ~CBracketsWithSeparatorCommand();

                virtual ECommandType GetCommand();
                void SetType(MANGLEBRACKETSWITHSEPARATORTYPE eType);

                virtual void WriteBeginBlock(BinaryEquationWriter* pWriter);
                virtual void WriteEndBlock(BinaryEquationWriter* pWriter);

            private:
                void Write(BinaryEquationWriter* pWriter, bool bBeginNode);

            private:
                MANGLEBRACKETSWITHSEPARATORTYPE eType;
            };

            CBaseCommand* PushCommand(ECommandType eType);
            void PopCommand();
            CBaseCommand* TopCommand();

		private:
            std::wstring rRet;

			bool bEmbel;
			std::stack<CBaseCommand*> m_aCommandStack;
	};
} 
