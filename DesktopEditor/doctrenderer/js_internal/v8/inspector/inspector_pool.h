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

#ifndef INSPECTOR_POOL_H
#define INSPECTOR_POOL_H

#include "inspector.h"

#include <unordered_map>

namespace NSJSBase
{

	class CInspectorPool
	{
		using storage_t = std::unordered_map<v8::Isolate*, CInspector>;

		storage_t m_Inspectors;

		// add new inspector
		CInspector& addInspector(v8::Isolate* pIsolate);

		// group id for V8 internals
		static int getContextGroupId();

		// private due to singleton pattern
		CInspectorPool() = default;
		~CInspectorPool() = default;

	public:
		CInspectorPool(const CInspectorPool&) = delete;
		CInspectorPool(CInspectorPool&&) = delete;
		CInspectorPool& operator=(const CInspectorPool&) = delete;
		CInspectorPool& operator=(CInspectorPool&&) = delete;

		// get instance
		static CInspectorPool& get();
		// get inspector by isolate
		CInspector& getInspector(v8::Isolate* pIsolate);
		// remove inspector from isolate
		void disposeInspector(v8::Isolate* pIsolate);
	};

}

#endif // INSPECTOR_POOL_H
