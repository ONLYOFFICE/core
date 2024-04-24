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

#include "../HashEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCHashEmbed <JSExport>
-(JSValue*) hash : (JSValue*)data : (JSValue*)size : (JSValue*)alg;
-(JSValue*) hash2 : (JSValue*)password : (JSValue*)salt : (JSValue*)spinCount : (JSValue*)alg;
@end

@interface CJSCHashEmbed : NSObject<IJSCHashEmbed, JSEmbedObjectProtocol>
{
@public
	CHashEmbed* m_internal;
}
@end

@implementation CJSCHashEmbed
EMBED_OBJECT_WRAPPER_METHODS(CHashEmbed);

FUNCTION_WRAPPER_JS_3(hash, hash)
FUNCTION_WRAPPER_JS_4(hash2, hash2)
@end

class CHashEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCHashEmbed alloc] init:(CHashEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CHashEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CHashEmbedAdapter();
	return m_pAdapter;
}

std::string CHashEmbed::getName() { return "CHashEmbed"; }

CJSEmbedObject* CHashEmbed::getCreator()
{
	return new CHashEmbed();
}
