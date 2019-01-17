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
#include <math.h>
#include "MemoryUtils.h"
#include "List.h"
#include "Object.h"
#include "Catalog.h"
#include "Graphics.h"
#include "GFont.h"
#include "Lexer.h"
#include "Annot.h"
#include "Dict.h"

//-------------------------------------------------------------------------------------------------------------------------------

#define annotFlagHidden    0x0002
#define annotFlagPrint     0x0004
#define annotFlagNoView    0x0020

#define fieldFlagReadOnly           0x00000001
#define fieldFlagRequired           0x00000002
#define fieldFlagNoExport           0x00000004
#define fieldFlagMultiline          0x00001000
#define fieldFlagPassword           0x00002000
#define fieldFlagNoToggleToOff      0x00004000
#define fieldFlagRadio              0x00008000
#define fieldFlagPushbutton         0x00010000
#define fieldFlagCombo              0x00020000
#define fieldFlagEdit               0x00040000
#define fieldFlagSort               0x00080000
#define fieldFlagFileSelect         0x00100000
#define fieldFlagMultiSelect        0x00200000
#define fieldFlagDoNotSpellCheck    0x00400000
#define fieldFlagDoNotScroll        0x00800000
#define fieldFlagComb               0x01000000
#define fieldFlagRichText           0x02000000
#define fieldFlagRadiosInUnison     0x02000000
#define fieldFlagCommitOnSelChange  0x04000000

#define fieldQuadLeft   0
#define fieldQuadCenter 1
#define fieldQuadRight  2

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// AnnotBorderStyle
	//-------------------------------------------------------------------------------------------------------------------------------
	AnnotBorderStyle::AnnotBorderStyle(AnnotBorderType eType, double dWidth, double *pDash, int nDashLength, double dR, double dG, double dB)
	{
		m_eType  = eType;
		m_dWidth = dWidth;
		m_pDash  = pDash;
		m_nDashLength = nDashLength;
		m_dR = dR;
		m_dG = dG;
		m_dB = dB;
	}
	AnnotBorderStyle::~AnnotBorderStyle()
	{
		MemUtilsFree(m_pDash);
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// Annot
	//-------------------------------------------------------------------------------------------------------------------------------
	Annot::Annot(GlobalParams *pGlobalParams, XRef *pXref, Dict *pAcroForm, Dict *pDict, Ref *pRef)
	{
		m_pGlobalParams = pGlobalParams;

		m_bValid = true;
		m_pXref = pXref;
		m_oRef = *pRef;
		m_seType = NULL;
		m_seAppBuffer = NULL;
		m_pBorderStyle = NULL;

		// SubType
		Object oDictItem;
		if (pDict->Search("Subtype", &oDictItem)->IsName())
		{
			m_seType = new StringExt(oDictItem.GetName());
		}
		oDictItem.Free();

		// Rect
		if (pDict->Search("Rect", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 4)
		{
			Object oTemp;
			m_dMinX = m_dMinY = m_dMaxX = m_dMaxY = 0;
			if (oDictItem.ArrayGet(0, &oTemp)->IsNum())
			{
				m_dMinX = oTemp.GetNum();
			}
			oTemp.Free();

			if (oDictItem.ArrayGet(1, &oTemp)->IsNum())
			{
				m_dMinY = oTemp.GetNum();
			}
			oTemp.Free();

			if (oDictItem.ArrayGet(2, &oTemp)->IsNum())
			{
				m_dMaxX = oTemp.GetNum();
			}
			oTemp.Free();

			if (oDictItem.ArrayGet(3, &oTemp)->IsNum())
			{
				m_dMaxY = oTemp.GetNum();
			}
			oTemp.Free();

			if (m_dMinX > m_dMaxX)
			{
				double dTemp = m_dMinX;
				m_dMinX = m_dMaxX;
				m_dMaxX = dTemp;
			}

			if (m_dMinY > m_dMaxY)
			{
				double dTemp = m_dMinY;
				m_dMinY = m_dMaxY;
				m_dMaxY = dTemp;
			}
		}
		else
		{
			// TO DO: Error "Bad bounding box for annotation"
			m_bValid = false;
		}
		oDictItem.Free();

		// F (flags)
		if (pDict->Search("F", &oDictItem)->IsInt())
		{
			m_nFlags = oDictItem.GetInt();
		}
		else
		{
			m_nFlags = 0;
		}
		oDictItem.Free();

		//Border style

		AnnotBorderType eBorderType = annotBorderSolid;
		double dBorderWidth = 1;
		double *pBorderDash = NULL;
		int nBorderDashLength = 0;
		double dBorderR = 0;
		double dBorderG = 0;
		double dBorderB = 1;
		if (pDict->Search("BS", &oDictItem)->IsDict())
		{
			Object oTemp;
			if (oDictItem.DictLookup("S", &oTemp)->IsName())
			{
				if (oTemp.IsName("S"))
				{
					eBorderType = annotBorderSolid;
				}
				else if (oTemp.IsName("D"))
				{
					eBorderType = annotBorderDashed;
				}
				else if (oTemp.IsName("B"))
				{
					eBorderType = annotBorderBeveled;
				}
				else if (oTemp.IsName("I"))
				{
					eBorderType = annotBorderInset;
				}
				else if (oTemp.IsName("U"))
				{
					eBorderType = annotBorderUnderlined;
				}
			}
			oTemp.Free();

			if (oDictItem.DictLookup("W", &oTemp)->IsNum())
			{
				dBorderWidth = oTemp.GetNum();
			}
			oTemp.Free();

			if (oDictItem.DictLookup("D", &oTemp)->IsArray())
			{
				nBorderDashLength = oTemp.ArrayGetLength();
				pBorderDash = (double *)MemUtilsMallocArray(nBorderDashLength, sizeof(double));

				for (int nIndex = 0; nIndex < nBorderDashLength; ++nIndex)
				{
					Object oArrayItem;
					if (oTemp.ArrayGet(nIndex, &oArrayItem)->IsNum())
					{
						pBorderDash[nIndex] = oArrayItem.GetNum();
					}
					else
					{
						pBorderDash[nIndex] = 1;
					}
					oArrayItem.Free();
				}
			}
			oTemp.Free();
		}
		else
		{
			oDictItem.Free();
			if (pDict->Search("Border", &oDictItem)->IsArray())
			{
				Object oTemp;
				if (oDictItem.ArrayGetLength() >= 3)
				{
					if (oDictItem.ArrayGet(2, &oTemp)->IsNum())
					{
						dBorderWidth = oTemp.GetNum();
					}
					oTemp.Free();
					if (oDictItem.ArrayGetLength() >= 4)
					{
						if (oDictItem.ArrayGet(3, &oTemp)->IsArray())
						{
							eBorderType = annotBorderDashed;
							nBorderDashLength = oTemp.ArrayGetLength();
							pBorderDash = (double *)MemUtilsMallocArray(nBorderDashLength, sizeof(double));

							for (int nIndex = 0; nIndex < nBorderDashLength; ++nIndex)
							{
								Object oArrayItem;
								if (oTemp.ArrayGet(nIndex, &oArrayItem)->IsNum())
								{
									pBorderDash[nIndex] = oArrayItem.GetNum();
								}
								else
								{
									pBorderDash[nIndex] = 1;
								}
								oArrayItem.Free();
							}
						}
						else
						{
							dBorderWidth = 0;
						}
						oTemp.Free();
					}
				}
			}
		}
		oDictItem.Free();

		// C (border color)
		// TO DO: надо сделать чтение случаев, когда в массике не только 3 элемента, а 0, 1, 3, 4 
		if (pDict->Search("C", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 3)
		{
			Object oTemp;
			if (oDictItem.ArrayGet(0, &oTemp)->IsNum())
			{
				dBorderR = oTemp.GetNum();
			}
			oTemp.Free();

			if (oDictItem.ArrayGet(1, &oTemp)->IsNum())
			{
				dBorderG = oTemp.GetNum();
			}
			oTemp.Free();

			if (oDictItem.ArrayGet(2, &oTemp)->IsNum())
			{
				dBorderB = oTemp.GetNum();
			}
			oTemp.Free();
		}
		oDictItem.Free();

		m_pBorderStyle = new AnnotBorderStyle(eBorderType, dBorderWidth, pBorderDash, nBorderDashLength, dBorderR, dBorderG, dBorderB);

		// AP (appereance)
		Object oAP;
		if (pDict->Search("AP", &oAP)->IsDict())
		{
			Object oAS;
			if (pDict->Search("AS", &oAS)->IsName())
			{
				Object oItem;
				if (oAP.DictLookup("N", &oItem)->IsDict())
				{
					Object oTemp;
					if (oItem.DictLookupAndCopy(oAS.GetName(), &oTemp)->IsRef())
					{
						oTemp.Copy(&m_oAppearance);
						m_bValid = true;
					}
					else
					{
						oTemp.Free();
						if (oItem.DictLookupAndCopy("Off", &oTemp)->IsRef())
						{
							oTemp.Copy(&m_oAppearance);
						}
					}
					oTemp.Free();
				}
				oItem.Free();
			}
			else
			{
				Object oItem;
				if (oAP.DictLookupAndCopy("N", &oItem)->IsRef())
				{
					oItem.Copy(&m_oAppearance);
				}
				oItem.Free();
			}
			oAS.Free();
		}
		oAP.Free();
	}
	Annot::~Annot()
	{
		if (m_seType)
		{
			delete m_seType;
		}
		m_oAppearance.Free();

		if (m_seAppBuffer)
		{
			delete m_seAppBuffer;
		}
		if (m_pBorderStyle)
		{
			delete m_pBorderStyle;
		}
	}
	void Annot::GenerateFieldAppearance(Dict *pField, Dict *pAnnot, Dict *pAcroForm)
	{
		// Тип Annotation должен быть Widget
		if (m_seType->Compare("Widget"))
		{
			return;
		}

		m_seAppBuffer = new StringExt();

		// Appearance characteristics (MK) dictionary
		Object oMKObject;
		Dict *pMKDict;
		if (pAnnot->Search("MK", &oMKObject)->IsDict())
		{
			pMKDict = oMKObject.GetDict();
		}
		else
		{
			pMKDict = NULL;
		}

		// Рисуем фон (BackGround)
		Object oDictItem;
		if (pMKDict)
		{
			if (pMKDict->Search("BG", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() > 0)
			{
				SetColor(oDictItem.GetArray(), true, 0);
				m_seAppBuffer->AppendFormat("0 0 {0:.2f} {1:.2f} re f\n", m_dMaxX - m_dMinX, m_dMaxY - m_dMinY);
			}
			oDictItem.Free();
		}

		// Считываем тип поля
		// FT (File Type)
		Object oFTobject;
		FieldLookup(pField, "FT", &oFTobject);

		// Ff (Filed flags)
		int nFiledFlags = 0;
		if (FieldLookup(pField, "Ff", &oDictItem)->IsInt())
		{
			nFiledFlags = oDictItem.GetInt();
		}
		else
		{
			nFiledFlags = 0;
		}
		oDictItem.Free();

		// Рисуем рамку
		if (pMKDict)
		{
			double dWidth = m_pBorderStyle->GetWidth();
			if (dWidth > 0)
			{
				// BC (Border Color), если такого поля нет, тогда используем цвет из BG
				pMKDict->Search("BC", &oDictItem);
				if (!(oDictItem.IsArray() && oDictItem.ArrayGetLength() > 0))
				{
					pMKDict->Search("BG", &oDictItem);
				}
				if (oDictItem.IsArray() && oDictItem.ArrayGetLength() > 0)
				{
					double dDx = m_dMaxX - m_dMinX;
					double dDy = m_dMaxY - m_dMinY;
					double *pDash;
					int nDashLength = 0;

					Object oTemp;
					bool bHasCaption = pMKDict->Search("CA", &oTemp)->IsString();
					oTemp.Free();

					if (oFTobject.IsName("Btn") && (nFiledFlags & fieldFlagRadio) && !bHasCaption)
					{
						double dRad = 0.5 * (dDx < dDy ? dDx : dDy);
						switch (m_pBorderStyle->GetType())
						{
							case annotBorderDashed:
							m_seAppBuffer->Append("[");
							m_pBorderStyle->GetDash(&pDash, &nDashLength);
							for (int nIndex = 0; nIndex < nDashLength; ++nIndex)
							{
								m_seAppBuffer->AppendFormat(" {0:.2f}", pDash[nIndex]);
							}
							m_seAppBuffer->Append("] 0 d\n");
							// Дальше как в случае не пунктирной линии
							case annotBorderSolid:
							case annotBorderUnderlined:
							m_seAppBuffer->AppendFormat("{0:.2f} w\n", dWidth);
							SetColor(oDictItem.GetArray(), false, 0);
							DrawCircle(0.5 * dDx, 0.5 * dDy, dRad - 0.5 * dWidth, false);
							break;
							case annotBorderBeveled:
							case annotBorderInset:
							m_seAppBuffer->AppendFormat("{0:.2f} w\n", 0.5 * dWidth);

							SetColor(oDictItem.GetArray(), false, 0);
							DrawCircle(0.5 * dDx, 0.5 * dDy, dRad - 0.25 * dWidth, false);

							SetColor(oDictItem.GetArray(), false, m_pBorderStyle->GetType() == annotBorderBeveled ? 1 : -1);
							DrawCircleTopLeft(0.5 * dDx, 0.5 * dDy, dRad - 0.75 * dWidth);

							SetColor(oDictItem.GetArray(), false, m_pBorderStyle->GetType() == annotBorderBeveled ? -1 : 1);
							DrawCircleBottomRight(0.5 * dDx, 0.5 * dDy, dRad - 0.75 * dWidth);
							break;
						}
					}
					else
					{
						switch (m_pBorderStyle->GetType())
						{
							case annotBorderDashed:
							m_seAppBuffer->Append("[");
							m_pBorderStyle->GetDash(&pDash, &nDashLength);
							for (int nIndex = 0; nIndex < nDashLength; ++nIndex)
							{
								m_seAppBuffer->AppendFormat(" {0:.2f}", pDash[nIndex]);
							}
							m_seAppBuffer->Append("] 0 d\n");
							// Дальше как в случае не пунктирной линии
							case annotBorderSolid:
							m_seAppBuffer->AppendFormat("{0:.2f} w\n", dWidth);
							SetColor(oDictItem.GetArray(), false, 0);
							m_seAppBuffer->AppendFormat("{0:.2f} {0:.2f} {1:.2f} {2:.2f} re s\n", 0.5 * dWidth, dDx - dWidth, dDy - dWidth);
							break;
							case annotBorderBeveled:
							case annotBorderInset:

							SetColor(oDictItem.GetArray(), true, m_pBorderStyle->GetType() == annotBorderBeveled ? 1 : -1);
							m_seAppBuffer->Append("0 0 m\n");
							m_seAppBuffer->AppendFormat("0 {0:.2f} l\n", dDy);
							m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} l\n", dDx, dDy);
							m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} l\n", dDx - dWidth, dDy - dWidth);
							m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} l\n", dWidth, dDy - dWidth);
							m_seAppBuffer->AppendFormat("{0:.2f} {0:.2f} l\n", dWidth);
							m_seAppBuffer->Append("f\n");

							SetColor(oDictItem.GetArray(), true, m_pBorderStyle->GetType() == annotBorderBeveled ? -1 : 1);
							m_seAppBuffer->Append("0 0 m\n");
							m_seAppBuffer->AppendFormat("{0:.2f} 0 l\n", dDx);
							m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} l\n", dDx, dDy);
							m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} l\n", dDx - dWidth, dDy - dWidth);
							m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} l\n", dDx - dWidth, dWidth);
							m_seAppBuffer->AppendFormat("{0:.2f} {0:.2f} l\n", dWidth);
							m_seAppBuffer->Append("f\n");
							break;
							case annotBorderUnderlined:
							m_seAppBuffer->AppendFormat("{0:.2f} w\n", dWidth);
							SetColor(oDictItem.GetArray(), false, 0);
							m_seAppBuffer->AppendFormat("0 0 m {0:.2f} 0 l s\n", dDx);
							break;
						}
						m_seAppBuffer->AppendFormat("{0:.2f} {0:.2f} {1:.2f} {2:.2f} re W n\n", dWidth, dDx - 2 * dWidth, dDy - 2 * dWidth);
					}
				}
				oDictItem.Free();
			}
		}

		// DR
		Object oDRobject;
		pAcroForm->Search("DR", &oDRobject);

		// Font dictionary
		GrFontDict *pFontDict;
		if (oDRobject.IsDict() && oDRobject.DictLookup("Font", &oDictItem)->IsDict())
		{
			pFontDict = new GrFontDict(m_pXref, NULL, oDictItem.GetDict(), m_pGlobalParams);
		}
		else
		{
			pFontDict = NULL;
		}
		oDictItem.Free();

		// DA (Default appereance string)
		StringExt *seDA;
		if (FieldLookup(pField, "DA", &oDictItem)->IsNull())
		{
			oDictItem.Free();
			pAcroForm->Search("DA", &oDictItem);
		}
		if (oDictItem.IsString())
		{
			seDA = oDictItem.GetString()->Copy();
		}
		else
		{
			seDA = NULL;
		}
		oDictItem.Free();

		// Рисуем содержимео Filed
		if (oFTobject.IsName("Btn"))
		{
			StringExt *seCaption = NULL;
			if (pMKDict)
			{
				if (pMKDict->Search("CA", &oDictItem)->IsString())
				{
					seCaption = oDictItem.GetString()->Copy();
				}
				oDictItem.Free();
			}

			// Radio button
			if (nFiledFlags & fieldFlagRadio)
			{
				// Акробат не рисует Caption, если нет AP
				if (FieldLookup(pField, "V", &oDictItem)->IsName())
				{
					Object oAppStream;
					if (pAnnot->Search("AS", &oAppStream)->IsName(oDictItem.GetName()))
					{
						if (seCaption)
						{
							DrawText(seCaption, seDA, pFontDict, false, 0, fieldQuadCenter, false, true);
						}
						else
						{
							if (pMKDict)
							{
								Object oTemp;
								if (pMKDict->Search("BC", &oTemp)->IsArray() && oTemp.ArrayGetLength() > 0)
								{
									double dDx = m_dMaxX - m_dMinX;
									double dDy = m_dMaxY - m_dMinY;
									SetColor(oTemp.GetArray(), true, 0);
									DrawCircle(0.5 * dDx, 0.5 * dDy, 0.2 * (dDx < dDy ? dDx : dDy), true);
								}
								oTemp.Free();
							}
						}
					}
					oAppStream.Free();
				}
				oDictItem.Free();
			}
			else if (nFiledFlags & fieldFlagPushbutton) // PushButton
			{
				if (seCaption)
				{
					DrawText(seCaption, seDA, pFontDict, false, 0, fieldQuadCenter, false, false);
				}
			}
			else  // CheckBox
			{
				// "Yes" - для включенного состояние, "off" -  для выключенного.
				if (FieldLookup(pField, "V", &oDictItem)->IsName("Yes"))
				{
					if (!seCaption)
					{
						seCaption = new StringExt("3"); // ZapfDingbats checkmark
					}
					DrawText(seCaption, seDA, pFontDict, false, 0, fieldQuadCenter, false, true);
				}
				oDictItem.Free();
			}
			if (seCaption)
			{
				delete seCaption;
			}
		}
		else if (oFTobject.IsName("Tx"))
		{
			// Строковые значения могут быть юникодными
			Object oValue;
			if (FieldLookup(pField, "V", &oValue)->IsString())
			{
				Object oTemp;
				int nQuadding = 0;
				if (FieldLookup(pField, "Q", &oTemp)->IsInt())
				{
					nQuadding = oTemp.GetInt();
				}
				else
				{
					nQuadding = fieldQuadLeft;
				}
				oTemp.Free();
				int nComb = 0;
				if (nFiledFlags & fieldFlagComb)
				{
					if (FieldLookup(pField, "MaxLen", &oTemp)->IsInt())
					{
						nComb = oTemp.GetInt();
					}
					oTemp.Free();
				}
				DrawText(oValue.GetString(), seDA, pFontDict, 0 != (nFiledFlags & fieldFlagMultiline), nComb, nQuadding, true, false);
			}
			oValue.Free();
		}
		else if (oFTobject.IsName("Ch"))
		{
			// Строковые значения могут быть юникодными
			int nQuadding = 0;
			Object oTemp;
			if (FieldLookup(pField, "Q", &oTemp)->IsInt())
			{
				nQuadding = oTemp.GetInt();
			}
			else
			{
				nQuadding = fieldQuadLeft;
			}
			oTemp.Free();

			// ComboBox
			if (nFiledFlags & fieldFlagCombo)
			{
				Object oValue;
				if (FieldLookup(pField, "V", &oValue)->IsString())
				{
					DrawText(oValue.GetString(), seDA, pFontDict, false, 0, nQuadding, true, false);
					//~ Acrobat draws a popup icon on the right side
				}
				oValue.Free();
			}
			else // ListBox
			{
				Object oOptions;
				if (pField->Search("Opt", &oOptions)->IsArray())
				{
					int nOptionsCount = oOptions.ArrayGetLength();
					// Option text
					StringExt **ppText = (StringExt **)MemUtilsMallocArray(nOptionsCount, sizeof(StringExt *));
					for (int nIndex = 0; nIndex < nOptionsCount; ++nIndex)
					{
						ppText[nIndex] = NULL;
						Object oTemp;
						oOptions.ArrayGet(nIndex, &oTemp);
						if (oTemp.IsString())
						{
							ppText[nIndex] = oTemp.GetString()->Copy();
						}
						else if (oTemp.IsArray() && oTemp.ArrayGetLength() == 2)
						{
							Object oArrayItem;
							if (oTemp.ArrayGet(1, &oArrayItem)->IsString())
							{
								ppText[nIndex] = oArrayItem.GetString()->Copy();
							}
							oArrayItem.Free();
						}
						oTemp.Free();
						if (!ppText[nIndex])
						{
							ppText[nIndex] = new StringExt();
						}
					}
					// Selected option(s)
					bool *pSelection = (bool *)MemUtilsMallocArray(nOptionsCount, sizeof(bool));
					//~ need to use the I field in addition to the V field
					Object oValue;
					FieldLookup(pField, "V", &oValue);
					for (int nIndex = 0; nIndex < nOptionsCount; ++nIndex)
					{
						pSelection[nIndex] = false;
						if (oValue.IsString())
						{
							if (!oValue.GetString()->Compare(ppText[nIndex]))
							{
								pSelection[nIndex] = true;
							}
						}
						else if (oValue.IsArray())
						{
							for (int nJ = 0; nJ < oValue.ArrayGetLength(); ++nJ)
							{
								Object oTemp;
								if (oValue.ArrayGet(nJ, &oTemp)->IsString() && !oTemp.GetString()->Compare(ppText[nIndex]))
								{
									pSelection[nIndex] = true;
								}
								oTemp.Free();
							}
						}
					}
					oValue.Free();

					// TI (Top Index)
					int nTopIndex = 0;
					if (pField->Search("TI", &oValue)->IsInt())
					{
						nTopIndex = oValue.GetInt();
					}
					else
					{
						nTopIndex = 0;
					}
					oValue.Free();

					DrawListBox(ppText, pSelection, nOptionsCount, nTopIndex, seDA, pFontDict, nQuadding);
					for (int nIndex = 0; nIndex < nOptionsCount; ++nIndex)
					{
						delete ppText[nIndex];
					}
					MemUtilsFree(ppText);
					MemUtilsFree(pSelection);
				}
				oOptions.Free();
			}
		}
		else if (oFTobject.IsName("Sig"))
		{
			//~unimp
		}
		else
		{
			// TO DO: Error "Unknown field type"
		}

		if (seDA)
		{
			delete seDA;
		}

		// Appearance stream dictionary
		Object oAppearanceDict;

		oAppearanceDict.InitDict(m_pXref);
		oAppearanceDict.DictAdd(CopyString("Length"), oDictItem.InitInt(m_seAppBuffer->GetLength()));
		oAppearanceDict.DictAdd(CopyString("Subtype"), oDictItem.InitName("Form"));
		oDictItem.InitArray(m_pXref);
		Object oTemp;
		oDictItem.ArrayAdd(oTemp.InitReal(0));
		oDictItem.ArrayAdd(oTemp.InitReal(0));
		oDictItem.ArrayAdd(oTemp.InitReal(m_dMaxX - m_dMinX));
		oDictItem.ArrayAdd(oTemp.InitReal(m_dMaxY - m_dMinY));
		oAppearanceDict.DictAdd(CopyString("BBox"), &oDictItem);

		// Resource dictionary
		if (oDRobject.IsDict())
		{
			oAppearanceDict.DictAdd(CopyString("Resources"), oDRobject.Copy(&oDictItem));
		}
		oDRobject.Free();

		// Appearance stream
		MemoryStream *pAppearStream = new MemoryStream(m_seAppBuffer->GetBuffer(), 0, m_seAppBuffer->GetLength(), &oAppearanceDict);
		m_oAppearance.Free();
		m_oAppearance.InitStream(pAppearStream);

		if (pFontDict)
		{
			delete pFontDict;
		}
		oFTobject.Free();
		oMKObject.Free();
	}

	// Устанавливаем цвет для заливки или обводки; цвет здается массиом <pArray> (в котором должно быть
	// 1, 3, или 4 элемента).  Если <nAdjust> равно +1, цвет color is высветляется, если <nAdjust> равно
	// -1, цвет затемняется, в противном случае, цвет не меняется.
	void Annot::SetColor(Array *pArray, bool bFill, int nAdjust)
	{
		double arrColor[4];

		int nComponentsCount = pArray->GetCount();
		if (nComponentsCount > 4)
		{
			nComponentsCount = 4;
		}
		for (int nIndex = 0; nIndex < nComponentsCount && nIndex < 4; ++nIndex)
		{
			Object oTemp;
			if (pArray->Get(nIndex, &oTemp)->IsNum())
			{
				arrColor[nIndex] = oTemp.GetNum();
			}
			else
			{
				arrColor[nIndex] = 0;
			}
			oTemp.Free();
		}
		if (4 == nComponentsCount)
		{
			nAdjust = -nAdjust;
		}
		if (nAdjust > 0)
		{
			for (int nIndex = 0; nIndex < nComponentsCount; ++nIndex)
			{
				arrColor[nIndex] = 0.5 * arrColor[nIndex] + 0.5;
			}
		}
		else if (nAdjust < 0)
		{
			for (int nIndex = 0; nIndex < nComponentsCount; ++nIndex)
			{
				arrColor[nIndex] = 0.5 * arrColor[nIndex];
			}
		}
		if (4 == nComponentsCount)
		{
			m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:c}\n", arrColor[0], arrColor[1], arrColor[2], arrColor[3], bFill ? 'k' : 'K');
		}
		else if (3 == nComponentsCount)
		{
			m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:s}\n", arrColor[0], arrColor[1], arrColor[2], bFill ? "rg" : "RG");
		}
		else
		{
			m_seAppBuffer->AppendFormat("{0:.2f} {1:c}\n", arrColor[0], bFill ? 'g' : 'G');
		}
	}

	// Рисуем текст или название для Field.
	void Annot::DrawText(StringExt *seText, StringExt *seDA, GrFontDict *pFontDict, bool bMultiline, int nComb, int nQuadding, bool bTextField, bool bForceZapfDingbats)
	{
		CList *pListDA;
		StringExt *seToken;
		double dX, dY, dWidth;
		int  i, j, k, nChar;

		// Парсим Default Appearance string

		int nTfPos = -1, nTmPos = -1;
		if (seDA)
		{
			pListDA = new CList();
			i = 0;
			while (i < seDA->GetLength())
			{
				while (i < seDA->GetLength() && Lexer::IsSpace(seDA->GetAt(i)))
				{
					++i;
				}
				if (i < seDA->GetLength())
				{
					for (j = i + 1; j < seDA->GetLength() && !Lexer::IsSpace(seDA->GetAt(j)); ++j);
					pListDA->Append(new StringExt(seDA, i, j - i));
					i = j;
				}
			}
			for (i = 2; i < pListDA->GetLength(); ++i)
			{
				if (i >= 2 && !((StringExt *)pListDA->GetByIndex(i))->Compare("Tf"))
				{
					nTfPos = i - 2;
				}
				else if (i >= 6 && !((StringExt *)pListDA->GetByIndex(i))->Compare("Tm"))
				{
					nTmPos = i - 6;
				}
			}
		}
		else
		{
			pListDA = NULL;
		}

		// ZapfDingbats
		if (bForceZapfDingbats)
		{
			if (nTfPos >= 0)
			{
				seToken = (StringExt *)pListDA->GetByIndex(nTfPos);
				if (seToken->Compare("/ZaDb"))
				{
					seToken->Clear();
					seToken->Append("/ZaDb");
				}
			}
		}

		// Считываем шрифт и размер шрифта
		GrFont *pFont = NULL;
		double dFontSize = 0;
		if (nTfPos >= 0)
		{
			seToken = (StringExt *)pListDA->GetByIndex(nTfPos);
			if (seToken->GetLength() >= 1 && seToken->GetAt(0) == '/')
			{
				if (!pFontDict || !(pFont = pFontDict->Search(seToken->GetBuffer() + 1)))
				{
					// TO DO: Error "Unknown font in field's DA string"
				}
			}
			else
			{
				// TO DO: Error "Invalid font name in 'Tf' operator in field's DA string"
			}
			seToken = (StringExt *)pListDA->GetByIndex(nTfPos + 1);
			dFontSize = atof(seToken->GetBuffer());
		}
		else
		{
			// TO DO: Error "Missing 'Tf' operator in field's DA string"
		}

		// Border Width
		double dBorderWidth = m_pBorderStyle->GetWidth();

		if (bTextField)
		{
			m_seAppBuffer->Append("/Tx BMC\n");
		}
		m_seAppBuffer->Append("q\n");
		m_seAppBuffer->Append("BT\n");

		// Многострочный текст
		if (bMultiline)
		{
			// Флаги nComb игнорируются в многострочном тексте

			double dWMax = m_dMaxX - m_dMinX - 2 * dBorderWidth - 4;

			// Вычислим размер шрифта (если он не был установлен)
			if (0 == dFontSize)
			{
				for (dFontSize = 20; dFontSize > 1; --dFontSize)
				{
					dY = m_dMaxY - m_dMinY;
					double dMaxLineWidth = 0;
					i = 0;
					while (i < seText->GetLength())
					{
						GetNextLine(seText, i, pFont, dFontSize, dWMax, &j, &dWidth, &k);
						if (dWidth > dMaxLineWidth)
						{
							dMaxLineWidth = dWidth;
						}
						i = k;
						dY -= dFontSize;
					}

					if (dY >= 0.33 * dFontSize)
					{
						break;
					}
				}
				if (nTfPos >= 0)
				{
					seToken = (StringExt *)pListDA->GetByIndex(nTfPos + 1);
					seToken->Clear();
					seToken->AppendFormat("{0:.2f}", dFontSize);
				}
			}

			// Начальная координата Y
			// (Каждая новая линий начинается с оператора Td)
			dY = m_dMaxY - m_dMinY;

			// Font Matrix
			if (nTmPos >= 0)
			{
				seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 4);
				seToken->Clear();
				seToken->Append('0');
				seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 5);
				seToken->Clear();
				seToken->AppendFormat("{0:.2f}", dY);
			}

			// Пишем DA-string
			if (pListDA)
			{
				for (i = 0; i < pListDA->GetLength(); ++i)
				{
					m_seAppBuffer->Append((StringExt *)pListDA->GetByIndex(i))->Append(' ');
				}
			}

			// Пишем Font Matrix (если она не была уже в DA-string)
			if (nTmPos < 0)
			{
				m_seAppBuffer->AppendFormat("1 0 0 1 0 {0:.2f} Tm\n", dY);
			}

			i = 0;
			double dPrevX = 0;
			while (i < seText->GetLength())
			{
				GetNextLine(seText, i, pFont, dFontSize, dWMax, &j, &dWidth, &k);
				// Вычислим начальную позицию текста
				switch (nQuadding)
				{
					case fieldQuadLeft:
					default:
					dX = dBorderWidth + 2;
					break;
					case fieldQuadCenter:
					dX = (m_dMaxX - m_dMinX - dWidth) / 2;
					break;
					case fieldQuadRight:
					dX = m_dMaxX - m_dMinX - dBorderWidth - 2 - dWidth;
					break;
				}

				m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} Td\n", dX - dPrevX, -dFontSize);
				m_seAppBuffer->Append('(');
				for (; i < j; ++i)
				{
					nChar = seText->GetAt(i) & 0xff;
					if (nChar == '(' || nChar == ')' || nChar == '\\')
					{
						m_seAppBuffer->Append('\\');
						m_seAppBuffer->Append(nChar);
					}
					else if (nChar < 0x20 || nChar >= 0x80)
					{
						m_seAppBuffer->AppendFormat("\\{0:03o}", nChar);
					}
					else
					{
						m_seAppBuffer->Append(nChar);
					}
				}
				m_seAppBuffer->Append(") Tj\n");

				// Следующая строка
				i = k;
				dPrevX = dX;
			}

		}
		else // Однострочный текст
		{
			if (nComb > 0)
			{
				dWidth = (m_dMaxX - m_dMinX - 2 * dBorderWidth) / nComb;

				// Вычислим размер шрифта (если он не был установлен)
				if (0 == dFontSize)
				{
					dFontSize = m_dMaxY - m_dMinY - 2 * dBorderWidth;
					if (dWidth < dFontSize)
					{
						dFontSize = dWidth;
					}
					dFontSize = floor(dFontSize);
					if (nTfPos >= 0)
					{
						seToken = (StringExt *)pListDA->GetByIndex(nTfPos + 1);
						seToken->Clear();
						seToken->AppendFormat("{0:.2f}", dFontSize);
					}
				}

				// Вычислим начальную позицию текста
				switch (nQuadding)
				{
					case fieldQuadLeft:
					default:
					dX = dBorderWidth + 2;
					break;
					case fieldQuadCenter:
					dX = dBorderWidth + 2 + 0.5 * (nComb - seText->GetLength()) * dWidth;
					break;
					case fieldQuadRight:
					dX = dBorderWidth + 2 + (nComb - seText->GetLength()) * dWidth;
					break;
				}
				dY = 0.5 * (m_dMaxY - m_dMinY) - 0.4 * dFontSize;

				// Font Matrix
				if (nTmPos >= 0)
				{
					seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 4);
					seToken->Clear();
					seToken->AppendFormat("{0:.2f}", dX);
					seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 5);
					seToken->Clear();
					seToken->AppendFormat("{0:.2f}", dY);
				}

				// DA-string
				if (pListDA)
				{
					for (i = 0; i < pListDA->GetLength(); ++i)
					{
						m_seAppBuffer->Append((StringExt *)pListDA->GetByIndex(i))->Append(' ');
					}
				}

				// Пишем Font Matrix (если она не была уже в DA-string)
				if (nTmPos < 0)
				{
					m_seAppBuffer->AppendFormat("1 0 0 1 {0:.2f} {1:.2f} Tm\n", dX, dY);
				}

				// write the text string
				//~ this should center (instead of left-justify) each character within
				//~     its comb cell
				for (i = 0; i < seText->GetLength(); ++i)
				{
					if (i > 0)
					{
						m_seAppBuffer->AppendFormat("{0:.2f} 0 Td\n", dWidth);
					}
					m_seAppBuffer->Append('(');
					nChar = seText->GetAt(i) & 0xff;
					if (nChar == '(' || nChar == ')' || nChar == '\\')
					{
						m_seAppBuffer->Append('\\');
						m_seAppBuffer->Append(nChar);
					}
					else if (nChar < 0x20 || nChar >= 0x80)
					{
						m_seAppBuffer->AppendFormat("{0:.2f} 0 Td\n", dWidth);
					}
					else
					{
						m_seAppBuffer->Append(nChar);
					}
					m_seAppBuffer->Append(") Tj\n");
				}
			}
			else // обычное форматирование (non-comb)
			{
				// Вычислим ширину строки
				if (pFont && !pFont->IsCIDFont())
				{
					dWidth = 0;
					for (i = 0; i < seText->GetLength(); ++i)
					{
						dWidth += ((Gr8BitFont *)pFont)->GetWidth(seText->GetAt(i));
					}
				}
				else
				{
					dWidth = seText->GetLength() * 0.5;
				}

				// Вычислим размер шрифта (если он не был установлен)
				if (0 == dFontSize)
				{
					dFontSize = m_dMaxY - m_dMinY - 2 * dBorderWidth;
					double dTempFontSize = (m_dMaxX - m_dMinX - 4 - 2 * dBorderWidth) / dWidth;
					if (dTempFontSize < dFontSize)
					{
						dFontSize = dTempFontSize;
					}
					dFontSize = floor(dFontSize);
					if (nTfPos >= 0)
					{
						seToken = (StringExt *)pListDA->GetByIndex(nTfPos + 1);
						seToken->Clear();
						seToken->AppendFormat("{0:.2f}", dFontSize);
					}
				}

				// Вычислим начальную позицию текста
				dWidth *= dFontSize;
				switch (nQuadding)
				{
					case fieldQuadLeft:
					default:
					dX = dBorderWidth + 2;
					break;
					case fieldQuadCenter:
					dX = (m_dMaxX - m_dMinX - dWidth) / 2;
					break;
					case fieldQuadRight:
					dX = m_dMaxX - m_dMinX - dBorderWidth - 2 - dWidth;
					break;
				}
				dY = 0.5 * (m_dMaxY - m_dMinY) - 0.4 * dFontSize;

				// Font Matrix
				if (nTmPos >= 0)
				{
					seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 4);
					seToken->Clear();
					seToken->AppendFormat("{0:.2f}", dX);
					seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 5);
					seToken->Clear();
					seToken->AppendFormat("{0:.2f}", dY);
				}

				// DA-string
				if (pListDA)
				{
					for (i = 0; i < pListDA->GetLength(); ++i)
					{
						m_seAppBuffer->Append((StringExt *)pListDA->GetByIndex(i))->Append(' ');
					}
				}

				// Пишем Font Matrix (если она не была уже в DA-string)
				if (nTmPos < 0)
				{
					m_seAppBuffer->AppendFormat("1 0 0 1 {0:.2f} {1:.2f} Tm\n", dX, dY);
				}

				// Пишем текст
				m_seAppBuffer->Append('(');
				for (i = 0; i < seText->GetLength(); ++i)
				{
					nChar = seText->GetAt(i) & 0xff;
					if (nChar == '(' || nChar == ')' || nChar == '\\')
					{
						m_seAppBuffer->Append('\\');
						m_seAppBuffer->Append(nChar);
					}
					else if (nChar < 0x20 || nChar >= 0x80)
					{
						m_seAppBuffer->AppendFormat("\\{0:03o}", nChar);
					}
					else
					{
						m_seAppBuffer->Append(nChar);
					}
				}
				m_seAppBuffer->Append(") Tj\n");
			}
		}

		m_seAppBuffer->Append("ET\n");
		m_seAppBuffer->Append("Q\n");

		if (bTextField)
		{
			m_seAppBuffer->Append("EMC\n");
		}

		if (pListDA)
		{
			DeleteCList(pListDA, StringExt);
		}
	}

	// Рисуем текст или название для Field.
	void Annot::DrawListBox(StringExt **ppText, bool *pSelection, int nOptionsCount, int nTopIndex, StringExt *seDA, GrFontDict *pFontDict, int nQuadding)
	{
		CList *pListDA;
		StringExt *seToken;
		int  i, j;

		// Парсим Default appearance string
		int nTfPos = -1, nTmPos = -1;
		if (seDA)
		{
			pListDA = new CList();
			i = 0;
			while (i < seDA->GetLength())
			{
				while (i < seDA->GetLength() && Lexer::IsSpace(seDA->GetAt(i)))
				{
					++i;
				}
				if (i < seDA->GetLength())
				{
					for (j = i + 1; j < seDA->GetLength() && !Lexer::IsSpace(seDA->GetAt(j)); ++j);
					pListDA->Append(new StringExt(seDA, i, j - i));
					i = j;
				}
			}
			for (i = 2; i < pListDA->GetLength(); ++i)
			{
				if (i >= 2 && !((StringExt *)pListDA->GetByIndex(i))->Compare("Tf"))
				{
					nTfPos = i - 2;
				}
				else if (i >= 6 && !((StringExt *)pListDA->GetByIndex(i))->Compare("Tm"))
				{
					nTmPos = i - 6;
				}
			}
		}
		else
		{
			pListDA = NULL;
		}

		// Считываем шрифт и размер шрифта
		GrFont *pFont = NULL;
		double dFontSize = 0;
		if (nTfPos >= 0)
		{
			seToken = (StringExt *)pListDA->GetByIndex(nTfPos);
			if (seToken->GetLength() >= 1 && seToken->GetAt(0) == '/')
			{
				if (!pFontDict || !(pFont = pFontDict->Search(seToken->GetBuffer() + 1)))
				{
					// TO DO: Error "Unknown font in field's DA string"
				}
			}
			else
			{
				// TO DO: Error "Invalid font name in 'Tf' operator in field's DA string"
			}
			seToken = (StringExt *)pListDA->GetByIndex(nTfPos + 1);
			dFontSize = atof(seToken->GetBuffer());
		}
		else
		{
			// TO DO: Error "Missing 'Tf' operator in field's DA string"
		}

		// Border width
		double dBorder = m_pBorderStyle->GetWidth();

		// Вычислим размер фонта, если он не был задан
		double dWidth = 0;
		if (0 == dFontSize)
		{
			double dMaxW = 0;
			for (i = 0; i < nOptionsCount; ++i)
			{
				if (pFont && !pFont->IsCIDFont())
				{
					dWidth = 0;
					for (j = 0; j < ppText[i]->GetLength(); ++j)
					{
						dWidth += ((Gr8BitFont *)pFont)->GetWidth(ppText[i]->GetAt(j));
					}
				}
				else
				{
					// грубая оценка
					dWidth = ppText[i]->GetLength() * 0.5;
				}
				if (dWidth > dMaxW)
				{
					dMaxW = dWidth;
				}
			}
			dFontSize = m_dMaxY - m_dMinY - 2 * dBorder;
			double dFontSize2 = (m_dMaxX - m_dMinX - 4 - 2 * dBorder) / dMaxW;

			if (dFontSize2 < dFontSize)
			{
				dFontSize = dFontSize2;
			}
			dFontSize = floor(dFontSize);

			if (nTfPos >= 0)
			{
				seToken = (StringExt *)pListDA->GetByIndex(nTfPos + 1);
				seToken->Clear();
				seToken->AppendFormat("{0:.2f}", dFontSize);
			}
		}

		// Рисуем текст
		double dY = m_dMaxY - m_dMinY - 1.1 * dFontSize;

		for (i = nTopIndex; i < nOptionsCount; ++i)
		{
			m_seAppBuffer->Append("q\n");

			// Background
			if (pSelection[i])
			{
				m_seAppBuffer->Append("0 g f\n");
				m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} re f\n", dBorder, dY - 0.2 * dFontSize, m_dMaxX - m_dMinX - 2 * dBorder, 1.1 * dFontSize);
			}

			m_seAppBuffer->Append("BT\n");

			// Вычислим ширину строки
			if (pFont && !pFont->IsCIDFont())
			{
				dWidth = 0;
				for (j = 0; j < ppText[i]->GetLength(); ++j)
				{
					dWidth += ((Gr8BitFont *)pFont)->GetWidth(ppText[i]->GetAt(j));
				}
			}
			else
			{
				// грубая оценка
				dWidth = ppText[i]->GetLength() * 0.5;
			}

			// Вычислим начальную позицию текста
			double dX = 0;
			dWidth *= dFontSize;
			switch (nQuadding)
			{
				case fieldQuadLeft:
				default:
				dX = dBorder + 2;
				break;
				case fieldQuadCenter:
				dX = (m_dMaxX - m_dMinX - dWidth) / 2;
				break;
				case fieldQuadRight:
				dX = m_dMaxX - m_dMinX - dBorder - 2 - dWidth;
				break;
			}

			// Font Matrix
			if (nTmPos >= 0)
			{
				seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 4);
				seToken->Clear();
				seToken->AppendFormat("{0:.2f}", dX);
				seToken = (StringExt *)pListDA->GetByIndex(nTmPos + 5);
				seToken->Clear();
				seToken->AppendFormat("{0:.2f}", dY);
			}

			// DA string
			if (pListDA)
			{
				for (j = 0; j < pListDA->GetLength(); ++j)
				{
					m_seAppBuffer->Append((StringExt *)pListDA->GetByIndex(j))->Append(' ');
				}
			}

			// Пишем Font Matrix (если он не был частью DA-string)
			if (nTmPos < 0)
			{
				m_seAppBuffer->AppendFormat("1 0 0 1 {0:.2f} {1:.2f} Tm\n", dX, dY);
			}

			if (pSelection[i])
			{
				m_seAppBuffer->Append("1 g\n");
			}

			// Пишем текст
			m_seAppBuffer->Append('(');
			for (j = 0; j < ppText[i]->GetLength(); ++j)
			{
				int nChar = ppText[i]->GetAt(j) & 0xff;
				if (nChar == '(' || nChar == ')' || nChar == '\\')
				{
					m_seAppBuffer->Append('\\');
					m_seAppBuffer->Append(nChar);
				}
				else if (nChar < 0x20 || nChar >= 0x80)
				{
					m_seAppBuffer->AppendFormat("\\{0:03o}", nChar);
				}
				else
				{
					m_seAppBuffer->Append(nChar);
				}
			}
			m_seAppBuffer->Append(") Tj\n");

			m_seAppBuffer->Append("ET\n");
			m_seAppBuffer->Append("Q\n");

			// Следующая строка
			dY -= 1.1 * dFontSize;
		}

		if (pListDA)
		{
			DeleteCList(pListDA, StringExt);
		}
	}

	void Annot::GetNextLine(StringExt *seText, int nStart, GrFont *pFont, double dFontSize, double dMaxW, int *pEnd, double *pWidth, int *pNext)
	{
		double dWidth = 0, dTempWidth;
		int j, k;


		for (j = nStart; j < seText->GetLength() && dWidth <= dMaxW; ++j)
		{
			int nChar = seText->GetAt(j) & 0xff;
			if (nChar == 0x0a || nChar == 0x0d)
			{
				break;
			}
			if (pFont && !pFont->IsCIDFont())
			{
				dTempWidth = ((Gr8BitFont *)pFont)->GetWidth(nChar) * dFontSize;
			}
			else
			{
				// грубая оценка
				dTempWidth = 0.5 * dFontSize;
			}
			dWidth += dTempWidth;
		}
		if (dWidth > dMaxW)
		{
			for (k = j; k > nStart && seText->GetAt(k - 1) != ' '; --k);
			for (; k > nStart && seText->GetAt(k - 1) == ' '; --k);
			if (k > nStart)
			{
				j = k;
			}

			if (j == nStart)
			{
				// Обрабатываем странный случай, когда первый символ слишком широкий для первой строки
				j = nStart + 1;
			}
		}
		*pEnd = j;

		// Вычисляем ширину
		dWidth = 0;
		for (k = nStart; k < j; ++k)
		{
			if (pFont && !pFont->IsCIDFont())
			{
				dTempWidth = ((Gr8BitFont *)pFont)->GetWidth(seText->GetAt(k)) * dFontSize;
			}
			else
			{
				dTempWidth = 0.5 * dFontSize;
			}
			dWidth += dTempWidth;
		}
		*pWidth = dWidth;

		while (j < seText->GetLength() && seText->GetAt(j) == ' ')
		{
			++j;
		}
		if (j < seText->GetLength() && seText->GetAt(j) == 0x0d)
		{
			++j;
		}
		if (j < seText->GetLength() && seText->GetAt(j) == 0x0a)
		{
			++j;
		}
		*pNext = j;
	}


	// Расстояние от центра до контрольных точек кривой Безье
	// = (4 * (sqrt(2) - 1) / 3) * r
	#define Kappa 0.55228475

	// Рисуем окружность с помощью кривых Безье. (dX, dY) - центр, dRad - радиус.
	// bFill - будем заливать или обводить?
	void Annot::DrawCircle(double dX, double dY, double dRad, bool bFill)
	{
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} m\n", dX + dRad, dY);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX + dRad, dY + Kappa * dRad, dX + Kappa * dRad, dY + dRad, dX, dY + dRad);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX - Kappa * dRad, dY + dRad, dX - dRad, dY + Kappa * dRad, dX - dRad, dY);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX - dRad, dY - Kappa * dRad, dX - Kappa * dRad, dY - dRad, dX, dY - dRad);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX + Kappa * dRad, dY - dRad, dX + dRad, dY - Kappa * dRad, dX + dRad, dY);
		m_seAppBuffer->Append(bFill ? "f\n" : "s\n");
	}

	// Рисуем половину окружности (левую верхнюю) с помощью кривых Безье. (dX, dY) - центр, dRad - радиус.
	void Annot::DrawCircleTopLeft(double dX, double dY, double dRad)
	{
		double dRad2 = dRad / sqrt(2.0);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} m\n", dX + dRad2, dY + dRad2);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX + (1 - Kappa) * dRad2, dY + (1 + Kappa) * dRad2, dX - (1 - Kappa) * dRad2, dY + (1 + Kappa) * dRad2, dX - dRad2, dY + dRad2);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX - (1 + Kappa) * dRad2, dY + (1 - Kappa) * dRad2, dX - (1 + Kappa) * dRad2, dY - (1 - Kappa) * dRad2, dX - dRad2, dY - dRad2);
		m_seAppBuffer->Append("S\n");
	}

	// Рисуем половину окружности (правую нижнюю) с помощью кривых Безье. (dX, dY) - центр, dRad - радиус.
	void Annot::DrawCircleBottomRight(double dX, double dY, double dRad)
	{
		double dRad2 = dRad / sqrt(2.0);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} m\n", dX - dRad2, dY - dRad2);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX - (1 - Kappa) * dRad2, dY - (1 + Kappa) * dRad2, dX + (1 - Kappa) * dRad2, dY - (1 + Kappa) * dRad2, dX + dRad2, dY - dRad2);
		m_seAppBuffer->AppendFormat("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f} {5:.2f} c\n", dX + (1 + Kappa) * dRad2, dY - (1 - Kappa) * dRad2, dX + (1 + Kappa) * dRad2, dY + (1 - Kappa) * dRad2, dX + dRad2, dY + dRad2);
		m_seAppBuffer->Append("S\n");
	}

	Object *Annot::FieldLookup(Dict *pField, char *sKey, Object *pObject)
	{
		Dict *pDict = pField;
		if (!pDict->Search(sKey, pObject)->IsNull())
		{
			return pObject;
		}
		pObject->Free();

		Object oParent;
		if (pDict->Search("Parent", &oParent)->IsDict())
		{
			FieldLookup(oParent.GetDict(), sKey, pObject);
		}
		else
		{
			pObject->InitNull();
		}
		oParent.Free();
		return pObject;
	}

	void Annot::Draw(Graphics *pGraphics, bool bPrinting)
	{
		// Flags
		if ((m_nFlags & annotFlagHidden) || (bPrinting && !(m_nFlags & annotFlagPrint)) || (!bPrinting && (m_nFlags & annotFlagNoView)))
		{
			return;
		}

		// Appearance stream
		bool bIsLink = m_seType && !m_seType->Compare("Link");

		Object oTemp;
		m_oAppearance.Fetch(m_pXref, &oTemp);
		pGraphics->DrawAnnotation(&oTemp, (bIsLink ? m_pBorderStyle : (AnnotBorderStyle *)NULL), m_dMinX, m_dMinY, m_dMaxX, m_dMaxY);
		oTemp.Free();
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Annots
	//-------------------------------------------------------------------------------------------------------------------------------

	Annots::Annots(GlobalParams *pGlobalParams, XRef *pXref, Catalog *pCatalog, Object *pAnnotsObject)
	{
		m_pGlobalParams = pGlobalParams;

		Annot *pAnnot = NULL;
		Ref oRef;

		m_ppAnnots = NULL;
		int nSize = 0;
		m_nAnnotsCount = 0;

		Dict *pAcroForm = (pCatalog->GetAcroForm()->IsDict() ? pCatalog->GetAcroForm()->GetDict() : NULL);
		if (pAnnotsObject->IsArray())
		{
			for (int nIndex = 0; nIndex < pAnnotsObject->ArrayGetLength(); ++nIndex)
			{
				Object oTemp;
				if (pAnnotsObject->ArrayGetCopy(nIndex, &oTemp)->IsRef())
				{
					oRef = oTemp.GetRef();
					oTemp.Free();
					pAnnotsObject->ArrayGet(nIndex, &oTemp);
				}
				else
				{
					oRef.nNum = oRef.nGen = -1;
				}
				if (oTemp.IsDict())
				{
					pAnnot = new Annot(m_pGlobalParams, pXref, pAcroForm, oTemp.GetDict(), &oRef);
					if (pAnnot->CheckValidation())
					{
						if (m_nAnnotsCount >= nSize)
						{
							nSize += 16;
							m_ppAnnots = (Annot **)MemUtilsReallocArray(m_ppAnnots, nSize, sizeof(Annot *));
						}
						m_ppAnnots[m_nAnnotsCount++] = pAnnot;
					}
					else
					{
						delete pAnnot;
					}
				}
				oTemp.Free();
			}
		}
	}

	Annots::~Annots()
	{
		for (int nIndex = 0; nIndex < m_nAnnotsCount; ++nIndex)
		{
			delete m_ppAnnots[nIndex];
		}
		MemUtilsFree(m_ppAnnots);
	}

	void Annots::GenerateAppearances(Dict *pAcroForm)
	{
		Ref oRef;

		Object oField;
		if (pAcroForm->Search("Fields", &oField)->IsArray())
		{
			for (int nIndex = 0; nIndex < oField.ArrayGetLength(); ++nIndex)
			{
				Object oItem;
				if (oField.ArrayGetCopy(nIndex, &oItem)->IsRef())
				{
					oRef = oItem.GetRef();
					oItem.Free();
					oField.ArrayGet(nIndex, &oItem);
				}
				else
				{
					oRef.nNum = oRef.nGen = -1;
				}
				if (oItem.IsDict())
				{
					ScanFieldAppearances(oItem.GetDict(), &oRef, NULL, pAcroForm);
				}
				oItem.Free();
			}
		}
		oField.Free();
	}

	void Annots::ScanFieldAppearances(Dict *pNode, Ref *pRef, Dict *pParent, Dict *pAcroForm)
	{
		Annot *pAnnot = NULL;
		Ref oRef2;

		Object oNodeItem;
		if (pNode->Search("Kids", &oNodeItem)->IsArray())
		{
			for (int nIndex = 0; nIndex < oNodeItem.ArrayGetLength(); ++nIndex)
			{
				Object oTemp;
				if (oNodeItem.ArrayGetCopy(nIndex, &oTemp)->IsRef())
				{
					oRef2 = oTemp.GetRef();
					oTemp.Free();
					oNodeItem.ArrayGet(nIndex, &oTemp);
				}
				else
				{
					oRef2.nNum = oRef2.nGen = -1;
				}
				if (oTemp.IsDict())
				{
					ScanFieldAppearances(oTemp.GetDict(), &oRef2, pNode, pAcroForm);
				}
				oTemp.Free();
			}
			oNodeItem.Free();
			return;
		}
		oNodeItem.Free();

		if ((pAnnot = FindAnnot(pRef)))
		{
			pNode->SearchAndCopy("Parent", &oNodeItem);
			if (!pParent || !oNodeItem.IsNull())
			{
				pAnnot->GenerateFieldAppearance(pNode, pNode, pAcroForm);
			}
			else
			{
				pAnnot->GenerateFieldAppearance(pParent, pNode, pAcroForm);
			}
			oNodeItem.Free();
		}
	}

	Annot *Annots::FindAnnot(Ref *pRef)
	{
		for (int nIndex = 0; nIndex < m_nAnnotsCount; ++nIndex)
		{
			if (m_ppAnnots[nIndex]->Match(pRef))
			{
				return m_ppAnnots[nIndex];
			}
		}
		return NULL;
	}
}
