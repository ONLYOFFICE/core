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

#include "svector.h"
#include "idirectoryentry.h"


namespace CFCPP
{
class DirectoryEntry : public IDirectoryEntry
{
public:

    static const _INT32 THIS_IS_GREATER = 1;
    static const _INT32 OTHER_IS_GREATER = -1;

    static const _INT32 NOSTREAM = 0xFFFFFFFF;
    static const _INT32 ZERO = 0;

    DirectoryEntry(std::wstring name, StgType stgType, SVector<IDirectoryEntry>& dirRepository);
    DirectoryEntry(std::wstring name, StgType stgType);

    RedBlackTree::PIRBNode getLeft() const override;
    RedBlackTree::PIRBNode getRight() const override;
    void setLeft(RedBlackTree::PIRBNode pNode) override;
    void setRight(RedBlackTree::PIRBNode pNode) override;
    _INT64 getSize() const override {return size;}
    void setSize(_INT64 value) override {size = value;}
    _INT32 getStateBits() const override {return stateBits;}
    void setStateBits(_INT32 value) override {stateBits = value;}

    inline void setColor(RedBlackTree::Color clr) override {stgColor = (StgColor)clr;}
    inline RedBlackTree::Color getColor()const override {return (RedBlackTree::Color)stgColor;}

    void setParent(RedBlackTree::PIRBNode pParent) override {parent = pParent;}
    inline RedBlackTree::PIRBNode getParent() const override {return parent.lock();}
    inline RedBlackTree::PIRBNode Grandparent() const override
        {return (parent.use_count() ? parent.lock()->getParent() : RedBlackTree::PIRBNode());}
    RedBlackTree::PIRBNode Sibling() const override; // check parent before using
    RedBlackTree::PIRBNode Uncle() const override;
    void AssignValueTo(RedBlackTree::PIRBNode other) override;

    _INT32 CompareTo(const RedBlackTree::PIRBNode& other) const override;
    std::wstring ToString() const override;

    inline _INT32 getChild() const override {return child;}
    inline void setChild(_INT32 value) override {child = value;}

    inline _INT32 getLeftSibling() const override {return leftSibling;}
    inline void setLeftSibling(_INT32 value) override {leftSibling = value;}
    inline _INT32 getRightSibling() const override {return rightSibling;}
    inline void setRightSibling(_INT32 value) override {rightSibling = value;}

    inline UINT64 getCreationDate() const override {return creationDate;}
    inline void setCreationDate(const UINT64& value) override {creationDate = value;}
    inline UINT64 getModifyDate() const override {return modifyDate;}
    inline void setModifyDate(const UINT64& value) override {modifyDate = value;}

    _INT32 getSid() const override;
    void setSid(_INT32 newSid) override;

    std::wstring GetEntryName() const override;
    void SetEntryName(const std::wstring &entryName) override;
    inline _UINT16 getNameLength() const override {return  nameLength;}

    void setStartSetc(_INT32 value) override {startSetc = value;};
    _INT32 getStartSetc() const override {return startSetc;};

    void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) override;
    void Write(Stream stream) const override;
    inline StgColor getStgColor() const override {return stgColor;}
    inline void setStgColor(StgColor value) override {stgColor = value;}
    inline StgType getStgType() const override {return stgType;}
    inline void setStgType(StgType value) override {stgType = value;}
    inline _GUID_ getStorageCLSID() const override {return storageCLSID;}
    inline void setStorageCLSID(_GUID_ value) override {storageCLSID = value;}
    _INT32 GetHashCode() const override;

    inline std::wstring Name() const {return GetEntryName();}

public:
    _UINT64 creationDate = 0;
    _UINT64 modifyDate = 0;
    _INT32 startSetc = 0xFFFFFFFE;
    _INT64 size = 0;
    _INT32 leftSibling = NOSTREAM;
    _INT32 rightSibling = NOSTREAM;
    _INT32 child = NOSTREAM;
    _INT32 stateBits = 0;
    static std::shared_ptr<IDirectoryEntry> New(std::wstring name, StgType stgType, SVector<IDirectoryEntry>& dirRepository);
    static std::shared_ptr<IDirectoryEntry> TryNew(std::wstring name, StgType stgType, SVector<IDirectoryEntry> &dirRepository);
    static std::shared_ptr<IDirectoryEntry> Mock(std::wstring name, StgType stgType);

private:
    static _UINT64 fnv_hash(const char *buffer, _INT64 lenght);

private:
    _INT32 sid = -1;
    char entryName[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    _UINT16 nameLength = 0;
    StgType stgType = StgType::StgInvalid;
    StgColor stgColor = StgColor::Red;
    SVector<IDirectoryEntry> emptyDir;
    SVector<IDirectoryEntry>& dirRepository;
    std::weak_ptr<RedBlackTree::IRBNode> parent;
	_GUID_ storageCLSID;
};

}
