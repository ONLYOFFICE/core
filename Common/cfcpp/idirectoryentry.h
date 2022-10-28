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

#include <map>
#include "stream.h"
#include "RBTree/irbnode.h"
#include "guid.h"

namespace CFCPP
{
enum StgType : int
{
    StgInvalid = 0,
    StgStorage = 1,
    StgStream = 2,
    StgLockbytes = 3,
    StgProperty = 4,
    StgRoot = 5
};

enum StgColor : int
{
    Red = 0,
    Black = 1
};

enum CFSVersion : int
{
    // Sector size 512 - default and very common
    Ver_3 = 3,
    // Sector size 4096 - faster and large, but rary
    Ver_4 = 4
};

class IDirectoryEntry : public RedBlackTree::IRBNode
{
public:
    virtual int getChild() const = 0;
    virtual void setChild(int value) = 0;
    virtual int getLeftSibling() const = 0;
    virtual void setLeftSibling(int value) = 0;
    virtual int getRightSibling() const = 0;
    virtual void setRightSibling(int value) = 0;
    virtual std::streamsize getSize() const = 0;
    virtual void setSize(std::streamsize value) = 0;
    virtual int getStateBits() const = 0;
    virtual void setStateBits(int value) = 0;


    virtual UINT64 getCreationDate() const = 0;
    virtual void setCreationDate(const UINT64& value) = 0;
    virtual UINT64 getModifyDate() const = 0;
    virtual void setModifyDate(const UINT64& value) = 0;

    virtual int getSid() const = 0;
    virtual void setSid(int newSid) = 0;

    virtual std::wstring GetEntryName() const = 0;
    virtual void SetEntryName(const std::wstring &entryName) = 0;
    virtual unsigned short getNameLength() const = 0;

    virtual void setStartSetc(int value) = 0;
    virtual int getStartSetc() const = 0;

    virtual void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) = 0;
    virtual void Write(Stream stream) const = 0;
    virtual StgColor getStgColor() const = 0;
    virtual void setStgColor(StgColor value) = 0;
    virtual StgType getStgType() const = 0;
    virtual void setStgType(StgType value) = 0;
    virtual _GUID_ getStorageCLSID() const = 0;
    virtual void setStorageCLSID(_GUID_ value) = 0;
    virtual int GetHashCode() const = 0;
};
}
