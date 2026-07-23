/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "BiffStructure.h"


#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct _ACL {
        unsigned char  AclRevision;
        unsigned char  Sbz1;
        _UINT16   AclSize;
        _UINT16   AceCount;
        _UINT16   Sbz2;
    } ACL;
    typedef ACL *PACL;

    typedef void* PSID;
    typedef _UINT16   SECURITY_DESCRIPTOR_CONTROL, *PSECURITY_DESCRIPTOR_CONTROL;

    typedef struct _SECURITY_DESCRIPTOR
    {
       unsigned char  Revision;
       unsigned char  Sbz1;
       SECURITY_DESCRIPTOR_CONTROL Control;
       PSID Owner;
       PSID Group;
       PACL Sacl;
       PACL Dacl;

    } SECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;
#else
	#include <windows.h>
#endif

namespace XLS
{

class CFRecord;

class SecurityDescriptor : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SecurityDescriptor)
public:
	SecurityDescriptor();
	SecurityDescriptor(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeSecurityDescriptor;

	void load(CFRecord& record) override;
	void save(CFRecord& record) override;

	SECURITY_DESCRIPTOR sd;
};

} // namespace XLS

