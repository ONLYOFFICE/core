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
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "MemoryUtils.h"
#include "GlobalParams.h"
#include "CharTypes.h"
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Stream.h"
#include "Lexer.h"
#include "Parser.h"
#include "GFont.h"
#include "GState.h"
#include "RendererOutputDev.h"
#include "OutputDevice.h"
#include "Page.h"
#include "Annot.h"
#include "Graphics.h"

// Под виндой переопределяется
#ifdef GetObject
#undef GetObject
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//-------------------------------------------------------------------------------------------------------------------------------
// Константы
//-------------------------------------------------------------------------------------------------------------------------------

#define functionMaxDepth 6

#define functionColorDelta (DoubleToColor(1 / 256.0))

#define axialMaxSplits 256

#define axialColorDelta (DoubleToColor(1 / 256.0))

#define radialMaxSplits 256

#define radialColorDelta (DoubleToColor(1 / 256.0))

#define gouraudMaxDepth 1

#define gouraudColorDelta (DoubleToColor(1 / 256.0))

#define patchMaxDepth 6

#define patchColorDelta (DoubleToColor(1 / 256.0))

//-------------------------------------------------------------------------------------------------------------------------------
// Таблица графических операторов
//-------------------------------------------------------------------------------------------------------------------------------

#ifdef WIN32 // this works around a bug in the VC7 compiler
#  pragma optimize("",off)
#endif

namespace PdfReader
{
	Operator Graphics::OperatorsTable[] =
	{
		{ "\"", 3, { argNum, argNum, argString }, &Graphics::OperatorMoveSetShowText },
		{ "'", 1, { argString }, &Graphics::OperatorMoveShowText },
		{ "B", 0, { argNone }, &Graphics::OperatorFillStroke },
		{ "B*", 0, { argNone }, &Graphics::OperatorEOFillStroke },
		{ "BDC", 2, { argName, argProps }, &Graphics::OperatorBeginMarkedContent },
		{ "BI", 0, { argNone }, &Graphics::OperatorBeginImage },
		{ "BMC", 1, { argName }, &Graphics::OperatorBeginMarkedContent },
		{ "BT", 0, { argNone }, &Graphics::OperatorBeginText },
		{ "BX", 0, { argNone }, &Graphics::OperatorBeginIgnoreUndef },
		{ "CS", 1, { argName }, &Graphics::OperatorSetStrokeColorSpace },
		{ "DP", 2, { argName, argProps }, &Graphics::OperatorMarkPoint },
		{ "Do", 1, { argName }, &Graphics::OperatorXObject },
		{ "EI", 0, { argNone }, &Graphics::OperatorEndImage },
		{ "EMC", 0, { argNone }, &Graphics::OperatorEndMarkedContent },
		{ "ET", 0, { argNone }, &Graphics::OperatorEndText },
		{ "EX", 0, { argNone }, &Graphics::OperatorEndIgnoreUndef },
		{ "F", 0, { argNone }, &Graphics::OperatorFill },
		{ "G", 1, { argNum }, &Graphics::OperatorSetStrokeGray },
		{ "ID", 0, { argNone }, &Graphics::OperatorImageData },
		{ "J", 1, { argInt }, &Graphics::OperatorSetLineCap },
		{ "K", 4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorSetStrokeCMYKColor },
		{ "M", 1, { argNum }, &Graphics::OperatorSetMiterLimit },
		{ "MP", 1, { argName }, &Graphics::OperatorMarkPoint },
		{ "Q", 0, { argNone }, &Graphics::OperatorRestore },
		{ "RG", 3, { argNum, argNum, argNum }, &Graphics::OperatorSetStrokeRGBColor },
		{ "S", 0, { argNone }, &Graphics::OperatorStroke },
		{ "SC", -4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorSetStrokeColor },
		{ "SCN", -33, { argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN }, &Graphics::OperatorSetStrokeColorN },
		{ "T*", 0, { argNone }, &Graphics::OperatorTextNextLine },
		{ "TD", 2, { argNum, argNum }, &Graphics::OperatorTextMoveSet },
		{ "TJ", 1, { argArray }, &Graphics::OperatorShowSpaceText },
		{ "TL", 1, { argNum }, &Graphics::OperatorSetTextLeading },
		{ "Tc", 1, { argNum }, &Graphics::OperatorSetCharSpacing },
		{ "Td", 2, { argNum, argNum }, &Graphics::OperatorTextMove },
		{ "Tf", 2, { argName, argNum }, &Graphics::OperatorSetFont },
		{ "Tj", 1, { argString }, &Graphics::OperatorShowText },
		{ "Tm", 6, { argNum, argNum, argNum, argNum, argNum, argNum }, &Graphics::OperatorSetTextMatrix },
		{ "Tr", 1, { argInt }, &Graphics::OperatorSetTextRender },
		{ "Ts", 1, { argNum }, &Graphics::OperatorSetTextRise },
		{ "Tw", 1, { argNum }, &Graphics::OperatorSetWordSpacing },
		{ "Tz", 1, { argNum }, &Graphics::OperatorSetHorizScaling },
		{ "W", 0, { argNone }, &Graphics::OperatorClip },
		{ "W*", 0, { argNone }, &Graphics::OperatorEOClip },
		{ "b", 0, { argNone }, &Graphics::OperatorCloseFillStroke },
		{ "b*", 0, { argNone }, &Graphics::OperatorCloseEOFillStroke },
		{ "c", 6, { argNum, argNum, argNum, argNum, argNum, argNum }, &Graphics::OperatorCurveTo },
		{ "cm", 6, { argNum, argNum, argNum, argNum, argNum, argNum }, &Graphics::OperatorConcat },
		{ "cs", 1, { argName }, &Graphics::OperatorSetFillColorSpace },
		{ "d", 2, { argArray, argNum }, &Graphics::OperatorSetDash },
		{ "d0", 2, { argNum, argNum }, &Graphics::OperatorSetCharWidth },
		{ "d1", 6, { argNum, argNum, argNum, argNum, argNum, argNum }, &Graphics::OperatorSetCacheDevice },
		{ "f", 0, { argNone }, &Graphics::OperatorFill },
		{ "f*", 0, { argNone }, &Graphics::OperatorEOFill },
		{ "g", 1, { argNum }, &Graphics::OperatorSetFillGray },
		{ "gs", 1, { argName }, &Graphics::OperatorSetExtGState },
		{ "h", 0, { argNone }, &Graphics::OperatorClosePath },
		{ "i", 1, { argNum }, &Graphics::OperatorSetFlat },
		{ "j", 1, { argInt }, &Graphics::OperatorSetLineJoin },
		{ "k", 4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorSetFillCMYKColor },
		{ "l", 2, { argNum, argNum }, &Graphics::OperatorLineTo },
		{ "m", 2, { argNum, argNum }, &Graphics::OperatorMoveTo },
		{ "n", 0, { argNone }, &Graphics::OperatorEndPath },
		{ "q", 0, { argNone }, &Graphics::OperatorSave },
		{ "re", 4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorRectangle },
		{ "rg", 3, { argNum, argNum, argNum }, &Graphics::OperatorSetFillRGBColor },
		{ "ri", 1, { argName }, &Graphics::OperatorSetRenderingIntent },
		{ "s", 0, { argNone }, &Graphics::OperatorCloseStroke },
		{ "sc", -4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorSetFillColor },
		{ "scn", -33, { argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN, argSCN, argSCN, argSCN,
		argSCN, argSCN, argSCN }, &Graphics::OperatorSetFillColorN },
		{ "sh", 1, { argName }, &Graphics::OperatorShadingFill },
		{ "v", 4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorCurveTo1 },
		{ "w", 1, { argNum }, &Graphics::OperatorSetLineWidth },
		{ "y", 4, { argNum, argNum, argNum, argNum }, &Graphics::OperatorCurveTo2 },
	};

#ifdef WIN32 // this works around a bug in the VC7 compiler
#  pragma optimize("",on)
#endif

#define OperatorsCount (sizeof(OperatorsTable) / sizeof(Operator))

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrResources
	//-------------------------------------------------------------------------------------------------------------------------------

	GrResources::GrResources(XRef *pXref, Dict *pResourcesDict, GrResources *pNext, GlobalParams *pGlobalParams)
	{
		m_pGlobalParams = pGlobalParams;

		if (pResourcesDict)
		{

			// build font dictionary
			m_pFonts = NULL;
			Object oFont;
			pResourcesDict->SearchAndCopy("Font", &oFont);
			if (oFont.IsRef())
			{
				Object oTemp;
				oFont.Fetch(pXref, &oTemp);
				if (oTemp.IsDict())
				{
					Ref oRef = oFont.GetRef();
					m_pFonts = new GrFontDict(pXref, &oRef, oTemp.GetDict(), pGlobalParams);
				}
				oTemp.Free();
			}
			else if (oFont.IsDict())
			{
				m_pFonts = new GrFontDict(pXref, NULL, oFont.GetDict(), pGlobalParams);
			}
			oFont.Free();

			//XObject
			pResourcesDict->Search("XObject", &m_oXObjectDict);

			//ColorSpace
			pResourcesDict->Search("ColorSpace", &m_oColorSpaceDict);

			//Pattern
			pResourcesDict->Search("Pattern", &m_oPatternDict);

			//Shading
			pResourcesDict->Search("Shading", &m_oShadingDict);

			//ExtGState
			pResourcesDict->Search("ExtGState", &m_oExtGStateDict);
		}
		else
		{
			m_pFonts = NULL;
			m_oXObjectDict.InitNull();
			m_oColorSpaceDict.InitNull();
			m_oPatternDict.InitNull();
			m_oShadingDict.InitNull();
			m_oExtGStateDict.InitNull();
		}

		m_pNext = pNext;
	}

	GrResources::~GrResources()
	{
		if (m_pFonts)
		{
			delete m_pFonts;
		}
		m_oXObjectDict.Free();
		m_oColorSpaceDict.Free();
		m_oPatternDict.Free();
		m_oShadingDict.Free();
		m_oExtGStateDict.Free();
	}

	GrFont *GrResources::LookupFont(char *sName)
	{
		GrFont *pFont = NULL;
		GrResources *pResources = NULL;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_pFonts)
			{
				if ((pFont = pResources->m_pFonts->Search(sName)))
					return pFont;
			}
		}
		// TO DO: Error "Unknown font tag"
		return NULL;
	}

	bool GrResources::LookupXObject(char *sName, Object *pObject)
	{
		GrResources *pResources = NULL;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_oXObjectDict.IsDict())
			{
				if (!pResources->m_oXObjectDict.DictLookup(sName, pObject)->IsNull())
					return true;
				pObject->Free();
			}
		}
		// TO DO: Error "XObject is unknown"
		return false;
	}

	bool GrResources::LookupAndCopyXObject(char *sName, Object *pObject)
	{
		GrResources *pResources = NULL;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_oXObjectDict.IsDict())
			{
				if (!pResources->m_oXObjectDict.DictLookupAndCopy(sName, pObject)->IsNull())
					return true;
				pObject->Free();
			}
		}
		// TO DO: Error "XObject is unknown"
		return false;
	}

	void GrResources::LookupColorSpace(char *sName, Object *pObject)
	{
		GrResources *pResources = NULL;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_oColorSpaceDict.IsDict())
			{
				if (!pResources->m_oColorSpaceDict.DictLookup(sName, pObject)->IsNull())
				{
					return;
				}
				pObject->Free();
			}
		}
		pObject->InitNull();
	}

	GrPattern *GrResources::LookupPattern(char *sName)
	{
		GrResources *pResources = NULL;
		GrPattern *pPattern = NULL;
		Object oTemp;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_oPatternDict.IsDict())
			{
				if (!pResources->m_oPatternDict.DictLookup(sName, &oTemp)->IsNull())
				{
					pPattern = GrPattern::Parse(&oTemp);
					oTemp.Free();
					return pPattern;
				}
				oTemp.Free();
			}
		}
		// TO DO: Error "Unknown pattern"
		return NULL;
	}

	GrShading *GrResources::LookupShading(char *sName)
	{
		GrResources *pResources = NULL;
		GrShading *pShading = NULL;
		Object oTemp;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_oShadingDict.IsDict())
			{
				if (!pResources->m_oShadingDict.DictLookup(sName, &oTemp)->IsNull())
				{
					pShading = GrShading::Parse(&oTemp);
					oTemp.Free();
					return pShading;
				}
				oTemp.Free();
			}
		}
		// TO DO: Error "Unknown shading"
		return NULL;
	}

	bool GrResources::LookupExtGState(char *sName, Object *pObject)
	{
		GrResources *pResources = NULL;

		for (pResources = this; pResources; pResources = pResources->m_pNext)
		{
			if (pResources->m_oExtGStateDict.IsDict())
			{
				if (!pResources->m_oExtGStateDict.DictLookup(sName, pObject)->IsNull())
				{
					return true;
				}
				pObject->Free();
			}
		}
		// TO DO: Error "ExtGState is unknown"
		return false;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Graphics
	//-------------------------------------------------------------------------------------------------------------------------------

	Graphics::Graphics(GlobalParams *pGlobalParams, XRef *pXref, OutputDev *pOut, int nPageNumber, Dict *pResorcesDict, double dHorDPI, double dVerDPI, PDFRectangle *pBox, PDFRectangle *pCropBox, int nRotate, bool(*pAbortCheckCallBack)(void *pData), void *pAbortCheckData)
	{
#ifdef _DEBUG
		std::wstring wsFileName = L"D:\\Output\\PDF Dump\\DumpPDF_Page" + std::to_wstring(nPageNumber) + L".bin";
		m_pDumpFile = NSFile::CFileBinary::OpenFileNative(wsFileName, L"wb");
#endif

		m_pGlobalParams = pGlobalParams;

		m_pXref    = pXref;
		m_bSubPage = false;

		// Resource stack
		m_pResources = new GrResources(m_pXref, pResorcesDict, NULL, m_pGlobalParams);

		// Initialize
		m_pOut    = pOut;
		m_pGState = new GrState(dHorDPI, dVerDPI, pBox, nRotate, m_pOut->UpSideDown());
		m_bFontChanged = false;
		m_eClip = clipNone;
		m_nIgnoreUndef = 0;
		m_pOut->StartPage(nPageNumber, m_pGState);
		m_pOut->SetDefaultCTM(m_pGState->GetCTM());
		m_pOut->UpdateAll(m_pGState);

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrBaseMatrix[nIndex] = m_pGState->GetCTM()[nIndex];
		}
		m_nFormDepth = 0;
		m_pAbortCheckCallBack = pAbortCheckCallBack;
		m_pAbortCheckData     = pAbortCheckData;

		if (pCropBox)
		{
			m_pGState->MoveTo(pCropBox->m_dLeft, pCropBox->m_dBottom);
			m_pGState->LineTo(pCropBox->m_dRight, pCropBox->m_dBottom);
			m_pGState->LineTo(pCropBox->m_dRight, pCropBox->m_dTop);
			m_pGState->LineTo(pCropBox->m_dLeft, pCropBox->m_dTop);
			m_pGState->ClosePath();
			m_pGState->Clip();
			m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

			if (m_pOut->UseClipTo())
				m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
			else
				m_pOut->Clip(m_pGState);

			m_pGState->ClearPath();
		}
	}

	Graphics::Graphics(GlobalParams *pGlobalParams, XRef *pXref, OutputDev *pOut, Dict *pResorcesDict, PDFRectangle *pBox, PDFRectangle *pCropBox, bool(*pAbortCheckCallBack)(void *pData), void *pAbortCheckData)
	{
#ifdef _DEBUG
		m_pDumpFile = NULL;
#endif

		m_pGlobalParams = pGlobalParams;

		m_pXref    = pXref;
		m_bSubPage = true;

		// Resource stack
		m_pResources = new GrResources(m_pXref, pResorcesDict, NULL, m_pGlobalParams);

		// Initialize
		m_pOut    = pOut;
		m_pGState = new GrState(72, 72, pBox, 0, false);
		m_bFontChanged = false;
		m_eClip = clipNone;
		m_nIgnoreUndef = 0;

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrBaseMatrix[nIndex] = m_pGState->GetCTM()[nIndex];
		}

		m_nFormDepth = 0;
		m_pAbortCheckCallBack = pAbortCheckCallBack;
		m_pAbortCheckData     = pAbortCheckData;

		if (pCropBox)
		{
			m_pGState->MoveTo(pCropBox->m_dLeft, pCropBox->m_dBottom);
			m_pGState->LineTo(pCropBox->m_dRight, pCropBox->m_dBottom);
			m_pGState->LineTo(pCropBox->m_dRight, pCropBox->m_dTop);
			m_pGState->LineTo(pCropBox->m_dLeft, pCropBox->m_dTop);
			m_pGState->ClosePath();
			m_pGState->Clip();
			m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

			if (m_pOut->UseClipTo())
				m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
			else
				m_pOut->Clip(m_pGState);

			m_pGState->ClearPath();
		}
	}

	Graphics::~Graphics()
	{
#ifdef _DEBUG

		if (m_pDumpFile)
			::fclose(m_pDumpFile);

#endif
		while (m_pGState->HasSaves())
		{
			RestoreGState();
		}
		if (!m_bSubPage)
		{
			m_pOut->EndPage();
		}
		while (m_pResources)
		{
			PopResources();
		}
		if (m_pGState)
		{
			delete m_pGState;
		}
	}

	void Graphics::Display(Object *pObject, bool bTopLevel)
	{
		if (pObject->IsArray())
		{
			for (int nIndex = 0; nIndex < pObject->ArrayGetLength(); ++nIndex)
			{
				Object oTemp;
				pObject->ArrayGet(nIndex, &oTemp);
				if (!oTemp.IsStream())
				{
					// TO DO: Error "Weird page contents"
					oTemp.Free();
					return;
				}
				oTemp.Free();
			}
		}
		else if (!pObject->IsStream())
		{
			// TO DO: Error "Weird page contents"
			return;
		}
		m_pParser = new Parser(m_pXref, new Lexer(m_pXref, pObject), false);
		if (NULL == m_pParser)
			return;
		StartParse(bTopLevel);
		delete m_pParser;
		m_pParser = NULL;
	}

	void Graphics::StartParse(bool bTopLevel)
	{
		Object arrArguments[maxArgs];
		int nArgumentsCount = 0;
		int nLastAbortCheck = 0;

		m_nUpdateLevel = 0;

		Object oTemp;
		m_pParser->GetObject(&oTemp);
		while (!oTemp.IsEOF())
		{
			if (m_pOut && m_pOut->IsStopped())
				break;

			if (oTemp.IsCommand()) // Если получили команду, тогда выполняем ее
			{
				// Тут распечатываются посланные команды 

#ifdef _DEBUG
				if (m_pDumpFile)
				{
					oTemp.Print(m_pDumpFile);
					for (int nIndex = 0; nIndex < nArgumentsCount; nIndex++)
					{
						::fprintf(m_pDumpFile, " ");
						arrArguments[nIndex].Print(m_pDumpFile);
					}
					::fprintf(m_pDumpFile, "\n");
					::fflush(m_pDumpFile);
				}
#endif

				ExecuteOperator(&oTemp, arrArguments, nArgumentsCount);
				oTemp.Free();

				for (int nIndex = 0; nIndex < nArgumentsCount; ++nIndex)
					arrArguments[nIndex].Free();
				nArgumentsCount = 0;

				// Производим переодические обновления того что выводим
				if (++m_nUpdateLevel >= 20000)
				{
					m_pOut->Dump();
					m_nUpdateLevel = 0;
				}

				// Проверяем отмену
				if (m_pAbortCheckCallBack)
				{
					if (m_nUpdateLevel - nLastAbortCheck > 10)
					{
						if ((*m_pAbortCheckCallBack)(m_pAbortCheckData))
						{
							break;
						}
						nLastAbortCheck = m_nUpdateLevel;
					}
				}
			}
			else if (nArgumentsCount < maxArgs) // Считываем аргумент
			{
				arrArguments[nArgumentsCount++] = oTemp;
			}
			else // Аргументов слишком много, значит что-то неправильно
			{
				// TO DO: Error "Too many args in content stream"
#ifdef _DEBUG	
				if (m_pDumpFile)
				{
					::fprintf(m_pDumpFile, "throwing away arg: ");
					oTemp.Print(m_pDumpFile);
					::fprintf(m_pDumpFile, "\n");
					::fflush(m_pDumpFile);
				}
#endif
				oTemp.Free();
			}

			// Считываем следующий объект
			m_pParser->GetObject(&oTemp);
		}
		oTemp.Free();

		// args at end with no command
		if (nArgumentsCount > 0)
		{
			// TO DO: Error "Leftover args in content stream"

#ifdef _DEBUG
			if (m_pDumpFile)
			{
				::fprintf(m_pDumpFile, "%d leftovers:", nArgumentsCount);
				for (int nIndex = 0; nIndex < nArgumentsCount; ++nIndex)
				{
					::fprintf(m_pDumpFile, " ");
					arrArguments[nIndex].Print(m_pDumpFile);
				}
				::fprintf(m_pDumpFile, "\n");
				::fflush(m_pDumpFile);
			}
#endif
			for (int nIndex = 0; nIndex < nArgumentsCount; ++nIndex)
				arrArguments[nIndex].Free();
		}

		if (bTopLevel && m_nUpdateLevel > 0)
		{
			m_pOut->Dump();
		}
	}

	void Graphics::ExecuteOperator(Object *pCommand, Object arrArguments[], int nArgumentsCount)
	{
		Operator *pOperator = NULL;
		// Ищем оператор
		char* sName = pCommand->GetCommand();

		if (!(pOperator = FindOperator(sName)))
		{
			if (m_nIgnoreUndef == 0) // Проверяем наличие незакрытого оператора BX
			{
				// TO DO: Error "Unknown operator"
			}
			return;
		}

		// Проверяем количество аргументов
		Object *pArguments = arrArguments;
		if (pOperator->nArgumentsCount >= 0)
		{
			if (nArgumentsCount < pOperator->nArgumentsCount)
			{
				// TO DO: Error "Too few args in operator"
				return;
			}
			if (nArgumentsCount > pOperator->nArgumentsCount)
			{
				// TO DO: Error "Too many args in operator"

				// Используем последние значения в качесте аргументов
				pArguments += nArgumentsCount - pOperator->nArgumentsCount;
				nArgumentsCount = pOperator->nArgumentsCount;
			}
		}
		else
		{
			if (nArgumentsCount > -pOperator->nArgumentsCount)
			{
				// TO DO: Error "Too many args in operator"
				return;
			}
		}

		// Проверяем типы аргументов
		for (int nIndex = 0; nIndex < nArgumentsCount; ++nIndex)
		{
			if (!CheckArgumentType(&pArguments[nIndex], pOperator->arrArguments[nIndex]))
			{
				// TO DO: Error "Some argument is wrong type"
				return;
			}
		}

		// Выполняем сам оператор
		(this->*pOperator->pFunction)(pArguments, nArgumentsCount);
	}
	Operator *Graphics::FindOperator(char *sName)
	{
		int nCompareRes = 0;

		int nStart = -1;
		int nEnd = OperatorsCount;
		// OperatorsTable[nStart] < sName < OperatorsTable[nEnd]

		while (nEnd - nStart > 1)
		{
			int nMiddle = (nStart + nEnd) / 2;
			nCompareRes = strcmp(OperatorsTable[nMiddle].sName, sName);
			if (nCompareRes < 0)
				nStart = nMiddle;
			else if (nCompareRes > 0)
				nEnd = nMiddle;
			else
				nStart = nEnd = nMiddle;
		}
		if (nCompareRes != 0)
			return NULL;
		return &OperatorsTable[nStart];
	}

	bool Graphics::CheckArgumentType(Object *pArgument, ArgType eType)
	{
		switch (eType)
		{
			case argBool:   return pArgument->IsBool();
			case argInt:    return pArgument->IsInt();
			case argNum:    return pArgument->IsNum();
			case argString: return pArgument->IsString();
			case argName:   return pArgument->IsName();
			case argArray:  return pArgument->IsArray();
			case argProps:  return pArgument->IsDict() || pArgument->IsName();
			case argSCN:    return pArgument->IsNum() || pArgument->IsName();
			case argNone:   return false;
		}
		return false;
	}

	int Graphics::GetPos()
	{
		return m_pParser ? m_pParser->GetPos() : -1;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Graphics state
	//-------------------------------------------------------------------------------------------------------------------------------

	// q
	void Graphics::OperatorSave(Object arrArguments[], int nArgumentsCount)
	{
		SaveGState();
	}
	// Q
	void Graphics::OperatorRestore(Object arrArguments[], int nArgumentsCount)
	{
		RestoreGState();
	}
	// cm
	void Graphics::OperatorConcat(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->ConcatCTM(arrArguments[0].GetNum(), arrArguments[1].GetNum(), arrArguments[2].GetNum(), arrArguments[3].GetNum(), arrArguments[4].GetNum(), arrArguments[5].GetNum());
		m_pOut->UpdateCTM(m_pGState, arrArguments[0].GetNum(), arrArguments[1].GetNum(), arrArguments[2].GetNum(), arrArguments[3].GetNum(), arrArguments[4].GetNum(), arrArguments[5].GetNum());
		m_bFontChanged = true;
	}
	// d
	void Graphics::OperatorSetDash(Object arrArguments[], int nArgumentsCount)
	{
		double *pDash;
		Array *pArray = arrArguments[0].GetArray();
		int nCount = pArray->GetCount();
		if (0 == nCount)
		{
			pDash = NULL;
		}
		else
		{
			pDash = (double *)MemUtilsMallocArray(nCount, sizeof(double));
			for (int nIndex = 0; nIndex < nCount; ++nIndex)
			{
				Object oTemp;
				pDash[nIndex] = pArray->Get(nIndex, &oTemp)->GetNum();
				oTemp.Free();
			}
		}
		m_pGState->SetLineDash(pDash, nCount, arrArguments[1].GetNum());
		m_pOut->UpdateLineDash(m_pGState);
	}
	// i 
	void Graphics::OperatorSetFlat(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetFlatness((int)arrArguments[0].GetNum());
		m_pOut->UpdateFlatness(m_pGState);
	}
	// j
	void Graphics::OperatorSetLineJoin(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetLineJoin(arrArguments[0].GetInt());
		m_pOut->UpdateLineJoin(m_pGState);
	}
	// J
	void Graphics::OperatorSetLineCap(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetLineCap(arrArguments[0].GetInt());
		m_pOut->UpdateLineCap(m_pGState);
	}
	// M
	void Graphics::OperatorSetMiterLimit(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetMiterLimit(arrArguments[0].GetNum());
		m_pOut->UpdateMiterLimit(m_pGState);
	}
	// w
	void Graphics::OperatorSetLineWidth(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetLineWidth(arrArguments[0].GetNum());
		m_pOut->UpdateLineWidth(m_pGState);
	}
	// ri
	void Graphics::OperatorSetRenderingIntent(Object arrArguments[], int nArgumentsCount)
	{
	}
	// gs
	void Graphics::OperatorSetExtGState(Object arrArguments[], int nArgumentsCount)
	{
		Object oDict;
		if (!m_pResources->LookupExtGState(arrArguments[0].GetName(), &oDict))
		{
			return;
		}
		if (!oDict.IsDict())
		{
			// TO DO: Error "ExtGState is wrong type"
			oDict.Free();
			return;
		}

		// Прозрачность
		Object oDictItem;
		// BM (blend mode)
		if (!oDict.DictLookup("BM", &oDictItem)->IsNull())
		{
			GraphicsBlendMode eMode;
			if (m_pGState->ParseBlendMode(&oDictItem, &eMode))
			{
				m_pGState->SetBlendMode(eMode);
				m_pOut->UpdateBlendMode(m_pGState);
			}
			else
			{
				// TO DO: Error "Invalid blend mode in ExtGState"
			}
		}
		oDictItem.Free();

		// ca (current alpha for filling)
		if (oDict.DictLookup("ca", &oDictItem)->IsNum())
		{
			m_pGState->SetFillOpacity(oDictItem.GetNum());
			m_pOut->UpdateFillOpacity(m_pGState);
		}
		oDictItem.Free();

		// CA (current alpha for stroking)
		if (oDict.DictLookup("CA", &oDictItem)->IsNum())
		{
			m_pGState->SetStrokeOpacity(oDictItem.GetNum());
			m_pOut->UpdateStrokeOpacity(m_pGState);
		}
		oDictItem.Free();

		// op (over print for filling)
		bool bHaveFillOP;
		if ((bHaveFillOP = (oDict.DictLookup("op", &oDictItem)->IsBool())))
		{
			m_pGState->SetFillOverprint(oDictItem.GetBool());
			m_pOut->UpdateFillOverprint(m_pGState);
		}
		oDictItem.Free();

		// OP (over print for stroking)
		if (oDict.DictLookup("OP", &oDictItem)->IsBool())
		{
			m_pGState->SetStrokeOverprint(oDictItem.GetBool());
			m_pOut->UpdateStrokeOverprint(m_pGState);
			if (!bHaveFillOP)
			{
				m_pGState->SetFillOverprint(oDictItem.GetBool());
				m_pOut->UpdateFillOverprint(m_pGState);
			}
		}
		oDictItem.Free();

		// SA (stroke adjustment)
		if (oDict.DictLookup("SA", &oDictItem)->IsBool())
		{
			m_pGState->SetStrokeAdjust(oDictItem.GetBool());
			m_pOut->UpdateStrokeAdjust(m_pGState);
		}
		oDictItem.Free();

		// TR2/TR (transfer function)
		// приоритетной является TR2
		Function *ppFunctions[4];
		if (oDict.DictLookup("TR2", &oDictItem)->IsNull())
		{
			oDictItem.Free();
			oDict.DictLookup("TR", &oDictItem);
		}
		if (oDictItem.IsName("Default") || oDictItem.IsName("Identity"))
		{
			ppFunctions[0] = ppFunctions[1] = ppFunctions[2] = ppFunctions[3] =  NULL;
			m_pGState->SetTransfer(ppFunctions);
			m_pOut->UpdateTransfer(m_pGState);
		}
		else if (oDictItem.IsArray() && oDictItem.ArrayGetLength() == 4)
		{
			int nIndex = 0;
			for (nIndex = 0; nIndex < 4; ++nIndex)
			{
				Object oTemp;
				oDictItem.ArrayGet(nIndex, &oTemp);
				ppFunctions[nIndex] = Function::Parse(&oTemp);
				oTemp.Free();
				if (!ppFunctions[nIndex])
				{
					break;
				}
			}
			if (4 == nIndex) // нормально ли все функции прочитались
			{
				m_pGState->SetTransfer(ppFunctions);
				m_pOut->UpdateTransfer(m_pGState);
			}
		}
		else if (oDictItem.IsName() || oDictItem.IsDict() || oDictItem.IsStream())
		{
			if ((ppFunctions[0] = Function::Parse(&oDictItem)))
			{
				ppFunctions[1] = ppFunctions[2] = ppFunctions[3] = NULL;
				m_pGState->SetTransfer(ppFunctions);
				m_pOut->UpdateTransfer(m_pGState);
			}
		}
		else if (!oDictItem.IsNull())
		{
			// TO DO: Error "Invalid transfer function in ExtGState"
		}
		oDictItem.Free();

		// SMask (soft mask)
		if (!oDict.DictLookup("SMask", &oDictItem)->IsNull())
		{
			if (oDictItem.IsName("None"))
			{
				m_pOut->ClearSoftMask(m_pGState);
			}
			else if (oDictItem.IsDict())
			{
				Object oTemp;
				bool bAlpha = false;
				if (oDictItem.DictLookup("S", &oTemp)->IsName("Alpha"))
				{
					bAlpha = true;
				}
				else // "Luminosity"
				{
					bAlpha = false;
				}
				oTemp.Free();

				ppFunctions[0] = NULL;
				if (!oDictItem.DictLookup("TR", &oTemp)->IsNull())
				{
					ppFunctions[0] = Function::Parse(&oTemp);
					if (ppFunctions[0]->GetInputSize() != 1 || ppFunctions[0]->GetOutputSize() != 1)
					{
						// TO DO: Error "Invalid transfer function in soft mask in ExtGState"
						if (ppFunctions[0])
							delete ppFunctions[0];
						ppFunctions[0] = NULL;
					}
				}
				oTemp.Free();

				bool bHaveBackDropColor = false;
				GrColor oBackDropColor;
				if ((bHaveBackDropColor = oDictItem.DictLookup("BC", &oTemp)->IsArray()))
				{
					for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
					{
						oBackDropColor.arrComp[nIndex] = 0;
					}
					for (int nIndex = 0; nIndex < oTemp.ArrayGetLength() && nIndex < GrColorMaxComps; ++nIndex)
					{
						Object oArrayItem;
						oTemp.ArrayGet(nIndex, &oArrayItem);
						if (oArrayItem.IsNum())
						{
							oBackDropColor.arrComp[nIndex] = DoubleToColor(oArrayItem.GetNum());
						}
						oArrayItem.Free();
					}
				}
				oTemp.Free();

				if (oDictItem.DictLookup("G", &oTemp)->IsStream())
				{
					Object oDict2;
					if (oTemp.StreamGetDict()->Search("Group", &oDict2)->IsDict())
					{
						GrColorSpace *pBlendingColorSpace = NULL;
						bool bIsolated = false, bKnockout = false;
						Object oTemp2;
						if (!oDict2.DictLookup("CS", &oTemp2)->IsNull())
						{
							pBlendingColorSpace = GrColorSpace::Parse(&oTemp2);
						}
						oTemp2.Free();
						if (oDict2.DictLookup("I", &oTemp2)->IsBool())
						{
							bIsolated = oTemp2.GetBool();
						}
						oTemp2.Free();
						if (oDict2.DictLookup("K", &oTemp2)->IsBool())
						{
							bKnockout = oTemp2.GetBool();
						}
						oTemp2.Free();
						if (!bHaveBackDropColor)
						{
							if (pBlendingColorSpace)
							{
								pBlendingColorSpace->GetDefaultColor(&oBackDropColor);
							}
							else
							{
								for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
								{
									oBackDropColor.arrComp[nIndex] = 0;
								}
							}
						}
						MakeSoftMask(&oTemp, bAlpha, pBlendingColorSpace, bIsolated, bKnockout, ppFunctions[0], &oBackDropColor);
						if (ppFunctions[0])
						{
							delete ppFunctions[0];
						}
					}
					else
					{
						// TO DO: Error "Invalid soft mask in ExtGState - missing group"
					}
					oDict2.Free();
				}
				else
				{
					// TO DO: Error "Invalid soft mask in ExtGState - missing group"
				}
				oTemp.Free();
			}
			else if (!oDictItem.IsNull())
			{
				// TO DO: Error "Invalid soft mask in ExtGState"
			}
		}
		oDictItem.Free();
		oDict.Free();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void Graphics::MakeSoftMask(Object *pStream, bool bAlpha, GrColorSpace *pBlendingColorSpace, bool bIsolated, bool bKnockout, Function *pTransferFunction, GrColor *pBackDropColor)
	{
		// Проверяем на чрезмерную рекурсию
		if (m_nFormDepth > 20)
		{
			return;
		}

		Dict *pDict = pStream->StreamGetDict();

		// Form type
		Object oDictItem;
		pDict->Search("FormType", &oDictItem);
		if (!(oDictItem.IsNull() || (oDictItem.IsInt() && oDictItem.GetInt() == 1)))
		{
			// TO DO: Error "Unknown form type"
		}
		oDictItem.Free();

		// BBox
		pDict->Search("BBox", &oDictItem);
		if (!oDictItem.IsArray())
		{
			oDictItem.Free();
			// TO DO: Error "Bad form bounding box"
			return;
		}
		double arrBBox[4];
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			Object oTemp;
			oDictItem.ArrayGet(nIndex, &oTemp);
			arrBBox[nIndex] = oTemp.GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		// Matrix
		double arrMatrix[6];
		pDict->Search("Matrix", &oDictItem);
		if (oDictItem.IsArray())
		{
			for (int nIndex = 0; nIndex < 6; ++nIndex)
			{
				Object oTemp;
				oDictItem.ArrayGet(nIndex, &oTemp);
				arrMatrix[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		else
		{
			arrMatrix[0] = 1; arrMatrix[1] = 0;
			arrMatrix[2] = 0; arrMatrix[3] = 1;
			arrMatrix[4] = 0; arrMatrix[5] = 0;
		}
		oDictItem.Free();

		// Resources
		pDict->Search("Resources", &oDictItem);
		Dict *pResourcesDict = (oDictItem.IsDict() ? oDictItem.GetDict() : (Dict *)NULL);

		// Рисуем
		++m_nFormDepth;
		DoForm(pStream, pResourcesDict, arrMatrix, arrBBox, true, true, pBlendingColorSpace, bIsolated, bKnockout, bAlpha, pTransferFunction, pBackDropColor);
		--m_nFormDepth;

		if (pBlendingColorSpace)
		{
			delete pBlendingColorSpace;
		}
		oDictItem.Free();
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Color and Color spaces
	//-------------------------------------------------------------------------------------------------------------------------------

	// g
	void Graphics::OperatorSetFillGray(Object arrArguments[], int nArgumentsCount)
	{
		GrColor oColor;

		m_pGState->SetFillPattern(NULL);
		m_pGState->SetFillColorSpace(new GrDeviceGrayColorSpace());
		m_pOut->UpdateFillColorSpace(m_pGState);
		oColor.arrComp[0] = DoubleToColor(arrArguments[0].GetNum());
		m_pGState->SetFillColor(&oColor);
		m_pOut->UpdateFillColor(m_pGState);
	}

	// G
	void Graphics::OperatorSetStrokeGray(Object arrArguments[], int nArgumentsCount)
	{
		GrColor oColor;

		m_pGState->SetStrokePattern(NULL);
		m_pGState->SetStrokeColorSpace(new GrDeviceGrayColorSpace());
		m_pOut->UpdateStrokeColorSpace(m_pGState);
		oColor.arrComp[0] = DoubleToColor(arrArguments[0].GetNum());
		m_pGState->SetStrokeColor(&oColor);
		m_pOut->UpdateStrokeColor(m_pGState);
	}

	// k
	void Graphics::OperatorSetFillCMYKColor(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetFillPattern(NULL);
		m_pGState->SetFillColorSpace(new GrDeviceCMYKColorSpace());
		m_pOut->UpdateFillColorSpace(m_pGState);

		GrColor oColor;
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
		}
		m_pGState->SetFillColor(&oColor);
		m_pOut->UpdateFillColor(m_pGState);
	}

	// K
	void Graphics::OperatorSetStrokeCMYKColor(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetStrokePattern(NULL);
		m_pGState->SetStrokeColorSpace(new GrDeviceCMYKColorSpace());
		m_pOut->UpdateStrokeColorSpace(m_pGState);

		GrColor oColor;
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
		}
		m_pGState->SetStrokeColor(&oColor);
		m_pOut->UpdateStrokeColor(m_pGState);
	}

	// rg
	void Graphics::OperatorSetFillRGBColor(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetFillPattern(NULL);
		m_pGState->SetFillColorSpace(new GrDeviceRGBColorSpace());
		m_pOut->UpdateFillColorSpace(m_pGState);

		GrColor oColor;
		for (int nIndex = 0; nIndex < 3; ++nIndex)
		{
			oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
		}
		m_pGState->SetFillColor(&oColor);
		m_pOut->UpdateFillColor(m_pGState);
	}

	// RG
	void Graphics::OperatorSetStrokeRGBColor(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetStrokePattern(NULL);
		m_pGState->SetStrokeColorSpace(new GrDeviceRGBColorSpace());
		m_pOut->UpdateStrokeColorSpace(m_pGState);

		GrColor oColor;
		for (int nIndex = 0; nIndex < 3; ++nIndex)
		{
			oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
		}
		m_pGState->SetStrokeColor(&oColor);
		m_pOut->UpdateStrokeColor(m_pGState);
	}

	// cs
	void Graphics::OperatorSetFillColorSpace(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetFillPattern(NULL);
		Object oTemp;
		m_pResources->LookupColorSpace(arrArguments[0].GetName(), &oTemp);
		GrColorSpace *pColorSpace = NULL;
		if (oTemp.IsNull())
		{
			pColorSpace = GrColorSpace::Parse(&arrArguments[0]);
		}
		else
		{
			pColorSpace = GrColorSpace::Parse(&oTemp);
		}
		oTemp.Free();

		if (pColorSpace)
		{
			m_pGState->SetFillColorSpace(pColorSpace);
			m_pOut->UpdateFillColorSpace(m_pGState);
			GrColor oColor;
			pColorSpace->GetDefaultColor(&oColor);
			m_pGState->SetFillColor(&oColor);
			m_pOut->UpdateFillColor(m_pGState);
		}
		else
		{
			// TO DO: Error "Bad color space (fill)"
		}
	}

	// CS
	void Graphics::OperatorSetStrokeColorSpace(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetStrokePattern(NULL);
		Object oTemp;
		m_pResources->LookupColorSpace(arrArguments[0].GetName(), &oTemp);
		GrColorSpace *pColorSpace = NULL;
		if (oTemp.IsNull())
		{
			pColorSpace = GrColorSpace::Parse(&arrArguments[0]);
		}
		else
		{
			pColorSpace = GrColorSpace::Parse(&oTemp);
		}
		oTemp.Free();

		if (pColorSpace)
		{
			m_pGState->SetStrokeColorSpace(pColorSpace);
			m_pOut->UpdateStrokeColorSpace(m_pGState);
			GrColor oColor;
			pColorSpace->GetDefaultColor(&oColor);
			m_pGState->SetStrokeColor(&oColor);
			m_pOut->UpdateStrokeColor(m_pGState);
		}
		else
		{
			// TO DO: Error "Bad color space (stroke)"
		}
	}

	// sc
	void Graphics::OperatorSetFillColor(Object arrArguments[], int nArgumentsCount)
	{
		if (nArgumentsCount != m_pGState->GetFillColorSpace()->GetComponentsCount())
		{
			// TO DO: Error "Incorrect number of arguments in 'sc' command"
			return;
		}
		m_pGState->SetFillPattern(NULL);

		GrColor oColor;
		for (int nIndex = 0; nIndex < nArgumentsCount; ++nIndex)
		{
			oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
		}
		m_pGState->SetFillColor(&oColor);
		m_pOut->UpdateFillColor(m_pGState);
	}

	// SC
	void Graphics::OperatorSetStrokeColor(Object arrArguments[], int nArgumentsCount)
	{
		if (nArgumentsCount != m_pGState->GetStrokeColorSpace()->GetComponentsCount())
		{
			// TO DO: Error "Incorrect number of arguments in 'SC' command"
			return;
		}
		m_pGState->SetStrokePattern(NULL);

		GrColor oColor;
		for (int nIndex = 0; nIndex < nArgumentsCount; ++nIndex)
		{
			oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
		}
		m_pGState->SetStrokeColor(&oColor);
		m_pOut->UpdateStrokeColor(m_pGState);
	}

	// scn
	void Graphics::OperatorSetFillColorN(Object arrArguments[], int nArgumentsCount)
	{
		GrPattern *pPattern = NULL;
		GrColor oColor;
		if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
		{
			if (nArgumentsCount > 1)
			{
				if (!((GrPatternColorSpace *)m_pGState->GetFillColorSpace())->GetUnder() || nArgumentsCount - 1 != ((GrPatternColorSpace *)m_pGState->GetFillColorSpace())->GetUnder()->GetComponentsCount())
				{
					// TO DO: Error "Incorrect number of arguments in 'scn' command"
					return;
				}
				for (int nIndex = 0; nIndex < nArgumentsCount - 1 && nIndex < GrColorMaxComps; ++nIndex)
				{
					if (arrArguments[nIndex].IsNum())
					{
						oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
					}
				}
				m_pGState->SetFillColor(&oColor);
				m_pOut->UpdateFillColor(m_pGState);
			}
			if (arrArguments[nArgumentsCount - 1].IsName() && (pPattern = m_pResources->LookupPattern(arrArguments[nArgumentsCount - 1].GetName())))
			{
				m_pGState->SetFillPattern(pPattern);
			}
		}
		else
		{
			if (nArgumentsCount != m_pGState->GetFillColorSpace()->GetComponentsCount())
			{
				// TO DO: Error "Incorrect number of arguments in 'scn' command"
				return;
			}
			m_pGState->SetFillPattern(NULL);
			for (int nIndex = 0; nIndex < nArgumentsCount && nIndex < GrColorMaxComps; ++nIndex)
			{
				if (arrArguments[nIndex].IsNum())
				{
					oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
				}
			}
			m_pGState->SetFillColor(&oColor);
			m_pOut->UpdateFillColor(m_pGState);
		}
	}

	// SCN
	void Graphics::OperatorSetStrokeColorN(Object arrArguments[], int nArgumentsCount)
	{
		GrPattern *pPattern = NULL;
		GrColor oColor;
		if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
		{
			if (nArgumentsCount > 1)
			{
				if (!((GrPatternColorSpace *)m_pGState->GetStrokeColorSpace())->GetUnder() || nArgumentsCount - 1 != ((GrPatternColorSpace *)m_pGState->GetStrokeColorSpace())->GetUnder()->GetComponentsCount())
				{
					// TO DO: Error "Incorrect number of arguments in 'SCN' command"
					return;
				}
				for (int nIndex = 0; nIndex < nArgumentsCount - 1 && nIndex < GrColorMaxComps; ++nIndex)
				{
					if (arrArguments[nIndex].IsNum())
					{
						oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
					}
				}
				m_pGState->SetStrokeColor(&oColor);
				m_pOut->UpdateStrokeColor(m_pGState);
			}
			if (arrArguments[nArgumentsCount - 1].IsName() && (pPattern = m_pResources->LookupPattern(arrArguments[nArgumentsCount - 1].GetName())))
			{
				m_pGState->SetStrokePattern(pPattern);
			}
		}
		else
		{
			if (nArgumentsCount != m_pGState->GetStrokeColorSpace()->GetComponentsCount())
			{
				// TO DO: Error "Incorrect number of arguments in 'SCN' command"
				return;
			}
			m_pGState->SetStrokePattern(NULL);
			for (int nIndex = 0; nIndex < nArgumentsCount && nIndex < GrColorMaxComps; ++nIndex)
			{
				if (arrArguments[nIndex].IsNum())
				{
					oColor.arrComp[nIndex] = DoubleToColor(arrArguments[nIndex].GetNum());
				}
			}
			m_pGState->SetStrokeColor(&oColor);
			m_pOut->UpdateStrokeColor(m_pGState);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Path construction
	//-------------------------------------------------------------------------------------------------------------------------------

	// m
	void Graphics::OperatorMoveTo(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->MoveTo(arrArguments[0].GetNum(), arrArguments[1].GetNum());
	}

	// l
	void Graphics::OperatorLineTo(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No current point in lineto"
			return;
		}
		m_pGState->LineTo(arrArguments[0].GetNum(), arrArguments[1].GetNum());
	}

	// c
	void Graphics::OperatorCurveTo(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No current point in curveto"
			return;
		}
		m_pGState->CurveTo((double)arrArguments[0].GetNum(), (double)arrArguments[1].GetNum(), (double)arrArguments[2].GetNum(), (double)arrArguments[3].GetNum(), (double)arrArguments[4].GetNum(), (double)arrArguments[5].GetNum());
	}

	// v
	void Graphics::OperatorCurveTo1(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No current point in curveto1"
			return;
		}
		m_pGState->CurveTo(m_pGState->GetCurX(), m_pGState->GetCurY(), (double)arrArguments[0].GetNum(), (double)arrArguments[1].GetNum(), (double)arrArguments[2].GetNum(), (double)arrArguments[3].GetNum());
	}

	// y
	void Graphics::OperatorCurveTo2(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No current point in curveto2"
			return;
		}
		m_pGState->CurveTo((double)arrArguments[0].GetNum(), (double)arrArguments[1].GetNum(), (double)arrArguments[2].GetNum(), (double)arrArguments[3].GetNum(), (double)arrArguments[2].GetNum(), (double)arrArguments[3].GetNum());
	}

	// re
	void Graphics::OperatorRectangle(Object arrArguments[], int nArgumentsCount)
	{
		double dX = arrArguments[0].GetNum();
		double dY = arrArguments[1].GetNum();
		double dWidth  = arrArguments[2].GetNum();
		double dHeight = arrArguments[3].GetNum();
		m_pGState->MoveTo(dX, dY);
		m_pGState->LineTo(dX + dWidth, dY);
		m_pGState->LineTo(dX + dWidth, dY + dHeight);
		m_pGState->LineTo(dX, dY + dHeight);
		m_pGState->ClosePath();
	}

	// h
	void Graphics::OperatorClosePath(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No current point in closepath"
			return;
		}
		m_pGState->ClosePath();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// path painting operators
	//-------------------------------------------------------------------------------------------------------------------------------

	// n
	void Graphics::OperatorEndPath(Object arrArguments[], int nArgumentsCount)
	{
		DoEndPath();
	}

	// S
	void Graphics::OperatorStroke(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in stroke"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
			{
				DoPatternStroke();
			}
			else
			{
				m_pOut->Stroke(m_pGState);
			}
		}
		DoEndPath();
	}

	// s
	void Graphics::OperatorCloseStroke(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in closepath/stroke"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			m_pGState->ClosePath();
			if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
			{
				DoPatternStroke();
			}
			else
			{
				m_pOut->Stroke(m_pGState);
			}
		}
		DoEndPath();
	}

	// f/F
	void Graphics::OperatorFill(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in fill"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
			{
				DoPatternFill(false);
			}
			else
			{
				m_pOut->Fill(m_pGState);
			}
		}
		DoEndPath();
	}

	// f*
	void Graphics::OperatorEOFill(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in eofill"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
			{
				DoPatternFill(true);
			}
			else
			{
				m_pOut->EoFill(m_pGState);
			}
		}
		DoEndPath();
	}

	// B
	void Graphics::OperatorFillStroke(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in fill/stroke"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			if (!(m_pGState->GetFillColorSpace()->GetMode() == csPattern) && !(m_pGState->GetStrokeColorSpace()->GetMode() == csPattern) && m_pOut->UseFillAndStroke())
			{
				m_pOut->FillStroke(m_pGState);
			}
			else
			{
				if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
				{
					DoPatternFill(false);
				}
				else
				{
					m_pOut->Fill(m_pGState);
				}
				if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
				{
					DoPatternStroke();
				}
				else
				{
					m_pOut->Stroke(m_pGState);
				}
			}
		}
		DoEndPath();
	}

	// b
	void Graphics::OperatorCloseFillStroke(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in closepath/fill/stroke"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			m_pGState->ClosePath();

			if (!(m_pGState->GetFillColorSpace()->GetMode() == csPattern) && !(m_pGState->GetStrokeColorSpace()->GetMode() == csPattern) && m_pOut->UseFillAndStroke())
			{
				m_pOut->FillStroke(m_pGState);
			}
			else
			{
				if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
				{
					DoPatternFill(false);
				}
				else
				{
					m_pOut->Fill(m_pGState);
				}
				if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
				{
					DoPatternStroke();
				}
				else
				{
					m_pOut->Stroke(m_pGState);
				}
			}
		}
		DoEndPath();
	}

	// B*
	void Graphics::OperatorEOFillStroke(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in eofill/stroke"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			if (!(m_pGState->GetFillColorSpace()->GetMode() == csPattern) && !(m_pGState->GetStrokeColorSpace()->GetMode() == csPattern) && m_pOut->UseFillAndStroke())
			{
				m_pOut->EoFillStroke(m_pGState);
			}
			else
			{
				if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
				{
					DoPatternFill(true);
				}
				else
				{
					m_pOut->EoFill(m_pGState);
				}
				if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
				{
					DoPatternStroke();
				}
				else
				{
					m_pOut->Stroke(m_pGState);
				}
			}
		}
		DoEndPath();
	}

	// b*
	void Graphics::OperatorCloseEOFillStroke(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->IsCurPoint())
		{
			// TO DO: Error "No path in closepath/eofill/stroke"
			return;
		}
		if (m_pGState->IsPathNonEmpty())
		{
			m_pGState->ClosePath();
			if (!(m_pGState->GetFillColorSpace()->GetMode() == csPattern) && !(m_pGState->GetStrokeColorSpace()->GetMode() == csPattern) && m_pOut->UseFillAndStroke())
			{
				m_pOut->EoFillStroke(m_pGState);
			}
			else
			{
				if (m_pGState->GetFillColorSpace()->GetMode() == csPattern)
				{
					DoPatternFill(true);
				}
				else
				{
					m_pOut->EoFill(m_pGState);
				}
				if (m_pGState->GetStrokeColorSpace()->GetMode() == csPattern)
				{
					DoPatternStroke();
				}
				else
				{
					m_pOut->Stroke(m_pGState);
				}
			}
		}
		DoEndPath();
	}

	// sh
	void Graphics::OperatorShadingFill(Object arrArguments[], int nArgumentsCount)
	{
		GrShading *pShading = NULL;

		if (!(pShading = m_pResources->LookupShading(arrArguments[0].GetName())))
		{
			return;
		}

		// Сохраняем текущий GState
		GrPath *pSavedPath = m_pGState->GetPath()->Copy();
		SaveGState();

		if (pShading->GetHasBBox())
		{
			double dXMin, dYMin, dXMax, dYMax;
			pShading->GetBBox(&dXMin, &dYMin, &dXMax, &dYMax);
			m_pGState->MoveTo(dXMin, dYMin);
			m_pGState->LineTo(dXMax, dYMin);
			m_pGState->LineTo(dXMax, dYMax);
			m_pGState->LineTo(dXMin, dYMax);
			m_pGState->ClosePath();
			m_pGState->Clip();
			m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

			if (m_pOut->UseClipTo())
				m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
			else
				m_pOut->Clip(m_pGState);

			m_pGState->ClearPath();
		}

		// Устанавливаем цветовое пространство	 
		m_pGState->SetFillColorSpace(pShading->GetColorSpace()->Copy());
		m_pOut->UpdateFillColorSpace(m_pGState);

		bool bAntialias = m_pOut->GetVectorAntialias();
		if (bAntialias)
		{
			m_pOut->SetVectorAntialias(false);
		}

		m_pOut->StartShadedFill(m_pGState);

		// Выполняем закраску, в зависимости от типа Shading
		switch (pShading->GetType())
		{
			case 1:
			DoFunctionShadingFill((GrFunctionShading *)pShading);
			break;
			case 2:
			DoAxialShadingFill((GrAxialShading *)pShading);
			break;
			case 3:
			DoRadialShadingFill((GrRadialShading *)pShading);
			break;
			case 4:
			case 5:
			DoGouraudTriangleShadingFill((GrGouraudTriangleShading *)pShading);
			break;
			case 6:
			case 7:
			DoPatchMeshShadingFill((GrPatchMeshShading *)pShading);
			break;
		}

		m_pOut->EndShadedFill();

		if (bAntialias)
		{
			m_pOut->SetVectorAntialias(true);
		}

		// Восстанавливаем GState
		RestoreGState();
		m_pGState->SetPath(pSavedPath);

		delete pShading;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void Graphics::DoPatternFill(bool bEOFill)
	{
		// Пропускаем объект Patterns, если мы выводим только текст из PDF
		if (!m_pOut->NeedNonText())
		{
			return;
		}

		GrPattern *pPattern = NULL;
		if (!(pPattern = m_pGState->GetFillPattern()))
		{
			return;
		}
		switch (pPattern->GetType())
		{
			case 1:
			DoTilingPatternFill((GrTilingPattern *)pPattern, false, bEOFill);
			break;
			case 2:
			DoShadingPatternFill((GrShadingPattern *)pPattern, false, bEOFill);
			break;
			default:
			// TO DO: Error "Unimplemented pattern type in fill"
			break;
		}
	}

	void Graphics::DoPatternStroke()
	{
		// Пропускаем объект Patterns, если мы выводим только текст из PDF
		if (!m_pOut->NeedNonText())
		{
			return;
		}

		GrPattern *pPattern = NULL;
		if (!(pPattern = m_pGState->GetStrokePattern()))
		{
			return;
		}
		switch (pPattern->GetType())
		{
			case 1:
			DoTilingPatternFill((GrTilingPattern *)pPattern, true, false);
			break;
			case 2:
			DoShadingPatternFill((GrShadingPattern *)pPattern, true, false);
			break;
			default:
			// TO DO: Error "Unimplemented pattern type in stroke"
			break;
		}
	}

	void Graphics::DoTilingPatternFill(GrTilingPattern  *pPattern, bool bStroke, bool bEOFill)
	{
		// Color space
		GrPatternColorSpace *pPatternCS = (GrPatternColorSpace *)(bStroke ? m_pGState->GetStrokeColorSpace() : m_pGState->GetFillColorSpace());

		// Строим матрицу преобразования (Pattern space) -> (Current space)
		double *pCTM          = m_pGState->GetCTM();
		double *pBaseMatrix   = m_arrBaseMatrix;
		double *pTilingMatrix = pPattern->GetMatrix();
		double dDet = 1 / (pCTM[0] * pCTM[3] - pCTM[1] * pCTM[2]);
		// InvCTM = (CTM)^(-1)
		double pInvCTM[6];
		pInvCTM[0] =  pCTM[3] * dDet;
		pInvCTM[1] = -pCTM[1] * dDet;
		pInvCTM[2] = -pCTM[2] * dDet;
		pInvCTM[3] =  pCTM[0] * dDet;
		pInvCTM[4] = (pCTM[2] * pCTM[5] - pCTM[3] * pCTM[4]) * dDet;
		pInvCTM[5] = (pCTM[1] * pCTM[4] - pCTM[0] * pCTM[5]) * dDet;
		// TempMatrix = TilingMatrix * BaseMatrix
		double pTempMatrix[6];
		pTempMatrix[0] = pTilingMatrix[0] * pBaseMatrix[0] + pTilingMatrix[1] * pBaseMatrix[2];
		pTempMatrix[1] = pTilingMatrix[0] * pBaseMatrix[1] + pTilingMatrix[1] * pBaseMatrix[3];
		pTempMatrix[2] = pTilingMatrix[2] * pBaseMatrix[0] + pTilingMatrix[3] * pBaseMatrix[2];
		pTempMatrix[3] = pTilingMatrix[2] * pBaseMatrix[1] + pTilingMatrix[3] * pBaseMatrix[3];
		pTempMatrix[4] = pTilingMatrix[4] * pBaseMatrix[0] + pTilingMatrix[5] * pBaseMatrix[2] + pBaseMatrix[4];
		pTempMatrix[5] = pTilingMatrix[4] * pBaseMatrix[1] + pTilingMatrix[5] * pBaseMatrix[3] + pBaseMatrix[5];
		// Matrix = TempMatrix * InvCTM 
		double pMatrix[6];
		pMatrix[0] = pTempMatrix[0] * pInvCTM[0] + pTempMatrix[1] * pInvCTM[2];
		pMatrix[1] = pTempMatrix[0] * pInvCTM[1] + pTempMatrix[1] * pInvCTM[3];
		pMatrix[2] = pTempMatrix[2] * pInvCTM[0] + pTempMatrix[3] * pInvCTM[2];
		pMatrix[3] = pTempMatrix[2] * pInvCTM[1] + pTempMatrix[3] * pInvCTM[3];
		pMatrix[4] = pTempMatrix[4] * pInvCTM[0] + pTempMatrix[5] * pInvCTM[2] + pInvCTM[4];
		pMatrix[5] = pTempMatrix[4] * pInvCTM[1] + pTempMatrix[5] * pInvCTM[3] + pInvCTM[5];

		// Строим матрицу преобразования (Device space) -> (Pattern space)
		double pInvTemp[6];
		dDet = 1 / (pTempMatrix[0] * pTempMatrix[3] - pTempMatrix[1] * pTempMatrix[2]);
		pInvTemp[0] =  pTempMatrix[3] * dDet;
		pInvTemp[1] = -pTempMatrix[1] * dDet;
		pInvTemp[2] = -pTempMatrix[2] * dDet;
		pInvTemp[3] =  pTempMatrix[0] * dDet;
		pInvTemp[4] = (pTempMatrix[2] * pTempMatrix[5] - pTempMatrix[3] * pTempMatrix[4]) * dDet;
		pInvTemp[5] = (pTempMatrix[1] * pTempMatrix[4] - pTempMatrix[0] * pTempMatrix[5]) * dDet;

		// Сохраняем текущий GState
		GrPath *pSavedPath = m_pGState->GetPath()->Copy();
		SaveGState();

		// Устанавливаем цветовое пространство для подкладки (для не цветовых Tiling patterns);
		// Устанавливаем разлиные параметры (цвет обводки, толщина линии)
		GrColorSpace *pColorSpace = NULL;
		if (pPattern->GetPaintType() == 2 && (pColorSpace = pPatternCS->GetUnder()))
		{
			m_pGState->SetFillColorSpace(pColorSpace->Copy());
			m_pOut->UpdateFillColorSpace(m_pGState);
			m_pGState->SetStrokeColorSpace(pColorSpace->Copy());
			m_pOut->UpdateStrokeColorSpace(m_pGState);
			m_pGState->SetStrokeColor(m_pGState->GetFillColor());
		}
		else
		{
			m_pGState->SetFillColorSpace(new GrDeviceGrayColorSpace());
			m_pOut->UpdateFillColorSpace(m_pGState);
			m_pGState->SetStrokeColorSpace(new GrDeviceGrayColorSpace());
			m_pOut->UpdateStrokeColorSpace(m_pGState);
		}
		m_pGState->SetFillPattern(NULL);
		m_pOut->UpdateFillColor(m_pGState);
		m_pGState->SetStrokePattern(NULL);
		m_pOut->UpdateStrokeColor(m_pGState);
		if (!bStroke)
		{
			m_pGState->SetLineWidth(0);
			m_pOut->UpdateLineWidth(m_pGState);
		}

		if (bStroke)
		{
			m_pGState->ClipToStrokePath();
			m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

			if (m_pOut->UseClipTo())
				m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
			else
				m_pOut->ClipToStrokePath(m_pGState);
		}
		else
		{
			m_pGState->Clip();
			if (bEOFill)
			{
				m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), true);

				if (m_pOut->UseClipTo())
					m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), true);
				else
					m_pOut->EoClip(m_pGState);
			}
			else
			{
				m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

				if (m_pOut->UseClipTo())
					m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
				else
					m_pOut->Clip(m_pGState);
			}
		}
		m_pGState->ClearPath();

		double dClipXMin, dClipYMin, dClipXMax, dClipYMax;
		m_pGState->GetClipBBox(&dClipXMin, &dClipYMin, &dClipXMax, &dClipYMax);
		if (dClipXMin > dClipXMax || dClipYMin > dClipYMax)
		{
			RestoreGState();
			m_pGState->SetPath(pSavedPath);
			return;
		}

		double dXMin, dYMin, dXMax, dYMax;
		dXMin = dXMax = dClipXMin * pInvTemp[0] + dClipYMin * pInvTemp[2] + pInvTemp[4];
		dYMin = dYMax = dClipXMin * pInvTemp[1] + dClipYMin * pInvTemp[3] + pInvTemp[5];
		double dTempX = dClipXMin * pInvTemp[0] + dClipYMax * pInvTemp[2] + pInvTemp[4];
		double dTempY = dClipXMin * pInvTemp[1] + dClipYMax * pInvTemp[3] + pInvTemp[5];
		if (dTempX < dXMin)
		{
			dXMin = dTempX;
		}
		else if (dTempX > dXMax)
		{
			dXMax = dTempX;
		}
		if (dTempY < dYMin)
		{
			dYMin = dTempY;
		}
		else if (dTempY > dYMax)
		{
			dYMax = dTempY;
		}
		dTempX = dClipXMax * pInvTemp[0] + dClipYMin * pInvTemp[2] + pInvTemp[4];
		dTempY = dClipXMax * pInvTemp[1] + dClipYMin * pInvTemp[3] + pInvTemp[5];
		if (dTempX < dXMin)
		{
			dXMin = dTempX;
		}
		else if (dTempX > dXMax)
		{
			dXMax = dTempX;
		}
		if (dTempY < dYMin)
		{
			dYMin = dTempY;
		}
		else if (dTempY > dYMax)
		{
			dYMax = dTempY;
		}
		dTempX = dClipXMax * pInvTemp[0] + dClipYMax * pInvTemp[2] + pInvTemp[4];
		dTempY = dClipXMax * pInvTemp[1] + dClipYMax * pInvTemp[3] + pInvTemp[5];
		if (dTempX < dXMin)
		{
			dXMin = dTempX;
		}
		else if (dTempX > dXMax)
		{
			dXMax = dTempX;
		}
		if (dTempY < dYMin)
		{
			dYMin = dTempY;
		}
		else if (dTempY > dYMax)
		{
			dYMax = dTempY;
		}

		double dStepX = fabs(pPattern->GetXStep());
		double dStepY = fabs(pPattern->GetYStep());
		int nX0 = (int)ceil((dXMin - pPattern->GetBBox()[2]) / dStepX);
		int nX1 = (int)floor((dXMax - pPattern->GetBBox()[0]) / dStepX) + 1;
		int nY0 = (int)ceil((dYMin - pPattern->GetBBox()[3]) / dStepY);
		int nY1 = (int)floor((dYMax - pPattern->GetBBox()[1]) / dStepY) + 1;
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			pTempMatrix[nIndex] = pMatrix[nIndex];
		}

		m_pOut->StartTilingFill(m_pGState);
		if (m_pOut->UseTilingPatternFill())
		{
			pTempMatrix[4] = pMatrix[4];
			pTempMatrix[5] = pMatrix[5];
			m_pOut->TilingPatternFill(m_pGState, pPattern->GetContentStream(), pPattern->GetPaintType(), pPattern->GetResourcesDict(), pTempMatrix, pPattern->GetBBox(), nX0, nY0, nX1, nY1, dStepX, dStepY);
		}
		else
		{
			if (m_pOut->UseSimpleTilingPatternFill())
			{
				m_pOut->StartSimpleTilingFill(m_pGState, nX0, nY0, nX1, nY1, dStepX, dStepY, dXMin, dYMin, dXMax, dYMax, pTempMatrix);
				
				for (int nY = nY0; nY < nY1; ++nY)
				{
					for (int nX = nX0; nX < nX1; ++nX)
					{
						double dX = nX * dStepX;
						double dY = nY * dStepY;
						pTempMatrix[4] = dX * pMatrix[0] + dY * pMatrix[2] + pMatrix[4];
						pTempMatrix[5] = dX * pMatrix[1] + dY * pMatrix[3] + pMatrix[5];
						DoForm(pPattern->GetContentStream(), pPattern->GetResourcesDict(), pTempMatrix, pPattern->GetBBox());
					}
				}

				m_pOut->EndSimpleTilingFill();
			}					
			else
			{
				for (int nY = nY0; nY < nY1; ++nY)
				{
					for (int nX = nX0; nX < nX1; ++nX)
					{
						if (m_pOut->IsStopped())
						{
							// Восстанавливаем GState
							RestoreGState();
							m_pGState->SetPath(pSavedPath);
							return;
						}

						m_pOut->StartTilingFillIteration();

						double dX = nX * dStepX;
						double dY = nY * dStepY;
						pTempMatrix[4] = dX * pMatrix[0] + dY * pMatrix[2] + pMatrix[4];
						pTempMatrix[5] = dX * pMatrix[1] + dY * pMatrix[3] + pMatrix[5];
						DoForm(pPattern->GetContentStream(), pPattern->GetResourcesDict(), pTempMatrix, pPattern->GetBBox());

						m_pOut->EndTilingFillIteration();
					}
				}
			}
		}

		m_pOut->EndTilingFill();

		// Восстанавливаем GState
		RestoreGState();
		m_pGState->SetPath(pSavedPath);
	}

	void Graphics::DoShadingPatternFill(GrShadingPattern *pPattern, bool bStroke, bool bEOFill)
	{
		GrShading *pShading = pPattern->GetShading();

		// Сохраняем текущий GState
		GrPath *pSavedPath = m_pGState->GetPath()->Copy();
		SaveGState();

		if (pShading->GetHasBBox())
		{
			double dXMin, dYMin, dXMax, dYMax;
			pShading->GetBBox(&dXMin, &dYMin, &dXMax, &dYMax);
			m_pGState->MoveTo(dXMin, dYMin);
			m_pGState->LineTo(dXMax, dYMin);
			m_pGState->LineTo(dXMax, dYMax);
			m_pGState->LineTo(dXMin, dYMax);
			m_pGState->ClosePath();
			m_pGState->Clip();
			m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

			if (m_pOut->UseClipTo())
				m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
			else
				m_pOut->Clip(m_pGState);

			m_pGState->SetPath(pSavedPath->Copy());
		}

		if (bStroke)
		{
			m_pGState->ClipToStrokePath();
			m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

			if (m_pOut->UseClipTo())
				m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
			else
				m_pOut->ClipToStrokePath(m_pGState);
		}
		else
		{
			m_pGState->Clip();
			if (bEOFill)
			{
				m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), true);

				if (m_pOut->UseClipTo())
					m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), true);
				else
					m_pOut->EoClip(m_pGState);
			}
			else
			{
				m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

				if (m_pOut->UseClipTo())
					m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
				else
					m_pOut->Clip(m_pGState);
			}
		}

		// Устанавливаем цветовое пространство
		m_pGState->SetFillColorSpace(pShading->GetColorSpace()->Copy());
		m_pOut->UpdateFillColorSpace(m_pGState);

		// Задний фон
		if (pShading->GetHasBackground())
		{
			m_pGState->SetFillColor(pShading->GetBackground());
			m_pOut->UpdateFillColor(m_pGState);
			m_pOut->Fill(m_pGState);
		}
		m_pGState->ClearPath();

		// Строим матрицу преобразования (Pattern space) -> (Current space)
		double *pCTM = m_pGState->GetCTM();
		double *pBaseMatrix = m_arrBaseMatrix;
		double *pPatternMatrix = pPattern->GetMatrix();

		// InvCTM = (CTM)^(-1)
		double pInvCTM[6];
		double dDet = 1 / (pCTM[0] * pCTM[3] - pCTM[1] * pCTM[2]);
		pInvCTM[0] =  pCTM[3] * dDet;
		pInvCTM[1] = -pCTM[1] * dDet;
		pInvCTM[2] = -pCTM[2] * dDet;
		pInvCTM[3] =  pCTM[0] * dDet;
		pInvCTM[4] = (pCTM[2] * pCTM[5] - pCTM[3] * pCTM[4]) * dDet;
		pInvCTM[5] = (pCTM[1] * pCTM[4] - pCTM[0] * pCTM[5]) * dDet;
		// TempMatrix = PatternMatrix * BaseMatrix
		double pTempMatrix[6];
		pTempMatrix[0] = pPatternMatrix[0] * pBaseMatrix[0] + pPatternMatrix[1] * pBaseMatrix[2];
		pTempMatrix[1] = pPatternMatrix[0] * pBaseMatrix[1] + pPatternMatrix[1] * pBaseMatrix[3];
		pTempMatrix[2] = pPatternMatrix[2] * pBaseMatrix[0] + pPatternMatrix[3] * pBaseMatrix[2];
		pTempMatrix[3] = pPatternMatrix[2] * pBaseMatrix[1] + pPatternMatrix[3] * pBaseMatrix[3];
		pTempMatrix[4] = pPatternMatrix[4] * pBaseMatrix[0] + pPatternMatrix[5] * pBaseMatrix[2] + pBaseMatrix[4];
		pTempMatrix[5] = pPatternMatrix[4] * pBaseMatrix[1] + pPatternMatrix[5] * pBaseMatrix[3] + pBaseMatrix[5];
		// Matrix = TempMatrix * InvCTM
		double pMatrix[6];
		pMatrix[0] = pTempMatrix[0] * pInvCTM[0] + pTempMatrix[1] * pInvCTM[2];
		pMatrix[1] = pTempMatrix[0] * pInvCTM[1] + pTempMatrix[1] * pInvCTM[3];
		pMatrix[2] = pTempMatrix[2] * pInvCTM[0] + pTempMatrix[3] * pInvCTM[2];
		pMatrix[3] = pTempMatrix[2] * pInvCTM[1] + pTempMatrix[3] * pInvCTM[3];
		pMatrix[4] = pTempMatrix[4] * pInvCTM[0] + pTempMatrix[5] * pInvCTM[2] + pInvCTM[4];
		pMatrix[5] = pTempMatrix[4] * pInvCTM[1] + pTempMatrix[5] * pInvCTM[3] + pInvCTM[5];

		// Устанавливаем новую матрицу
		m_pGState->ConcatCTM(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], pMatrix[4], pMatrix[5]);
		m_pOut->UpdateCTM(m_pGState, pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], pMatrix[4], pMatrix[5]);

		bool bAntialias = m_pOut->GetVectorAntialias();
		if (bAntialias)
		{
			m_pOut->SetVectorAntialias(false);
		}

		m_pOut->StartShadedFill(m_pGState);

		// Выполняем закраску, в зависимости от типа Shading
		switch (pShading->GetType())
		{
			case 1:
			DoFunctionShadingFill((GrFunctionShading *)pShading);
			break;
			case 2:
			DoAxialShadingFill((GrAxialShading *)pShading);
			break;
			case 3:
			DoRadialShadingFill((GrRadialShading *)pShading);
			break;
			case 4:
			case 5:
			DoGouraudTriangleShadingFill((GrGouraudTriangleShading *)pShading);
			break;
			case 6:
			case 7:
			DoPatchMeshShadingFill((GrPatchMeshShading *)pShading);
			break;
		}

		m_pOut->EndShadedFill();

		if (bAntialias)
		{
			m_pOut->SetVectorAntialias(true);
		}

		// Восстанавливаем GState
		RestoreGState();
		m_pGState->SetPath(pSavedPath);
	}

	void Graphics::DoFunctionShadingFill(GrFunctionShading *pShading)
	{
		// Сначала предоставляем возможность OuputDevice самому сделать Shading
		if (m_pOut->UseFunctionalShadedFills() && m_pOut->FunctionShadedFill(m_pGState, pShading))
		{
			return;
		}

		// Если Output Device не поддерживает данный ShadingType, тогда делаем его сами
		// с помощью GrPath.
		double dMinX, dMinY, dMaxX, dMaxY;
		GrColor arrColors[4];

		pShading->GetDomain(&dMinX, &dMinY, &dMaxX, &dMaxY);
		pShading->GetColor(dMinX, dMinY, &arrColors[0]);
		pShading->GetColor(dMinX, dMaxY, &arrColors[1]);
		pShading->GetColor(dMaxX, dMinY, &arrColors[2]);
		pShading->GetColor(dMaxX, dMaxY, &arrColors[3]);
		DoFunctionShadingFill(pShading, dMinX, dMinY, dMaxX, dMaxY, arrColors, 0);
	}

	void Graphics::DoFunctionShadingFill(GrFunctionShading *pShading, double dMinX, double dMinY, double dMaxX, double dMaxY, GrColor *pColors, int nDepth)
	{
		if (m_pOut->IsStopped())
			return;

		int nComponentsCount = pShading->GetColorSpace()->GetComponentsCount();

		// Сравниваем цвета в углах
		int nColorIndex = 0;
		for (nColorIndex = 0; nColorIndex < 4; ++nColorIndex)
		{
			int nComponentIndex;
			for (nComponentIndex = 0; nComponentIndex < nComponentsCount; ++nComponentIndex)
			{
				if (abs(pColors[nColorIndex].arrComp[nComponentIndex] - pColors[(nColorIndex + 1) & 3].arrComp[nComponentIndex]) > functionColorDelta)
				{
					break;
				}
			}
			if (nComponentIndex < nComponentsCount)
			{
				break;
			}
		}

		double dCenterX = 0.5 * (dMinX + dMaxX);
		double dCenterY = 0.5 * (dMinY + dMaxY);

		// Угловые вершины прямоугольников  достаточно близки (или мы достигли предела количетсву иттераций)
		// Заливаем прямоугольник. Даже если изначально вершины близки проводим одну иттерацию деления, чтобы
		// случая, когда все 4 вершины имеют одинаковый цвет.
		if ((nColorIndex == 4 && nDepth > 0) || nDepth == functionMaxDepth)
		{
			// Используем цвет центра прямоугольника
			GrColor oFillColor;
			pShading->GetColor(dCenterX, dCenterY, &oFillColor);
			m_pGState->SetFillColor(&oFillColor);
			m_pOut->UpdateFillColor(m_pGState);

			double *pMatrix = pShading->GetMatrix();
			// Заливка прямоугольника
			m_pGState->MoveTo(dMinX * pMatrix[0] + dMinY * pMatrix[2] + pMatrix[4], dMinX * pMatrix[1] + dMinY * pMatrix[3] + pMatrix[5]);
			m_pGState->LineTo(dMaxX * pMatrix[0] + dMinY * pMatrix[2] + pMatrix[4], dMaxX * pMatrix[1] + dMinY * pMatrix[3] + pMatrix[5]);
			m_pGState->LineTo(dMaxX * pMatrix[0] + dMaxY * pMatrix[2] + pMatrix[4], dMaxX * pMatrix[1] + dMaxY * pMatrix[3] + pMatrix[5]);
			m_pGState->LineTo(dMinX * pMatrix[0] + dMaxY * pMatrix[2] + pMatrix[4], dMinX * pMatrix[1] + dMaxY * pMatrix[3] + pMatrix[5]);
			m_pGState->ClosePath();
			m_pOut->Fill(m_pGState);
			m_pGState->ClearPath();
		}
		else // 4 угловые вершины не достаточно близки, делим дальше на прямоугольники
		{

			// pColors[0]               oColorCT                    pColors[2]
			//    (dMinX,dMinY)      (dCenterX,dMinY)         (dMaxX,dMinY)
			//          +---------------------+---------------------+
			//          |                     |                     |
			//          |        Top Left     |      Top Right      |
			// oColorLC |             oColorCC|                     | oColorRC
			//          +---------------------+---------------------+
			//    (dMinX,dCenterY)   (dCenterX,dCenterY)      (dMaxX,dCenterY)
			//          |      Bottom Left    |     Bottom Right    |
			//          |                     |                     |
			//          +---------------------+---------------------+
			//    (dMinX,dMaxY)      (dCenterX,dMaxY)         (dMaxX,dMaxY)
			// pColors[1]              oColorCB                     pColors[3]

			GrColor oColorLC, oColorRC, oColorCT, oColorCB, oColorCC, arrSubColors[4];
			pShading->GetColor(dMinX, dCenterY, &oColorLC);
			pShading->GetColor(dMaxX, dCenterY, &oColorRC);
			pShading->GetColor(dCenterX, dMinY, &oColorCT);
			pShading->GetColor(dCenterX, dMaxY, &oColorCB);
			pShading->GetColor(dCenterX, dCenterY, &oColorCC);

			// Верхний левый прямоугольник
			arrSubColors[0] = pColors[0];
			arrSubColors[1] = oColorLC;
			arrSubColors[2] = oColorCT;
			arrSubColors[3] = oColorCC;
			DoFunctionShadingFill(pShading, dMinX, dMinY, dCenterX, dCenterY, arrSubColors, nDepth + 1);

			// Левый нижний прямоугольник
			arrSubColors[0] = oColorLC;
			arrSubColors[1] = pColors[1];
			arrSubColors[2] = oColorCC;
			arrSubColors[3] = oColorCB;
			DoFunctionShadingFill(pShading, dMinX, dCenterY, dCenterX, dMaxY, arrSubColors, nDepth + 1);

			// Правый верхний прямоугольник
			arrSubColors[0] = oColorCT;
			arrSubColors[1] = oColorCC;
			arrSubColors[2] = pColors[2];
			arrSubColors[3] = oColorRC;
			DoFunctionShadingFill(pShading, dCenterX, dMinY, dMaxX, dCenterY, arrSubColors, nDepth + 1);

			// Правый нижний прямоугольник
			arrSubColors[0] = oColorCC;
			arrSubColors[1] = oColorCB;
			arrSubColors[2] = oColorRC;
			arrSubColors[3] = pColors[3];
			DoFunctionShadingFill(pShading, dCenterX, dCenterY, dMaxX, dMaxY, arrSubColors, nDepth + 1);
		}
	}

	void Graphics::DoAxialShadingFill(GrAxialShading *pShading)
	{
		int nJ, nK, nKK;

		// Сначала предоставляем возможность OuputDevice самому сделать Shading
		if (m_pOut->UseAxialShadedFills() && m_pOut->AxialShadedFill(m_pGState, pShading))
		{
			return;
		}

		// get the clip region bbox
		double dMinX, dMinY, dMaxX, dMaxY;
		m_pGState->GetUserClipBBox(&dMinX, &dMinY, &dMaxX, &dMaxY);

		// Вычисляем макимальное и минимальное значения параметра T
		double dTmin, dTmax;
		double dX0, dY0, dX1, dY1;
		pShading->GetCoords(&dX0, &dY0, &dX1, &dY1);
		double dDx = dX1 - dX0;
		double dDy = dY1 - dY0;
		bool bDxZero = fabs(dDx) < 0.01;
		bool bDyZero = fabs(dDy) < 0.01;
		if (bDxZero && bDyZero)
		{
			dTmin = dTmax = 0;
		}
		else
		{
			double dMult = 1 / (dDx * dDx + dDy * dDy);
			dTmin = dTmax = ((dMinX - dX0) * dDx + (dMinY - dY0) * dDy) * dMult;
			double dTemp  = ((dMinX - dX0) * dDx + (dMaxY - dY0) * dDy) * dMult;
			if (dTemp < dTmin)
			{
				dTmin = dTemp;
			}
			else if (dTemp > dTmax)
			{
				dTmax = dTemp;
			}
			dTemp = ((dMaxX - dX0) * dDx + (dMinY - dY0) * dDy) * dMult;
			if (dTemp < dTmin)
			{
				dTmin = dTemp;
			}
			else if (dTemp > dTmax)
			{
				dTmax = dTemp;
			}
			dTemp = ((dMaxX - dX0) * dDx + (dMaxY - dY0) * dDy) * dMult;
			if (dTemp < dTmin)
			{
				dTmin = dTemp;
			}
			else if (dTemp > dTmax)
			{
				dTmax = dTemp;
			}
			if (dTmin < 0 && !pShading->GetExtendStart())
			{
				dTmin = 0;
			}
			if (dTmax > 1 && !pShading->GetExtendEnd())
			{
				dTmax = 1;
			}
		}

		// Считываем пределы параметра T
		double dT0 = pShading->GetDomain0();
		double dT1 = pShading->GetDomain1();

		// Для каждой точки (Tx, Ty) оси, рассмотрим линию ,проходяющую через данную точку перпендикулярно оси:
		//
		//     x(s) = Tx + s * -dDy   -->   s = (x - Tx) / -dDy
		//     y(s) = Ty + s *  dDx    -->  s = (y - Ty) /  dDx
		//
		// Рассмотрим точки пересечения данной линии с внешним ректом(в котором все рисуем Bbox). В общем случае
		// мы имеем 4 точки пересечения:
		//
		//     s0 = (dMinX - Tx) / -dDy
		//     s1 = (dMaxX - Tx) / -dDy
		//     s2 = (dMinY - Ty) /  dDx
		//     s3 = (dMaxY - Ty) /  dDx
		//
		// нас интересуют два средних значения s.
		//
		// В случае, когда dDx = 0, возьмем s0 и s1; в случае, когда dDy = 0, возьмем s2 и s3.
		//
		// Далее каждый полигон, который мы будем заполнять будет ограничен двумя такими линиями, перпендикулярными оси.
		//
		// Делим ось таким образом до тех пор, пока разница цветов между двумя соседними линиями не станет достаточно
		// малой. Далее кажды отдельный полигон закрашиваем одним цветом.

		// Делаем как минимум одну такую иттерацию, чтобы избежать проблем в случае, когда оба конца оси имеют
		// одинаковый цвет.

		int nComponentsCount = pShading->GetColorSpace()->GetComponentsCount();
		double arrTa[axialMaxSplits + 1];
		arrTa[0] = dTmin;
		int arrNext[axialMaxSplits + 1];
		arrNext[0] = axialMaxSplits / 2;
		arrTa[axialMaxSplits / 2] = 0.5 * (dTmin + dTmax);
		arrNext[axialMaxSplits / 2] = axialMaxSplits;
		arrTa[axialMaxSplits] = dTmax;

		// Вычисляем значение цвета при t = dTmin
		double dTt = 0;
		if (dTmin < 0)
		{
			dTt = dT0;
		}
		else if (dTmin > 1)
		{
			dTt = dT1;
		}
		else
		{
			dTt = dT0 + (dT1 - dT0) * dTmin;
		}
		GrColor oColor0, oColor1;
		pShading->GetColor(dTt, &oColor0);

		// Вычисляем координаты точки, лежаще на оси, при значении параметра t = dTmin;
		// после этого вычисляем координаты точек пересечения для линии при t = dTmin.
		double dTx = dX0 + dTmin * dDx;
		double dTy = dY0 + dTmin * dDy;
		double arrS[4];

		double dSmin, dSmax;

		if (bDxZero && bDyZero)
		{
			dSmin = dSmax = 0;
		}
		else if (bDxZero)
		{
			dSmin = (dMinX - dTx) / -dDy;
			dSmax = (dMaxX - dTx) / -dDy;
			if (dSmin > dSmax)
			{
				double dTemp = dSmin;
				dSmin = dSmax;
				dSmax = dTemp;
			}
		}
		else if (bDyZero)
		{
			dSmin = (dMinY - dTy) / dDx;
			dSmax = (dMaxY - dTy) / dDx;
			if (dSmin > dSmax)
			{
				double dTemp = dSmin;
				dSmin = dSmax;
				dSmax = dTemp;
			}
		}
		else
		{
			arrS[0] = (dMinY - dTy) / dDx;
			arrS[1] = (dMaxY - dTy) / dDx;
			arrS[2] = (dMinX - dTx) / -dDy;
			arrS[3] = (dMaxX - dTx) / -dDy;
			for (nJ = 0; nJ < 3; ++nJ)
			{
				nKK = nJ;
				for (nK = nJ + 1; nK < 4; ++nK)
				{
					if (arrS[nK] < arrS[nKK])
					{
						nKK = nK;
					}
				}
				double dTemp = arrS[nJ];
				arrS[nJ] = arrS[nKK];
				arrS[nKK] = dTemp;
			}
			dSmin = arrS[1];
			dSmax = arrS[2];
		}
		double dStartX0 = dTx - dSmin * dDy;
		double dStartY0 = dTy + dSmin * dDx;
		double dEndX0 = dTx - dSmax * dDy;
		double dEndY0 = dTy + dSmax * dDx;

		int nSplitsCount = 0;
		while (nSplitsCount < axialMaxSplits)
		{
			if (m_pOut->IsStopped())
				return;

			// Делим пока разница цветов не станет достаточной маленькой или пока мы не достигнем предела количеству иттераций
			nJ = arrNext[nSplitsCount];
			while (nJ > nSplitsCount + 1)
			{
				if (m_pOut->IsStopped())
					return;

				if (arrTa[nJ] < 0)
				{
					dTt = dT0;
				}
				else if (arrTa[nJ] > 1)
				{
					dTt = dT1;
				}
				else
				{
					dTt = dT0 + (dT1 - dT0) * arrTa[nJ];
				}
				pShading->GetColor(dTt, &oColor1);
				for (nK = 0; nK < nComponentsCount; ++nK)
				{
					if (abs(oColor1.arrComp[nK] - oColor0.arrComp[nK]) > axialColorDelta)
					{
						break;
					}
				}
				if (nK == nComponentsCount)
				{
					break;
				}
				nK = (nSplitsCount + nJ) / 2;
				arrTa[nK] = 0.5 * (arrTa[nSplitsCount] + arrTa[nJ]);
				arrNext[nSplitsCount] = nK;
				arrNext[nK] = nJ;
				nJ = nK;
			}

			// Используем среднее значение цвета
			for (nK = 0; nK < nComponentsCount; ++nK)
			{
				oColor0.arrComp[nK] = (oColor0.arrComp[nK] + oColor1.arrComp[nK]) / 2;
			}

			// Вычисляем координаты точки, лежаще на оси, при данном значении параметра t;
			// после этого вычисляем координаты точек пересечения для линии данном t.
			dTx = dX0 + arrTa[nJ] * dDx;
			dTy = dY0 + arrTa[nJ] * dDy;
			if (bDxZero && bDyZero)
			{
				dSmin = dSmax = 0;
			}
			else if (bDxZero)
			{
				dSmin = (dMinX - dTx) / -dDy;
				dSmax = (dMaxX - dTx) / -dDy;
				if (dSmin > dSmax)
				{
					double dTemp = dSmin;
					dSmin = dSmax;
					dSmax = dTemp;
				}
			}
			else if (bDyZero)
			{
				dSmin = (dMinY - dTy) / dDx;
				dSmax = (dMaxY - dTy) / dDx;
				if (dSmin > dSmax)
				{
					double dTemp = dSmin;
					dSmin = dSmax;
					dSmax = dTemp;
				}
			}
			else
			{
				arrS[0] = (dMinY - dTy) / dDx;
				arrS[1] = (dMaxY - dTy) / dDx;
				arrS[2] = (dMinX - dTx) / -dDy;
				arrS[3] = (dMaxX - dTx) / -dDy;
				for (nJ = 0; nJ < 3; ++nJ)
				{
					nKK = nJ;
					for (nK = nJ + 1; nK < 4; ++nK)
					{
						if (arrS[nK] < arrS[nKK])
						{
							nKK = nK;
						}
					}
					double dTemp = arrS[nJ];
					arrS[nJ] = arrS[nKK];
					arrS[nKK] = dTemp;
				}
				dSmin = arrS[1];
				dSmax = arrS[2];
			}
			double dStartX1 = dTx - dSmin * dDy;
			double dStartY1 = dTy + dSmin * dDx;
			double dEndX1 = dTx - dSmax * dDy;
			double dEndY1 = dTy + dSmax * dDx;

			// Устанавливаем цвет
			m_pGState->SetFillColor(&oColor0);
			m_pOut->UpdateFillColor(m_pGState);

			// Закрашиваем
			m_pGState->MoveTo(dStartX0, dStartY0);
			m_pGState->LineTo(dEndX0, dEndY0);
			m_pGState->LineTo(dEndX1, dEndY1);
			m_pGState->LineTo(dStartX1, dStartY1);
			m_pGState->ClosePath();
			m_pOut->Fill(m_pGState);
			m_pGState->ClearPath();

			// Начальные значения для следующего полигона
			dStartX0 = dStartX1;
			dStartY0 = dStartY1;
			dEndX0 = dEndX1;
			dEndY0 = dEndY1;
			oColor0 = oColor1;
			nSplitsCount = arrNext[nSplitsCount];
		}
	}

	void Graphics::DoRadialShadingFill(GrRadialShading *pShading)
	{
		// Сначала предоставляем возможность OuputDevice самому сделать Shading
		if (m_pOut->UseRadialShadedFills() && m_pOut->RadialShadedFill(m_pGState, pShading))
		{
			return;
		}
		// Если Output Device не поддерживает данный ShadingType, тогда делаем его сами
		// с помощью GrPath.

		double dFirstX, dFirstY, dFirstRad, dSecondX, dSecondY, dSecondRad;
		pShading->GetCoords(&dFirstX, &dFirstY, &dFirstRad, &dSecondX, &dSecondY, &dSecondRad);
		double dT0 = pShading->GetDomain0();
		double dT1 = pShading->GetDomain1();
		int nComponentsCount = pShading->GetColorSpace()->GetComponentsCount();

		// Вычисляем точку, в которой r(s) = 0; проверяме вложенность окружностей; и 
		// вычисляем углы тангенциальных линий
		bool bEnclosed = false;
		double dTheta = 0, dAlpha = 0;;
		double dZeroS = 0;
		if (dFirstX == dSecondX && dFirstY == dSecondY)
		{
			bEnclosed = true;
			dTheta = 0;
			dZeroS = 0;
		}
		else if (dFirstRad == dSecondRad)
		{
			bEnclosed = false;
			dTheta = 0;
			dZeroS = 0;
		}
		else
		{
			dZeroS = -dFirstRad / (dSecondRad - dFirstRad);
			double dZeroX = dFirstX + dZeroS * (dSecondX - dFirstX);
			double dZeroY = dFirstY + dZeroS * (dSecondY - dFirstY);
			bEnclosed = ((dZeroX - dFirstX) * (dZeroX - dFirstX) + (dZeroY - dFirstY) * (dZeroY - dFirstY) <= dFirstRad * dFirstRad);
			dTheta = asin(dFirstRad / sqrt((dFirstX - dZeroX) * (dFirstX - dZeroX) + (dFirstY - dZeroY) * (dFirstY - dZeroY)));
			if (dFirstRad > dSecondRad)
			{
				dTheta = -dTheta;
			}
		}
		if (bEnclosed)
		{
			dAlpha = 0;
		}
		else
		{
			dAlpha = atan2(dSecondY - dFirstY, dSecondX - dFirstX);
		}

		double dXMin, dYMin, dXMax, dYMax;
		m_pGState->GetUserClipBBox(&dXMin, &dYMin, &dXMax, &dYMax);

		double dSmin, dSmax;
		if (bEnclosed)
		{
			dSmin = 0;
			dSmax = 1;
		}
		else
		{
			dSmin = 1;
			dSmax = 0;
			// x(s) + r(s) = dXMin
			if ((dSecondX + dSecondRad) - (dFirstX + dFirstRad) != 0)
			{
				double dTempS = (dXMin - (dFirstX + dFirstRad)) / ((dSecondX + dSecondRad) - (dFirstX + dFirstRad));
				if (dTempS < dSmin)
				{
					dSmin = dTempS;
				}
				else if (dTempS > dSmax)
				{
					dSmax = dTempS;
				}
			}
			// x(s) - r(s) = dXMax
			if ((dSecondX - dSecondRad) - (dFirstX - dFirstRad) != 0)
			{
				double dTempS = (dXMax - (dFirstX - dFirstRad)) / ((dSecondX - dSecondRad) - (dFirstX - dFirstRad));
				if (dTempS < dSmin)
				{
					dSmin = dTempS;
				}
				else if (dTempS > dSmax)
				{
					dSmax = dTempS;
				}
			}
			// y(s) + r(s) = dYMin
			if ((dSecondY + dSecondRad) - (dFirstY + dFirstRad) != 0)
			{
				double dTempS = (dYMin - (dFirstY + dFirstRad)) / ((dSecondY + dSecondRad) - (dFirstY + dFirstRad));
				if (dTempS < dSmin)
				{
					dSmin = dTempS;
				}
				else if (dTempS > dSmax)
				{
					dSmax = dTempS;
				}
			}
			// y(s) - r(s) = dYMax
			if ((dSecondY - dSecondRad) - (dFirstY - dFirstRad) != 0)
			{
				double dTempS = (dYMax - (dFirstY - dFirstRad)) / ((dSecondY - dSecondRad) - (dFirstY - dFirstRad));
				if (dTempS < dSmin)
				{
					dSmin = dTempS;
				}
				else if (dTempS > dSmax)
				{
					dSmax = dTempS;
				}
			}

			// Проверяем относительно dZeroS
			if (dFirstRad < dSecondRad)
			{
				if (dSmin < dZeroS)
				{
					dSmin = dZeroS;
				}
			}
			else if (dFirstRad > dSecondRad)
			{
				if (dSmax > dZeroS)
				{
					dSmax = dZeroS;
				}
			}

			if (!pShading->GetExtendFirst() && dSmin < 0)
			{
				dSmin = 0;
			}
			if (!pShading->GetExtendSecond() && dSmax > 1)
			{
				dSmax = 1;
			}
		}

		double *pCTM = m_pGState->GetCTM();
		double dTemp = fabs(pCTM[0]);
		if (fabs(pCTM[1]) > dTemp)
		{
			dTemp = fabs(pCTM[1]);
		}
		if (fabs(pCTM[2]) > dTemp)
		{
			dTemp = fabs(pCTM[2]);
		}
		if (fabs(pCTM[3]) > dTemp)
		{
			dTemp = fabs(pCTM[3]);
		}
		if (dFirstRad > dSecondRad)
		{
			dTemp *= dFirstRad;
		}
		else
		{
			dTemp *= dSecondRad;
		}
		int nStepsCount = 0;
		if (dTemp < 1)
		{
			nStepsCount = 3;
		}
		else
		{
			nStepsCount = (int)(M_PI / acos(1 - 0.1 / dTemp));
			if (nStepsCount < 3)
			{
				nStepsCount = 3;
			}
			else if (nStepsCount > 200)
			{
				nStepsCount = 200;
			}
		}

		// Везде далее A - первая окружность, B - вторая
		int    nIndexA = 0;
		double dSA = dSmin;
		double dTA = dT0 + dSA * (dT1 - dT0);
		double dXA = dFirstX + dSA * (dSecondX - dFirstX);
		double dYA = dFirstY + dSA * (dSecondY - dFirstY);
		double dRadA = dFirstRad + dSA * (dSecondRad - dFirstRad);
		double dAngle = 0;
		int nCounter = 0;
		GrColor oColorA;
		if (dTA < dT0)
		{
			pShading->GetColor(dT0, &oColorA);
		}
		else if (dTA > dT1)
		{
			pShading->GetColor(dT1, &oColorA);
		}
		else
		{
			pShading->GetColor(dTA, &oColorA);
		}

		while (nIndexA < radialMaxSplits)
		{
			if (m_pOut->IsStopped())
				return;

			int nIndexB = radialMaxSplits;
			double dSB = dSmax;
			double dTB = dT0 + dSB * (dT1 - dT0);
			GrColor oColorB;
			if (dTB < dT0)
			{
				pShading->GetColor(dT0, &oColorB);
			}
			else if (dTB > dT1)
			{
				pShading->GetColor(dT1, &oColorB);
			}
			else
			{
				pShading->GetColor(dTB, &oColorB);
			}

			while (nIndexB - nIndexA > 1)
			{
				if (m_pOut->IsStopped())
					return;

				for (nCounter = 0; nCounter < nComponentsCount; ++nCounter)
				{
					if (abs(oColorB.arrComp[nCounter] - oColorA.arrComp[nCounter]) > radialColorDelta)
					{
						break;
					}
				}
				if (nCounter == nComponentsCount && nIndexB < radialMaxSplits)
				{
					break;
				}
				nIndexB = (nIndexA + nIndexB) / 2;
				dSB = dSmin + ((double)nIndexB / (double)radialMaxSplits) * (dSmax - dSmin);
				dTB = dT0 + dSB * (dT1 - dT0);
				if (dTB < dT0)
				{
					pShading->GetColor(dT0, &oColorB);
				}
				else if (dTB > dT1)
				{
					pShading->GetColor(dT1, &oColorB);
				}
				else
				{
					pShading->GetColor(dTB, &oColorB);
				}
			}

			// Вычислим центр и радиус окружности
			double dXB = dFirstX + dSB * (dSecondX - dFirstX);
			double dYB = dFirstY + dSB * (dSecondY - dFirstY);
			double dRadB = dFirstRad + dSB * (dSecondRad - dFirstRad);

			// Используем среднее значение цвета двух окружностей
			for (nCounter = 0; nCounter < nComponentsCount; ++nCounter)
			{
				oColorA.arrComp[nCounter] = (oColorA.arrComp[nCounter] + oColorB.arrComp[nCounter]) / 2;
			}
			m_pGState->SetFillColor(&oColorA);
			m_pOut->UpdateFillColor(m_pGState);

			if (bEnclosed)
			{

				// Строим Path для первой окружности (против часовой)
				m_pGState->MoveTo(dXA + dRadA, dYA);
				for (nCounter = 1; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = ((double)nCounter / (double)nStepsCount) * 2 * M_PI;
					m_pGState->LineTo(dXA + dRadA * cos(dAngle), dYA + dRadA * sin(dAngle));
				}
				m_pGState->ClosePath();

				// Строим Path для второй окружности (по часовой)
				m_pGState->MoveTo(dXB + dRadB, dYB);
				for (nCounter = 1; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = -((double)nCounter / (double)nStepsCount) * 2 * M_PI;
					m_pGState->LineTo(dXB + dRadB * cos(dAngle), dYB + dRadB * sin(dAngle));
				}
				m_pGState->ClosePath();

			}
			else
			{

				// Строим первый subpath (по часовой)
				m_pGState->MoveTo(dXA + dRadA * cos(dAlpha + dTheta + 0.5 * M_PI), dYA + dRadA * sin(dAlpha + dTheta + 0.5 * M_PI));
				for (nCounter = 0; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = dAlpha + dTheta + 0.5 * M_PI - ((double)nCounter / (double)nStepsCount) * (2 * dTheta + M_PI);
					m_pGState->LineTo(dXB + dRadB * cos(dAngle), dYB + dRadB * sin(dAngle));
				}
				for (nCounter = 0; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = dAlpha - dTheta - 0.5 * M_PI + ((double)nCounter / (double)nStepsCount) * (2 * dTheta - M_PI);
					m_pGState->LineTo(dXA + dRadA * cos(dAngle), dYA + dRadA * sin(dAngle));
				}
				m_pGState->ClosePath();

				// Строим второй subpath (против часовой)
				m_pGState->MoveTo(dXA + dRadA * cos(dAlpha + dTheta + 0.5 * M_PI), dYA + dRadA * sin(dAlpha + dTheta + 0.5 * M_PI));
				for (nCounter = 0; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = dAlpha + dTheta + 0.5 * M_PI + ((double)nCounter / (double)nStepsCount) * (-2 * dTheta + M_PI);
					m_pGState->LineTo(dXB + dRadB * cos(dAngle), dYB + dRadB * sin(dAngle));
				}
				for (nCounter = 0; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = dAlpha - dTheta - 0.5 * M_PI + ((double)nCounter / (double)nStepsCount) * (2 * dTheta + M_PI);
					m_pGState->LineTo(dXA + dRadA * cos(dAngle), dYA + dRadA * sin(dAngle));
				}
				m_pGState->ClosePath();
			}

			// Закрашиваем
			m_pOut->Fill(m_pGState);
			m_pGState->ClearPath();

			// Начальные данные для следующего шага
			nIndexA = nIndexB;
			dSA = dSB;
			dTA = dTB;
			dXA = dXB;
			dYA = dYB;
			dRadA = dRadB;
			oColorA = oColorB;
		}

		// Если выставлены флаги продолжать рисовать за пределами двух окружностей
		if (bEnclosed)
		{
			// Продолжаем за меньшую окружность
			if ((pShading->GetExtendFirst() && dFirstRad <= dSecondRad) || (pShading->GetExtendSecond() && dSecondRad < dFirstRad))
			{
				if (dFirstRad <= dSecondRad)
				{
					dTA = dT0;
					dRadA = dFirstRad;
					dXA = dFirstX;
					dYA = dFirstY;
				}
				else
				{
					dTA = dT1;
					dRadA = dSecondRad;
					dXA = dSecondX;
					dYA = dSecondY;
				}
				pShading->GetColor(dTA, &oColorA);
				m_pGState->SetFillColor(&oColorA);
				m_pOut->UpdateFillColor(m_pGState);
				m_pGState->MoveTo(dXA + dRadA, dYA);
				for (nCounter = 1; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = ((double)nCounter / (double)nStepsCount) * 2 * M_PI;
					m_pGState->LineTo(dXA + dRadA * cos(dAngle), dYA + dRadA * sin(dAngle));
				}
				m_pGState->ClosePath();
				m_pOut->Fill(m_pGState);
				m_pGState->ClearPath();
			}

			// Продолжаем за большую окружность
			if ((pShading->GetExtendFirst() && dFirstRad > dSecondRad) || (pShading->GetExtendSecond() && dSecondRad >= dFirstRad))
			{
				if (dFirstRad > dSecondRad)
				{
					dTA = dT0;
					dRadA = dFirstRad;
					dXA = dFirstX;
					dYA = dFirstY;
				}
				else
				{
					dTA = dT1;
					dRadA = dSecondRad;
					dXA = dSecondX;
					dYA = dSecondY;
				}
				pShading->GetColor(dTA, &oColorA);
				m_pGState->SetFillColor(&oColorA);
				m_pOut->UpdateFillColor(m_pGState);
				m_pGState->MoveTo(dXMin, dYMin);
				m_pGState->LineTo(dXMin, dYMax);
				m_pGState->LineTo(dXMax, dYMax);
				m_pGState->LineTo(dXMax, dYMin);
				m_pGState->ClosePath();
				m_pGState->MoveTo(dXA + dRadA, dYA);
				for (nCounter = 1; nCounter < nStepsCount; ++nCounter)
				{
					dAngle = ((double)nCounter / (double)nStepsCount) * 2 * M_PI;
					m_pGState->LineTo(dXA + dRadA * cos(dAngle), dYA + dRadA * sin(dAngle));
				}
				m_pGState->ClosePath();
				m_pOut->Fill(m_pGState);
				m_pGState->ClearPath();
			}
		}
	}

	void Graphics::DoGouraudTriangleShadingFill(GrGouraudTriangleShading *pShading)
	{
		double dAx, dAy, dBx, dBy, dCx, dCy;
		GrColor oColorA, oColorB, oColorC;
		for (int nIndex = 0; nIndex < pShading->GetTrianglesCount(); ++nIndex)
		{
			pShading->GetTriangle(nIndex, &dAx, &dAy, &oColorA, &dBx, &dBy, &oColorB, &dCx, &dCy, &oColorC);
			GouraudFillTriangle(dAx, dAy, &oColorA, dBx, dBy, &oColorB, dCx, dCy, &oColorC, pShading->GetColorSpace()->GetComponentsCount(), 0);
		}
	}

	void Graphics::GouraudFillTriangle(double dAx, double dAy, GrColor *pColorA, double dBx, double dBy, GrColor *pColorB, double dCx, double dCy, GrColor *pColorC, int nComponentsCount, int nDepth)
	{
		if (m_pOut->IsStopped())
			return;

		double dABx, dABy, dBCx, dBCy, dACx, dACy;
		GrColor oColorAB, oColorBC, oColorAC;
		int nIndex;

		for (nIndex = 0; nIndex < nComponentsCount; ++nIndex)
		{
			if (abs(pColorA->arrComp[nIndex] - pColorB->arrComp[nIndex]) > gouraudColorDelta || abs(pColorB->arrComp[nIndex] - pColorC->arrComp[nIndex]) > gouraudColorDelta)
			{
				break;
			}
		}
		if (nIndex == nComponentsCount || nDepth == gouraudMaxDepth)
		{
			m_pGState->SetFillColor(pColorA);
			m_pOut->UpdateFillColor(m_pGState);
			m_pGState->MoveTo(dAx, dAy);
			m_pGState->LineTo(dBx, dBy);
			m_pGState->LineTo(dCx, dCy);
			m_pGState->ClosePath();
			m_pOut->Fill(m_pGState);
			m_pGState->ClearPath();
		}
		else
		{
			dABx = 0.5 * (dAx + dBx);
			dABy = 0.5 * (dAy + dBy);
			dBCx = 0.5 * (dBx + dCx);
			dBCy = 0.5 * (dBy + dCy);
			dACx = 0.5 * (dCx + dAx);
			dACy = 0.5 * (dCy + dAy);
			for (nIndex = 0; nIndex < nComponentsCount; ++nIndex)
			{
				oColorAB.arrComp[nIndex] = (pColorA->arrComp[nIndex] + pColorB->arrComp[nIndex]) / 2;
				oColorBC.arrComp[nIndex] = (pColorB->arrComp[nIndex] + pColorC->arrComp[nIndex]) / 2;
				oColorAC.arrComp[nIndex] = (pColorC->arrComp[nIndex] + pColorA->arrComp[nIndex]) / 2;
			}
			GouraudFillTriangle(dAx, dAy, pColorA, dABx, dABy, &oColorAB, dACx, dACy, &oColorAC, nComponentsCount, nDepth + 1);
			GouraudFillTriangle(dABx, dABy, &oColorAB, dBx, dBy, pColorB, dBCx, dBCy, &oColorBC, nComponentsCount, nDepth + 1);
			GouraudFillTriangle(dABx, dABy, &oColorAB, dBCx, dBCy, &oColorBC, dACx, dACy, &oColorAC, nComponentsCount, nDepth + 1);
			GouraudFillTriangle(dACx, dACy, &oColorAC, dBCx, dBCy, &oColorBC, dCx, dCy, pColorC, nComponentsCount, nDepth + 1);
		}
	}

	void Graphics::DoPatchMeshShadingFill(GrPatchMeshShading *pShading)
	{
		// Устанавливаем цветовое пространство	 
		m_pGState->SetStrokeColorSpace(pShading->GetColorSpace()->Copy());
		m_pOut->UpdateStrokeColorSpace(m_pGState);

		int nStart;

		if (pShading->GetPatchesCount() > 128)
		{
			nStart = 3;
		}
		else if (pShading->GetPatchesCount() > 64)
		{
			nStart = 2;
		}
		else if (pShading->GetPatchesCount() > 16)
		{
			nStart = 1;
		}
		else
		{
			nStart = 0;
		}
		for (int nIndex = 0; nIndex < pShading->GetPatchesCount(); ++nIndex)
		{
			MeshFillPatch(pShading->GetPatch(nIndex), pShading->GetColorSpace()->GetComponentsCount(), nStart);
		}
	}

	void Graphics::MeshFillPatch(GrPatch *pPatch, int nComponentsCount, int nDepth)
	{
		if (m_pOut->IsStopped())
			return;

		GrPatch oPatch00, oPatch01, oPatch10, oPatch11;
		double arrX[4][8], arrY[4][8];
		double dMidX, dMidY;
		int nIndex;

		for (nIndex = 0; nIndex < nComponentsCount; ++nIndex)
		{
			if (abs(pPatch->arrColor[0][0].arrComp[nIndex] - pPatch->arrColor[0][1].arrComp[nIndex]) > patchColorDelta ||
				abs(pPatch->arrColor[0][1].arrComp[nIndex] - pPatch->arrColor[1][1].arrComp[nIndex]) > patchColorDelta ||
				abs(pPatch->arrColor[1][1].arrComp[nIndex] - pPatch->arrColor[1][0].arrComp[nIndex]) > patchColorDelta ||
				abs(pPatch->arrColor[1][0].arrComp[nIndex] - pPatch->arrColor[0][0].arrComp[nIndex]) > patchColorDelta
				)
			{
				break;
			}
		}
		if (nIndex == nComponentsCount || nDepth == patchMaxDepth)
		{
			m_pGState->SetFillColor(&pPatch->arrColor[0][0]);
			m_pGState->SetStrokeColor(&pPatch->arrColor[0][0]);
			m_pOut->UpdateFillColor(m_pGState);
			m_pOut->UpdateStrokeColor(m_pGState);
			m_pGState->MoveTo(pPatch->arrX[0][0], pPatch->arrY[0][0]);
			m_pGState->CurveTo(pPatch->arrX[0][1], pPatch->arrY[0][1], pPatch->arrX[0][2], pPatch->arrY[0][2], pPatch->arrX[0][3], pPatch->arrY[0][3]);
			m_pGState->CurveTo(pPatch->arrX[1][3], pPatch->arrY[1][3], pPatch->arrX[2][3], pPatch->arrY[2][3], pPatch->arrX[3][3], pPatch->arrY[3][3]);
			m_pGState->CurveTo(pPatch->arrX[3][2], pPatch->arrY[3][2], pPatch->arrX[3][1], pPatch->arrY[3][1], pPatch->arrX[3][0], pPatch->arrY[3][0]);
			m_pGState->CurveTo(pPatch->arrX[2][0], pPatch->arrY[2][0], pPatch->arrX[1][0], pPatch->arrY[1][0], pPatch->arrX[0][0], pPatch->arrY[0][0]);
			m_pGState->ClosePath();
			m_pOut->FillStroke(m_pGState);
			m_pGState->ClearPath();
		}
		else
		{
			for (nIndex = 0; nIndex < 4; ++nIndex)
			{
				arrX[nIndex][0] = pPatch->arrX[nIndex][0];
				arrY[nIndex][0] = pPatch->arrY[nIndex][0];
				arrX[nIndex][1] = 0.5 * (pPatch->arrX[nIndex][0] + pPatch->arrX[nIndex][1]);
				arrY[nIndex][1] = 0.5 * (pPatch->arrY[nIndex][0] + pPatch->arrY[nIndex][1]);
				dMidX = 0.5 * (pPatch->arrX[nIndex][1] + pPatch->arrX[nIndex][2]);
				dMidY = 0.5 * (pPatch->arrY[nIndex][1] + pPatch->arrY[nIndex][2]);
				arrX[nIndex][6] = 0.5 * (pPatch->arrX[nIndex][2] + pPatch->arrX[nIndex][3]);
				arrY[nIndex][6] = 0.5 * (pPatch->arrY[nIndex][2] + pPatch->arrY[nIndex][3]);
				arrX[nIndex][2] = 0.5 * (arrX[nIndex][1] + dMidX);
				arrY[nIndex][2] = 0.5 * (arrY[nIndex][1] + dMidY);
				arrX[nIndex][5] = 0.5 * (dMidX + arrX[nIndex][6]);
				arrY[nIndex][5] = 0.5 * (dMidY + arrY[nIndex][6]);
				arrX[nIndex][3] = arrX[nIndex][4] = 0.5 * (arrX[nIndex][2] + arrX[nIndex][5]);
				arrY[nIndex][3] = arrY[nIndex][4] = 0.5 * (arrY[nIndex][2] + arrY[nIndex][5]);
				arrX[nIndex][7] = pPatch->arrX[nIndex][3];
				arrY[nIndex][7] = pPatch->arrY[nIndex][3];
			}
			for (nIndex = 0; nIndex < 4; ++nIndex)
			{
				oPatch00.arrX[0][nIndex] = arrX[0][nIndex];
				oPatch00.arrY[0][nIndex] = arrY[0][nIndex];
				oPatch00.arrX[1][nIndex] = 0.5 * (arrX[0][nIndex] + arrX[1][nIndex]);
				oPatch00.arrY[1][nIndex] = 0.5 * (arrY[0][nIndex] + arrY[1][nIndex]);
				dMidX = 0.5 * (arrX[1][nIndex] + arrX[2][nIndex]);
				dMidY = 0.5 * (arrY[1][nIndex] + arrY[2][nIndex]);
				oPatch10.arrX[2][nIndex] = 0.5 * (arrX[2][nIndex] + arrX[3][nIndex]);
				oPatch10.arrY[2][nIndex] = 0.5 * (arrY[2][nIndex] + arrY[3][nIndex]);
				oPatch00.arrX[2][nIndex] = 0.5 * (oPatch00.arrX[1][nIndex] + dMidX);
				oPatch00.arrY[2][nIndex] = 0.5 * (oPatch00.arrY[1][nIndex] + dMidY);
				oPatch10.arrX[1][nIndex] = 0.5 * (dMidX + oPatch10.arrX[2][nIndex]);
				oPatch10.arrY[1][nIndex] = 0.5 * (dMidY + oPatch10.arrY[2][nIndex]);
				oPatch00.arrX[3][nIndex] = 0.5 * (oPatch00.arrX[2][nIndex] + oPatch10.arrX[1][nIndex]);
				oPatch00.arrY[3][nIndex] = 0.5 * (oPatch00.arrY[2][nIndex] + oPatch10.arrY[1][nIndex]);
				oPatch10.arrX[0][nIndex] = oPatch00.arrX[3][nIndex];
				oPatch10.arrY[0][nIndex] = oPatch00.arrY[3][nIndex];
				oPatch10.arrX[3][nIndex] = arrX[3][nIndex];
				oPatch10.arrY[3][nIndex] = arrY[3][nIndex];
			}
			for (nIndex = 4; nIndex < 8; ++nIndex)
			{
				oPatch01.arrX[0][nIndex - 4] = arrX[0][nIndex];
				oPatch01.arrY[0][nIndex - 4] = arrY[0][nIndex];
				oPatch01.arrX[1][nIndex - 4] = 0.5 * (arrX[0][nIndex] + arrX[1][nIndex]);
				oPatch01.arrY[1][nIndex - 4] = 0.5 * (arrY[0][nIndex] + arrY[1][nIndex]);
				dMidX = 0.5 * (arrX[1][nIndex] + arrX[2][nIndex]);
				dMidY = 0.5 * (arrY[1][nIndex] + arrY[2][nIndex]);
				oPatch11.arrX[2][nIndex - 4] = 0.5 * (arrX[2][nIndex] + arrX[3][nIndex]);
				oPatch11.arrY[2][nIndex - 4] = 0.5 * (arrY[2][nIndex] + arrY[3][nIndex]);
				oPatch01.arrX[2][nIndex - 4] = 0.5 * (oPatch01.arrX[1][nIndex - 4] + dMidX);
				oPatch01.arrY[2][nIndex - 4] = 0.5 * (oPatch01.arrY[1][nIndex - 4] + dMidY);
				oPatch11.arrX[1][nIndex - 4] = 0.5 * (dMidX + oPatch11.arrX[2][nIndex - 4]);
				oPatch11.arrY[1][nIndex - 4] = 0.5 * (dMidY + oPatch11.arrY[2][nIndex - 4]);
				oPatch01.arrX[3][nIndex - 4] = 0.5 * (oPatch01.arrX[2][nIndex - 4] + oPatch11.arrX[1][nIndex - 4]);
				oPatch01.arrY[3][nIndex - 4] = 0.5 * (oPatch01.arrY[2][nIndex - 4] + oPatch11.arrY[1][nIndex - 4]);
				oPatch11.arrX[0][nIndex - 4] = oPatch01.arrX[3][nIndex - 4];
				oPatch11.arrY[0][nIndex - 4] = oPatch01.arrY[3][nIndex - 4];
				oPatch11.arrX[3][nIndex - 4] = arrX[3][nIndex];
				oPatch11.arrY[3][nIndex - 4] = arrY[3][nIndex];
			}
			for (nIndex = 0; nIndex < nComponentsCount; ++nIndex)
			{
				oPatch00.arrColor[0][0].arrComp[nIndex] = pPatch->arrColor[0][0].arrComp[nIndex];
				oPatch00.arrColor[0][1].arrComp[nIndex] = (pPatch->arrColor[0][0].arrComp[nIndex] + pPatch->arrColor[0][1].arrComp[nIndex]) / 2;
				oPatch01.arrColor[0][0].arrComp[nIndex] = oPatch00.arrColor[0][1].arrComp[nIndex];
				oPatch01.arrColor[0][1].arrComp[nIndex] = pPatch->arrColor[0][1].arrComp[nIndex];
				oPatch01.arrColor[1][1].arrComp[nIndex] = (pPatch->arrColor[0][1].arrComp[nIndex] + pPatch->arrColor[1][1].arrComp[nIndex]) / 2;
				oPatch11.arrColor[0][1].arrComp[nIndex] = oPatch01.arrColor[1][1].arrComp[nIndex];
				oPatch11.arrColor[1][1].arrComp[nIndex] = pPatch->arrColor[1][1].arrComp[nIndex];
				oPatch11.arrColor[1][0].arrComp[nIndex] = (pPatch->arrColor[1][1].arrComp[nIndex] + pPatch->arrColor[1][0].arrComp[nIndex]) / 2;
				oPatch10.arrColor[1][1].arrComp[nIndex] = oPatch11.arrColor[1][0].arrComp[nIndex];
				oPatch10.arrColor[1][0].arrComp[nIndex] = pPatch->arrColor[1][0].arrComp[nIndex];
				oPatch10.arrColor[0][0].arrComp[nIndex] = (pPatch->arrColor[1][0].arrComp[nIndex] + pPatch->arrColor[0][0].arrComp[nIndex]) / 2;
				oPatch00.arrColor[1][0].arrComp[nIndex] = oPatch10.arrColor[0][0].arrComp[nIndex];
				oPatch00.arrColor[1][1].arrComp[nIndex] = (oPatch00.arrColor[1][0].arrComp[nIndex] + oPatch01.arrColor[1][1].arrComp[nIndex]) / 2;
				oPatch01.arrColor[1][0].arrComp[nIndex] = oPatch00.arrColor[1][1].arrComp[nIndex];
				oPatch11.arrColor[0][0].arrComp[nIndex] = oPatch00.arrColor[1][1].arrComp[nIndex];
				oPatch10.arrColor[0][1].arrComp[nIndex] = oPatch00.arrColor[1][1].arrComp[nIndex];
			}
			MeshFillPatch(&oPatch00, nComponentsCount, nDepth + 1);
			MeshFillPatch(&oPatch10, nComponentsCount, nDepth + 1);
			MeshFillPatch(&oPatch01, nComponentsCount, nDepth + 1);
			MeshFillPatch(&oPatch11, nComponentsCount, nDepth + 1);
		}
	}

	void Graphics::DoEndPath()
	{
		if (m_pGState->IsCurPoint() && m_eClip != clipNone)
		{
			m_pGState->Clip();
			if (m_eClip == clipNormal)
			{
				m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

				if (m_pOut->UseClipTo())
					m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
				else
					m_pOut->Clip(m_pGState);
			}
			else
			{
				m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), true);

				if (m_pOut->UseClipTo())
					m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), true);
				else
					m_pOut->EoClip(m_pGState);
			}
		}
		m_eClip = clipNone;
		m_pGState->ClearPath();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Clipping paths
	//-------------------------------------------------------------------------------------------------------------------------------

	// W
	void Graphics::OperatorClip(Object arrArguments[], int nArgumentsCount)
	{
		m_eClip = clipNormal;
	}

	// W*
	void Graphics::OperatorEOClip(Object arrArguments[], int nArgumentsCount)
	{
		m_eClip = clipEO;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Text objects
	//-------------------------------------------------------------------------------------------------------------------------------

	// BT
	void Graphics::OperatorBeginText(Object arrArguments[], int nArgumentsCount)
	{
		m_pOut->BegintTextObject(m_pGState);
		m_pGState->SetTextMatrix(1, 0, 0, 1, 0, 0);
		m_pGState->TextMoveTo(0, 0);
		m_pOut->UpdateTextMatrix(m_pGState);
		m_pOut->UpdateTextPos(m_pGState);
		m_bFontChanged = true;
	}

	// ET
	void Graphics::OperatorEndText(Object arrArguments[], int nArgumentsCount)
	{
		m_pOut->EndTextObject(m_pGState);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Text state
	//-------------------------------------------------------------------------------------------------------------------------------

	// Tc
	void Graphics::OperatorSetCharSpacing(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetCharSpace(arrArguments[0].GetNum());
		m_pOut->UpdateCharSpace(m_pGState);
	}

	// Tf
	void Graphics::OperatorSetFont(Object arrArguments[], int nArgumentsCount)
	{
		GrFont *pFont = NULL;

		if (!(pFont = m_pResources->LookupFont(arrArguments[0].GetName())))
		{
			return;
		}
#ifdef _DEBUG
		if (m_pDumpFile)
		{
			::fprintf(m_pDumpFile, "  Font:\n");
			::fprintf(m_pDumpFile, "        Tag  = %s\n", pFont->GetTag()->GetBuffer());
			::fprintf(m_pDumpFile, "        Name = %s\n", pFont->GetBaseName() ? pFont->GetBaseName()->GetBuffer() : "???");
			::fprintf(m_pDumpFile, "               %g\n", arrArguments[1].GetNum());
			::fflush(m_pDumpFile);
		}
#endif
		m_pGState->SetFont(pFont, arrArguments[1].GetNum());
		m_bFontChanged = true;
	}

	// TL
	void Graphics::OperatorSetTextLeading(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetLeading(arrArguments[0].GetNum());
	}

	// Tr
	void Graphics::OperatorSetTextRender(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetRenderMode(arrArguments[0].GetInt());
		m_pOut->UpdateRender(m_pGState);
	}

	// Ts
	void Graphics::OperatorSetTextRise(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetRise(arrArguments[0].GetNum());
		m_pOut->UpdateRise(m_pGState);
	}

	// Tw
	void Graphics::OperatorSetWordSpacing(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetWordSpace(arrArguments[0].GetNum());
		m_pOut->UpdateWordSpace(m_pGState);
	}

	// Tz
	void Graphics::OperatorSetHorizScaling(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetHorizScaling(arrArguments[0].GetNum());
		m_pOut->UpdateHorizScaling(m_pGState);
		m_bFontChanged = true;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Text positioning
	//-------------------------------------------------------------------------------------------------------------------------------

	// Td
	void Graphics::OperatorTextMove(Object arrArguments[], int nArgumentsCount)
	{
		double dDX = m_pGState->GetTextLineX() + arrArguments[0].GetNum();
		double dDy = m_pGState->GetTextLineY() + arrArguments[1].GetNum();
		m_pGState->TextMoveTo(dDX, dDy);
		m_pOut->UpdateTextPos(m_pGState);
	}

	// TD
	void Graphics::OperatorTextMoveSet(Object arrArguments[], int nArgumentsCount)
	{
		double dDx = m_pGState->GetTextLineX() + arrArguments[0].GetNum();
		double dDy = arrArguments[1].GetNum();
		m_pGState->SetLeading(-dDy);
		dDy += m_pGState->GetTextLineY();
		m_pGState->TextMoveTo(dDx, dDy);
		m_pOut->UpdateTextPos(m_pGState);
	}

	// Tm
	void Graphics::OperatorSetTextMatrix(Object arrArguments[], int nArgumentsCount)
	{
		m_pGState->SetTextMatrix(arrArguments[0].GetNum(), arrArguments[1].GetNum(), arrArguments[2].GetNum(), arrArguments[3].GetNum(), arrArguments[4].GetNum(), arrArguments[5].GetNum());
		m_pGState->TextMoveTo(0, 0);
		m_pOut->UpdateTextMatrix(m_pGState);
		m_pOut->UpdateTextPos(m_pGState);
		m_bFontChanged = true;
	}

	// T*
	void Graphics::OperatorTextNextLine(Object arrArguments[], int nArgumentsCount)
	{
		double dDx = m_pGState->GetTextLineX();
		double dDy = m_pGState->GetTextLineY() - m_pGState->GetLeading();
		m_pGState->TextMoveTo(dDx, dDy);
		m_pOut->UpdateTextPos(m_pGState);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Text showing
	//-------------------------------------------------------------------------------------------------------------------------------

	// Tj
	void Graphics::OperatorShowText(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->GetFont())
		{
			// TO DO: Error "No font in show")
			return;
		}
		if (m_bFontChanged)
		{
			m_pOut->UpdateFont(m_pGState);
			m_bFontChanged = false;
		}
		m_pOut->BeginStringOperator(m_pGState);
		DoShowText(arrArguments[0].GetString());
		m_pOut->EndStringOperator(m_pGState);
	}

	// '
	void Graphics::OperatorMoveShowText(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->GetFont())
		{
			// TO DO: Error "No font in move/show"
			return;
		}
		if (m_bFontChanged)
		{
			m_pOut->UpdateFont(m_pGState);
			m_bFontChanged = false;
		}
		double dDx = m_pGState->GetTextLineX();
		double dDy = m_pGState->GetTextLineY() - m_pGState->GetLeading();
		m_pGState->TextMoveTo(dDx, dDy);
		m_pOut->UpdateTextPos(m_pGState);
		m_pOut->BeginStringOperator(m_pGState);
		DoShowText(arrArguments[0].GetString());
		m_pOut->EndStringOperator(m_pGState);
	}

    // '\'
	void Graphics::OperatorMoveSetShowText(Object arrArguments[], int nArgumentsCount)
	{
		if (m_pGState->GetFont())
		{
			// TO DO: Error "No font in move/set/show"
			return;
		}
		if (m_bFontChanged)
		{
			m_pOut->UpdateFont(m_pGState);
			m_bFontChanged = false;
		}
		m_pGState->SetWordSpace(arrArguments[0].GetNum());
		m_pGState->SetCharSpace(arrArguments[1].GetNum());

		double dDx = m_pGState->GetTextLineX();
		double dDy = m_pGState->GetTextLineY() - m_pGState->GetLeading();

		m_pGState->TextMoveTo(dDx, dDy);
		m_pOut->UpdateWordSpace(m_pGState);
		m_pOut->UpdateCharSpace(m_pGState);
		m_pOut->UpdateTextPos(m_pGState);
		m_pOut->BeginStringOperator(m_pGState);
		DoShowText(arrArguments[2].GetString());
		m_pOut->EndStringOperator(m_pGState);
	}

	// TJ
	void Graphics::OperatorShowSpaceText(Object arrArguments[], int nArgumentsCount)
	{
		if (!m_pGState->GetFont())
		{
			// TO DO: Error "No font in show/space"
			return;
		}
		if (m_bFontChanged)
		{
			m_pOut->UpdateFont(m_pGState);
			m_bFontChanged = false;
		}
		m_pOut->BeginStringOperator(m_pGState);
		int nWMode = m_pGState->GetFont()->GetWMode();
		Array *pArray = arrArguments[0].GetArray();

		for (int nIndex = 0; nIndex < pArray->GetCount(); ++nIndex)
		{
			Object oTemp;
			pArray->Get(nIndex, &oTemp);
			if (oTemp.IsNum())
			{
				if (nWMode)
				{
					m_pGState->TextShift(0, -oTemp.GetNum() * 0.001 * fabs(m_pGState->GetFontSize()));
				}
				else
				{
					m_pGState->TextShift(-oTemp.GetNum() * 0.001 * fabs(m_pGState->GetFontSize()), 0);
				}
				m_pOut->UpdateTextShift(m_pGState, oTemp.GetNum());
			}
			else if (oTemp.IsString())
			{
				DoShowText(oTemp.GetString());
			}
			else
			{
				// TO DO: Error "Element of show/space array must be number or string"
			}
			oTemp.Free();
		}
		m_pOut->EndStringOperator(m_pGState);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void Graphics::DoShowText(StringExt *seString)
	{
		CharCode nCode;
		Unicode arrUnicode[8];
		double x, y, dx, dy, dx2, dy2, tdx, tdy;
		double originX, originY, tOriginX, tOriginY;

		char *pBuffer;
		int nLen, nCurLen, uLen;

		GrFont *pFont = m_pGState->GetFont();
		int nWMode = pFont->GetWMode();

		if (m_pOut->UseDrawChar())
		{
			m_pOut->BeginString(m_pGState, seString);
		}

		// Обработка шрифта Type 3
		if (pFont->GetType() == fontType3 && m_pOut->InterpretType3Chars())
		{
			double *pMatrix = m_pGState->GetCTM();
			double pOldCTM[6], pNewCTM[6];
			for (int nIndex = 0; nIndex < 6; ++nIndex)
			{
				pOldCTM[nIndex] = pMatrix[nIndex];
			}
			pMatrix = m_pGState->GetTextMatrix();
			pNewCTM[0] = pMatrix[0] * pOldCTM[0] + pMatrix[1] * pOldCTM[2];
			pNewCTM[1] = pMatrix[0] * pOldCTM[1] + pMatrix[1] * pOldCTM[3];
			pNewCTM[2] = pMatrix[2] * pOldCTM[0] + pMatrix[3] * pOldCTM[2];
			pNewCTM[3] = pMatrix[2] * pOldCTM[1] + pMatrix[3] * pOldCTM[3];

			pMatrix = pFont->GetFontMatrix();
			pNewCTM[0] = pMatrix[0] * pNewCTM[0] + pMatrix[1] * pNewCTM[2];
			pNewCTM[1] = pMatrix[0] * pNewCTM[1] + pMatrix[1] * pNewCTM[3];
			pNewCTM[2] = pMatrix[2] * pNewCTM[0] + pMatrix[3] * pNewCTM[2];
			pNewCTM[3] = pMatrix[2] * pNewCTM[1] + pMatrix[3] * pNewCTM[3];

			pNewCTM[0] *= m_pGState->GetFontSize();
			pNewCTM[1] *= m_pGState->GetFontSize();
			pNewCTM[2] *= m_pGState->GetFontSize();
			pNewCTM[3] *= m_pGState->GetFontSize();
			pNewCTM[0] *= m_pGState->GetHorizScaling();
			pNewCTM[2] *= m_pGState->GetHorizScaling();

			double dRiseX, dRiseY;
			m_pGState->TextTransformDelta(0, m_pGState->GetRise(), &dRiseX, &dRiseY);
			double dCurX = m_pGState->GetCurX();
			double dCurY = m_pGState->GetCurY();
			double dLineX = m_pGState->GetTextLineX();
			double dLineY = m_pGState->GetTextLineY();
			Parser *pOldParser = m_pParser;
			pBuffer = seString->GetBuffer();
			nLen = seString->GetLength();
			while (nLen > 0)
			{
				nCurLen = pFont->GetNextChar(pBuffer, nLen, &nCode, arrUnicode, (int)(sizeof(arrUnicode) / sizeof(Unicode)), &uLen, &dx, &dy, &originX, &originY);
				dx = dx * m_pGState->GetFontSize() + m_pGState->GetCharSpace();
				if (nCurLen == 1 && *pBuffer == ' ')
				{
					dx += m_pGState->GetWordSpace();
				}
				dx *= m_pGState->GetHorizScaling();
				dy *= m_pGState->GetFontSize();
				m_pGState->TextTransformDelta(dx, dy, &tdx, &tdy);
				m_pGState->Transform(dCurX + dRiseX, dCurY + dRiseY, &x, &y);
				SaveGState();
				m_pGState->SetCTM(pNewCTM[0], pNewCTM[1], pNewCTM[2], pNewCTM[3], x, y);
				//Значения CTM concat здесь неправильные (но они никогда не используются)
				m_pOut->UpdateCTM(m_pGState, 1, 0, 0, 1, 0, 0);
				if (!m_pOut->BeginType3Char(m_pGState, dCurX + dRiseX, dCurY + dRiseY, tdx, tdy, nCode, arrUnicode, uLen))
				{
					Object oCharProc;
					((Gr8BitFont *)pFont)->GetCharProc(nCode, &oCharProc);
					Dict *pResourcesDict;
					if ((pResourcesDict = ((Gr8BitFont *)pFont)->GetResources()))
					{
						PushResources(pResourcesDict);
					}
					if (oCharProc.IsStream())
					{
						Display(&oCharProc, false);
					}
					else
					{
						// TO DO: Error "Missing or bad Type3 CharProc entry"
					}
					m_pOut->EndType3Char(m_pGState);
					if (pResourcesDict)
					{
						PopResources();
					}
					oCharProc.Free();
				}
				RestoreGState();
				// Поскольку функция RestoreGState() не восстанавливает текущей позиции, 
				// поэтому нам нужно сохранять и восстанавливать ее самим
				dCurX += tdx;
				dCurY += tdy;
				m_pGState->MoveTo(dCurX, dCurY);
				m_pGState->TextSetPos(dLineX, dLineY);
				pBuffer += nCurLen;
				nLen    -= nCurLen;
			}
			m_pParser = pOldParser;
		}
		else if (m_pOut->UseDrawChar())
		{
			double dRiseX, dRiseY;
			m_pGState->TextTransformDelta(0, m_pGState->GetRise(), &dRiseX, &dRiseY);
			pBuffer = seString->GetBuffer();
			nLen = seString->GetLength();
			while (nLen > 0)
			{
				nCurLen = pFont->GetNextChar(pBuffer, nLen, &nCode, arrUnicode, (int)(sizeof(arrUnicode) / sizeof(Unicode)), &uLen, &dx, &dy, &originX, &originY);
				if (nWMode)
				{
					dx *= m_pGState->GetFontSize();
					dy = dy * m_pGState->GetFontSize() + m_pGState->GetCharSpace();
					if (nCurLen == 1 && *pBuffer == ' ')
					{
						dy += m_pGState->GetWordSpace();
					}
				}
				else
				{
					dx = dx * m_pGState->GetFontSize() + m_pGState->GetCharSpace();
					if (nCurLen == 1 && *pBuffer == ' ')
					{
						dx += m_pGState->GetWordSpace();
					}
					dx *= m_pGState->GetHorizScaling();
					dy *= m_pGState->GetFontSize();
				}
				m_pGState->TextTransformDelta(dx, dy, &tdx, &tdy);
				originX *= m_pGState->GetFontSize();
				originY *= m_pGState->GetFontSize();
				m_pGState->TextTransformDelta(originX, originY, &tOriginX, &tOriginY);
				m_pOut->DrawChar(m_pGState, m_pGState->GetCurX() + dRiseX, m_pGState->GetCurY() + dRiseY, tdx, tdy, tOriginX, tOriginY, nCode, nCurLen, arrUnicode, uLen);
				m_pGState->Shift(tdx, tdy);
				pBuffer += nCurLen;
				nLen    -= nCurLen;
			}
		}
		else
		{
			dx = dy = 0;
			pBuffer = seString->GetBuffer();
			nLen = seString->GetLength();
			int nCharsCount = 0, nSpacesCount = 0;
			while (nLen > 0)
			{
				nCurLen = pFont->GetNextChar(pBuffer, nLen, &nCode, arrUnicode, (int)(sizeof(arrUnicode) / sizeof(Unicode)), &uLen, &dx2, &dy2, &originX, &originY);
				dx += dx2;
				dy += dy2;
				if (nCurLen == 1 && *pBuffer == ' ')
				{
					++nSpacesCount;
				}
				++nCharsCount;
				pBuffer += nCurLen;
				nLen    -= nCurLen;
			}
			if (nWMode)
			{
				dx *= m_pGState->GetFontSize();
				dy = dy * m_pGState->GetFontSize() + nCharsCount * m_pGState->GetCharSpace() + nSpacesCount * m_pGState->GetWordSpace();
			}
			else
			{
				dx = dx * m_pGState->GetFontSize() + nCharsCount * m_pGState->GetCharSpace() + nSpacesCount * m_pGState->GetWordSpace();
				dx *= m_pGState->GetHorizScaling();
				dy *= m_pGState->GetFontSize();
			}
			m_pGState->TextTransformDelta(dx, dy, &tdx, &tdy);
			m_pOut->DrawString(m_pGState, seString);
			m_pGState->Shift(tdx, tdy);
		}

		if (m_pOut->UseDrawChar())
		{
			m_pOut->EndString(m_pGState);
		}

		m_nUpdateLevel += 10 * seString->GetLength();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// XObjects
	//-------------------------------------------------------------------------------------------------------------------------------

	// Do
	void Graphics::OperatorXObject(Object arrArguments[], int nArgumentsCount)
	{
		char *sName = arrArguments[0].GetName();
		Object oXObject;
		if (!m_pResources->LookupXObject(sName, &oXObject))
		{
			return;
		}
		if (!oXObject.IsStream())
		{
			// TO DO: Error "XObject is wrong type"
			oXObject.Free();
			return;
		}
#if OPI_SUPPORT
		Object oOPI;
		oXObject.streamGetDict()->lookup("OPI", &oOPI);
		if ( oOPI.IsDict() ) 
		{
			m_pOut->opiBegin( state, oOPI.GetDict() );
		}
#endif
		Object oSub;
		oXObject.StreamGetDict()->Search("Subtype", &oSub);
		if (oSub.IsName("Image"))
		{
			if (m_pOut->NeedNonText())
			{
				Object oRef;
				m_pResources->LookupAndCopyXObject(sName, &oRef);
				DoImage(&oRef, oXObject.GetStream(), false);
				oRef.Free();
			}
		}
		else if (oSub.IsName("Form"))
		{
			Object oRef;
			m_pResources->LookupAndCopyXObject(sName, &oRef);
			if (m_pOut->UseDrawForm() && oRef.IsRef())
			{
				m_pOut->DrawForm(oRef.GetRef());
			}
			else
			{
				DoForm(&oXObject);
			}
			oRef.Free();
		}
		else if (oSub.IsName("PS"))
		{
			Object oLevel;
			oXObject.StreamGetDict()->Search("Level1", &oLevel);
			m_pOut->PSXObject(oXObject.GetStream(), oLevel.IsStream() ? oLevel.GetStream() : (Stream *)NULL);
			// Добавленно
			oLevel.Free();
		}
		else if (oSub.IsName())
		{
			// TO DO: Error "Unknown XObject subtype"
		}
		else
		{
			// TO DO: Error "XObject subtype is missing or wrong type"
		}
		oSub.Free();
#if OPI_SUPPORT
		if ( oOPI.IsDict() ) 
		{
			m_pOut->opiEnd( state, oOPI.GetDict() );
		}
		oOPI.Free();
#endif
		oXObject.Free();
	}

	void Graphics::DoImage(Object *pRef, Stream *pStream, bool bInlineImage)
	{
		// Считываем информацию из потока
		int nBitsPerComponent = 0;
		StreamColorSpaceMode eCSMode = streamCSNone;
		pStream->GetImageParams(&nBitsPerComponent, &eCSMode);

		Dict *pDict = pStream->GetDict();

		// Width
		Object oDictItem;
		pDict->Search("Width", &oDictItem);
		if (oDictItem.IsNull())
		{
			oDictItem.Free();
			pDict->Search("W", &oDictItem);
		}
		if (!oDictItem.IsInt())
		{
			oDictItem.Free();
			// TO DO: Error "Bad image parameters"
			return;
		}
		int nWidth = oDictItem.GetInt();
		oDictItem.Free();

		// Height
		pDict->Search("Height", &oDictItem);
		if (oDictItem.IsNull())
		{
			oDictItem.Free();
			pDict->Search("H", &oDictItem);
		}
		if (!oDictItem.IsInt())
		{
			oDictItem.Free();
			// TO DO: Error "Bad image parameters"
			return;
		}
		int nHeight = oDictItem.GetInt();
		oDictItem.Free();

		// Проверяем: может быть это маска?
		pDict->Search("ImageMask", &oDictItem);
		if (oDictItem.IsNull())
		{
			oDictItem.Free();
			pDict->Search("IM", &oDictItem);
		}
		bool bMask = false;
		if (oDictItem.IsBool())
		{
			bMask = oDictItem.GetBool();
		}
		else if (!oDictItem.IsNull())
		{
			oDictItem.Free();
			// TO DO: Error "Bad image parameters"
			return;
		}
		oDictItem.Free();

		// BitsPerComponent
		if (nBitsPerComponent == 0)
		{
			pDict->Search("BitsPerComponent", &oDictItem);
			if (oDictItem.IsNull())
			{
				oDictItem.Free();
				pDict->Search("BPC", &oDictItem);
			}
			if (oDictItem.IsInt())
			{
				nBitsPerComponent = oDictItem.GetInt();
			}
			else if (bMask)
			{
				nBitsPerComponent = 1;
			}
			else
			{
				oDictItem.Free();
				// TO DO: Error "Bad image parameters"
				return;
			}
			oDictItem.Free();
		}

		if (bMask)
		{
			// У маски это значение nBitsPerComponent должно быть 1
			if (nBitsPerComponent != 1)
			{
				// TO DO: Error "Bad image parameters"
				return;
			}
			bool bInvert = false;
			pDict->Search("Decode", &oDictItem);
			if (oDictItem.IsNull())
			{
				oDictItem.Free();
				pDict->Search("D", &oDictItem);
			}
			if (oDictItem.IsArray())
			{
				Object oTemp;
				oDictItem.ArrayGet(0, &oTemp);
				if (oTemp.IsInt() && oTemp.GetInt() == 1)
					bInvert = true;
				oTemp.Free();
			}
			else if (!oDictItem.IsNull())
			{
				oDictItem.Free();
				// TO DO: Error "Bad image parameters"
				return;
			}
			oDictItem.Free();

			// Рисуем маску
			m_pOut->DrawImageMask(m_pGState, pRef, pStream, nWidth, nHeight, bInvert, bInlineImage);
		}
		else
		{
			// ColorSpace
			pDict->Search("ColorSpace", &oDictItem);
			if (oDictItem.IsNull())
			{
				oDictItem.Free();
				pDict->Search("CS", &oDictItem);
			}
			if (oDictItem.IsName())
			{
				Object oTemp;
				m_pResources->LookupColorSpace(oDictItem.GetName(), &oTemp);
				if (!oTemp.IsNull())
				{
					oDictItem.Free();
					oDictItem = oTemp;
				}
				else
				{
					oTemp.Free();
				}
			}
			GrColorSpace *pColorSpace;
			if (!oDictItem.IsNull())
			{
				pColorSpace = GrColorSpace::Parse(&oDictItem);
			}
			else if (eCSMode == streamCSDeviceGray)
			{
				pColorSpace = new GrDeviceGrayColorSpace();
			}
			else if (eCSMode == streamCSDeviceRGB)
			{
				pColorSpace = new GrDeviceRGBColorSpace();
			}
			else if (eCSMode == streamCSDeviceCMYK)
			{
				pColorSpace = new GrDeviceCMYKColorSpace();
			}
			else
			{
				pColorSpace = NULL;
			}
			oDictItem.Free();
			if (!pColorSpace)
			{
				// TO DO: Error "Bad image parameters"
				return;
			}

			// Decode
			pDict->Search("Decode", &oDictItem);
			if (oDictItem.IsNull())
			{
				oDictItem.Free();
				pDict->Search("D", &oDictItem);
			}
			GrImageColorMap *pColorMap = new GrImageColorMap(nBitsPerComponent, &oDictItem, pColorSpace);
			oDictItem.Free();
			if (!pColorMap->IsValid())
			{
				delete pColorMap;
				// TO DO: Error "Bad image parameters"
				return;
			}

			// Mask/SMask
			bool bHaveColorKeyMask = false, bHaveExplicitMask = false, bHaveSoftMask = false;
			Stream *pMaskStream = NULL;
			int nMaskWidth = 0, nMaskHeight = 0;
			bool bMaskInvert = false;
			GrImageColorMap *pMaskColorMap = NULL;
			int arrMaskColors[2 * GrColorMaxComps];

			Object oMask, oSMask;
			pDict->Search("Mask", &oMask);
			pDict->Search("SMask", &oSMask);

			Dict *pMaskDict = NULL;

			if (oSMask.IsStream())
			{
				if (bInlineImage)
				{
					// Тут наверное нужно освободить память oMask, oSMask
					// TO DO: Error "Bad image parameters"
					return;
				}
				pMaskStream = oSMask.GetStream();
				pMaskDict = oSMask.StreamGetDict();

				// Width
				pMaskDict->Search("Width", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("W", &oDictItem);
				}
				if (!oDictItem.IsInt())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				nMaskWidth = oDictItem.GetInt();
				oDictItem.Free();

				// Height
				pMaskDict->Search("Height", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("H", &oDictItem);
				}
				if (!oDictItem.IsInt())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				nMaskHeight = oDictItem.GetInt();
				oDictItem.Free();

				// BitsPerComponent
				pMaskDict->Search("BitsPerComponent", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("BPC", &oDictItem);
				}
				if (!oDictItem.IsInt())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				int nMaskBitsPerComponent = oDictItem.GetInt();
				oDictItem.Free();

				// ColorSpace
				pMaskDict->Search("ColorSpace", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("CS", &oDictItem);
				}
				if (oDictItem.IsName())
				{
					Object oTemp;
					m_pResources->LookupColorSpace(oDictItem.GetName(), &oTemp);
					if (!oTemp.IsNull())
					{
						oDictItem.Free();
						oDictItem = oTemp;
					}
					else
					{
						oTemp.Free();
					}
				}
				GrColorSpace  *pMaskColorSpace = GrColorSpace::Parse(&oDictItem);
				oDictItem.Free();
				if (!pMaskColorSpace || pMaskColorSpace->GetMode() != csDeviceGray)
				{
					// TO DO: Error "Bad image parameters"
					return;
				}

				// Decode
				pMaskDict->Search("Decode", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("D", &oDictItem);
				}
				pMaskColorMap = new GrImageColorMap(nMaskBitsPerComponent, &oDictItem, pMaskColorSpace);
				oDictItem.Free();
				if (!pMaskColorMap->IsValid())
				{
					delete pMaskColorMap;
					// TO DO: Error "Bad image parameters"
					return;
				}

				bHaveSoftMask = true;
			}
			else if (oMask.IsArray())
			{
				for (int nIndex = 0; nIndex < oMask.ArrayGetLength() && nIndex < 2 * GrColorMaxComps; ++nIndex)
				{
					Object oTemp;
					oMask.ArrayGet(nIndex, &oTemp);
					arrMaskColors[nIndex] = oTemp.GetInt();
					oTemp.Free();
				}
				bHaveColorKeyMask = true;
			}
			else if (oMask.IsStream())
			{
				if (bInlineImage)
				{
					// TO DO: Error "Bad image parameters"
					return;
				}
				pMaskStream = oMask.GetStream();
				pMaskDict   = oMask.StreamGetDict();

				// Width
				pMaskDict->Search("Width", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("W", &oDictItem);
				}
				if (!oDictItem.IsInt())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				nMaskWidth = oDictItem.GetInt();
				oDictItem.Free();

				// Height
				pMaskDict->Search("Height", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("H", &oDictItem);
				}
				if (!oDictItem.IsInt())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				nMaskHeight = oDictItem.GetInt();
				oDictItem.Free();

				// ImageMask
				pMaskDict->Search("ImageMask", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("IM", &oDictItem);
				}
				if (!oDictItem.IsBool() || !oDictItem.GetBool())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				oDictItem.Free();

				// Decode
				bMaskInvert = false;
				pMaskDict->Search("Decode", &oDictItem);
				if (oDictItem.IsNull())
				{
					oDictItem.Free();
					pMaskDict->Search("D", &oDictItem);
				}
				if (oDictItem.IsArray())
				{
					Object oTemp;
					oDictItem.ArrayGet(0, &oTemp);
					if (oTemp.IsInt() && oTemp.GetInt() == 1)
					{
						bMaskInvert = true;
					}
					oTemp.Free();
				}
				else if (!oDictItem.IsNull())
				{
					oDictItem.Free();
					// TO DO: Error "Bad image parameters"
					return;
				}
				oDictItem.Free();
				bHaveExplicitMask = true;
			}

			// Рисуем
			if (bHaveSoftMask)
			{
				m_pOut->DrawSoftMaskedImage(m_pGState, pRef, pStream, nWidth, nHeight, pColorMap, pMaskStream, nMaskWidth, nMaskHeight, pMaskColorMap);
				delete pMaskColorMap;
			}
			else if (bHaveExplicitMask)
			{
				m_pOut->DrawMaskedImage(m_pGState, pRef, pStream, nWidth, nHeight, pColorMap, pMaskStream, nMaskWidth, nMaskHeight, bMaskInvert);
			}
			else
			{
				m_pOut->DrawImage(m_pGState, pRef, pStream, nWidth, nHeight, pColorMap, bHaveColorKeyMask ? arrMaskColors : (int *)NULL, bInlineImage);
			}
			delete pColorMap;

			oMask.Free();
			oSMask.Free();
		}


		int nUpdater = 0;
		if ((nUpdater = nWidth * nHeight) > 1000)
		{
			nUpdater = 1000;
		}
		m_nUpdateLevel += nUpdater;

		return;
	}

	void Graphics::DoForm(Object *pStream)
	{
		double arrMatrix[6], arrBBox[4];

		// Проверяем глубину рекурсии
		if (m_nFormDepth > 20)
		{
			return;
		}

		Dict *pDict = pStream->StreamGetDict();
		Object oDictItem;

		// FormType
		pDict->Search("FormType", &oDictItem);
		if (!(oDictItem.IsNull() || (oDictItem.IsInt() && oDictItem.GetInt() == 1)))
		{
			// TO DO: Error "Unknown form type"
		}
		oDictItem.Free();

		// BBox
		Object oBBox;
		pDict->Search("BBox", &oBBox);
		if (!oBBox.IsArray())
		{
			oBBox.Free();
			// TO DO: Error "Bad form bounding box"
			return;
		}

		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			Object oTemp;
			oBBox.ArrayGet(nIndex, &oTemp);
			arrBBox[nIndex] = oTemp.GetNum();
			oTemp.Free();
		}
		oBBox.Free();

		// Matrix
		Object oMatrix;
		pDict->Search("Matrix", &oMatrix);
		if (oMatrix.IsArray())
		{
			for (int nIndex = 0; nIndex < 6; ++nIndex)
			{
				Object oTemp;
				oMatrix.ArrayGet(nIndex, &oTemp);
				arrMatrix[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		else
		{
			arrMatrix[0] = 1; arrMatrix[1] = 0;
			arrMatrix[2] = 0; arrMatrix[3] = 1;
			arrMatrix[4] = 0; arrMatrix[5] = 0;
		}
		oMatrix.Free();

		// Resources
		Object oResources;
		pDict->Search("Resources", &oResources);
		Dict *pResourcesDict = oResources.IsDict() ? oResources.GetDict() : (Dict *)NULL;

		// Transparency Group
		bool bTranspGroup = false, bIsolated = false, bKnockout = false;
		GrColorSpace *pBlendingColorSpace = NULL;
		if (pDict->Search("Group", &oDictItem)->IsDict())
		{
			Object oTransp;
			if (oDictItem.DictLookup("S", &oTransp)->IsName("Transparency"))
			{
				bTranspGroup = true;
				Object oTemp;
				if (!oDictItem.DictLookup("CS", &oTemp)->IsNull())
				{
					pBlendingColorSpace = GrColorSpace::Parse(&oTemp);
				}
				oTemp.Free();
				if (oDictItem.DictLookup("I", &oTemp)->IsBool())
				{
					bIsolated = oTemp.GetBool();
				}
				oTemp.Free();
				if (oDictItem.DictLookup("K", &oTemp)->IsBool())
				{
					bKnockout = oTemp.GetBool();
				}
				oTemp.Free();
			}
			oTransp.Free();
		}
		oDictItem.Free();

		// Рисуем
		++m_nFormDepth;
		DoForm(pStream, pResourcesDict, arrMatrix, arrBBox, bTranspGroup, false, pBlendingColorSpace, bIsolated, bKnockout);
		--m_nFormDepth;

		if (pBlendingColorSpace)
		{
			delete pBlendingColorSpace;
		}
		oResources.Free();
	}

	void Graphics::DoForm(Object *pStream, Dict *pResourcesDict, double *pMatrix, double *pBBox, bool bTranspGroup, bool bSMask, GrColorSpace *pBlendingColorSpace, bool bIsolated, bool bKnockout, bool bAlpha, Function *pTransferFunctions, GrColor *pBackdropColor)
	{
		// Записываем текущую директорию ресурсов в стек
		PushResources(pResourcesDict);

		// Сохраняем текущий GState
		SaveGState();

		// Очищаем Path(если там что-то было)
		m_pGState->ClearPath();

		// Сохраняем текущий парсер
		Parser *pOldParser = m_pParser;

		// CTM
		m_pGState->ConcatCTM(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], pMatrix[4], pMatrix[5]);
		m_pOut->UpdateCTM(m_pGState, pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], pMatrix[4], pMatrix[5]);

		// BBox
		m_pGState->MoveTo(pBBox[0], pBBox[1]);
		m_pGState->LineTo(pBBox[2], pBBox[1]);
		m_pGState->LineTo(pBBox[2], pBBox[3]);
		m_pGState->LineTo(pBBox[0], pBBox[3]);
		m_pGState->ClosePath();
		m_pGState->Clip();
		m_pGState->GetClip()->ClipToPath(m_pGState->GetPath()->Copy(), m_pGState->GetCTM(), false);

		if (m_pOut->UseClipTo())
			m_pOut->ClipToPath(m_pGState, m_pGState->GetPath(), m_pGState->GetCTM(), false);
		else
			m_pOut->Clip(m_pGState);

		m_pGState->ClearPath();

		if (bSMask || bTranspGroup)
		{
			if (m_pGState->GetBlendMode() != grBlendNormal)
			{
				m_pGState->SetBlendMode(grBlendNormal);
				m_pOut->UpdateBlendMode(m_pGState);
			}
			if (!m_pOut->UseSimpleTransparentGroup())
			{
				if (m_pGState->GetFillOpacity() != 1)
				{
					m_pGState->SetFillOpacity(1);
					m_pOut->UpdateFillOpacity(m_pGState);
				}
				if (m_pGState->GetStrokeOpacity() != 1)
				{
					m_pGState->SetStrokeOpacity(1);
					m_pOut->UpdateStrokeOpacity(m_pGState);
				}
			}
			m_pOut->ClearSoftMask(m_pGState);
			m_pOut->BeginTransparencyGroup(m_pGState, pBBox, pBlendingColorSpace, bIsolated, bKnockout, bSMask);
		}

		double arrOldBaseMatrix[6];
		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			arrOldBaseMatrix[nIndex] = m_arrBaseMatrix[nIndex];
			m_arrBaseMatrix[nIndex] = m_pGState->GetCTM()[nIndex];
		}

		// Рисуем саму форму
		Display(pStream, false);

		if (bSMask || bTranspGroup)
		{
			m_pOut->EndTransparencyGroup(m_pGState);
		}

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrBaseMatrix[nIndex] = arrOldBaseMatrix[nIndex];
		}


		// Восстанавливаем начальные данные
		m_pParser = pOldParser;
		RestoreGState();
		PopResources();

		if (bSMask)
		{
			m_pOut->SetSoftMask(m_pGState, pBBox, bAlpha, pTransferFunctions, pBackdropColor);
		}
		else if (bTranspGroup)
		{
			m_pOut->PaintTransparencyGroup(m_pGState, pBBox);
		}

		return;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Inline images
	//-------------------------------------------------------------------------------------------------------------------------------

	// BI
	void Graphics::OperatorBeginImage(Object arrArguments[], int nArgumentsCount)
	{
		Stream *pStream = BuildImageStream();

		if (pStream)
		{
			DoImage(NULL, pStream, true);

			// Ищем оператор EI
			int nChar1 = pStream->GetUndecodedStream()->GetChar();
			int nChar2 = pStream->GetUndecodedStream()->GetChar();

			while (!(nChar1 == 'E' && nChar2 == 'I') && nChar2 != EOF)
			{
				nChar1 = nChar2;
				nChar2 = pStream->GetUndecodedStream()->GetChar();
			}
			delete pStream;
		}
	}

	// ID
	void Graphics::OperatorImageData(Object arrArguments[], int nArgumentsCount)
	{
		// TO DO: Error "Internal: got 'ID' operator"
	}

	// EI
	void Graphics::OperatorEndImage(Object arrArguments[], int nArgumentsCount)
	{
		// TO DO: Error "Internal: got 'EI' operator"
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	Stream *Graphics::BuildImageStream()
	{
		Object oDict;
		oDict.InitDict(m_pXref);
		Object oObject;
		m_pParser->GetObject(&oObject);

		while (!oObject.IsCommand("ID") && !oObject.IsEOF())
		{
			if (!oObject.IsName())
			{
				// TO DO: Error "Inline image dictionary key must be a name object"
				oObject.Free();
			}
			else
			{
				char* sKey = CopyString(oObject.GetName());
				oObject.Free();
				m_pParser->GetObject(&oObject);
				if (oObject.IsEOF() || oObject.IsError())
				{
					MemUtilsFree(sKey);
					break;
				}
				oDict.DictAdd(sKey, &oObject);
			}
			m_pParser->GetObject(&oObject);
		}
		if (oObject.IsEOF())
		{
			// TO DO: Error "End of file in inline image"
			oObject.Free();
			oDict.Free();
			return NULL;
		}
		oObject.Free();

		Stream *pStream = new EmbedStream(m_pParser->GetStream(), &oDict, false, 0);
		pStream = pStream->AddFilters(&oDict);

		return pStream;
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	// Type 3 fonts
	//-------------------------------------------------------------------------------------------------------------------------------

	// d0
	void Graphics::OperatorSetCharWidth(Object arrArguments[], int nArgumentsCount)
	{
		m_pOut->Type3D0(m_pGState, arrArguments[0].GetNum(), arrArguments[1].GetNum());
	}

	// d1
	void Graphics::OperatorSetCacheDevice(Object arrArguments[], int nArgumentsCount)
	{
		m_pOut->Type3D1(m_pGState, arrArguments[0].GetNum(), arrArguments[1].GetNum(), arrArguments[2].GetNum(), arrArguments[3].GetNum(), arrArguments[4].GetNum(), arrArguments[5].GetNum());
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Compatibility
	//-------------------------------------------------------------------------------------------------------------------------------

	// BX
	void Graphics::OperatorBeginIgnoreUndef(Object arrArguments[], int nArgumentsCount)
	{
		++m_nIgnoreUndef;
	}

	// EX
	void Graphics::OperatorEndIgnoreUndef(Object arrArguments[], int nArgumentsCount)
	{
		if (m_nIgnoreUndef > 0)
			--m_nIgnoreUndef;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Marked content
	//-------------------------------------------------------------------------------------------------------------------------------

	// BDC/BMC
	void Graphics::OperatorBeginMarkedContent(Object arrArguments[], int nArgumentsCount)
	{
	}

	// EMC
	void Graphics::OperatorEndMarkedContent(Object arrArguments[], int nArgumentsCount)
	{
	}

	// DP/MP
	void Graphics::OperatorMarkPoint(Object arrArguments[], int nArgumentsCount)
	{
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Дополнительные функции
	//-------------------------------------------------------------------------------------------------------------------------------

	void Graphics::DrawAnnotation(Object *pStream, AnnotBorderStyle *pBorderStyle, double dMinX, double dMinY, double dMaxX, double dMaxY)
	{
		double dX, dY;

		// Преобразовываем Annotation BBox из стандартного пользовательского пространства в
		// текущее пользовательское пространство: (BBox * BaseMatrix) * InvCTM
		double *pCTM = m_pGState->GetCTM();
		double dDet = 1 / (pCTM[0] * pCTM[3] - pCTM[1] * pCTM[2]);
		double arrInvCTM[6];
		arrInvCTM[0] =  pCTM[3] * dDet;
		arrInvCTM[1] = -pCTM[1] * dDet;
		arrInvCTM[2] = -pCTM[2] * dDet;
		arrInvCTM[3] =  pCTM[0] * dDet;
		arrInvCTM[4] = (pCTM[2] * pCTM[5] - pCTM[3] * pCTM[4]) * dDet;
		arrInvCTM[5] = (pCTM[1] * pCTM[4] - pCTM[0] * pCTM[5]) * dDet;

		dX = m_arrBaseMatrix[0] * dMinX + m_arrBaseMatrix[2] * dMinY + m_arrBaseMatrix[4];
		dY = m_arrBaseMatrix[1] * dMinX + m_arrBaseMatrix[3] * dMinY + m_arrBaseMatrix[5];
		double dAnnotX0 = arrInvCTM[0] * dX + arrInvCTM[2] * dY + arrInvCTM[4];
		double dAnnotY0 = arrInvCTM[1] * dX + arrInvCTM[3] * dY + arrInvCTM[5];

		dX = m_arrBaseMatrix[0] * dMaxX + m_arrBaseMatrix[2] * dMaxY + m_arrBaseMatrix[4];
		dY = m_arrBaseMatrix[1] * dMaxX + m_arrBaseMatrix[3] * dMaxY + m_arrBaseMatrix[5];
		double dAnnotX1 = arrInvCTM[0] * dX + arrInvCTM[2] * dY + arrInvCTM[4];
		double dAnnotY1 = arrInvCTM[1] * dX + arrInvCTM[3] * dY + arrInvCTM[5];

		if (dAnnotX0 > dAnnotX1)
		{
			dX = dAnnotX0;
			dAnnotX0 = dAnnotX1;
			dAnnotX1 = dX;
		}
		if (dAnnotY0 > dAnnotY1)
		{
			dY = dAnnotY0;
			dAnnotY0 = dAnnotY1;
			dAnnotY1 = dY;
		}

		if (pStream->IsStream())
		{
			Dict *pDict = pStream->StreamGetDict();

			// BBox
			Object oBBox;
			pDict->Search("BBox", &oBBox);
			double arrBBox[4];
			if (!oBBox.IsArray())
			{
				oBBox.Free();
				// TO DO: Error "Bad form bounding box"
				return;
			}
			for (int nIndex = 0; nIndex < 4; ++nIndex)
			{
				Object oTemp;
				oBBox.ArrayGet(nIndex, &oTemp);
				arrBBox[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
			oBBox.Free();

			// Matrix
			Object oMatrix;
			pDict->Search("Matrix", &oMatrix);
			double arrMatrix[6];
			if (oMatrix.IsArray())
			{
				for (int nIndex = 0; nIndex < 6; ++nIndex)
				{
					Object oTemp;
					oMatrix.ArrayGet(nIndex, &oTemp);
					arrMatrix[nIndex] = oTemp.GetNum();
					oTemp.Free();
				}
			}
			else
			{
				arrMatrix[0] = 1; arrMatrix[1] = 0;
				arrMatrix[2] = 0; arrMatrix[3] = 1;
				arrMatrix[4] = 0; arrMatrix[5] = 0;
			}
			oMatrix.Free();

			double dFormX0 = arrBBox[0] * arrMatrix[0] + arrBBox[1] * arrMatrix[2] + arrMatrix[4];
			double dFormY0 = arrBBox[0] * arrMatrix[1] + arrBBox[1] * arrMatrix[3] + arrMatrix[5];
			double dFormX1 = arrBBox[2] * arrMatrix[0] + arrBBox[3] * arrMatrix[2] + arrMatrix[4];
			double dFormY1 = arrBBox[2] * arrMatrix[1] + arrBBox[3] * arrMatrix[3] + arrMatrix[5];

			if (dFormX0 > dFormX1)
			{
				dX = dFormX0;
				dFormX0 = dFormX1;
				dFormX1 = dX;
			}
			if (dFormY0 > dFormY1)
			{
				dY = dFormY0;
				dFormY0 = dFormY1;
				dFormY1 = dY;
			}

			// Растягиваем до размеров Annotation BBox
			double dScaleX, dScaleY;
			if (dFormX1 == dFormX0)
			{
				// Такого не должно быть
				dScaleX = 1;
			}
			else
			{
				dScaleX = (dAnnotX1 - dAnnotX0) / (dFormX1 - dFormX0);
			}
			if (dFormY1 == dFormY0)
			{
				// Такого не должно быть
				dScaleY = 1;
			}
			else
			{
				dScaleY = (dAnnotY1 - dAnnotY0) / (dFormY1 - dFormY0);
			}

			arrMatrix[0] *= dScaleX;
			arrMatrix[2] *= dScaleX;
			arrMatrix[4] = (arrMatrix[4] - dFormX0) * dScaleX + dAnnotX0;
			arrMatrix[1] *= dScaleY;
			arrMatrix[3] *= dScaleY;
			arrMatrix[5] = (arrMatrix[5] - dFormY0) * dScaleY + dAnnotY0;

			// Resources
			Object oResources;
			pDict->Search("Resources", &oResources);
			Dict *pResourcesDict = oResources.IsDict() ? oResources.GetDict() : (Dict *)NULL;

			// Рисуем
			DoForm(pStream, pResourcesDict, arrMatrix, arrBBox);

			oResources.Free();
		}

		// Рисуем рамку
		if (pBorderStyle && pBorderStyle->GetWidth() > 0)
		{
			if (m_pGState->GetStrokeColorSpace()->GetMode() != csDeviceRGB)
			{
				m_pGState->SetStrokePattern(NULL);
				m_pGState->SetStrokeColorSpace(new GrDeviceRGBColorSpace());
				m_pOut->UpdateStrokeColorSpace(m_pGState);
			}
			double dR, dG, dB;
			pBorderStyle->GetColor(&dR, &dG, &dB);
			GrColor oColor;
			oColor.arrComp[0] = DoubleToColor(dR);
			oColor.arrComp[1] = DoubleToColor(dG);
			oColor.arrComp[2] = DoubleToColor(dB);
			m_pGState->SetStrokeColor(&oColor);
			m_pOut->UpdateStrokeColor(m_pGState);

			dX = (m_arrBaseMatrix[0] + m_arrBaseMatrix[2]) * arrInvCTM[0] + (m_arrBaseMatrix[1] + m_arrBaseMatrix[3]) * arrInvCTM[2];
			dY = (m_arrBaseMatrix[0] + m_arrBaseMatrix[2]) * arrInvCTM[1] + (m_arrBaseMatrix[1] + m_arrBaseMatrix[3]) * arrInvCTM[3];
			dX = sqrt(0.5 * (dX * dX + dY * dY));
			m_pGState->SetLineWidth(dX * pBorderStyle->GetWidth());
			m_pOut->UpdateLineWidth(m_pGState);

			double *pDash;
			int nDashLength;

			pBorderStyle->GetDash(&pDash, &nDashLength);

			if (pBorderStyle->GetType() == annotBorderDashed && nDashLength > 0)
			{
				double *pDashNew = (double *)MemUtilsMallocArray(nDashLength, sizeof(double));
				for (int nIndex = 0; nIndex < nDashLength; ++nIndex)
				{
					pDashNew[nIndex] = dX * pDash[nIndex];
				}
				m_pGState->SetLineDash(pDashNew, nDashLength, 0);
				m_pOut->UpdateLineDash(m_pGState);
			}

			m_pGState->ClearPath();
			m_pGState->MoveTo(dAnnotX0, m_pOut->UpSideDown() ? dAnnotY1 : dAnnotY0);
			m_pGState->LineTo(dAnnotX1, m_pOut->UpSideDown() ? dAnnotY1 : dAnnotY0);
			if (pBorderStyle->GetType() != annotBorderUnderlined)
			{
				m_pGState->LineTo(dAnnotX1, m_pOut->UpSideDown() ? dAnnotY0 : dAnnotY1);
				m_pGState->LineTo(dAnnotX0, m_pOut->UpSideDown() ? dAnnotY0 : dAnnotY1);
				m_pGState->ClosePath();
			}
			m_pOut->Stroke(m_pGState);
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Работаем со стеками GState и ResourcesDict
	//-------------------------------------------------------------------------------------------------------------------------------
	void Graphics::SaveGState()
	{
		m_pOut->SaveGState(m_pGState);
		m_pGState = m_pGState->Save();
	}

	void Graphics::RestoreGState()
	{
		m_pGState = m_pGState->Restore();
		m_pOut->RestoreGState(m_pGState);
	}

	void Graphics::PushResources(Dict *pResourcesDict)
	{
		m_pResources = new GrResources(m_pXref, pResourcesDict, m_pResources, m_pGlobalParams);
	}

	void Graphics::PopResources()
	{
		if (m_pResources)
		{
			GrResources *pResourcesNext = m_pResources->GetNext();
			delete m_pResources;
			m_pResources = pResourcesNext;
		}
		else
			m_pResources = NULL;
	}
}
