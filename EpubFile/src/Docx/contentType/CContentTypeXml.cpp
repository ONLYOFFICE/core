#include "CContentTypeXml.h"
#include "CContentType.h"

CContentTypeXml::CContentTypeXml()
    : CXmlFile()
{
}

void CContentTypeXml::SetDefoult()
{
    CContentType *oContentType = new CContentType;
    oContentType->SetDefoult();
    SetXmlStructure(oContentType);
}
