#include "stdafx.h"
#include "CommonWriter.h"
#include "../Common/BinReaderWriterDefines.h"

namespace BinXlsxRW {
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
	void BinaryCommonWriter::WriteColor(const OOX::Spreadsheet::CColor& color, OOX::Spreadsheet::CIndexedColors* pIndexedColors, OOX::CTheme* theme)
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
				if(NULL != pIndexedColors && nIndex < pIndexedColors->m_arrItems.size())
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