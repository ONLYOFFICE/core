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
#include "../NativeBuilderEmbed.h"
#include "../../docbuilder_p.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCBuilderDocumentEmbed <JSExport>
-(JSValue*) IsValid;
-(JSValue*) GetBinary;
-(JSValue*) GetFolder;
-(JSValue*) CloseFile;
-(JSValue*) GetImageMap;
@end

@interface CJSCBuilderDocumentEmbed : NSObject<IJSCBuilderDocumentEmbed, JSEmbedObjectProtocol>
{
@public
    CBuilderDocumentEmbed* m_internal;
}
@end

@implementation CJSCBuilderDocumentEmbed

EMBED_OBJECT_WRAPPER_METHODS(CBuilderDocumentEmbed)

FUNCTION_WRAPPER_JS(IsValid, builder_doc_IsValid)
FUNCTION_WRAPPER_JS(GetBinary, builder_doc_GetBinary)
FUNCTION_WRAPPER_JS(GetFolder, builder_doc_GetFolder)
FUNCTION_WRAPPER_JS(CloseFile, builder_doc_CloseFile)
FUNCTION_WRAPPER_JS(GetImageMap, builder_doc_GetImageMap)

@end

@protocol IJSCNativeBuilder <JSExport>
-(JSValue*) OpenFile : (JSValue*)sPath : (JSValue*)sParams;
-(JSValue*) CreateFile : (JSValue*)type;
-(JSValue*) SetTmpFolder : (JSValue*)path;
-(JSValue*) SaveFile : (JSValue*)t : (JSValue*)path : (JSValue*)params;
-(JSValue*) CloseFile;
-(JSValue*) OpenTmpFile : (JSValue*)path : (JSValue*)params;
@end

@interface CJSCNativeBuilder : NSObject<IJSCNativeBuilder, JSEmbedObjectProtocol>
{
@public
    CBuilderEmbed* m_internal;
}
@end

@implementation CJSCNativeBuilder

EMBED_OBJECT_WRAPPER_METHODS(CBuilderEmbed)

FUNCTION_WRAPPER_JS_2(OpenFile, builder_OpenFile)
FUNCTION_WRAPPER_JS_1(CreateFile, builder_CreateFile)
FUNCTION_WRAPPER_JS_1(SetTmpFolder, builder_SetTmpFolder)
FUNCTION_WRAPPER_JS_3(SaveFile, builder_SaveFile)
FUNCTION_WRAPPER_JS(CloseFile, builder_CloseFile)
FUNCTION_WRAPPER_JS_2(OpenTmpFile, builder_OpenTmpFile)

@end

JSSmart<CJSValue> CBuilderEmbed::builder_OpenTmpFile(JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
    std::wstring sPath = path->toStringW();
    std::wstring sParams = params->toStringW();
    CJSCBuilderDocumentEmbed* p = [[CJSCBuilderDocumentEmbed alloc] init];
    p->m_internal->m_pBuilder = m_pBuilder;
    p->m_internal->OpenFile(sPath, sParams);
    CJSValueJSC* pRet = new CJSValueJSC();
    pRet->value = [JSValue valueWithObject:(id)p inContext:NSJSBase::CJSContextPrivate::GetCurrentContext()];
    return pRet;
}

void builder_CreateNative(const std::string& name, JSSmart<CJSContext> context, NSDoctRenderer::CDocBuilder* builder)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        CJSCNativeBuilder* p = [[CJSCNativeBuilder alloc] init];
        p->m_internal->m_pBuilder = builder;
        return p;
    };
}
