/*
* (c) Copyright Ascensio System SIA 2010-2019
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
* You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
