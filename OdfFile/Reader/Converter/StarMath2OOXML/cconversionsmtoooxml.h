#ifndef CCONVERSIONSMTOOOXML_H
#define CCONVERSIONSMTOOOXML_H
#include "cstarmathpars.h"
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"



namespace StarMath {
//delete XmlWrite
	class CConversionSMtoOOXML
	{
	public:
		CConversionSMtoOOXML();
		void StartConversion(std::vector<CElement*> arPars);
		static void StandartProperties(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute);
		static void PropertiesMFPR(bool bType,const std::wstring& wsType, XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute);
		static void PropertiesNaryPr(const TypeElement& enTypeOp,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute);
		static void PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute);
		static void BlockRecording(const std::wstring& wsNameBlock,CElement* CValueBlock,XmlUtils::CXmlWriter* pXmlWrite);
		static void PropertiesDPr(XmlUtils::CXmlWriter* pXmlWrite,const TypeElement& enTypeBracket,CAttribute* pAttribute);
		static void PropertiesMPr(XmlUtils::CXmlWriter* pXmlWrite,const TypeElement& enTypeMatrix,CAttribute* pAttribute);
		static void NodeGrade(XmlUtils::CXmlWriter* pXmlWrite,CElement* pValueGrade,CAttribute* pAttribute);
		static void WriteCtrlPrNode(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute);
		static void WriteChrNode(const std::wstring& wsTypeOp,XmlUtils::CXmlWriter* pXmlWrite);
		static void WriteLimLocNode(const std::wstring& wsTypeLimLock,XmlUtils::CXmlWriter* pXmlWrite);
		static void WrtieRPrFName(const TypeElement& enTypeOp,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute);
		void EndConversion();
		std::wstring GetOOXML();
	private:
		static void BracketTypeNotation(const std::wstring& wsOpenBracket,const std::wstring& wsCloseBracket, XmlUtils::CXmlWriter* pXmlWrite);
		XmlUtils::CXmlWriter* m_pXmlWrite;
	};
}
#endif // CCONVERSIONSMTOOOXML_H
