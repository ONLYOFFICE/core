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

#include <vector>
#include <string>

namespace oox {

class rels;

class external_items
{
public:
    enum Type 
	{
		typeUnknown = 0, 
		typeImage, 
		typeChart, 
		typeShape, 
		typeTable, 
		typeHyperlink, 
		typeComment, 
		typeMedia, 
		typeGroup, 
		typeExternalLink, 
		typeOleObject, 
		typeActiveX, 
		typeControl,
		typeControlProps
	};

    external_items() 
    {
		count_charts	= 0;
 		count_shape		= 0;
 		count_image		= 0;
 		count_tables	= 0;
 		count_media		= 0;
		count_activeX	= 0;	
		count_embeddings= 0;
		count_controls	= 0;	
	}

    struct item 
    {
        item(
            std::wstring const & _uri,
            Type _type,
            bool _mediaInternal, int id,
			std::wstring const & _rId);

		int id;
        std::wstring uri;
        Type type;
        bool mediaInternal;
		std::wstring  rId;
		//int count_used;
		//int count_add;
    };
    typedef std::vector< item > items_array;
	
	size_t count_charts;
	size_t count_image;
	size_t count_media;
	size_t count_shape;
	size_t count_tables;
	size_t count_activeX;
	size_t count_embeddings;
	size_t count_controls;

    //std::wstring add_or_find(const std::wstring & href, Type type, bool & isInternal);// possible references to the same object
    std::wstring add_image		(const std::wstring & file_name, int bin_id);
	std::wstring add_chart		(std::wstring & oox_target);
	std::wstring add_embedding	(std::wstring & oox_target, const std::wstring & info);
	
	std::wstring add_control_activeX(std::wstring & oox_target);
	std::wstring add_control_props	(std::wstring & oox_target);

	std::wstring find_image	(int id,  std::wstring & oox_target, bool & isExternal);
	std::wstring find_image	(	const std::wstring & oox_target, bool & isExternal);

	void dump_rels(rels & Rels);
    items_array & items() { return items_; }

	void create_media_path(const std::wstring & out_path);
	void create_activeX_path(const std::wstring & out_path);
	void create_embeddings_path(const std::wstring & out_path);

	std::wstring activeX_path();
	std::wstring media_path();
	std::wstring embeddings_path();

private:
	std::wstring create_file_name(const std::wstring & uri, external_items::Type type, size_t Num);

	std::wstring	media_path_;
	std::wstring	activeX_path_;
	std::wstring	embeddings_path_;

    items_array		items_;

};

}
