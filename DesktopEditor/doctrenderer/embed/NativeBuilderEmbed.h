/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef NATIVECONTROLBUILDER
#define NATIVECONTROLBUILDER

#include "../docbuilder.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;
class CBuilderEmbed : public CJSEmbedObject
{
public:
    NSDoctRenderer::CDocBuilder* m_pBuilder;

    CBuilderEmbed() : m_pBuilder(NULL) {}
    ~CBuilderEmbed() { if(m_pBuilder) RELEASEOBJECT(m_pBuilder); }

    virtual void* getObject() { return (void*)m_pBuilder; }

public:
    JSSmart<CJSValue> builder_OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams);
    JSSmart<CJSValue> builder_CreateFile(JSSmart<CJSValue> type);
    JSSmart<CJSValue> builder_SetTmpFolder(JSSmart<CJSValue> path);
    JSSmart<CJSValue> builder_SaveFile(JSSmart<CJSValue> type, JSSmart<CJSValue> path, JSSmart<CJSValue> params);
    JSSmart<CJSValue> builder_CloseFile();
    JSSmart<CJSValue> builder_OpenTmpFile(JSSmart<CJSValue> path, JSSmart<CJSValue> params);
};

class CBuilderDocumentEmbed : public CJSEmbedObject
{
public:
    NSDoctRenderer::CDocBuilder* m_pBuilder;
    bool m_bIsValid;
    std::wstring m_sFolder;

public:
    CBuilderDocumentEmbed() : m_pBuilder(NULL), m_bIsValid(false) {}
    ~CBuilderDocumentEmbed() { if(m_pBuilder) RELEASEOBJECT(m_pBuilder); }

    virtual void* getObject() { return (void*)m_pBuilder; }
    NSDoctRenderer::CDocBuilder_Private* GetPrivate(NSDoctRenderer::CDocBuilder* pBuilder) { return pBuilder->GetPrivate(); }

public:
    void OpenFile(const std::wstring& sFile, const std::wstring& sParams);
    void CloseFile();

public:
    JSSmart<CJSValue> builder_doc_IsValid();
    JSSmart<CJSValue> builder_doc_GetBinary();
    JSSmart<CJSValue> builder_doc_GetFolder();
    JSSmart<CJSValue> builder_doc_CloseFile();
    JSSmart<CJSValue> builder_doc_GetImageMap();
};

void builder_CreateNative(const std::string& name, JSSmart<CJSContext> context, NSDoctRenderer::CDocBuilder* builder);

#endif // NATIVECONTROLBUILDER
