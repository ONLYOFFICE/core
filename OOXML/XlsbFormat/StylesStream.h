/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#ifndef STYLESSTREAM_H
#define STYLESSTREAM_H


#include "../../DesktopEditor/common/Types.h"
#include "../Base/Base.h"
#include "../XlsxFormat/WritingElement.h"
#include <string>
#include <memory.h>
#include <iostream>
#include "../../MsBinaryFile/XlsFile/Format/Logic/CompositeObject.h"


namespace XLSB
{
    class StreamCacheReader;

    class StylesStream;
    typedef boost::shared_ptr<StylesStream>		StylesStreamPtr;

    class StylesStream: public XLS::CompositeObject
    {
        BASE_OBJECT_DEFINE_CLASS_NAME(StylesStream)
    public:
        StylesStream();
        ~StylesStream();

        XLS::BaseObjectPtr clone();

        virtual const bool loadContent(XLS::BinProcessor& proc);

        XLS::BaseObjectPtr               m_BrtBeginStyleSheet;
        XLS::BaseObjectPtr               m_FMTS;
        XLS::BaseObjectPtr               m_FONTS;
        XLS::BaseObjectPtr               m_FILLS;
        XLS::BaseObjectPtr               m_BORDERS;
        XLS::BaseObjectPtr               m_CELLSTYLEXFS;
        XLS::BaseObjectPtr               m_CELLXFS;
        XLS::BaseObjectPtr               m_STYLES;
        XLS::BaseObjectPtr               m_DXFS;
        XLS::BaseObjectPtr               m_TABLESTYLES;
        XLS::BaseObjectPtr               m_COLORPALETTE;
        XLS::BaseObjectPtr               m_BrtEndStyleSheet;
        XLS::BaseObjectPtr               m_FRTSTYLESHEET;

    };

}

#endif // STYLESSTREAM_H

