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

#include "../PointerEmbed.h"
#include "../../js_internal/v8/v8_base.h"

v8::Handle<v8::ObjectTemplate> CreatePointerTemplate(v8::Isolate* isolate)
{
	v8::EscapableHandleScope handle_scope(isolate);

	v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
	result->SetInternalFieldCount(1);

	return handle_scope.Escape(result);
}

JSSmart<CJSValue> CPointerEmbedObject::createObject()
{
	v8::Isolate* isolate = CV8Worker::GetCurrent();
	//v8::HandleScope scope(isolate);

	v8::Handle<v8::ObjectTemplate> pointerTemplate = v8::ObjectTemplate::New(isolate);
	pointerTemplate->SetInternalFieldCount(1);

	v8::Local<v8::Object> obj = pointerTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), this));

	NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);

	CJSValueV8* returnValue = new CJSValueV8();
	returnValue->value = obj;

	JSSmart<CJSValue> ret = returnValue;
	return ret;
}
