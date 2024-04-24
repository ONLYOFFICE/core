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
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.

#include "../Embed.h"
#import "js_embed.h"

@protocol IJSCTestEmbed <JSExport>
#ifdef ENABLE_SUM_DEL
-(JSValue*) FunctionSum : (JSValue*)param1 : (JSValue*)param2;
-(JSValue*) FunctionDel : (JSValue*)param1 : (JSValue*)param2;
#endif
#ifdef ENABLE_GET
-(JSValue*) FunctionGet;
#endif
-(JSValue*) FunctionSquare : (JSValue*)param;
@end

@interface CJSCTestEmbed : NSObject<IJSCTestEmbed, JSEmbedObjectProtocol>
{
@public
	CTestEmbed* m_internal;
}
@end

@implementation CJSCTestEmbed
EMBED_OBJECT_WRAPPER_METHODS(CTestEmbed);

#ifdef ENABLE_SUM_DEL
-(JSValue*) FunctionSum : (JSValue*)param1 : (JSValue*)param2
{
	JSSmart<CJSValue> ret = m_internal->FunctionSum(CJSEmbedObjectAdapterJSC::Native2Value(param1), CJSEmbedObjectAdapterJSC::Native2Value(param2));
	return CJSEmbedObjectAdapterJSC::Value2Native(ret);
}
-(JSValue*) FunctionDel : (JSValue*)param1 : (JSValue*)param2
{
	JSSmart<CJSValue> ret = m_internal->FunctionDel(CJSEmbedObjectAdapterJSC::Native2Value(param1), CJSEmbedObjectAdapterJSC::Native2Value(param2));
	return CJSEmbedObjectAdapterJSC::Value2Native(ret);
}
#endif

#ifdef ENABLE_GET
-(JSValue*) FunctionGet
{
	JSSmart<CJSValue> ret = m_internal->FunctionGet();
	return CJSEmbedObjectAdapterJSC::Value2Native(ret);
}
#endif

-(JSValue*) FunctionSquare : (JSValue*)param
{
	JSSmart<CJSValue> ret = m_internal->FunctionSquare(CJSEmbedObjectAdapterJSC::Native2Value(param));
	return CJSEmbedObjectAdapterJSC::Value2Native(ret);
}
@end

class CTestEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCTestEmbed alloc] init:(CTestEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CTestEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CTestEmbedAdapter();
	return m_pAdapter;
}

std::string CTestEmbed::getName() { return "CTestEmbed"; }

CJSEmbedObject* CTestEmbed::getCreator()
{
	return new CTestEmbed();
}
