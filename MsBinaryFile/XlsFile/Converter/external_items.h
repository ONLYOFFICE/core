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

    //std::wstring add_or_find(const std::wstring & href, Type type, bool & isInternal);//возможны ссылки на один и тот же объект
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
