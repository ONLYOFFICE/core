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
#pragma once

#include "oox_rels.h"

#include "../../../OOXML/Base/Unit.h"

namespace NSFonts
{
    class IApplicationFonts;
}

namespace cpdoccore { 
namespace oox {

class mediaitems
{
public:

    mediaitems(const std::wstring & odfPacket);
	virtual ~mediaitems();

    struct item 
    {
        item(	std::wstring const &	href,
				_rels_type				type,
				std::wstring const &	outputName,
				bool					mediaInternal,
				std::wstring const &	Id,
				_rels_type_place		type_place);

        std::wstring		href;
        _rels_type			type;
		_rels_type_place	type_place;
        std::wstring		outputName;
        bool				mediaInternal;
        bool				valid;
		std::wstring		Id;
		int					count_used;
		int					count_add;
    };
    typedef std::vector< item > items_array;
	
	size_t count_charts;
	size_t count_image;
	size_t count_media;
	size_t count_audio;
	size_t count_video;
	size_t count_slide;
	size_t count_shape;
	size_t count_tables;
	size_t count_object;
	size_t count_activeX;
	size_t count_control;

	bool is_internal_path(const std::wstring& uri, const std::wstring& packetRoot);

	void set_font_directory(std::wstring pathFonts);
	NSFonts::IApplicationFonts *applicationFonts() {return applicationFonts_;}

    std::wstring add_or_find(const std::wstring & href, _rels_type type, bool & isInternal, _rels_type_place type_place);//links to the same object are possible
    std::wstring add_or_find(const std::wstring & href, _rels_type type, bool & isInternal, std::wstring & ref, _rels_type_place type_place);
	std::wstring add_or_find_anim_audio(const std::wstring& href, bool& isInternal, std::wstring& ref);
    
	void add_rels(bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, _rels_type_place type_place);
	std::wstring add_control_props	(std::wstring & oox_target);

	void dump_rels(rels & Rels, _rels_type_place type);
    
	items_array & items() { return items_; }

	static std::wstring get_rel_type(_rels_type type)
	{
		switch (type)
		{
			case typeImage:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";
			case typeChart:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart";
			case typeMsObject:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package";
			case typePDF:
			case typeOleObject:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject";
			case typeHyperlink:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";
			case typeMedia:		return L"http://schemas.microsoft.com/office/2007/relationships/media";
			case typeAudio:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio";
			case typeVideo:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video";
			case typeSlide:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide";
			case typeExternalLink:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLinkPath";
			case typeActiveX:		return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/control";
			case typeControlProps:	return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/ctrlProp";
			default:
				return L"";
		}
	}
	static _rels_type detectMediaType(const std::wstring & fileName)
	{
		size_t pos = fileName.rfind(L".");

		std::wstring sExt = (pos != std::wstring::npos ? fileName.substr(pos + 1) : L"");

		if (sExt.empty()) return typeMedia;

		sExt = XmlUtils::GetLower(sExt);

		if (sExt == L"wmv") return typeVideo;
		if (sExt == L"avi") return typeVideo;
		if (sExt == L"m4v") return typeVideo;
		if (sExt == L"mp4") return typeVideo;
		if (sExt == L"mov") return typeVideo;
		if (sExt == L"mkv") return typeVideo;
		if (sExt == L"webm") return typeVideo;

		if (sExt == L"wmv") return typeVideo;
		if (sExt == L"wma") return typeAudio;
		if (sExt == L"wav") return typeAudio;		
		if (sExt == L"mp3") return typeAudio;
		if (sExt == L"m4a") return typeAudio;

		return typeMedia;
	}

	bool pdf2image(const std::wstring& pdf_file_name, const std::wstring& image_file_name);

private:
	std::wstring create_file_name			(const std::wstring & uri, _rels_type type, bool & isInternal, size_t Num);
	std::wstring detectImageFileExtension	(const std::wstring &fileName);
   
	items_array		items_;
    std::wstring	odf_packet_;

    NSFonts::IApplicationFonts	*applicationFonts_;
};

typedef _CP_PTR(mediaitems) mediaitems_ptr;
}
}

