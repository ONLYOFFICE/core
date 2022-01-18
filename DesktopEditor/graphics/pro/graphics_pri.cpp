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

