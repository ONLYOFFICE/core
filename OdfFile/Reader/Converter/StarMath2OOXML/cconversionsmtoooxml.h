#ifndef CCONVERSIONSMTOOOXML_H
#define CCONVERSIONSMTOOOXML_H
#include "cstarmathpars.h"
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"



namespace StarMath {
	class CConversionSMtoOOXML
	{
	public:
		CConversionSMtoOOXML();
		void StartConversion(std::vector<CElement*> arPars);
		static void StandartProperties(XmlUtils::CXmlWriter* pXmlWrite);
		static void PropertiesMFPR(bool bType,const std::wstring& wsType, XmlUtils::CXmlWriter* pXmlWrite);
		static void PropertiesNaryPr(const std::wstring& wsTypeOperator,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWrite);
		static void PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWrite);
		static void BlockRecording(const std::wstring& wsNameBlock,CElement* CValueBlock,XmlUtils::CXmlWriter* pXmlWrite);
		void EndConversion();
		std::wstring GetOOXML() ;
	private:
		XmlUtils::CXmlWriter* m_oXmlWrite;
	};
}
#endif // CCONVERSIONSMTOOOXML_H
