#ifndef CSDT_H
#define CSDT_H

#include "../CElement.h"
#include "../document/CDocumentXml.h"
#include "../../CBookToc.h"
#include "../docRels/CDocRelationshipsXml.h"
#include "CSdtPr.h"
#include "CSdtEndPr.h"
#include "CSdtContent.h"

class CSdt : public CElement
{
    CSdtPr      *m_oSdtPr;
    CSdtEndPr   *m_oSdtEndPr;
    CSdtContent *m_oSdtContent;

    int nCountHyperlinks;
public:
    CSdt();

    void SetDefoult() override;
    void AddHeader(std::pair<std::wstring, std::wstring> pTextAndRef, CDocumentXml *oWhereToAdd, CDocRelationshipsXml *oDocRel);

};

#endif // CSDT_H
