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

#include "../NativeControlEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSNativeControlEmbed
{
#define CURRENTWRAPPER CNativeControlEmbed

	FUNCTION_WRAPPER_V8_1(_SetFilePath, SetFilePath)
	FUNCTION_WRAPPER_V8_0(_GetFilePath, GetFilePath)
	FUNCTION_WRAPPER_V8_1(_SetFileId, SetFileId)
	FUNCTION_WRAPPER_V8_0(_GetFileId, GetFileId)
	FUNCTION_WRAPPER_V8_1(_GetFileBinary, GetFileBinary)
	FUNCTION_WRAPPER_V8_1(_GetFontBinary, GetFontBinary)
	FUNCTION_WRAPPER_V8_0(_GetFontsDirectory, GetFontsDirectory)
	FUNCTION_WRAPPER_V8_1(_GetFileString, GetFileString)
	FUNCTION_WRAPPER_V8_0(_GetEditorType, GetEditorType)
	FUNCTION_WRAPPER_V8_0(_CheckNextChange, CheckNextChange)
	FUNCTION_WRAPPER_V8_0(_GetCountChanges, GetCountChanges)
	FUNCTION_WRAPPER_V8_1(_GetChangesFile, GetChangesFile)
	FUNCTION_WRAPPER_V8_2(_Save_End, Save_End)
	FUNCTION_WRAPPER_V8_1(_AddImageInChanges, AddImageInChanges)
	FUNCTION_WRAPPER_V8_1(_ConsoleLog, ConsoleLog)
	FUNCTION_WRAPPER_V8_1(_ZipOpen, ZipOpen)
	FUNCTION_WRAPPER_V8_1(_ZipOpenBase64, ZipOpenBase64)
	FUNCTION_WRAPPER_V8_1(_ZipFileAsString, ZipFileAsString)
	FUNCTION_WRAPPER_V8_1(_ZipFileAsBinary, ZipFileAsBinary)
	FUNCTION_WRAPPER_V8_0(_ZipClose, ZipClose)
	FUNCTION_WRAPPER_V8_1(_GetImageUrl, GetImageUrl)
	FUNCTION_WRAPPER_V8_0(_GetImagesPath, GetImagesPath)
	FUNCTION_WRAPPER_V8_1(_GetImageOriginalSize, GetImageOriginalSize)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "SetFilePath",	_SetFilePath);
		NSV8Objects::Template_Set(result, "GetFilePath",	_GetFilePath);
		NSV8Objects::Template_Set(result, "SetFileId",	_SetFileId);
		NSV8Objects::Template_Set(result, "GetFileId",	_GetFileId);
		NSV8Objects::Template_Set(result, "GetFileBinary",	_GetFileBinary);
		NSV8Objects::Template_Set(result, "GetFontBinary",	_GetFontBinary);
		NSV8Objects::Template_Set(result, "GetFontsDirectory",	_GetFontsDirectory);
		NSV8Objects::Template_Set(result, "GetFileString",	_GetFileString);
		NSV8Objects::Template_Set(result, "GetEditorType",	_GetEditorType);
		NSV8Objects::Template_Set(result, "CheckNextChange",	_CheckNextChange);
		NSV8Objects::Template_Set(result, "GetCountChanges",	_GetCountChanges);
		NSV8Objects::Template_Set(result, "GetChangesFile",	_GetChangesFile);
		NSV8Objects::Template_Set(result, "Save_End",	_Save_End);
		NSV8Objects::Template_Set(result, "AddImageInChanges",	_AddImageInChanges);
		NSV8Objects::Template_Set(result, "ConsoleLog",	_ConsoleLog);
		NSV8Objects::Template_Set(result, "ZipOpen",	_ZipOpen);
		NSV8Objects::Template_Set(result, "ZipOpenBase64",	_ZipOpenBase64);
		NSV8Objects::Template_Set(result, "ZipFileAsString",	_ZipFileAsString);
		NSV8Objects::Template_Set(result, "ZipFileAsBinary",	_ZipFileAsBinary);
		NSV8Objects::Template_Set(result, "ZipClose",	_ZipClose);
		NSV8Objects::Template_Set(result, "GetImageUrl",	_GetImageUrl);
		NSV8Objects::Template_Set(result, "GetImagesPath",	_GetImagesPath);
		NSV8Objects::Template_Set(result, "GetImageOriginalSize",	_GetImageOriginalSize);

		return handle_scope.Escape(result);
	}
}

class CNativeControlEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSNativeControlEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CNativeControlEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CNativeControlEmbedAdapter();
	return m_pAdapter;
}

std::string CNativeControlEmbed::getName() { return "CNativeControlEmbed"; }

CJSEmbedObject* CNativeControlEmbed::getCreator()
{
	return new CNativeControlEmbed();
}
