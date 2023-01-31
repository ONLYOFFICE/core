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
		//Запоминаем позицию чтобы в конце записать туда длину
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
