#pragma once

#include <string>
#include <vector>
#include <iosfwd>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/CPSharedPtr.h>

namespace cpdoccore { 
namespace oox {

class relationship : public xml::element_impl<relationship>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;

public:
    relationship() {}

    relationship(const std::wstring & Id,
        const std::wstring & Type,
        const std::wstring & Target,
        const std::wstring & TargetMode = L"") : id_(Id), type_(Type), target_(Target), target_mode_(TargetMode) 
    {}

    virtual ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

public:
    const std::wstring & id() const { return id_; }
    const std::wstring & type() const { return type_; }
    const std::wstring & target() const { return target_; }
    const std::wstring & target_mode() const { return target_mode_; }

private:
    std::wstring id_;
    std::wstring type_;
    std::wstring target_;
    std::wstring target_mode_;
};

class rels;
typedef _CP_PTR(rels) rels_ptr;

/// \class rels
class rels : public xml::element_impl<rels>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;

public:
    virtual ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;
    std::vector<relationship> & relationships() { return relationship_; }
    void add(relationship const & r);
    bool empty() { return relationship_.empty(); } 
    
private:
    std::vector<relationship> relationship_;

};

}
}
