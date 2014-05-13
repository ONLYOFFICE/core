#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace odf {

class rels;

class _mediaitems
{
public:
    enum Type { typeUnknown = 0, typeImage, typeAudio, typeVideo};
	//oleObject ???

    _mediaitems()
    {
 		count_image		=0;
 		count_media		=0;
	}

    struct item 
    {
        item(
            std::wstring const & _oox_ref,
            Type _type,
            std::wstring const & _odf_ref);

        std::wstring oox_ref;
        Type type;
        std::wstring odf_ref;
    };
    typedef std::vector< item > items_array;
	
	size_t count_image;
	size_t count_media;

    void add_or_find(const std::wstring & oox_ref, Type type, std::wstring & odf_ref);
    
	void dump_rels(rels & Rels);
    items_array & items() { return items_; }

private:
    items_array items_;

};

}
}

