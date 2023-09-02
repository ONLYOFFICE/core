#include "cconversionsmtoooxml.h"
namespace ConversionSM2OOXML {
	CConversionSMtoOOXML::CConversionSMtoOOXML()
	{
	}
	void CConversionSMtoOOXML::StartConversion(std::vector<StarMath::CElement*> arPars)
	{
		m_oXmlWrite.WriteNodeBegin(L"m:oMathPara",false);
		m_oXmlWrite.WriteNodeBegin(L"m:oMath",false);
		m_oXmlWrite.WriteNodeBegin(L"m:r",false);
		StandartProperties();
		for(StarMath::CElement* m_oTempElement:arPars)
		{
			ConversionOneElement(m_oTempElement);
		}
		EndConversion();
		std::wcout<< m_oXmlWrite.GetXmlString()<<std::endl;
	}
	void CConversionSMtoOOXML::ConversionOneElement(StarMath::CElement *m_oElement)
	{
		if(m_oElement->GetType() == 0)
		{
			ConversNumber(dynamic_cast<StarMath::CNumber*>(m_oElement));
		}
		else if(m_oElement->GetType() == 1)
		{
			ConversBinOperator(dynamic_cast<StarMath::CBinaryOperator*> (m_oElement));
		}
	}
	void CConversionSMtoOOXML::ConversBinOperator(StarMath::CBinaryOperator *m_oElement)
	{
		if(m_oElement->GetTypeBin() == 3 || m_oElement->GetTypeBin() == 4)
		{
			m_oXmlWrite.WriteNodeBegin(L"m:t",false);
			ConversionOneElement(m_oElement->GetLeftArg());
			if(m_oElement->GetTypeRight() == 0)
			{
				if(m_oElement->GetTypeBin() == 3) m_oXmlWrite.WriteString(L"+");
				else m_oXmlWrite.WriteString(L"-");
				ConversionOneElement(m_oElement->GetRightArg());
			}
			m_oXmlWrite.WriteNodeEnd(L"m:t",false,false);
		}
	}
	void CConversionSMtoOOXML::ConversNumber(StarMath::CNumber *m_oElement)
	{
		m_oXmlWrite.WriteString(m_oElement->GetValue());
	}
	void CConversionSMtoOOXML::StandartProperties()
	{
		m_oXmlWrite.WriteNodeBegin(L"m:rPr",false);
		m_oXmlWrite.WriteNodeBegin(L"m:sty",true);
		m_oXmlWrite.WriteAttribute(L"m:val",L"p");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeEnd(L"m:rPr",false,false);
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
		m_oXmlWrite.WriteNodeBegin(L"w:lang",true);
		m_oXmlWrite.WriteAttribute(L"w:val",L"en-US");
		m_oXmlWrite.WriteNodeEnd(L"w",true,true);
		m_oXmlWrite.WriteNodeEnd(L"w:rPr",false,false);
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		m_oXmlWrite.WriteNodeEnd(L"m:r",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:oMath",false,false);
		m_oXmlWrite.WriteNodeEnd(L"m:oMathPara",false,false);
	}
}
