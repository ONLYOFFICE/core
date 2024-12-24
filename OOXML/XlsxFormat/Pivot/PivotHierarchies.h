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

#include "../../DocxFormat/Drawing/DrawingExt.h"

#include "../WritingElement.h"
#include "../FileTypes_Spreadsheet.h"

#include "../../Common/ComplexTypes.h"

namespace OOX
{
namespace Spreadsheet
{
///pivot cache hierarchies
    class CGroupMember : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CGroupMember)
        WritingElement_XlsbConstructors(CGroupMember)
        CGroupMember(){}
        virtual ~CGroupMember() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();
        
        nullable_bool		m_oGroup;
        nullable_string		m_oUniqueName;
    };

    class CGroupMembers : public WritingElementWithChilds<CGroupMember>
    {
    public:
        WritingElement_AdditionMethods(CGroupMembers)
        WritingElement_XlsbConstructors(CGroupMembers)
        CGroupMembers(){}
        virtual ~CGroupMembers() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();
        
        nullable_int		 m_oCount;
    };

    class CGroup : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CGroup)
        WritingElement_XlsbConstructors(CGroup)
        CGroup(){}
        virtual ~CGroup() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();
        
        nullable_int                m_oId;
        nullable_string		        m_oName;
        nullable_string		        m_oUniqueName;
        nullable_string		        m_oCaption;
        nullable_string		        m_oUniqueParent;

        nullable<CGroupMembers>     m_oGroupMembers;
    };

    class CGroups : public WritingElementWithChilds<CGroup>
    {
    public:
        WritingElement_AdditionMethods(CGroups)
        WritingElement_XlsbConstructors(CGroups)
        CGroups(){}
        virtual ~CGroups() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();
        
        nullable_int		 m_oCount;
    };

    class CGroupLevel : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CGroupLevel)
        WritingElement_XlsbConstructors(CGroupLevel)
        CGroupLevel(){}
        virtual ~CGroupLevel() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();
        
        nullable<CGroups>	m_oGroups;

        nullable_string		m_oUniqueName;
        nullable_string		m_oCaption;
        nullable_bool       m_oUser;
        nullable_bool       m_oCustomRollUp;
   
    };

    class CGroupLevels : public WritingElementWithChilds<CGroupLevel>
    {
    public:
        WritingElement_AdditionMethods(CGroupLevels)
        WritingElement_XlsbConstructors(CGroupLevels)
        CGroupLevels(){}
        virtual ~CGroupLevels() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();
        
        nullable_int		 m_oCount;
    };

    class CfieldsUsage : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CfieldsUsage)
        WritingElement_XlsbConstructors(CfieldsUsage)
        CfieldsUsage(){}
        virtual ~CfieldsUsage() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        
        nullable_int		 m_oCount;
        std::vector<_INT32>  m_oFieldUsage;
    };

    class CpivotCacheHierarchy : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CpivotCacheHierarchy)
        WritingElement_XlsbConstructors(CpivotCacheHierarchy)
        CpivotCacheHierarchy(){}
        virtual ~CpivotCacheHierarchy() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable_bool       m_oSet;
        nullable_bool       m_oTime;
        nullable_bool       m_oMeasure;
        nullable_bool       m_oMeasures;
        nullable_bool       m_oOneField;
        nullable_bool       m_oHidden;
        nullable_bool		m_oAttribute;
        nullable_bool		m_oUnbalanced;
        nullable_bool		m_oUnbalancedGroup;
        nullable_bool		m_oKeyAttribute;

        nullable_string		m_oMeasureGroup;
        nullable_string		m_oUniqueName;
        nullable_string		m_oCaption;
        nullable_string		m_oAllCaption;
        nullable_string     m_oDefaultMemberUniqueName;
        nullable_string     m_oAllUniqueName;
        nullable_string     m_oDimensionUniqueName;
        nullable_string     m_oDisplayFolder;

        nullable_uint		m_oCount;
        nullable_uint		m_oParentSet;
        nullable_uint		m_oMemberValueDatatype;
        nullable_int		m_oIconSet;


        nullable<CfieldsUsage>	m_oFieldsUsage;
        nullable<CGroupLevels>	m_oGroupLevels;

    };
    class CpivotCacheHierarchies : public WritingElementWithChilds<CpivotCacheHierarchy>
    {
    public:
        WritingElement_AdditionMethods(CpivotCacheHierarchies)
        WritingElement_XlsbConstructors(CpivotCacheHierarchies)
        CpivotCacheHierarchies(){}
        virtual ~CpivotCacheHierarchies() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;

    };
///pivot table hierarchies
    class CMemberProperty : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CMemberProperty)
        WritingElement_XlsbConstructors(CMemberProperty)
        CMemberProperty(){}
        virtual ~CMemberProperty() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable_bool       m_oShowCell;
        nullable_bool       m_oShowTip;
        nullable_bool       m_oShowAsCaption;

        nullable_string		m_oName;

        nullable_uint		m_oNameLen;
        nullable_uint		m_oPPos;
        nullable_uint		m_oPLen;
        nullable_uint		m_oLevel;
        nullable_uint		m_oField;
    };

    class CMemberProperties : public WritingElementWithChilds<CMemberProperty>
    {
    public:
        WritingElement_AdditionMethods(CMemberProperties)
        WritingElement_XlsbConstructors(CMemberProperties)
        CMemberProperties(){}
        virtual ~CMemberProperties() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;

    };

    class CMember : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CMember)
        WritingElement_XlsbConstructors(CMember)
        CMember(){}
        virtual ~CMember() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable_string		m_oName;

    };


    class CMembers : public WritingElementWithChilds<CMember>
    {
    public:
        WritingElement_AdditionMethods(CMembers)
        WritingElement_XlsbConstructors(CMembers)
        CMembers(){}
        virtual ~CMembers() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable_uint	m_oCount;
        nullable_uint	m_oLevel;

    };

    class CpivotTableHierarchy : public WritingElementWithChilds<CMembers>
    {
    public:
        WritingElement_AdditionMethods(CpivotTableHierarchy)
        WritingElement_XlsbConstructors(CpivotTableHierarchy)
        CpivotTableHierarchy(){}
        virtual ~CpivotTableHierarchy() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable_bool       m_oOutline;
        nullable_bool       m_oMultipleItemSelectionAllowed;
        nullable_bool       m_oSubtotalTop;
        nullable_bool       m_oShowInFieldList;
        nullable_bool       m_oDragToRow;
        nullable_bool       m_oDragToCol;
        nullable_bool		m_oDragToPage;
        nullable_bool		m_oDragToData;
        nullable_bool		m_oDragOff;
        nullable_bool		m_oIncludeNewItemsInFilter;

        nullable_string		m_oCaption;


        nullable<CMemberProperties>                            m_oMemberProperties;
        nullable<OOX::Drawing::COfficeArtExtensionList>		   m_oExtLst;

    };

    class CpivotTableHierarchies : public WritingElementWithChilds<CpivotTableHierarchy>
    {
    public:
        WritingElement_AdditionMethods(CpivotTableHierarchies)
        WritingElement_XlsbConstructors(CpivotTableHierarchies)
        CpivotTableHierarchies(){}
        virtual ~CpivotTableHierarchies() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;

    };
///pivot hierarhies usage
    class CHierarchyUsage : public WritingElement
    {
    public:
        WritingElement_AdditionMethods(CHierarchyUsage)
        WritingElement_XlsbConstructors(CHierarchyUsage)
        CHierarchyUsage(){}
        virtual ~CHierarchyUsage() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);

        nullable_int		m_oHierarchyUsage;
        bool                m_oRowHierarchy = false;

    };

    class CHierarchiesUsage : public WritingElementWithChilds<CHierarchyUsage>
    {
    public:
        WritingElement_AdditionMethods(CHierarchiesUsage)
        WritingElement_XlsbConstructors(CHierarchiesUsage)
        CHierarchiesUsage(){}
        virtual ~CHierarchiesUsage() {}

        virtual void fromXML(XmlUtils::CXmlNode& node)
        {
        }
        virtual std::wstring toXML() const
        {
            return _T("");
        }
        virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
        virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
        void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
        void fromBin(XLS::BaseObjectPtr& obj);
        XLS::BaseObjectPtr toBin();

        nullable_int		m_oCount;
        bool                m_oRowHierarchy = false;

    };
}
}
