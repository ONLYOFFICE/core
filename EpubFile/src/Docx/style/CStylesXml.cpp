#include "CStylesXml.h"
#include "CStyles.h"

CStylesXml::CStylesXml()
    : CXmlFile()
{

}

void CStylesXml::SetDefoult()
{
    CStyles *oStyles = new CStyles;
    oStyles->SetDefoult();
    SetXmlStructure(oStyles);
}
