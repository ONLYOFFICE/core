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
#include "../../js_internal/v8/v8_base.h"

namespace NSZipEmbed
{
#define CURRENTWRAPPER CZipEmbed

	FUNCTION_WRAPPER_V8_1(_open, open)
	FUNCTION_WRAPPER_V8_0(_create, create)
	FUNCTION_WRAPPER_V8_0(_save, save)
	FUNCTION_WRAPPER_V8_1(_getFile, getFile)
	FUNCTION_WRAPPER_V8_2(_addFile, addFile)
	FUNCTION_WRAPPER_V8_1(_removeFile, removeFile)
	FUNCTION_WRAPPER_V8_0(_close, close)
	FUNCTION_WRAPPER_V8_0(_getPaths, getPaths)
	FUNCTION_WRAPPER_V8_2(_decodeImage, decodeImage)
	FUNCTION_WRAPPER_V8_6(_encodeImageData, encodeImageData)
	FUNCTION_WRAPPER_V8_2(_encodeImage, encodeImage)
	FUNCTION_WRAPPER_V8_1(_getImageType, getImageType)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "open",	_open);
		NSV8Objects::Template_Set(result, "create",	_create);
		NSV8Objects::Template_Set(result, "save",	_save);
		NSV8Objects::Template_Set(result, "getFile",	_getFile);
		NSV8Objects::Template_Set(result, "addFile",	_addFile);
		NSV8Objects::Template_Set(result, "removeFile",	_removeFile);
		NSV8Objects::Template_Set(result, "close",	_close);
		NSV8Objects::Template_Set(result, "getPaths",	_getPaths);
		NSV8Objects::Template_Set(result, "decodeImage",	_decodeImage);
		NSV8Objects::Template_Set(result, "encodeImageData",	_encodeImageData);
		NSV8Objects::Template_Set(result, "encodeImage",	_encodeImage);
		NSV8Objects::Template_Set(result, "getImageType",	_getImageType);

		return handle_scope.Escape(result);
	}
}

class CZipEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSZipEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
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
