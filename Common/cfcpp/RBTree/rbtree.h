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

#include <iterator>
#include <functional>
#include <list>
#include "irbnode.h"
#include "action.h"

namespace RedBlackTree
{
class RBTree
{
public:
    RBTree(){}
    RBTree(PIRBNode root);

    const PIRBNode getRoot() const;
    void setRoot(const PIRBNode &newRoot);

    bool TryLookup(PIRBNode pattern, PIRBNode& val);
    void Insert(PIRBNode newNode);
    void Delete(PIRBNode pattern, PIRBNode& deletedAlt);

    void VisitTree(Action<PIRBNode> action);
    void VisitTreeNodes(Action<PIRBNode> action);

private:
    static Color NodeColor(PIRBNode n);
    static PIRBNode MaximumNode(PIRBNode node);

    PIRBNode LookupNode(PIRBNode pattern);
    void ReplaceNode(PIRBNode oldn, PIRBNode newn);
    void RotateLeft(PIRBNode node);
    void RotateRight(PIRBNode node);

    void InsertCase1(PIRBNode node);
    void InsertCase2(PIRBNode node);
    void InsertCase3(PIRBNode node);
    void InsertCase4(PIRBNode node);
    void InsertCase5(PIRBNode node);

    void DeleteCase1(PIRBNode node);
    void DeleteCase2(PIRBNode node);
    void DeleteCase3(PIRBNode node);
    void DeleteCase4(PIRBNode node);
    void DeleteCase5(PIRBNode node);
    void DeleteCase6(PIRBNode node);

    void DoVisitTree(Action<PIRBNode> action, PIRBNode walker);
    void DoVisitTreeNodes(Action<PIRBNode> action, PIRBNode walker);

public:

   class iterator : public std::iterator<std::output_iterator_tag, std::ptrdiff_t, IRBNode, IRBNode*, PIRBNode>
   {
       PIRBNode current;
       RBTree* tree;
   public:
       iterator(RBTree *tree);
       iterator& operator++();
       inline bool operator==(const iterator &other) const {return current == other.current;}
       inline bool operator!=(const iterator &other) const {return current != other.current;}
       inline PIRBNode operator*() {return current;}
   };

   iterator begin() {return iterator(this);}
   iterator end() {return iterator(nullptr);}

private:
    PIRBNode root;
};

}
