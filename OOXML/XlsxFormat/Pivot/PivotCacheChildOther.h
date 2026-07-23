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
