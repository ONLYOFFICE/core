#include "CDocRelationshipsXml.h"
#include "CDocRelationships.h"

CDocRelationshipsXml::CDocRelationshipsXml()
    : CXmlFile()
{

}

void CDocRelationshipsXml::SetDefoult()
{
    CDocRelationships *oDocRelationships = new CDocRelationships;
    oDocRelationships->SetDefoult();
    SetXmlStructure(oDocRelationships);
}
