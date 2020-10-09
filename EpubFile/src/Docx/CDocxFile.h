#ifndef CDOCXFILE_H
#define CDOCXFILE_H

#include <iostream>

#include "document/CDocumentXml.h"
#include "contentType/CContentTypeXml.h"
#include "fontTable/CFontTableXml.h"
#include "settings/CSettingsXml.h"
#include "webSettings/CWebSettingsXml.h"
#include "style/CStylesXml.h"
#include "rels/CRelationshipsXml.h"
#include "docRels/CDocRelationshipsXml.h"
#include "app/CAppXml.h"
#include "core/CCoreXml.h"

#include "document/CParagraph.h"
#include "../CBookToc.h"

class CDocxFile
{
    std::wstring m_sTempDir;
    std::wstring m_sRelsDir;
    std::wstring m_sWordDir;
    std::wstring m_sDocPropsDir;

    CDocumentXml        *m_oDocumentXml;
    CContentTypeXml     *m_oContentTypeXml;
    CFontTableXml       *m_oFontTableXml;
    CSettingsXml        *m_oSettingsXml;
    CWebSettingsXml     *m_oWebSettingsXml;
    CStylesXml          *m_oStylesXml;
    CRelationshipsXml   *m_oRelationshipsXml;
    CDocRelationshipsXml*m_oDocRelationshipsXml;
    CAppXml             *m_oAppXml;
    CCoreXml            *m_oCoreXml;

    std::wstring sTemplate;
    std::wstring sApplication;
    std::wstring sCompany;
    int nTotalTime;
    int nCountPages;
    int nCountCharacters;
    int nDocSecurity;
    int nCharactersWithSpaces;
    double dAppVersion;
    bool bScaleCrop;
    bool bLinksUpToDate;
    bool bSharedDoc;
    bool bHyperlinksChanged;

    bool CreateRelsDir();
    bool CreateWordDir();
    bool CreateDocPropsDir();

    bool CreateDocumentXml();
    bool CreateFontTableXml();
    bool CreateSettingsXml();
    bool CreateWebSettingsXml();
    bool CreateStylesXml();
    bool CreateContentTypesXml();
    bool CreateThemeXml();
    bool CreateRelsXml();
    bool CreateDocRelsXml();
    bool CreateAppXml();
    bool CreateCoreXml();
public:
    CDocxFile();
    ~CDocxFile();

    bool CreateTempFiles(std::wstring sPathFile, std::wstring sPathTempDir);

    bool AddParagraph(std::wstring sText, bool bNewPAge = false);
    bool AddParagraph(CParagraph *oParagraph);
    bool AddBookToc(CBookToc *oBookToc);
    bool AddRelationship(CElement *oRelationship);
//    bool AddHypelink(std::wstring sText, );
//    bool OpenFile(std::wstring sPathFile);

    bool SaveToFile();
};

#endif // CDOCXFILE_H
