# Copyright (C) Ascensio System SIA, 2009-2026
#
# This program is a free software product. You can redistribute it and/or
# modify it under the terms of the GNU Affero General Public License (AGPL)
# version 3 as published by the Free Software Foundation, together with the
# additional terms provided in the LICENSE file.
#
# This program is distributed WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
# details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
#
# You can contact Ascensio System SIA by email at info@onlyoffice.com
# or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
# LV-1050, Latvia, European Union.
#
# The interactive user interfaces in modified versions of the Program
# are required to display Appropriate Legal Notices in accordance with
# Section 5 of the GNU AGPL version 3.
#
# No trademark rights are granted under this License.
#
# All non-code elements of the Product, including illustrations,
# icon sets, and technical writing content, are licensed under the
# Creative Commons Attribution-ShareAlike 4.0 International License:
# https://creativecommons.org/licenses/by-sa/4.0/legalcode
#
# This license applies only to such non-code elements and does not
# modify or replace the licensing terms applicable to the Program's
# source code, which remains licensed under the GNU Affero General
# Public License v3.
#
# SPDX-License-Identifier: AGPL-3.0-only

include(freetype.pri)

FONT_ENGINE_PATH = $$PWD/../../fontengine

HEADERS += \
	$$PWD/Fonts.h \
	$$FONT_ENGINE_PATH/ApplicationFonts.h \
	$$FONT_ENGINE_PATH/FontFile.h \
	$$FONT_ENGINE_PATH/FontPath.h \
	$$FONT_ENGINE_PATH/GlyphString.h \
	$$FONT_ENGINE_PATH/FontManager.h \
	$$FONT_ENGINE_PATH/FontConverter.h

SOURCES += \
	$$FONT_ENGINE_PATH/ApplicationFonts.cpp \
	$$FONT_ENGINE_PATH/FontFile.cpp \
	$$FONT_ENGINE_PATH/FontManager.cpp \
	$$FONT_ENGINE_PATH/FontPath.cpp \
	$$FONT_ENGINE_PATH/GlyphString.cpp

core_mac {
    OBJECTIVE_SOURCES += $$FONT_ENGINE_PATH/ApplicationFonts_mac.mm
    LIBS += -framework Foundation
    LIBS += -framework CoreText
}

# Application fonts worker
HEADERS += $$FONT_ENGINE_PATH/ApplicationFontsWorker.h
SOURCES += $$FONT_ENGINE_PATH/ApplicationFontsWorker.cpp

# deprecated!!! remove after 6.5 ------------------
HEADERS += $$FONT_ENGINE_PATH/FontsAssistant.h
SOURCES += $$FONT_ENGINE_PATH/FontsAssistant.cpp
# -------------------------------------------------

# ------------------- shaper ----------------------
HEADERS += $$FONT_ENGINE_PATH/TextShaper.h
SOURCES += $$FONT_ENGINE_PATH/TextShaper.cpp

include($$PWD/textshaper.pri)

enable_support_shaper {
	include($$PWD/../../../Common/3dParty/harfbuzz/harfbuzz.pri)
}
# -------------------------------------------------

# ------------------- brotli ----------------------
include($$PWD/../../../Common/3dParty/brotli/brotli.pri)
# -------------------------------------------------

# ------------------- hyphen ----------------------
HEADERS += $$FONT_ENGINE_PATH/languages.h

HEADERS += $$FONT_ENGINE_PATH/TextHyphen.h
SOURCES += $$FONT_ENGINE_PATH/TextHyphen.cpp
# -------------------------------------------------

core_ios {
	LIBS += -framework Foundation
}

SOURCES += $$PWD/pro_Fonts.cpp

support_font_converter {
SOURCES += \
	$$FONT_ENGINE_PATH/fontconverter/StringExt.cpp \
	$$FONT_ENGINE_PATH/fontconverter/Hash.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontConverter.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileEncodings.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileTrueType.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileType1.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileType1C.cpp
}
