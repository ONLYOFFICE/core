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

#include <iostream>
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/File.h"

int main()
{
    // sax reader
    XmlUtils::CXmlLiteReader oLightReader;
    /*
    Output:
    Returns success of file opening
    Reads file into m_pStream stream
    Stream length in m_lStreamLen
    Creates reader
    After reading the file is closed
    Notes:
    xmlreader.h - xml handling functions
    */
    bool res1 = oLightReader.FromFile(NSFile::GetProcessDirectory() + L"/../../../examples/test.xml");
    std::wcout << res1 << std::endl; // true

    /*
    Returns node ordinal number (seems not)
    Notes:
    Returns XmlNodeType_None(0)
    */
    XmlUtils::XmlNodeType res2 = oLightReader.GetNodeType();
    std::wcout << res2<< " " << oLightReader.GetName() << std::endl; // 0 empty, because current node is absent

    /*
    GetName() - returns current node name
    GetDepth() - returns current node depth
    */

    /*
    Returns success of reading node of specified type
    Saves node type by reference
    Notes:
    Value XmlNodeType_None(0) is converted to XmlNodeType_Element(1)
    */
    bool res3 = oLightReader.Read(res2); // Will read company
    std::wcout << res3 << " " << res2 << " " << oLightReader.GetName() << std::endl; // true 1

    /*
    Notes:
    Returns success of reading next node
    In reader depth changes if node is nested
    And current node
    */
    bool res4 = oLightReader.ReadNextNode(); // Will read staff
    std::wcout << res4 << " " << oLightReader.GetName() << std::endl; // true
    bool res5 = oLightReader.ReadNextNode(); // Will read firstname
    std::wcout << res5 << " " << oLightReader.GetName() << std::endl; // true

    /*
    Returns success of reading next node at same depth - 1
    Outer tag doesn't count
    */
    bool res6 = oLightReader.ReadNextSiblingNode(1); // Will read lastname
    std::wcout << res6 << " " << oLightReader.GetName() << std::endl; // true
    //bool res7 = oLightReader.ReadNextSiblingNode(1); // Will read closing staff
    //std::wcout << res7 << std::endl; // false, because read node is at different depth

    /*
    Notes:
    Forcibly reads node at same depth,
    If no node, creates text node
    */
    bool res7 = oLightReader.ReadNextSiblingNode2(1); // Will read text (WHAT IS THIS) parent will be staff
    std::wcout << res7 << " " << oLightReader.GetName() << std::endl; // true

    return 0;
}
