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

#include <memory>
#include <string>
#include "../../../OOXML/Base/Types_32.h"


namespace RedBlackTree
{
class IRBNode;
using PIRBNode = std::shared_ptr<IRBNode>;
using WPIRBNode = std::weak_ptr<IRBNode>;

enum Color
{
    RED = 0,
    BLACK = 1
};

class IRBNode : public std::enable_shared_from_this<IRBNode>
{
public:
    virtual void setLeft(PIRBNode pNode) = 0;
    virtual void setRight(PIRBNode pNode) = 0;
    virtual PIRBNode getLeft() const = 0;
    virtual PIRBNode getRight() const = 0;

    virtual void setColor(Color clr) = 0;
    virtual Color getColor()const = 0;

    virtual void setParent(PIRBNode pParent) = 0;
    virtual PIRBNode getParent() const = 0;

    virtual PIRBNode Grandparent() const = 0;
    virtual PIRBNode Sibling() const = 0;
    virtual PIRBNode Uncle() const = 0;

    virtual void AssignValueTo(PIRBNode other) = 0;

    virtual _INT32 CompareTo(const PIRBNode& other) const = 0;
    virtual std::wstring ToString() const = 0;
};
}
