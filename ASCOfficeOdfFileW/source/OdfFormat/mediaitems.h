#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace odf {

class rels;

class mediaitems
{
public:
    enum Type { typeUnknown = 0, typeImage, typeAudio, typeVideo};
	//oleObject ???

    mediaitems(const std::wstring & ooxPacket) : oox_packet_(ooxPacket)
    {
 		count_image		=0;
 		count_media		=0;
	}

    struct item 
    {
        item(
            std::wstring const & _href,
            Type _type,
             std::wstring const & _outputName);

        std::wstring href;
        Type type;
        std::wstring outputName;
    };
    typedef std::vector< item > items_array;
	
	size_t count_image;
	size_t count_media;

    void add_or_find(const std::wstring & href, Type type, std::wstring & ref);
    
	void dump_rels(rels & Rels);
    items_array & items() { return items_; }

private:
    items_array items_;
    std::wstring oox_packet_;

};

}
}

