#pragma once

#include <vector>
#include <string>

namespace oox {

class rels;

class external_items
{
public:
    enum Type { typeUnknown = 0, typeImage, typeChart, typeShape, typeTable, typeHyperlink, typeComment, typeMedia};

    external_items() 
    {
		count_charts	=0;
 		count_shape		=0;
 		count_image		=0;
 		count_tables	=0;
 		count_media		=0;
			
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

    //std::wstring add_or_find(const std::wstring & href, Type type, bool & isInternal);//возможны ссылки на один и тот же объект
    std::wstring add_image(const std::wstring & file_name, int bin_id);
	std::wstring find_image(int id, std::wstring & target, bool & isExternal);

	void dump_rels(rels & Rels);
    items_array & items() { return items_; }

private:
	std::wstring create_file_name(const std::wstring & uri, external_items::Type type, size_t Num);

    items_array items_;

};

}
