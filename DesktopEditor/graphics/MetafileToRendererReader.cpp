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
#include <string>
#include "MetafileToRenderer.h"

#include "./commands/DocInfo.h"
#include "./commands/FormField.h"
#include "./commands/AnnotField.h"

namespace NSOnlineOfficeBinToPdf
{
	template<typename T>
	inline IAdvancedCommand* Read_Command(CBufferReader* pReader, IMetafileToRenderter* pCorrector)
	{
		T* command = new T();
		if (!command->Read(pReader, pCorrector))
			RELEASEOBJECT(command);
		return command;
	}
	template<typename T>
	inline IAdvancedCommand* Read_Command_Len(CBufferReader* pReader, IMetafileToRenderter* pCorrector, int nLen)
	{
		T* command = new T();
		if (!command->Read(pReader, pCorrector, nLen))
			RELEASEOBJECT(command);
		return command;
	}

	IAdvancedCommand* CBufferReader::Read(int type, IMetafileToRenderter* pCorrector, int nLen)
	{
		switch (type)
		{
		case ctHyperlink:        return Read_Command<CHyperlinkCommand>(this, pCorrector);
		case ctLink:             return Read_Command<CLinkCommand>     (this, pCorrector);
		case ctDocInfo:          return Read_Command<CDocInfoCommand>  (this, pCorrector);
		case ctAnnotField:       return Read_Command<CAnnotFieldInfo>  (this, pCorrector);
		case ctFormField:        return Read_Command<CFormFieldInfo>   (this, pCorrector);
		case ctAnnotFieldDelete: return Read_Command<CAnnotFieldDelete>(this, pCorrector);
		case ctWidgetsInfo:      return Read_Command_Len<CWidgetsInfo> (this, pCorrector, nLen);
		case ctShapeStart:       return Read_Command_Len<CShapeStart>  (this, pCorrector, nLen);
		case ctShapeEnd:         return new CEmptyComand(IAdvancedCommand::AdvancedCommandType::ShapeEnd);
		case ctPageClear:        return new CEmptyComand(IAdvancedCommand::AdvancedCommandType::PageClear);
		case ctPageRotate:       return Read_Command<CPageRotate>      (this, pCorrector);
		case ctHeadings:         return Read_Command<CHeadings>        (this, pCorrector);
		case ctRedact:           return Read_Command<CRedact>          (this, pCorrector);
		case ctRedactMeta:       return Read_Command<CRedactMeta>      (this, pCorrector);
		default: break;
		}

		return NULL;
	}
}
