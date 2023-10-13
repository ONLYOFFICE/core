#include "cconversionsmtoooxml.h"
#include "../../../../DesktopEditor/common/File.h"
#include <fstream>
/*namespace StarMath {
	CConversionSMtoOOXML::CConversionSMtoOOXML()
	{
	}
	void CConversionSMtoOOXML::StartConversion(std::vector<StarMath::CElement*> arPars)
	{
		m_oXmlWrite.WriteNodeBegin(L"m:oMathPara",false);
		m_oXmlWrite.WriteNodeBegin(L"m:oMath",false);
		for(CElement* m_oTempElement:arPars)
		{
			ConversionOneElement(m_oTempElement);
		}
		EndConversion();
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(L"Test.txt");
		oFile.WriteStringUTF8(m_oXmlWrite.GetXmlString());
		oFile.CloseFile();
	}
	void CConversionSMtoOOXML::ConversionOneElement(CElement *m_oElement)
	{
		switch (m_oElement->GetType())
		{
		case Text:
			ConversText(dynamic_cast<CText*>(m_oElement));
		break;
		case BinOperator:
			ConversBinOperator(dynamic_cast<CBinaryOperator*> (m_oElement));
		break;
		case Operator:
			ConversOperator(dynamic_cast<COperator*> (m_oElement));
		break;
		default:
		break;
		}
	}
	void CConversionSMtoOOXML::ConversBinOperator(CBinaryOperator *m_oElement)
	{
		if(m_oElement->GetTypeBin() == plus || m_oElement->GetTypeBin() == minus || m_oElement->GetTypeBin() == multipl || m_oElement->GetTypeBin() == cdot || m_oElement->GetTypeBin() == times || m_oElement->GetTypeBin() == div || m_oElement->GetTypeBin() == odivide || m_oElement->GetTypeBin() == oplus || m_oElement->GetTypeBin() == ominus || m_oElement->GetTypeBin() == odot || m_oElement->GetTypeBin() == otimes)
		{
			ConversionOneElement(m_oElement->GetFirstElement());
			m_oXmlWrite.WriteNodeBegin(L"m:r",false);
			StandartProperties();
			m_oXmlWrite.WriteNodeBegin(L"m:t",false);
			switch (m_oElement->GetTypeBin())
			{
			case plus: m_oXmlWrite.WriteString(L"+");break;
			case minus:
				m_oXmlWrite.WriteString(L"-");
			break;
			case multipl:
				m_oXmlWrite.WriteString(L"*");
			break;
			case cdot:
				m_oXmlWrite.WriteString(L"\u00B7");
			break;
			case times:
				m_oXmlWrite.WriteString(L"\u00D7");
			break;
			case div:
				m_oXmlWrite.WriteString(L"\u00F7");
			break;
			case odivide:
				m_oXmlWrite.WriteString(L"\u2298");
			break;
			case oplus:
				m_oXmlWrite.WriteString(L"\u2295");
			break;
			case ominus:
				m_oXmlWrite.WriteString(L"\u2296");
			break;
			case odot:
				m_oXmlWrite.WriteString(L"\u2299");
			break;
			case otimes:
				m_oXmlWrite.WriteString(L"\u2297");
			break;
			default:
			break;
			}
			if(m_oElement->GetTypeSecondElement() == Text)
			{
				CText* m_oNumber = dynamic_cast<CText*> (m_oElement->GetSecondElement());
				m_oXmlWrite.WriteString(m_oNumber->GetValue());
				m_oXmlWrite.WriteNodeEnd(L"m:t",false,false);
				m_oXmlWrite.WriteNodeEnd(L"m:r",false,false);
			}
			else
			{
				m_oXmlWrite.WriteNodeEnd(L"m:t",false,false);
				m_oXmlWrite.WriteNodeEnd(L"m:r",false,false);
				ConversionOneElement(m_oElement->GetSecondElement());
			}

		}
		else if(m_oElement->GetTypeBin() == over || m_oElement->GetTypeBin() == division)
		{
			m_oXmlWrite.WriteNodeBegin(L"m:f",false);
			if(m_oElement->GetTypeBin() == division) PropertiesMFPR(true,L"lin");
			else PropertiesMFPR(false,L"");
			BlockRecording(L"m:num",m_oElement->GetFirstElement());
			BlockRecording(L"m:den",m_oElement->GetSecondElement());
			m_oXmlWrite.WriteNodeEnd(L"m:f",false,false);
		}
	}
	void CConversionSMtoOOXML::ConversText(CText *m_oElement)
	{
		m_oXmlWrite.WriteNodeBegin(L"m:r",false);
		StandartProperties();
		m_oXmlWrite.WriteNodeBegin(L"m:t",false);
		m_oXmlWrite.WriteString(m_oElement->GetValue());
		m_oXmlWrite.WriteNodeEnd(L"m:t",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:r",false,false);
	}
	void CConversionSMtoOOXML::ConversOperator(COperator *m_oElement)
	{
		if(m_oElement->GetTypeOp() == sum)
		{
			m_oXmlWrite.WriteNodeBegin(L"m:nary",false);
			PropertiesNaryPr(L"\u2211",nullptr == m_oElement->GetFrom(),nullptr == m_oElement->GetTo());
			if(m_oElement->GetFrom() == nullptr) m_oXmlWrite.WriteNode(L"m:sub",L"");
			else
			{
				BlockRecording(L"m:sub",m_oElement->GetFrom());
			}
			if(m_oElement->GetTo() == nullptr) m_oXmlWrite.WriteNode(L"m:sup",L"");
			else
			{
				BlockRecording(L"m:sup",m_oElement->GetTo());
			}
			BlockRecording(L"m:e",m_oElement->GetValueOp());
			m_oXmlWrite.WriteNodeEnd(L"m:nary",false,false);
		}
	}
	void CConversionSMtoOOXML::StandartProperties()
	{
		m_oXmlWrite.WriteNodeBegin(L"w:rPr",false);
		m_oXmlWrite.WriteNodeBegin(L"w:rFonts",true);
		m_oXmlWrite.WriteAttribute(L"w:hAnsi",L"Cambria Math");
		m_oXmlWrite.WriteAttribute(L"w:ascii",L"Cambria Math");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeBegin(L"w:sz",true);
		m_oXmlWrite.WriteAttribute(L"w:val",L"40");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeBegin(L"w:szCs",true);
		m_oXmlWrite.WriteAttribute(L"w:val",L"40");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeEnd(L"w:rPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesMFPR(bool bType,const std::wstring& wsType)
	{
		m_oXmlWrite.WriteNodeBegin(L"m:fPr",false);
		if(bType)
		{
			m_oXmlWrite.WriteNodeBegin(L"m:type",true);
			m_oXmlWrite.WriteAttribute(L"m:val",wsType);
			m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		}
		m_oXmlWrite.WriteNodeBegin(L"m:ctrlPr",false);
		m_oXmlWrite.WriteNodeBegin(L"w:rPr",false);
		m_oXmlWrite.WriteNodeBegin(L"w:rFonts",true);
		m_oXmlWrite.WriteAttribute(L"w:hAnsi",L"Cambria Math");
		m_oXmlWrite.WriteAttribute(L"w:ascii",L"Cambria Math");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNode(L"w:i",L"");
		//m_oXmlWrite.WriteNode(L"w:iCs",L"");
		m_oXmlWrite.WriteNodeBegin(L"w:sz",true);
		m_oXmlWrite.WriteAttribute(L"w:val",L"40");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeBegin(L"w:szCs",true);
		m_oXmlWrite.WriteAttribute(L"w:val",L"40");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeEnd(L"w:rPr",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:ctrlPr",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:fPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesNaryPr(const std::wstring& wsTypeOperator,bool bEmptySub,bool bEmptySup)
	{
		m_oXmlWrite.WriteNodeBegin(L"m:naryPr",false);
		m_oXmlWrite.WriteNodeBegin(L"m:chr",true);
		m_oXmlWrite.WriteAttribute(L"m:val",wsTypeOperator);
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeBegin(L"m:limLoc",true);
		m_oXmlWrite.WriteAttribute(L"m:val",L"undOvr");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		if(bEmptySub)
		{
			m_oXmlWrite.WriteNodeBegin(L"m:subHide",true);
			m_oXmlWrite.WriteAttribute(L"m:val",L"1");
			m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		}
		if(bEmptySup)
		{
			m_oXmlWrite.WriteNodeBegin(L"m:supHide",true);
			m_oXmlWrite.WriteAttribute(L"m:val",L"1");
			m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		}
		m_oXmlWrite.WriteNodeBegin(L"m:ctrlPr",false);
		m_oXmlWrite.WriteNodeBegin(L"w:rPr",false);
		m_oXmlWrite.WriteNodeBegin(L"w:rFonts",true);
		m_oXmlWrite.WriteAttribute(L"w:hAnsi",L"Cambria Math");
		m_oXmlWrite.WriteAttribute(L"w:ascii",L"Cambria Math");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNode(L"w:i",L"");
		m_oXmlWrite.WriteNodeEnd(L"w:rPr",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:ctrlPr",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:naryPr",false,false);
	}
	void CConversionSMtoOOXML::BlockRecording(const std::wstring &wsNameBlock, CElement *CValueBlock)
	{
		m_oXmlWrite.WriteNodeBegin(wsNameBlock,false);
		ConversionOneElement(CValueBlock);
		m_oXmlWrite.WriteNodeEnd(wsNameBlock,false,false);
	}
	std::wstring CConversionSMtoOOXML::GetOOXML()
	{
		return m_oXmlWrite.GetXmlString();
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		//m_oXmlWrite.WriteNodeEnd(L"m:r",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:oMath",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:oMathPara",false,false);
	}
}
*/
