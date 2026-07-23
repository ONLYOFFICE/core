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
#include "../../agg-2.4/src/agg_arc.cpp"
#include "../../agg-2.4/src/agg_bezier_arc.cpp"
#include "../../agg-2.4/src/agg_arrowhead.cpp"
#include "../../agg-2.4/src/ctrl/agg_cbox_ctrl.cpp"
#include "../../agg-2.4/src/agg_curves.cpp"
#include "../../agg-2.4/src/agg_gsv_text.cpp"
#include "../../agg-2.4/src/agg_image_filters.cpp"
#include "../../agg-2.4/src/agg_line_aa_basics.cpp"
#include "../../agg-2.4/src/agg_line_profile_aa.cpp"
#include "../../agg-2.4/src/agg_rounded_rect.cpp"
#include "../../agg-2.4/src/agg_sqrt_tables.cpp"
#include "../../agg-2.4/src/agg_trans_affine.cpp"
#include "../../agg-2.4/src/agg_bspline.cpp"
#include "../../agg-2.4/src/agg_vcgen_bspline.cpp"
#include "../../agg-2.4/src/agg_vcgen_contour.cpp"
#include "../../agg-2.4/src/agg_vcgen_dash.cpp"
#include "../../agg-2.4/src/agg_vcgen_markers_term.cpp"
#include "../../agg-2.4/src/agg_vcgen_smooth_poly1.cpp"
#include "../../agg-2.4/src/agg_vcgen_stroke.cpp"

#include "../../fontengine/ApplicationFonts.cpp"
#include "../../fontengine/FontFile.cpp"
#include "../../fontengine/FontManager.cpp"
#include "../../fontengine/FontPath.cpp"
#include "../../fontengine/GlyphString.cpp"

#include "../../fontengine/fontconverter/StringExt.cpp"
#include "../../fontengine/fontconverter/Hash.cpp"
#include "../../fontengine/fontconverter/FontConverter.cpp"
#include "../../fontengine/fontconverter/FontFileEncodings.cpp"
#include "../../fontengine/fontconverter/FontFileTrueType.cpp"
#include "../../fontengine/fontconverter/FontFileType1.cpp"
#include "../../fontengine/fontconverter/FontFileType1C.cpp"

#include "../../raster/ImageFileFormatChecker.cpp"
#include "../../raster/BgraFrame.cpp"
#include "../../raster/Jp2/J2kFile.cpp"
#include "../../raster/Jp2/Reader.cpp"
#include "../../raster/Metafile/Common/MetaFileTypes.cpp"
#include "../../raster/Metafile/Common/MetaFileUtils.cpp"
#include "../../raster/Metafile/Emf/EmfClip.cpp"
#include "../../raster/Metafile/Emf/EmfObjects.cpp"
#include "../../raster/Metafile/Emf/EmfPath.cpp"
#include "../../raster/Metafile/Emf/EmfPlayer.cpp"
#include "../../raster/Metafile/Emf/EmfFile.cpp"
#include "../../raster/Metafile/Wmf/WmfClip.cpp"
#include "../../raster/Metafile/Wmf/WmfObjects.cpp"
#include "../../raster/Metafile/Wmf/WmfPlayer.cpp"
#include "../../raster/Metafile/MetaFile.cpp"
#include "../../raster/JBig2/source/JBig2File.cpp"
#include "../../raster/Metafile/StarView/SvmClip.cpp"
#include "../../raster/Metafile/StarView/SvmFile.cpp"
#include "../../raster/Metafile/StarView/SvmObjects.cpp"
#include "../../raster/Metafile/StarView/SvmPlayer.cpp"

