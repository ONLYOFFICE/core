#include "CFontTableXml.h"
#include "CFontTable.h"

CFontTableXml::CFontTableXml()
    :CXmlFile()
{
}

void CFontTableXml::SetDefoult()
{
    CFontTable *oFontTable = new CFontTable();
    oFontTable->SetDefoult();
    SetXmlStructure(oFontTable);
}
