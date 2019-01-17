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
#include "../../../PPTXFormat/Logic/Media/WavAudioFile.cpp"
#include "../../../PPTXFormat/Logic/Colors/SchemeClr.cpp"
#include "../../../PPTXFormat/Logic/Fills/Blip.cpp"
#include "../../../PPTXFormat/Logic/Fills/BlipFill.cpp"
#include "../../../PPTXFormat/Logic/Table/TableCell.cpp"
#include "../../../PPTXFormat/Logic/Timing/BuildNodeBase.cpp"
#include "../../../PPTXFormat/Logic/Timing/TimeNodeBase.cpp"
#include "../../../PPTXFormat/Logic/Transitions/TransitionBase.cpp"
#include "../../../PPTXFormat/Logic/Bg.cpp"
#include "../../../PPTXFormat/Logic/CNvGrpSpPr.cpp"
#include "../../../PPTXFormat/Logic/ContentPart.cpp"
#include "../../../PPTXFormat/Logic/CxnSp.cpp"
#include "../../../PPTXFormat/Logic/EffectDag.cpp"
#include "../../../PPTXFormat/Logic/EffectLst.cpp"
#include "../../../PPTXFormat/Logic/EffectProperties.cpp"
#include "../../../PPTXFormat/Logic/EffectStyle.cpp"
#include "../../../PPTXFormat/Logic/Geometry.cpp"
#include "../../../PPTXFormat/Logic/GraphicFrame.cpp"
#include "../../../PPTXFormat/Logic/GrpSpPr.cpp"
#include "../../../PPTXFormat/Logic/Hyperlink.cpp"
#include "../../../PPTXFormat/Logic/NvGraphicFramePr.cpp"
#include "../../../PPTXFormat/Logic/NvGrpSpPr.cpp"
#include "../../../PPTXFormat/Logic/Pic.cpp"
#include "../../../PPTXFormat/Logic/Shape.cpp"
#include "../../../PPTXFormat/Logic/SmartArt.cpp"
#include "../../../PPTXFormat/Logic/SpPr.cpp"
#include "../../../PPTXFormat/Logic/SpTree.cpp"
#include "../../../PPTXFormat/Logic/SpTreeElem.cpp"
#include "../../../PPTXFormat/Logic/TxBody.cpp"
#include "../../../PPTXFormat/Logic/UniColor.cpp"
#include "../../../PPTXFormat/Logic/UniEffect.cpp"
#include "../../../PPTXFormat/Logic/UniFill.cpp"
#include "../../../PPTXFormat/Logic/Runs/MathParaWrapper.cpp"
#include "../../../PPTXFormat/Logic/Controls.cpp"
#include "../../../PPTXFormat/FileContainer.cpp"
#include "../../../PPTXFormat/FileFactory.cpp"
#include "../../../PPTXFormat/FileMap.cpp"
#include "../../../PPTXFormat/Folder.cpp"
