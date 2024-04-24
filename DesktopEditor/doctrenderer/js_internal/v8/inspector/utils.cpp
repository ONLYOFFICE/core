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

#include "utils.h"

#include "../v8_base.h"

namespace NSJSBase
{

	std::string convertToString(v8::Isolate* pIsolate, const v8_inspector::StringView oStringView)
	{
		int length = static_cast<int>(oStringView.length());
		v8::Local<v8::String> message =
			(
				oStringView.is8Bit()
				? v8::String::NewFromOneByte(pIsolate, reinterpret_cast<const uint8_t*>(oStringView.characters8()), v8::NewStringType::kNormal, length).ToLocalChecked()
				: v8::String::NewFromTwoByte(pIsolate, reinterpret_cast<const uint16_t*>(oStringView.characters16()), v8::NewStringType::kNormal, length).ToLocalChecked()
			);
		v8::String::Utf8Value result(V8IsolateFirstArg message);
		return *result;
	}

	v8_inspector::StringView convertToStringView(const std::string& str)
	{
		const uint8_t* pStringView = reinterpret_cast<const uint8_t*>(str.c_str());
		return { pStringView, str.length() };
	}

	v8::Local<v8::Object> parseJson(const v8::Local<v8::Context>& context, const std::string& sJson)
	{
		v8::MaybeLocal<v8::Value> jsonValue = v8::JSON::Parse(context, CreateV8String(context->GetIsolate(), sJson));
		if (jsonValue.IsEmpty())
		{
			return v8::Local<v8::Object>();
		}
		return jsonValue.ToLocalChecked()->ToObject(context).ToLocalChecked();
	}

	std::string getPropertyFromJson(v8::Isolate* pIsolate, const v8::Local<v8::Object>& jsonObject, const std::string& sPropertyName)
	{
		v8::Local<v8::Value> property = jsonObject->Get(pIsolate->GetCurrentContext(), CreateV8String(pIsolate, sPropertyName)).ToLocalChecked();
		v8::String::Utf8Value utf8Value(V8IsolateFirstArg property);
		return *utf8Value;
	}

}
