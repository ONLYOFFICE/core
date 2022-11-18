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
#pragma once 

// main
//#include "CurrentUserAtom.h"
#include "UserEditAtom.h"
#include "SlideListWithText.h"
#include "PersistDirectoryAtom.h"
#include "DocumentRecords.h"
#include "DocumentTextInfo.h"

//Animation
#include "Animations/_includer.h"

// drawing records
#include "Drawing/ChildAnchor.h"
#include "Drawing/BlipStoreContainer.h"
#include "Drawing/BlipStoreEntry.h"
#include "Drawing/ClientAnchor.h"
#include "Drawing/DrawingGroup.h"
#include "Drawing/DrawingRecord.h"
#include "Drawing/GroupShape.h"
#include "Drawing/Shape.h"
#include "Drawing/ShapeProperties.h"
#include "Drawing/TextBox.h"
#include "Drawing/RegGroupItems.h"
#include "Drawing/DrawingContainer.h"

#include "OfficeArtClientData.h"
#include "OfficeArtClientTextbox.h"

// others

#include "BlipEntityAtom.h"
#include "BookmarkEntityAtom.h"
#include "BookmarkSeedAtom.h"
#include "CString.h"
#include "ColorSchemeAtom.h"
#include "DocRoutingSlipAtom.h"
#include "DocumentAtom.h"
#include "DocInfoListContainer.h"
#include "ExControlAtom.h"
#include "EndDocument.h"
#include "ExHyperlinkAtom.h"
#include "ExMediaAtom.h"
#include "ExVideoContainer.h"
#include "ExObjListAtom.h"
#include "ExObjRefAtom.h"
#include "ExOleEmbedAtom.h"
#include "ExOleLinkAtom.h"
#include "ExOleObjAtom.h"
#include "FontEntityAtom.h"
#include "GridSpacing10Atom.h"
#include "GuideAtom.h"
#include "HeadersFootersAtom.h"
#include "KinsokuAtom.h"
#include "MasterPersistAtom.h"
#include "MasterTextPropAtom.h"
#include "MetafileBlob.h"
#include "MouseTextInteractiveInfoAtom.h"
#include "NamedShowSlidesAtom.h"
#include "NormalViewSetInfoAtom.h"
#include "NotesAtom.h"
#include "NotesPersistAtom.h"
#include "NoZoomViewInfoAtom.h"
#include "OutlineTextRefAtom.h"
#include "PlaceHolderAtom.h"
#include "TextRulerAtom.h"
#include "ShapeFlags10Atom.h"
#include "ShapeFlagsAtom.h"
#include "SlideAtom.h"
#include "SlideContainer.h"
#include "SlideColorSchemeAtom.h"
#include "SlidePersistAtom.h"
#include "SlideViewInfoAtom.h"
#include "SoundCollAtom.h"
#include "SoundDataBlob.h"
#include "SSDocInfoAtom.h"
#include "SSlideLayoutAtom.h"
#include "SSSlideInfoAtom.h"
#include "StyleTextPropAtom.h"
#include "TextSpecInfoAtom.h"
#include "TextBookmarkAtom.h"
#include "TextBytesAtom.h"
#include "TextCFExceptionAtom.h"
#include "TextCharsAtom.h"
#include "TextHeaderAtom.h"
#include "TextMasterStyleAtom.h"
#include "TextPFExceptionAtom.h"
#include "TextSIExceptionAtom.h"
#include "VBAInfoAtom.h"
#include "ViewInfoAtom.h"
#include "ZoomViewInfoAtom.h"

#include "Animations/_includer.h"
#include "InteractiveInfoAtom.h"
#include "TextInteractiveInfoAtom.h"
#include "MouseInteractiveInfoContainer.h"

//sound
#include "ExMIDIAudioContainer.h"
#include "ExCDAudioContainer.h"
#include "ExWAVAudioLinkContainer.h"
#include "ExWAVAudioEmbeddedContainer.h"

#include "ExObjListContainer.h"
#include "SoundCollectionContainer.h"
#include "SoundContainer.h"

#include "RoundTrip.h"

