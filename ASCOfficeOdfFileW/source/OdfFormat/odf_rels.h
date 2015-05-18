#pragma once

#include <vector>
#include <string>

//#include <iosfwd>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/CPSharedPtr.h>

namespace cpdoccore { 
namespace odf {

class relationship /*: public xml::element_impl<relationship>*/
{
public:
    relationship() {}

    relationship(const std::wstring & Type,const std::wstring & Target) :  type_(Type), target_(Target)
    {}

    void serialize(::std::wostream & _Wostream);

    const std::wstring & type() const { return type_; }
    const std::wstring & target() const { return target_; }

private:
    std::wstring type_;
    std::wstring target_;
};

class rels;
typedef _CP_PTR(rels) rels_ptr;

/// \class rels
class rels
{
public:
     rels() {}

    void serialize(std::wostream & _Wostream);
    
	std::vector<relationship> & relationships() { return relationship_; }
    void add(relationship const & r);
    bool empty() { return relationship_.empty(); } 
    
private:
    std::vector<relationship> relationship_;

};

}
}
