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

#include "RBTree/action.h"
#include "cfstream.h"

namespace RedBlackTree
{
class RBTree;
}

namespace CFCPP
{
class CompoundFile_impl;
class CFStorage : public CFItem
{
public:
    CFStorage(CompoundFile_impl* compFile, const std::weak_ptr<IDirectoryEntry> &dirEntry);

    std::shared_ptr<RedBlackTree::RBTree> getChildren();
    std::shared_ptr<CFStream> AddStream(const std::wstring& streamName);
    std::shared_ptr<CFStream> GetStream(const std::wstring& streamName);

    std::shared_ptr<CFStorage> AddStorage(const std::wstring& storageName);
    std::shared_ptr<CFStorage> GetStorage(const std::wstring& storageName);

    bool TryGetStream(const std::wstring& streamName, std::shared_ptr<CFStream> &cfStream);
    std::shared_ptr<CFStorage> TryGetStorage(const std::wstring& storageName);
    bool TryGetStorage(const std::wstring& storageName, std::shared_ptr<CFStorage> &cfStorage);

    void VisitEntries(RedBlackTree::Action<std::shared_ptr<CFItem>> action, bool recursive);

    void Delete(const std::wstring& entryName);
    void RenameItem(const std::wstring& oldItemName, const std::wstring& newItemName);

    _INT64 size() const {return CFItem::size();}

private:
    std::shared_ptr<RedBlackTree::RBTree> LoadChildren(_INT32 SID);

private:
    std::shared_ptr<RedBlackTree::RBTree> children;
};
}
