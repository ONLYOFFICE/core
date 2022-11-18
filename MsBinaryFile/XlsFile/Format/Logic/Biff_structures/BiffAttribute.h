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

#include "BiffStructure.h"

namespace XLS
{

class BiffAttribute : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BiffAttribute)
public:
    BiffAttribute() {}
    BiffAttribute(const std::wstring  name_init) : attrib_name(name_init) {}

	void setName(const std::wstring  name_init)
	{
		attrib_name = name_init;
	}
	static const ElementType	type = typeBiffAttribute;

	_CP_OPT(std::wstring) attrib_name;
};




template<class Type>
class BiffAttributeSimple : public BiffAttribute
{
public:
    BiffAttributeSimple() {}
    BiffAttributeSimple<Type>(const Type& val_init) : val(val_init) {}
    BiffAttributeSimple<Type>(const Type& val_init, const std::wstring & attrib_name) : val(val_init), BiffAttribute(attrib_name) {}

    _CP_OPT(Type) & value() {return val;}

	BiffStructurePtr clone() 
	{
        return BiffStructurePtr(new BiffAttributeSimple<Type>(*this));
    }
	virtual void load(CFRecord& record)
	{
		version = record.getGlobalWorkbookInfo()->Version;
		record >> val;
    }

    operator Type () const { return (val ? static_cast<Type>(*val) : (Type)0); }
	
    BiffAttributeSimple<Type> operator= (const BiffAttributeSimple<Type>& other)
	{
		val = other.val;
		return *this;
    }

    BiffAttributeSimple<Type> operator= (const Type& other_val)
	{
		 val = other_val;
		 return *this;
    }

    bool operator==(const Type & F2){return (val == F2);}

protected:
    boost::optional<Type>	val;
	int						version;

};

template<class Type>
class ForwardOnlyParam : public BiffAttributeSimple<Type>
{
public:
    ForwardOnlyParam() {}
    ForwardOnlyParam(const Type& val_init)
    {
        BiffAttributeSimple<Type>::val = val_init;
    }

    ForwardOnlyParam<Type> operator= (const ForwardOnlyParam<Type>& other)
	{
       BiffAttributeSimple<Type>::val = other.val;
	   return *this;
    }

};


template<class Type>
class BackwardOnlyParam : public BiffAttributeSimple<Type>
{
public:
    BackwardOnlyParam() {}
    BackwardOnlyParam(const Type& val_init)
    {
         BiffAttributeSimple<Type>::val = val_init;
    }

    BackwardOnlyParam<Type> operator= (const BackwardOnlyParam<Type>& other)
	{
        BiffAttributeSimple<Type>::val = other.val;
		return *this;
    }

};



} // namespace XLS
