#pragma once

#include <string>
#include <vector>


namespace oox {
///////////////////////////////////////////////////////////////////////////////////////    
class default_content_type
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

public:
    default_content_type(const std::wstring & Extension, const std::wstring & contentType) : extension_(Extension),
        content_type_(contentType)
    {}
    ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

    const std::wstring & extension() const { return extension_; }
    const std::wstring & content_type() const { return content_type_; }
    
private:
    std::wstring extension_;
    std::wstring content_type_;
};

///////////////////////////////////////////////////////////////////////////////////////
class override_content_type 
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

public:
    override_content_type(const std::wstring & PartName, const std::wstring & contentType) : part_name_(PartName),
        content_type_(contentType)
    {}
    ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

    const std::wstring & part_name() const { return part_name_; }
    const std::wstring & content_type() const { return content_type_; }

public:
    std::wstring part_name_;
    std::wstring content_type_;

};

///////////////////////////////////////////////////////////////////////////////////////
/// \class content_type
class content_type 
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

    void add_default(default_content_type const & _Ct)
    {
        get_default().push_back(_Ct);        
    }

    void add_default(std::wstring const & Extension, std::wstring const & contentType)
    {
        add_default(default_content_type(Extension, contentType));
    }

    void add_override(override_content_type const & _Ct)
    {
        get_override().push_back(_Ct);
    }

    void add_override(std::wstring const & PartName, std::wstring const & contentType)
    {
        add_override(override_content_type(PartName, contentType));    
    }

    std::vector<default_content_type> & get_default() { return default_; };
    std::vector<override_content_type> & get_override() { return override_; };

private:
    std::vector<default_content_type> default_;
	std::vector<override_content_type> override_;

};

}