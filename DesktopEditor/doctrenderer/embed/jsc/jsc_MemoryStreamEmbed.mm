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

#include "../MemoryStreamEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCMemoryStreamEmbed <JSExport>
-(JSValue*) Copy : (JSValue*)stream : (JSValue*)pos : (JSValue*)len;
-(JSValue*) ClearNoAttack;
-(JSValue*) WriteByte : (JSValue*)value;
-(JSValue*) WriteBool : (JSValue*)value;
-(JSValue*) WriteLong : (JSValue*)value;
-(JSValue*) WriteDouble : (JSValue*)value;
-(JSValue*) WriteDouble2 : (JSValue*)value;
-(JSValue*) WriteStringA : (JSValue*)value;
-(JSValue*) WriteString : (JSValue*)value;
-(JSValue*) WriteString2 : (JSValue*)value;
@end

@interface CJSCMemoryStreamEmbed : NSObject<IJSCMemoryStreamEmbed, JSEmbedObjectProtocol>
{
@public
	CMemoryStreamEmbed* m_internal;
}
@end

@implementation CJSCMemoryStreamEmbed
EMBED_OBJECT_WRAPPER_METHODS(CMemoryStreamEmbed);

FUNCTION_WRAPPER_JS_3(Copy, Copy)
FUNCTION_WRAPPER_JS_0(ClearNoAttack, ClearNoAttack)
FUNCTION_WRAPPER_JS_1(WriteByte, WriteByte)
FUNCTION_WRAPPER_JS_1(WriteBool, WriteBool)
FUNCTION_WRAPPER_JS_1(WriteLong, WriteLong)
FUNCTION_WRAPPER_JS_1(WriteDouble, WriteDouble)
FUNCTION_WRAPPER_JS_1(WriteDouble2, WriteDouble2)
FUNCTION_WRAPPER_JS_1(WriteStringA, WriteStringA)
FUNCTION_WRAPPER_JS_1(WriteString, WriteString)
FUNCTION_WRAPPER_JS_1(WriteString2, WriteString2)
@end

class CMemoryStreamEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCMemoryStreamEmbed alloc] init:(CMemoryStreamEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CMemoryStreamEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CMemoryStreamEmbedAdapter();
	return m_pAdapter;
}

std::string CMemoryStreamEmbed::getName() { return "CMemoryStreamEmbed"; }

CJSEmbedObject* CMemoryStreamEmbed::getCreator()
{
	return new CMemoryStreamEmbed();
}
