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

namespace DocFileFormat
{
	// 2.3.17.11 Diagram Boolean Properties		-	[MS-ODRAW] — v20130726

	class DiagramBooleanProperties
	{
	public:
		DiagramBooleanProperties(unsigned int op)
		{
			fPseudoInline		=	FormatUtils::BitmaskToBool(op, 0x1);
			fDoLayout			=	FormatUtils::BitmaskToBool(op, 0x2);
			fReverse			=	FormatUtils::BitmaskToBool(op, 0x4);
			fDoFormat			=	FormatUtils::BitmaskToBool(op, 0x8);

			//unused: 0x10 - 0x8000

			fUsefPseudoInline	=	FormatUtils::BitmaskToBool(op, 0x10000);
			fUsefDoLayout		=	FormatUtils::BitmaskToBool(op, 0x20000);
			fUsefReverse		=	FormatUtils::BitmaskToBool(op, 0x40000);
			fUsefDoFormat		=	FormatUtils::BitmaskToBool(op, 0x80000);
		}

	public:

		bool fPseudoInline;
		bool fDoLayout;
		bool fReverse;
		bool fDoFormat;

		bool fUsefPseudoInline;
		bool fUsefDoLayout;
		bool fUsefReverse;
		bool fUsefDoFormat;
	};
}