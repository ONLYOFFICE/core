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