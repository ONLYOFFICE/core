#include "cconversionsmtoooxml.h"
#include "../../../../DesktopEditor/common/File.h"
#include <fstream>
namespace StarMath {
	CConversionSMtoOOXML::CConversionSMtoOOXML()
	{
	}
	void CConversionSMtoOOXML::StartConversion(std::vector<StarMath::CElement*> arPars)
	{
		m_oXmlWrite = new XmlUtils::CXmlWriter;
		m_oXmlWrite->WriteNodeBegin(L"m:oMathPara",false);
		m_oXmlWrite->WriteNodeBegin(L"m:oMath",false);
		for(CElement* oTempElement:arPars)
		{
			oTempElement->ConversionToOOXML(m_oXmlWrite);
		}
		EndConversion();
//		NSFile::CFileBinary oFile;
//		oFile.CreateFileW(L"Test.txt");
//		oFile.WriteStringUTF8(m_oXmlWrite->GetXmlString());
//		oFile.CloseFile();
	}
	void CConversionSMtoOOXML::StandartProperties(XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"w:rPr",false);
		pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
		pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
		pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"w:sz",true);
		pXmlWrite->WriteAttribute(L"w:val",L"40");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"w:szCs",true);
		pXmlWrite->WriteAttribute(L"w:val",L"40");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesMFPR(bool bType,const std::wstring& wsType, XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:fPr",false);
		if(bType)
		{
			pXmlWrite->WriteNodeBegin(L"m:type",true);
			pXmlWrite->WriteAttribute(L"m:val",wsType);
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		pXmlWrite->WriteNodeBegin(L"w:rPr",false);
		pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
		pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
		pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNode(L"w:i",L"");
		//m_pXmlWrite->WriteNode(L"w:iCs",L"");
		pXmlWrite->WriteNodeBegin(L"w:sz",true);
		pXmlWrite->WriteAttribute(L"w:val",L"40");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"w:szCs",true);
		pXmlWrite->WriteAttribute(L"w:val",L"40");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:fPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesNaryPr(const std::wstring& wsTypeOperator,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:naryPr",false);
		pXmlWrite->WriteNodeBegin(L"m:chr",true);
		pXmlWrite->WriteAttribute(L"m:val",wsTypeOperator);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:limLoc",true);
		pXmlWrite->WriteAttribute(L"m:val",L"undOvr");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		if(bEmptySub)
		{
			pXmlWrite->WriteNodeBegin(L"m:subHide",true);
			pXmlWrite->WriteAttribute(L"m:val",L"1");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		if(bEmptySup)
		{
			pXmlWrite->WriteNodeBegin(L"m:supHide",true);
			pXmlWrite->WriteAttribute(L"m:val",L"1");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		pXmlWrite->WriteNodeBegin(L"w:rPr",false);
		pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
		pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
		pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNode(L"w:i",L"");
		pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:naryPr",false,false);
	}
	void CConversionSMtoOOXML::BlockRecording(const std::wstring &wsNameBlock, CElement *CValueBlock,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(wsNameBlock,false);
		CValueBlock->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeEnd(wsNameBlock,false,false);
	}
	std::wstring CConversionSMtoOOXML::GetOOXML()
	{
		return m_oXmlWrite->GetXmlString();
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		//m_pXmlWrite->WriteNodeEnd(L"m:r",false,false);
		m_oXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
		m_oXmlWrite->WriteNodeEnd(L"m:oMathPara",false,false);
	}
	void CConversionSMtoOOXML::PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:funcPr", false);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr", false);
		StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr", false,false);
		pXmlWrite->WriteNodeEnd(L"m:funcPr",false,false);
	}

	void CConversionSMtoOOXML::PropertiesDPr(XmlUtils::CXmlWriter *pXmlWrite, const TypeElement &enTypeBracket)
	{
		pXmlWrite->WriteNodeBegin(L"m:dPr",false);
		switch(enTypeBracket)
		{
			case TypeElement::langle:
				BracketTypeNotation(L"\u2329",L"\u232A",pXmlWrite);
				break;
			case TypeElement::square:
				BracketTypeNotation(L"\u005B",L"\u005D",pXmlWrite);
				break;
			case TypeElement::ldbracket:
				BracketTypeNotation(L"\u27E6",L"\u27E7",pXmlWrite);
				break;
			case TypeElement::lbrace:
				BracketTypeNotation(L"\u007B",L"\u007D",pXmlWrite);
				break;
			case TypeElement::lceil:
				BracketTypeNotation(L"\u23A1",L"\u23A4",pXmlWrite);
				break;
			case TypeElement::lfloor:
				BracketTypeNotation(L"\u23A3",L"\u23A6",pXmlWrite);
				break;
			case TypeElement::lline:
				BracketTypeNotation(L"\u23AA",L"\u23AA",pXmlWrite);
				break;
			case TypeElement::ldline:
				BracketTypeNotation(L"\u2016",L"\u2016",pXmlWrite);
				break;
		}
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr");
		StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:dPr",false,false);
	}
	void CConversionSMtoOOXML::BracketTypeNotation(const std::wstring &wsOpenBracket, const std::wstring &wsCloseBracket, XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:begChr", true);
		pXmlWrite->WriteAttribute(L"m:val",wsOpenBracket);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:endChr", true);
		pXmlWrite->WriteAttribute(L"m:val", wsCloseBracket);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::PropertiesMPr(XmlUtils::CXmlWriter *pXmlWrite, const TypeElement &enTypeMatrix)
	{
		pXmlWrite->WriteNodeBegin(L"m:mPr",false);
		pXmlWrite->WriteNodeBegin(L"m:mcs",false);
		pXmlWrite->WriteNodeBegin(L"m:mc",false);
		pXmlWrite->WriteNodeBegin(L"m:mcPr",false);
		pXmlWrite->WriteNodeBegin(L"m:count",true);
		switch(enTypeMatrix)
		{
			case TypeElement::matrix:
				pXmlWrite->WriteAttribute(L"m:val",L"2");
				break;
			default:
				pXmlWrite->WriteAttribute(L"m:val",L"1");
				break;
		}
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:mcJc",true);
		pXmlWrite->WriteAttribute(L"m:val",L"center");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:mcPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mc",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mcs",false,false);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mPr",false,false);
	}
	void CConversionSMtoOOXML::BlocGrade(XmlUtils::CXmlWriter *pXmlWrite,CElement* pValueGrade)
	{
		pXmlWrite->WriteNodeBegin(L"m:d",false);
		pXmlWrite->WriteNodeBegin(L"m:dPr",false);
		pXmlWrite->WriteNodeBegin(L"m:begChr",true);
		pXmlWrite->WriteAttribute(L"m:val",L"");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:endChr",true);
		pXmlWrite->WriteAttribute(L"m:val",L"\u23AA");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false);
		pXmlWrite->WriteNodeEnd(L"m:dPr",false,false);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		pValueGrade->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		pXmlWrite->WriteNodeEnd(L"m:d",false,false);
	}
}

