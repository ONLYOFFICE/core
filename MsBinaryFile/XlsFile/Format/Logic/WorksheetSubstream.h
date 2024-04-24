﻿/*
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

#include "CommonSubstream.h"

namespace XLS
{;

class WorksheetSubstream;
typedef boost::shared_ptr<WorksheetSubstream>	WorksheetSubstreamPtr;

class WorksheetSubstream: public CompositeObject, public CommonSubstream
{
	BASE_OBJECT_DEFINE_CLASS_NAME(WorksheetSubstream)
public:
	WorksheetSubstream(const size_t ws_index);
	~WorksheetSubstream();

	BaseObjectPtr clone();

	virtual const	bool loadContent	(BinProcessor& proc);

	static const ElementType type = typeWorksheetSubstream;

	BaseObjectPtr					m_DefaultRowHeight;
	BaseObjectPtr					m_SHFMLA_SET;
	BaseObjectPtr					m_Dimensions;
	BaseObjectPtr					m_CONDFMTS;
	BaseObjectPtr					m_DxGCol;
	BaseObjectPtr					m_DVAL;
	BaseObjectPtr					m_LRng;
	BaseObjectPtr					m_PLV;
	
	std::vector<BaseObjectPtr>		m_arMergeCells;
	std::vector<BaseObjectPtr>		m_arPIVOTVIEW;
	std::vector<BaseObjectPtr>		m_arQUERYTABLE;
	std::vector<BaseObjectPtr>		m_arNote;
	std::vector<BaseObjectPtr>		m_arHLINK;
	std::vector<BaseObjectPtr>		m_arLabel;
	std::vector<BaseObjectPtr>		m_arBIGNAME;

	std::vector<BaseObjectPtr>		m_arSXAddl;
};

} // namespace XLS

