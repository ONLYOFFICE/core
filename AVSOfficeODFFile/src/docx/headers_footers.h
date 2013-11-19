#ifndef _CPDOCCORE_DOCX_HEADERS_FOOTERS_H_
#define _CPDOCCORE_DOCX_HEADERS_FOOTERS_H_

#include <string>
#include <boost/unordered_map.hpp>

#include "docx_rels.h"
namespace cpdoccore { 

namespace odf {
class header_footer_impl;
}

namespace oox {

//class rels;

class headers_footers
{
public:
    headers_footers() : size_(0),enable_write_(false){}
    enum Type { header, footer, headerLeft, footerLeft, headerFirst, footerFirst };
    std::wstring add(const std::wstring & StyleName, const std::wstring & Content, Type type,rels &_rels);
   
	struct instance 
    {
        instance(const std::wstring & Id,
            const std::wstring & Content,
            Type _Type, const std::wstring & Name) : id_(Id), content_(Content), type_(_Type), name_(Name){}
        std::wstring id_;
        std::wstring content_;
        Type type_;
        std::wstring name_;
		rels rels_;
    };

    void dump_rels(rels & Rels) const;
    bool write_sectPr(const std::wstring & StyleName, std::wostream & _Wostream) const;

    typedef boost::shared_ptr<instance> instance_ptr;
    typedef std::vector<instance_ptr> instances_array;
    typedef boost::unordered_map<std::wstring, instances_array> instances_map; 
    const instances_map & instances() const { return instances_; }

	bool get_enable_write(){return enable_write_;}
	void set_enable_write(bool val){enable_write_ = val;}

private:
	bool enable_write_;
	static std::wstring create_id(size_t i);
	static std::wstring create_name(size_t i, Type _Type);
	instances_map instances_;
	size_t size_;
   
};

}
}


#endif

