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
#include "../../js_internal/v8/v8_base.h"

namespace NSNativeBuilderEmbed
{
#define CURRENTWRAPPER CBuilderEmbed

	FUNCTION_WRAPPER_V8_2(_OpenFile, OpenFile)
	FUNCTION_WRAPPER_V8_1(_CreateFile, CreateFile)
	FUNCTION_WRAPPER_V8_1(_SetTmpFolder, SetTmpFolder)
	FUNCTION_WRAPPER_V8_3(_SaveFile, SaveFile)
	FUNCTION_WRAPPER_V8_0(_CloseFile, CloseFile)
	FUNCTION_WRAPPER_V8_2(_OpenTmpFile, OpenTmpFile)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "OpenFile",	_OpenFile);
		NSV8Objects::Template_Set(result, "CreateFile",	_CreateFile);
		NSV8Objects::Template_Set(result, "SetTmpFolder",	_SetTmpFolder);
		NSV8Objects::Template_Set(result, "SaveFile",	_SaveFile);
		NSV8Objects::Template_Set(result, "CloseFile",	_CloseFile);
		NSV8Objects::Template_Set(result, "OpenTmpFile",	_OpenTmpFile);

		return handle_scope.Escape(result);
	}
}

class CBuilderEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSNativeBuilderEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
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
