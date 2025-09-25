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
#include "PivotTable.h"

namespace OOX
{
namespace Spreadsheet
{

class CPivotDimension : public WritingElement
{
public:
    WritingElement_AdditionMethods(CPivotDimension)
    WritingElement_XlsbConstructors(CPivotDimension)
    CPivotDimension(){}
    virtual ~CPivotDimension() {}

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

    nullable_bool		m_oMeasure;
    nullable_string		m_oName;
    nullable_string		m_oUniqueName;
    nullable_string		m_oCaption;
};

class CPivotDimensions : public WritingElementWithChilds<CPivotDimension>
{
public:
    WritingElement_AdditionMethods(CPivotDimensions)
    WritingElement_XlsbConstructors(CPivotDimensions)
    CPivotDimensions(){}
    virtual ~CPivotDimensions() {}

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

class CPivotMeasureGroup : public WritingElement
{
public:
    WritingElement_AdditionMethods(CPivotMeasureGroup)
    WritingElement_XlsbConstructors(CPivotMeasureGroup)
    CPivotMeasureGroup(){}
    virtual ~CPivotMeasureGroup() {}

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
    nullable_string		m_oCaption;
};

class CPivotMeasureGroups : public WritingElementWithChilds<CPivotMeasureGroup>
{
public:
    WritingElement_AdditionMethods(CPivotMeasureGroups)
    WritingElement_XlsbConstructors(CPivotMeasureGroups)
    CPivotMeasureGroups(){}
    virtual ~CPivotMeasureGroups() {}

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

class CMeasureDimensionMap : public WritingElement
{
public:
    WritingElement_AdditionMethods(CMeasureDimensionMap)
    WritingElement_XlsbConstructors(CMeasureDimensionMap)
    CMeasureDimensionMap(){}
    virtual ~CMeasureDimensionMap() {}

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

    nullable_uint       m_oMeasureGroup;
    nullable_uint		m_oDimension;
};

class CMeasureDimensionMaps: public WritingElementWithChilds<CMeasureDimensionMap>
{
public:
    WritingElement_AdditionMethods(CMeasureDimensionMaps)
    WritingElement_XlsbConstructors(CMeasureDimensionMaps)
    CMeasureDimensionMaps(){}
    virtual ~CMeasureDimensionMaps() {}

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

class CCalculatedItem : public WritingElement
{
public:
    WritingElement_AdditionMethods(CCalculatedItem)
    WritingElement_XlsbConstructors(CCalculatedItem)
    CCalculatedItem(){}
    virtual ~CCalculatedItem() {}

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

    nullable<CPivotArea>          m_oPivotArea;
    nullable_uint       m_oField;
    nullable_string		m_oFormula;
    nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
};

class CCalculatedItems: public WritingElementWithChilds<CCalculatedItem>
{
public:
    WritingElement_AdditionMethods(CCalculatedItems)
    WritingElement_XlsbConstructors(CCalculatedItems)
    CCalculatedItems(){}
    virtual ~CCalculatedItems() {}

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

}
}
