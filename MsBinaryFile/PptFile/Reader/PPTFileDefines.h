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

#if defined(_WIN32) || defined (_WIN64)
    #include <windows.h>
#endif

#include "../../../DesktopEditor/common/ASCVariant.h"

typedef unsigned char BOOL1;

#define		PSFLAG_CONTAINER							0xFF

#define		PPT_DEFAULT_SLIDE_DURATION						5000
/////////////////////////////////////////////////////////////////////////

#define		RECORD_TYPE_UNKNOWN								0
#define     RECORD_TYPE_DOCUMENT							1000
#define		RECORD_TYPE_DOCUMENT_ATOM						1001
#define		RECORD_TYPE_ENDDOCUMENT							1002
#define		RECORD_TYPE_SLIDE								1006
#define		RECORD_TYPE_SLIDE_ATOM							1007
#define		RECORD_TYPE_NOTES								1008
#define		RECORD_TYPE_NOTES_ATOM							1009
#define		RECORD_TYPE_ENVIRONMENT							1010
#define		RECORD_TYPE_SLIDE_PERSIST_ATOM					1011
#define		RECORD_TYPE_SSLIDE_LAYOUT_ATOM					1015
#define		RECORD_TYPE_MAINMASTER							1016
#define		RECORD_TYPE_SSSLIDE_INFO_ATOM					1017
#define		RECORD_TYPE_SLIDE_VIEWINFO						1018
#define		RECORD_TYPE_GUIDE_ATOM							1019
#define		RECORD_TYPE_VIEWINFO							1020
#define		RECORD_TYPE_VIEWINFO_ATOM						1021
#define		RECORD_TYPE_SLIDE_VIEWINFO_ATOM					1022
#define		RECORD_TYPE_VBAINFO								1023
#define		RECORD_TYPE_VBAINFO_ATOM						1024
#define		RECORD_TYPE_SSDOCINFO_ATOM						1025
#define		RECORD_TYPE_SUMMARY								1026
#define		RECORD_TYPE_DOC_ROUTING_SLIP					1030
#define		RECORD_TYPE_OUTLINE_VIEWINFO					1031
#define		RECORD_TYPE_SORTER_VIEWINFO						1032
#define		RECORD_TYPE_EXOBJLIST							1033
#define		RECORD_TYPE_EXOBJLIST_ATOM						1034
#define		RECORD_TYPE_PPDRAWING_GROUP						1035
#define		RECORD_TYPE_PPDRAWING							1036
#define		RECORD_TYPE_RoundTripTheme12Atom							1038
#define		RECORD_TYPE_RoundTripColorMapping12Atom					1039
#define		RECORD_TYPE_NAMEDSHOWS							1040
#define		RECORD_TYPE_NAMEDSHOW							1041
#define		RECORD_TYPE_NAMEDSHOW_SLIDES					1042
#define		RECORD_TYPE_SHEET_PROPERTIES					1044
#define		RECORD_TYPE_NORMALVIEW_SETINFO9					1045

#define		RECORD_TYPE_ORIGINALMAINMASTERID							1052
#define		RECORD_TYPE_COMPOSITEMASTERID								1053
#define		RECORD_TYPE_ROUNDTRIPCONTENTMASTERINFO12					1054
#define		RECORD_TYPE_ROUNDTRIPSHAPEID12								1055
#define		RECORD_TYPE_ROUNDTRIPHFPLACEHOLDER12						1056
#define		RECORD_TYPE_ROUNDTRIPCONTENTMASTERID						1058
#define		RECORD_TYPE_ROUNDTRIPOARTTEXTSTYLES12						1059
#define     RECORD_TYPE_ROUNDTRIPHEADERFOOTERDEFAULTS12ATOM				1060
#define     RECORD_TYPE_ROUNDTRIPDOCFLAGS12								1061
#define		RECORD_TYPE_ROUNDTRIPSHAPECHECKSUMFORCUSTOMLAYOUTS12		1062
#define		RECORD_TYPE_ROUNDTRIPNOTESMASTERTEXTSTYLES12				1063
#define		RECORD_TYPE_ROUNDTRIPCUSTOMTABLESTYLES12					1064

#define		RECORD_TYPE_LIST								2000
#define		RECORD_TYPE_FONTCOLLECTION						2005
#define		RECORD_TYPE_FONTCOLLECTION10					2006
#define		RECORD_TYPE_BOOKMARKCOLLECTION					2019
#define		RECORD_TYPE_SOUNDCOLLECTION						2020
#define		RECORD_TYPE_SOUNDCOLL_ATOM						2021
#define		RECORD_TYPE_SOUND								2022
#define		RECORD_TYPE_SOUNDDATA							2023
#define		RECORD_TYPE_BOOKMARK_SEED_ATOM					2025
#define		RECORD_TYPE_COLOR_SCHEME_ATOM					2032
#define		RECORD_TYPE_OESHAPE_ATOM						3008
#define		RECORD_TYPE_EX_OBJREF_ATOM						3009
#define		RECORD_TYPE_OEPLACEHOLDER_ATOM					3011
#define		RECORD_TYPE_GPOPUBLICINT_ATOM					3024
#define		RECORD_TYPE_GRCOLOR								3020
#define		RECORD_TYPE_GRECT_ATOM							3025
#define		RECORD_TYPE_GRATIO_ATOM							3031
#define		RECORD_TYPE_GSCALING_ATOM						3032
#define		RECORD_TYPE_GPOINT_ATOM							3034
#define		RECORD_TYPE_OUTLINE_TEXTREF_ATOM				3998
#define		RECORD_TYPE_TEXT_HEADER_ATOM					3999
#define		RECORD_TYPE_TEXT_CHARS_ATOM						4000
#define		RECORD_TYPE_STYLE_TEXTPROP_ATOM					4001
#define		RECORD_TYPE_BASE_TEXTPROP_ATOM					4002
#define		RECORD_TYPE_TXMASTERSTYLE_ATOM					4003
#define		RECORD_TYPE_TXCFSTYLE_ATOM						4004
#define		RECORD_TYPE_TXPFSTYLE_ATOM						4005
#define		RECORD_TYPE_TEXTRULER_ATOM						4006
#define		RECORD_TYPE_TEXTBOOKMARK_ATOM					4007
#define		RECORD_TYPE_TEXTBYTES_ATOM						4008
#define		RECORD_TYPE_TXSISTYLE_ATOM						4009
#define		RECORD_TYPE_TEXTSPECINFO_ATOM					4010
#define		RECORD_TYPE_DEFAULTRULER_ATOM					4011
#define		RECORD_TYPE_FONTENTITY_ATOM						4023
#define		RECORD_TYPE_FONT_EMBEDDEDDATA					4024
#define		RECORD_TYPE_CSTRING								4026
#define		RECORD_TYPE_METAFILE							4033
#define		RECORD_TYPE_EXOLEOBJ_ATOM						4035
#define		RECORD_TYPE_SRKINSOKU							4040
#define		RECORD_TYPE_HANDOUT								4041
#define		RECORD_TYPE_EXEMBED								4044
#define		RECORD_TYPE_EXEMBEDATOM							4045
#define		RECORD_TYPE_EXLINK								4046
#define		RECORD_TYPE_BOOKMARK_ENTITY_ATOM				4048
#define		RECORD_TYPE_EXLINK_ATOM							4049
#define		RECORD_TYPE_SRKINSOKU_ATOM						4050
#define		RECORD_TYPE_EXHYPERLINK_ATOM					4051
#define		RECORD_TYPE_EXHYPERLINK							4055

#define		RECORD_TYPE_SLIDENUMBER_META_ATOM				0x0FD8

#define		RECORD_TYPE_HEADERSFOOTERS						0x0FD9
#define		RECORD_TYPE_HEADERSFOOTERS_ATOM					4058
#define		RECORD_TYPE_TXINTERACTIVEINFO_ATOM				4063
#define		RECORD_TYPE_CHARFORMAT_ATOM						4066
#define		RECORD_TYPE_PARAFORMAT_ATOM						4067
#define		RECORD_TYPE_RECOLORINFO_ATOM					4071
#define		RECORD_TYPE_EXQUICKTIMEMOVIE					4074
#define		RECORD_TYPE_EXQUICKTIMEMOVIEDATA				4075
#define		RECORD_TYPE_EXCONTROL							4078
#define		RECORD_TYPE_SLIDELISTWITHTEXT					4080
#define     RECORD_TYPE_ANIMATIONINFO_ATOM					4081
#define		RECORD_TYPE_INTERACTIVEINFO						4082
#define		RECORD_TYPE_INTERACTIVEINFO_ATOM				4083
#define		RECORD_TYPE_USER_EDIT_ATOM						4085
#define		RECORD_TYPE_CURRENTUSER_ATOM					4086

#define		RECORD_TYPE_DATETIME_META_ATOM					0x0FF7
#define		RECORD_TYPE_GENERICDATE_META_ATOM				0x0FF8
#define		RECORD_TYPE_HEADER_META_ATOM					0x0FF9
#define		RECORD_TYPE_FOOTER_META_ATOM					0x0FFA

#define		RECORD_TYPE_EXCONTROL_ATOM						4091
#define		RECORD_TYPE_EX_MEDIA_ATOM						4100
#define		RECORD_TYPE_EX_VIDEOCONTAINER					4101
#define		RECORD_TYPE_EX_AVIMOVIE							4102
#define		RECORD_TYPE_EX_MCIMOVIE							4103
#define		RECORD_TYPE_EX_MIDIAUDIO						4109
#define		RECORD_TYPE_EX_CDAUDIO							4110
#define		RECORD_TYPE_EX_WAVAUDIOEMBEDDED					4111
#define		RECORD_TYPE_EX_WAVAUDIOLINK						4112
#define		RECORD_TYPE_EX_OLEOBJSTG						4113
#define		RECORD_TYPE_EX_CDAUDIO_ATOM						4114
#define		RECORD_TYPE_EXWAVAUDIOEMBEDDED_ATOM				4115
#define		RECORD_TYPE_ANIMATIONINFO						4116
#define		RECORD_TYPE_RTFDATETIME_META_ATOM				0x1015
#define		RECORD_TYPE_PROGTAGS							5000
#define		RECORD_TYPE_PROGSTRINGTAG						5001
#define		RECORD_TYPE_PROGBINARYTAG						5002
#define		RECORD_TYPE_BINARYTAGDATA						5003
#define		RECORD_TYPE_PRPUBLICINTOPTIONS					6000
#define		RECORD_TYPE_PERSISTPTRFULLBLOCK					6001
#define		RECORD_TYPE_PERSISTPTRINCREMENTALBLOCK			6002
#define		RECORD_TYPE_GSCALING_ATOM2						10001
#define		RECORD_TYPE_GRCOLOR_ATOM2						10002
#define     RECORD_TYPE_CHARTBUILD_ATOM                     11013
#define     RECORD_TYPE_DIAGRAMBUILD                        11014
#define     RECORD_TYPE_DIAGRAMBUILD_ATOM                   11015

//// Records ~12000 seem to be related to the Comments used in PPT 2000/XP
//// (Comments in PPT97 are normal Escher text boxes)
#define		RECORD_TYPE_COMMENT2000							12000
#define		RECORD_TYPE_COMMENT2000ATOM						12001
#define		RECORD_TYPE_COMMENT2000SUMMARY					12004
#define		RECORD_TYPE_COMMENT2000SUMMARYATOM				12005

//// Records ~12050 seem to be related to Document Encryption
#define		RECORD_TYPE_DOCUMENT_ENCRYPTION_ATOM			0x2F14


//records greater then 0xF000 belong to with Microsoft Office Drawing format also known as Escher
#define		RECORD_TYPE_ESCHER_DGG_CONTAINER							0xF000
#define		RECORD_TYPE_ESCHER_DGG										0xF006
#define		RECORD_TYPE_ESCHER_CLSID									0xF016
#define		RECORD_TYPE_ESCHER_OPT										0xF00B
#define		RECORD_TYPE_ESCHER_BSTORE_CONTAINER							0xF001
#define		RECORD_TYPE_ESCHER_BSE										0xF007

#define		RECORD_TYPE_ESCHER_BLIP_START								0xF018
#define		RECORD_TYPE_ESCHER_BLIP_END									0xF117
#define		RECORD_TYPE_ESCHER_BLIP_EMF									0xF01A
#define		RECORD_TYPE_ESCHER_BLIP_WMF									0xF01B
#define		RECORD_TYPE_ESCHER_BLIP_PICT								0xF01C
#define		RECORD_TYPE_ESCHER_BLIP_JPEG								0xF01D
#define		RECORD_TYPE_ESCHER_BLIP_PNG									0xF01E
#define		RECORD_TYPE_ESCHER_BLIP_DIB									0xF01F
#define		RECORD_TYPE_ESCHER_BLIP_TIFF								0xF029

#define		RECORD_TYPE_ESCHER_DG_CONTAINER								0xF002
#define		RECORD_TYPE_ESCHER_DG										0xF008
#define		RECORD_TYPE_ESCHER_REGROUPITEMS								0xF118
#define		RECORD_TYPE_ESCHER_COLORSCHEME								0xF120
#define		RECORD_TYPE_ESCHER_SPGR_CONTAINER							0xF003
#define		RECORD_TYPE_ESCHER_SP_CONTAINER								0xF004
#define		RECORD_TYPE_ESCHER_SPGR										0xF009
#define		RECORD_TYPE_ESCHER_SP										0xF00A
#define		RECORD_TYPE_ESCHER_TEXTBOX									0xF00C
#define		RECORD_TYPE_ESCHER_CLIENTTEXTBOX							0xF00D
#define		RECORD_TYPE_ESCHER_ANCHOR									0xF00E
#define		RECORD_TYPE_ESCHER_CHILDANCHOR								0xF00F
#define		RECORD_TYPE_ESCHER_CLIENTANCHOR								0xF010
#define		RECORD_TYPE_ESCHER_CLIENTDATA								0xF011
#define		RECORD_TYPE_ESCHER_SOLVERCONTAINER							0xF005
#define		RECORD_TYPE_ESCHER_CONNECTORRULE							0xF012
#define		RECORD_TYPE_ESCHER_ALIGNRULE								0xF013
#define		RECORD_TYPE_ESCHER_ARCRULE									0xF014
#define		RECORD_TYPE_ESCHER_CLIENTRULE								0xF015
#define		RECORD_TYPE_ESCHER_CALLOUTRULE								0xF017
#define		RECORD_TYPE_ESCHER_SELECTION								0xF119
#define		RECORD_TYPE_ESCHER_COLORMRU									0xF11A
#define		RECORD_TYPE_ESCHER_DELETEDPSPL								0xF11D
#define		RECORD_TYPE_ESCHER_SPLITMENUCOLORS							0xF11E
#define		RECORD_TYPE_ESCHER_OLEOBJECT								0xF11F
#define     RECORD_TYPE_ESCHER_SECONDARY_OPT							0xF121
#define		RECORD_TYPE_ESCHER_TETRIARY_OPT								0xF122
#define     RECORD_TYPE_TIME_NODE                                       0xF127
#define     RECORD_TYPE_TIME_ANIMATE_BEHAVIOR                           0xF134

