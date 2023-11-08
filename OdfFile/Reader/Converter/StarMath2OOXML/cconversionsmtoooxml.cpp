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
	void CConversionSMtoOOXML::StandartProperties(XmlUtils::CXmlWriter* oXmlWrite)
	{
		oXmlWrite->WriteNodeBegin(L"w:rPr",false);
		oXmlWrite->WriteNodeBegin(L"w:rFonts",true);
		oXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
		oXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNodeBegin(L"w:sz",true);
		oXmlWrite->WriteAttribute(L"w:val",L"40");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNodeBegin(L"w:szCs",true);
		oXmlWrite->WriteAttribute(L"w:val",L"40");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesMFPR(bool bType,const std::wstring& wsType, XmlUtils::CXmlWriter* oXmlWrite)
	{
		oXmlWrite->WriteNodeBegin(L"m:fPr",false);
		if(bType)
		{
			oXmlWrite->WriteNodeBegin(L"m:type",true);
			oXmlWrite->WriteAttribute(L"m:val",wsType);
			oXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		oXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		oXmlWrite->WriteNodeBegin(L"w:rPr",false);
		oXmlWrite->WriteNodeBegin(L"w:rFonts",true);
		oXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
		oXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNode(L"w:i",L"");
		//m_oXmlWrite->WriteNode(L"w:iCs",L"");
		oXmlWrite->WriteNodeBegin(L"w:sz",true);
		oXmlWrite->WriteAttribute(L"w:val",L"40");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNodeBegin(L"w:szCs",true);
		oXmlWrite->WriteAttribute(L"w:val",L"40");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
		oXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		oXmlWrite->WriteNodeEnd(L"m:fPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesNaryPr(const std::wstring& wsTypeOperator,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* oXmlWrite)
	{
		oXmlWrite->WriteNodeBegin(L"m:naryPr",false);
		oXmlWrite->WriteNodeBegin(L"m:chr",true);
		oXmlWrite->WriteAttribute(L"m:val",wsTypeOperator);
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNodeBegin(L"m:limLoc",true);
		oXmlWrite->WriteAttribute(L"m:val",L"undOvr");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		if(bEmptySub)
		{
			oXmlWrite->WriteNodeBegin(L"m:subHide",true);
			oXmlWrite->WriteAttribute(L"m:val",L"1");
			oXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		if(bEmptySup)
		{
			oXmlWrite->WriteNodeBegin(L"m:supHide",true);
			oXmlWrite->WriteAttribute(L"m:val",L"1");
			oXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		oXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		oXmlWrite->WriteNodeBegin(L"w:rPr",false);
		oXmlWrite->WriteNodeBegin(L"w:rFonts",true);
		oXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
		oXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
		oXmlWrite->WriteNodeEnd(L"w",true,true);
		oXmlWrite->WriteNode(L"w:i",L"");
		oXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
		oXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		oXmlWrite->WriteNodeEnd(L"m:naryPr",false,false);
	}
	void CConversionSMtoOOXML::BlockRecording(const std::wstring &wsNameBlock, CElement *CValueBlock,XmlUtils::CXmlWriter* oXmlWrite)
	{
		oXmlWrite->WriteNodeBegin(wsNameBlock,false);
		CValueBlock->ConversionToOOXML(oXmlWrite);
		oXmlWrite->WriteNodeEnd(wsNameBlock,false,false);
	}
	std::wstring CConversionSMtoOOXML::GetOOXML()
	{
		return m_oXmlWrite->GetXmlString();
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		//m_oXmlWrite->WriteNodeEnd(L"m:r",false,false);
		m_oXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
		m_oXmlWrite->WriteNodeEnd(L"m:oMathPara",false,false);
	}
}

