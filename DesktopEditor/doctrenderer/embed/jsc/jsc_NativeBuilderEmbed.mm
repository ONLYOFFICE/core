/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../NativeBuilderEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCBuilderEmbed <JSExport>
-(JSValue*) OpenFile : (JSValue*)sPath : (JSValue*)sParams;
-(JSValue*) CreateFile : (JSValue*)type;
-(JSValue*) SetTmpFolder : (JSValue*)path;
-(JSValue*) SaveFile : (JSValue*)type : (JSValue*)path : (JSValue*)params;
-(JSValue*) CloseFile;
-(JSValue*) OpenTmpFile : (JSValue*)path : (JSValue*)params;
@end

@interface CJSCBuilderEmbed : NSObject<IJSCBuilderEmbed, JSEmbedObjectProtocol>
{
@public
	CBuilderEmbed* m_internal;
}
@end

@implementation CJSCBuilderEmbed
EMBED_OBJECT_WRAPPER_METHODS(CBuilderEmbed);

FUNCTION_WRAPPER_JS_2(OpenFile, OpenFile)
FUNCTION_WRAPPER_JS_1(CreateFile, CreateFile)
FUNCTION_WRAPPER_JS_1(SetTmpFolder, SetTmpFolder)
FUNCTION_WRAPPER_JS_3(SaveFile, SaveFile)
FUNCTION_WRAPPER_JS_0(CloseFile, CloseFile)
FUNCTION_WRAPPER_JS_2(OpenTmpFile, OpenTmpFile)
@end

class CBuilderEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCBuilderEmbed alloc] init:(CBuilderEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CBuilderEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CBuilderEmbedAdapter();
	return m_pAdapter;
}

std::string CBuilderEmbed::getName() { return "CBuilderEmbed"; }

CJSEmbedObject* CBuilderEmbed::getCreator()
{
	return new CBuilderEmbed();
}
