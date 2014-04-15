#include "stdafx.h"
#include "CommonWriter.h"
#include "../Common/BinReaderWriterDefines.h"

namespace BinXlsxRW {
	BinaryCommonWriter::BinaryCommonWriter(Streams::CBufferedStream &oCBufferedStream):m_oStream(oCBufferedStream)
	{
	}
	int BinaryCommonWriter::WriteItemStart(BYTE type)
	{
		//type
		m_oStream.WriteByte(type);
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
		m_oStream.Seek(nStart);
		m_oStream.WriteLong(nEnd - nStart - 4);
		m_oStream.Seek(nEnd);
	}
	void BinaryCommonWriter::WriteColor(const OOX::Spreadsheet::CColor& color, OOX::Spreadsheet::CIndexedColors* pIndexedColors, OOX::CTheme* theme)
	{
		if(color.m_oAuto.IsInit() && color.m_oAuto->ToBool())
		{
			m_oStream.WriteByte(c_oSer_ColorObjectType::Type);
			m_oStream.WriteByte(c_oSerPropLenType::Byte);
			m_oStream.WriteByte(c_oSer_ColorType::Auto);
		}
		else
		{
			bool bEmpty = true;
			SimpleTypes::Spreadsheet::CHexColor oRgbColor;
			if(color.m_oIndexed.IsInit())
			{
				int nIndex = (int)color.m_oIndexed->GetValue();
				if(NULL != pIndexedColors && nIndex < pIndexedColors->m_arrItems.GetSize())
				{
					OOX::Spreadsheet::CRgbColor* pRgbColor = pIndexedColors->m_arrItems[nIndex];
					if(pRgbColor->m_oRgb.IsInit())
					{
						bEmpty = false;
						oRgbColor = pRgbColor->m_oRgb.get();
					}
				}
				else
				{
					unsigned char ucA;
					unsigned char ucR;
					unsigned char ucG;
					unsigned char ucB;
					if(OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(nIndex, ucR, ucG, ucB, ucA))
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
				m_oStream.WriteByte(c_oSer_ColorObjectType::Theme);
				m_oStream.WriteByte(c_oSerPropLenType::Byte);
				m_oStream.WriteByte((BYTE)color.m_oThemeColor->GetValue());
			}
			if(color.m_oTint.IsInit())
			{
				m_oStream.WriteByte(c_oSer_ColorObjectType::Tint);
				m_oStream.WriteByte(c_oSerPropLenType::Double);
				m_oStream.WriteDouble(color.m_oTint->GetValue());
			}
			if(!bEmpty)
			{
				m_oStream.WriteByte(c_oSer_ColorObjectType::Rgb);
				m_oStream.WriteByte(c_oSerPropLenType::Long);
				m_oStream.WriteLong(oRgbColor.ToInt());
			}
		}
	}
	void BinaryCommonWriter::WriteSafeArray(SAFEARRAY* pBinaryObj)
	{
		int nCurPos = WriteItemWithLengthStart();
		m_oStream.WritePointer((BYTE *)pBinaryObj->pvData, pBinaryObj->rgsabound[0].cElements);
		WriteItemWithLengthEnd(nCurPos);
	}
}