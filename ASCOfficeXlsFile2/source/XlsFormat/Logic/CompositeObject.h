/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "BaseObject.h"
#include "BinProcessor.h"

namespace XLS
{;

class CFStreamCacheReader;

// Logical representation of composite objects
//
// Another new function is to buffer objects, that had to be read but was absent in the bin stream.
// The buffered records shall be checked for existence later.
// This feature is specially for files saved in OpenOffice Calc those don't conform to the Microsoft specification
// Call them "wanted objects"
//
class CompositeObject : public BaseObject
{
public:
	CompositeObject();
	~CompositeObject();

	//virtual const bool doSomething(BinProcessor& proc);
	virtual const bool read(CFStreamCacheReader& reader, BaseObject* parent, const bool mandatory); // Read self and children
	//virtual const bool write(BinWriterProcessor& proc, const bool is_mandatory); // Write self and children
	virtual const bool loadContent(BinProcessor& proc) = 0;

	virtual const bool loadContentRead(BinReaderProcessor& proc);

	static const ElementType	type = typeCompositeObject;
	
	virtual ElementType get_type() { return type; }
};

// This class helps to process parenthesis in ABNF specification
class ABNFParenthesis : public CompositeObject
{
public:
	ABNFParenthesis(){};

	virtual const bool read(CFStreamCacheReader& reader, BaseObject* parent, const bool is_mandatory);

	static const ElementType	type = typeABNFParenthesis;
	

};


} // namespace XLS
