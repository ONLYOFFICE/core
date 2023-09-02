#ifndef CCONVERSIONSMTOOOXML_H
#define CCONVERSIONSMTOOOXML_H
#include "cstarmathpars.h"
#include "X:/Rabotka/Clone/core/DesktopEditor/xml/include/xmlwriter.h"

namespace ConversionSM2OOXML {
	class CConversionSMtoOOXML
	{
	public:
		CConversionSMtoOOXML();
		//friend class StarMath::CStarMathPars;
		void StartConversion(std::vector<StarMath::CElement*> arPars);
		void StandartProperties();
		void ConversionOneElement(StarMath::CElement* m_oElement);
		void ConversBinOperator(StarMath::CBinaryOperator* m_oElement);
		void ConversNumber(StarMath::CNumber* m_oElement);
		void EndConversion();
	private:
		XmlUtils::CXmlWriter m_oXmlWrite;
	};
}
#endif // CCONVERSIONSMTOOOXML_H
