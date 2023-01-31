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

#include "Ptg.h"

namespace XLS
{

class OperatorPtg : public Ptg
{
public:
	OperatorPtg(const unsigned short ptg_id_init) : Ptg(ptg_id_init) {};
	virtual const std::wstring& getOperatorSymbols() const = 0;
	virtual const int getOperatorPriority() const = 0;
	virtual const bool isPercent() const {return false;};
	virtual const bool isRightAssociative() const {return false;}; // Must be overridden for right associative operators
};

typedef boost::shared_ptr<OperatorPtg> OperatorPtgPtr;


template<class T, unsigned short fixedPtgId, int priority>
class BOperatorPtg_T: public OperatorPtg
{
public:
	BOperatorPtg_T() : OperatorPtg(fixedPtgId) {};
	
	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false)
	{
		if (ptg_stack.empty()) 
			return;

		std::wstring operand2(ptg_stack.top());
		ptg_stack.pop();
		
		if (ptg_stack.empty())
			return;
		ptg_stack.top() += getOperatorSymbols() + operand2;
	};
	virtual const unsigned short getPtgId() const {return fixedPtgId;};
	virtual const std::wstring& getOperatorSymbols() const {return T::getSymbols();};
	virtual const int getOperatorPriority() const {return priority;};
};


template<class T, unsigned short fixedType, int priority>
class UOperatorPtg_T: public OperatorPtg
{
public:
	UOperatorPtg_T() : OperatorPtg(fixedType) {};
	void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false)
	{
		if (ptg_stack.empty())
			return;

		ptg_stack.top() = T::getSymbols() + ptg_stack.top();
	};
	virtual const unsigned short getPtgId() const {return fixedType;};
	virtual const std::wstring& getOperatorSymbols() const {return T::getSymbols();};
	virtual const int getOperatorPriority() const {return priority;};
};


class PtgAdd : public BOperatorPtg_T<PtgAdd, 0x03, 25>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgAdd)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgAdd(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"+");
		return symbols;
	}
};


class PtgSub : public BOperatorPtg_T<PtgSub, 0x04, 25>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgSub)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgSub(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"-");
		return symbols;
	}
};


class PtgMul : public BOperatorPtg_T<PtgMul, 0x05, 20>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgMul)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgMul(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"*");
		return symbols;
	}
};


class PtgDiv : public BOperatorPtg_T<PtgDiv, 0x06, 20>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgDiv)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgDiv(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"/");
		return symbols;
	}
};


class PtgPower : public BOperatorPtg_T<PtgPower, 0x07, 15>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgPower)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgPower(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"^");
		return symbols;
	}
};


class PtgEq : public BOperatorPtg_T<PtgEq, 0x0B, 35>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgEq)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgEq(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"=");
		return symbols;
	}
};


class PtgNe : public BOperatorPtg_T<PtgNe, 0x0E, 35>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgNe)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgNe(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"<>");
		return symbols;
	}
};


class PtgGt : public BOperatorPtg_T<PtgGt, 0x0D, 35>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgGt)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgGt(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L">");
		return symbols;
	}
};


class PtgGe : public BOperatorPtg_T<PtgGe, 0x0C, 35>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgGe)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgGe(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L">=");
		return symbols;
	}
};


class PtgLt : public BOperatorPtg_T<PtgLt, 0x09, 35>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgLt)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgLt(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"<");
		return symbols;
	}
};


class PtgLe : public BOperatorPtg_T<PtgLe, 0x0A, 35>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgLe)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgLe(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"<=");
		return symbols;
	}
};


class PtgConcat : public BOperatorPtg_T<PtgConcat, 0x08, 30>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgConcat)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgConcat(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"&");
		return symbols;
	}
};


class PtgIsect : public BOperatorPtg_T<PtgIsect, 0x0F, 5>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgIsect)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgIsect(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L" ");
		return symbols;
	}
};


class PtgRange : public BOperatorPtg_T<PtgRange, 0x11, 4>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgRange)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgRange(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L":");
		return symbols;
	}
};


class PtgUnion : public BOperatorPtg_T<PtgUnion, 0x10, 6>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgUnion)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgUnion(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L",");
		return symbols;
	}
};


class PtgUminus : public UOperatorPtg_T<PtgUminus, 0x13, 10>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgUminus)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgUminus(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"-");
		return symbols;
	}
	virtual const bool isRightAssociative() const {return true;};
};


class PtgUplus : public UOperatorPtg_T<PtgUplus, 0x12, 10>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgUplus)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgUplus(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"+");
		return symbols;
	}
	virtual const bool isRightAssociative() const {return true;};
};


class PtgPercent : public UOperatorPtg_T<PtgPercent, 0x14, 11>
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgPercent)
public:
	BiffStructurePtr clone() { return BiffStructurePtr(new PtgPercent(*this)); };

	static const std::wstring& getSymbols()
	{
		static const std::wstring symbols(L"%");
		return symbols;
	}
	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false)
	{
		ptg_stack.top() += getOperatorSymbols();
	};
	virtual const bool isPercent() const {return true;};
};


} // namespace XLS

