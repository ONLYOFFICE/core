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
#ifdef _WIN64
#pragma comment(lib, "../../../../build/lib/win_64/doctrenderer.lib")
#else
#pragma comment(lib, "../../../../build/lib/win_32/doctrenderer.lib")
#endif

#define I_DOCBUILDER_VALUE_HEADER \
[object, uuid("2637FDFA-8473-4CB8-B90B-C23CB949D009"), dual, pointer_default(unique)] 

#define I_DOCBUILDER_CONTEXT_SCOPE_HEADER \
[object, uuid("656ae95c-ae91-4dc0-88bf-0b770fc2d552"), dual, pointer_default(unique)] 

#define I_DOCBUILDER_CONTEXT_HEADER \
[object, uuid("0416975a-65c3-4015-85e4-55d9dafec5fc"), dual, pointer_default(unique)] 

#define I_DOCBUILDER_HEADER \
[object, uuid("0C07B7E7-86A4-42E1-8E42-2FA961992E0F"), dual, pointer_default(unique)] 


#define C_DOCBUILDER_VALUE_HEADER \
[coclass, uuid("85C41585-25D7-40F1-9CC6-FA17052650F4"), threading(apartment), vi_progid("ONLYOFFICE.BuilderValue"), progid("ONLYOFFICE.BuilderValue.1"), version(1.0)]

#define C_DOCBUILDER_CONTEXT_SCOPE_HEADER \
[coclass, uuid("c54e2b15-ff5b-45a2-aa15-89a02605c30c"), threading(apartment), vi_progid("ONLYOFFICE.BuilderContextScope"), progid("ONLYOFFICE.BuilderContextScope.1"), version(1.0)]

#define C_DOCBUILDER_CONTEXT_HEADER \
[coclass, uuid("299250bb-16c2-4ab4-8a49-a0c350d66bb5"), threading(apartment), vi_progid("ONLYOFFICE.BuilderContext"), progid("ONLYOFFICE.BuilderContext.1"), version(1.0)]

#define C_DOCBUILDER_HEADER \
[coclass, uuid("9BF69F3C-1506-41B9-B8EE-2839948C02E9"), threading(apartment), vi_progid("ONLYOFFICE.Builder"), progid("ONLYOFFICE.Builder.1"), version(1.0)]

#define I_DOCBUILDER_VALUE IONLYOFFICEDocBuilderValue
#define I_DOCBUILDER_CONTEXT_SCOPE IONLYOFFICEDocBuilderContextScope
#define I_DOCBUILDER_CONTEXT IONLYOFFICEDocBuilderContext
#define I_DOCBUILDER IONLYOFFICEDocBuilder

#define C_DOCBUILDER_VALUE CONLYOFFICEDocBuilderValue
#define C_DOCBUILDER_CONTEXT_SCOPE CONLYOFFICEDocBuilderContextScope
#define C_DOCBUILDER_CONTEXT CONLYOFFICEDocBuilderContext
#define C_DOCBUILDER CONLYOFFICEDocBuilder