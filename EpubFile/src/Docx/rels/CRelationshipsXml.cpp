#include "CRelationshipsXml.h"
#include "CRelationships.h"

CRelationshipsXml::CRelationshipsXml()
    : CXmlFile()
{

}

void CRelationshipsXml::SetDefoult()
{
    CRelationships *oRelationships = new CRelationships;
    oRelationships->SetDefoult();
    SetXmlStructure(oRelationships);
}
