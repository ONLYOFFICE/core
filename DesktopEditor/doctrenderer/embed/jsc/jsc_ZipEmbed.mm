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

#include "../ZipEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCZipEmbed <JSExport>
-(JSValue*) open : (JSValue*)typedArray_or_Folder;
-(JSValue*) create;
-(JSValue*) save;
-(JSValue*) getFile : (JSValue*)filePath;
-(JSValue*) addFile : (JSValue*)filePath : (JSValue*)typedArray;
-(JSValue*) removeFile : (JSValue*)filePath;
-(JSValue*) close;
-(JSValue*) getPaths;
-(JSValue*) decodeImage : (JSValue*)typedArray : (JSValue*)isRgba;
-(JSValue*) encodeImageData : (JSValue*)typedArray : (JSValue*)w : (JSValue*)h : (JSValue*)stride : (JSValue*)format : (JSValue*)isRgba;
-(JSValue*) encodeImage : (JSValue*)typedArray : (JSValue*)format;
-(JSValue*) getImageType : (JSValue*)typedArray;
@end

@interface CJSCZipEmbed : NSObject<IJSCZipEmbed, JSEmbedObjectProtocol>
{
@public
	CZipEmbed* m_internal;
}
@end

@implementation CJSCZipEmbed
EMBED_OBJECT_WRAPPER_METHODS(CZipEmbed);

FUNCTION_WRAPPER_JS_1(open, open)
FUNCTION_WRAPPER_JS_0(create, create)
FUNCTION_WRAPPER_JS_0(save, save)
FUNCTION_WRAPPER_JS_1(getFile, getFile)
FUNCTION_WRAPPER_JS_2(addFile, addFile)
FUNCTION_WRAPPER_JS_1(removeFile, removeFile)
FUNCTION_WRAPPER_JS_0(close, close)
FUNCTION_WRAPPER_JS_0(getPaths, getPaths)
FUNCTION_WRAPPER_JS_2(decodeImage, decodeImage)
FUNCTION_WRAPPER_JS_6(encodeImageData, encodeImageData)
FUNCTION_WRAPPER_JS_2(encodeImage, encodeImage)
FUNCTION_WRAPPER_JS_1(getImageType, getImageType)
@end

class CZipEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCZipEmbed alloc] init:(CZipEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CZipEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CZipEmbedAdapter();
	return m_pAdapter;
}

std::string CZipEmbed::getName() { return "CZipEmbed"; }

CJSEmbedObject* CZipEmbed::getCreator()
{
	return new CZipEmbed();
}
