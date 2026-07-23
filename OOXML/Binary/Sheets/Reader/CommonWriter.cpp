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
//#include "stdafx.h"
#include "CommonWriter.h"
#include "../Common/BinReaderWriterDefines.h"
#include "../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../XlsxFormat/Styles/Styles.h"
#include "../../../XlsxFormat/Worksheets/Worksheet.h"

namespace BinXlsxRW 
{
	BinaryCommonWriter::BinaryCommonWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream):m_oStream(oCBufferedStream)
	{
	}
	int BinaryCommonWriter::WriteItemStart(BYTE type)
	{
		//type
		m_oStream.WriteBYTE(type);
		return WriteItemWithLengthStart();
	}
	void BinaryCommonWriter::WriteItemEnd(int nStart)
	{
		WriteItemWithLengthEnd(nStart);
	}
	int BinaryCommonWriter::WriteItemWithLengthStart()
	{
		//Save position to write length at the end
		int nStartPos = m_oStream.GetPosition();
		m_oStream.Skip(4);	
		return nStartPos;
	}
	void BinaryCommonWriter::WriteItemWithLengthEnd(int nStart)
	{
		//Length
		int nEnd = m_oStream.GetPosition();
		m_oStream.SetPosition(nStart);
		m_oStream.WriteLONG(nEnd - nStart - 4);
		m_oStream.SetPosition(nEnd);
	}
	void BinaryCommonWriter::WriteColor(const OOX::Spreadsheet::CColor& color, OOX::Spreadsheet::CIndexedColors* pIndexedColors)
	{
		if(color.m_oAuto.IsInit() && color.m_oAuto->ToBool())
		{
			m_oStream.WriteBYTE(c_oSer_ColorObjectType::Type);
			m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oStream.WriteBYTE(c_oSer_ColorType::Auto);
		}
		else
		{
			bool bEmpty = true;
			SimpleTypes::Spreadsheet::CHexColor oRgbColor;
			
			if(color.m_oIndexed.IsInit())
			{
				int nIndex = (int)color.m_oIndexed->GetValue();
				
				bool bDefault = true;

				if (pIndexedColors)
				{
					std::map<int, OOX::Spreadsheet::CRgbColor*>::iterator pFind = pIndexedColors->mapIndexedColors.find(nIndex);

					if(pFind != pIndexedColors->mapIndexedColors.end())
					{
						OOX::Spreadsheet::CRgbColor* pRgbColor = pFind->second;
						if(pRgbColor->m_oRgb.IsInit())
						{
							bEmpty = false;
							oRgbColor = pRgbColor->m_oRgb.get();

							bDefault = false;
						}
					}
				}
				if (bDefault)
				{
					unsigned char ucA;
					unsigned char ucR;
					unsigned char ucG;
					unsigned char ucB;
					//64 is auto color
					if(64 != nIndex && OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(nIndex, ucR, ucG, ucB, ucA))
					{
						bEmpty = false;
						oRgbColor.Set_A(ucA);
						oRgbColor.Set_R(ucR);
						oRgbColor.Set_G(ucG);
						oRgbColor.Set_B(ucB);
					}
				}
			}
			else if(color.m_oRgb.IsInit())
			{
				bEmpty = false;
				oRgbColor = color.m_oRgb.get();
			}
			if(color.m_oThemeColor.IsInit())
			{
				m_oStream.WriteBYTE(c_oSer_ColorObjectType::Theme);
				m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oStream.WriteBYTE((BYTE)color.m_oThemeColor->GetValue());
			}
			if(color.m_oTint.IsInit())
			{
				m_oStream.WriteBYTE(c_oSer_ColorObjectType::Tint);
				m_oStream.WriteBYTE(c_oSerPropLenType::Double);
				m_oStream.WriteDoubleReal(color.m_oTint->GetValue());
			}
			if(!bEmpty)
			{
				m_oStream.WriteBYTE(c_oSer_ColorObjectType::Rgb);
				m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oStream.WriteLONG(oRgbColor.ToInt());
			}
		}
	}
	void BinaryCommonWriter::WriteBytesArray(BYTE* pData, long nDataSize)
	{
		int nCurPos = WriteItemWithLengthStart();
		m_oStream.WriteBYTEArray(pData, nDataSize);
		WriteItemWithLengthEnd(nCurPos);
	}
}
