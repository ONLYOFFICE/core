/*
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

#include "../../cximage/libpsd/adjustment.c"
#include "../../cximage/libpsd/bevel_emboss.c"
#include "../../cximage/libpsd/bitmap.c"
#include "../../cximage/libpsd/blend.c"
#include "../../cximage/libpsd/boundary.c"
#include "../../cximage/libpsd/brightness_contrast.c"
#include "../../cximage/libpsd/channel_image.c"
#include "../../cximage/libpsd/channel_mixer.c"
#include "../../cximage/libpsd/color_balance.c"
#include "../../cximage/libpsd/color_mode.c"
#include "../../cximage/libpsd/color_overlay.c"
#include "../../cximage/libpsd/color.c"
#include "../../cximage/libpsd/curves.c"
#include "../../cximage/libpsd/drop_shadow.c"
#include "../../cximage/libpsd/effects.c"
#include "../../cximage/libpsd/file_header.c"
#include "../../cximage/libpsd/fixed.c"
#include "../../cximage/libpsd/gaussian_blur.c"
#include "../../cximage/libpsd/gradient_blend.c"
#include "../../cximage/libpsd/gradient_fill.c"
#include "../../cximage/libpsd/gradient_map.c"
#include "../../cximage/libpsd/gradient_overlay.c"
#include "../../cximage/libpsd/hue_saturation.c"
