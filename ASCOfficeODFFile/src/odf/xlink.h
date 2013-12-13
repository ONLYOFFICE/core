#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore {
namespace odf {

class xlink_type
{
public:
    enum type
    {
        Simple
    };

    xlink_type() {}
    xlink_type(type _Type) : type_(_Type) {}
    type get_type() const
    {
        return type_;
    };
    static xlink_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const xlink_type & _Val);

class xlink_actuate
{
public:
    enum type
    {
        OnRequest,
		OnLoad
    };

    xlink_actuate() {}
    xlink_actuate(type _Type) : type_(_Type) {}
    type get_type() const
    {
        return type_;
    };
    static xlink_actuate parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const xlink_actuate & _Val);

class xlink_show
{
public:
    enum type
    {
        New,
        Replace,
		Embed
    };
    xlink_show() {}
    xlink_show(type _Type) : type_(_Type) {}
    type get_type() const
    {
        return type_;
    };
    static xlink_show parse(const std::wstring & Str);

private:
    type type_;
};

std::wostream & operator << (std::wostream & _Wostream, const xlink_show & _Val);

}

APPLY_PARSE_XML_ATTRIBUTES(odf::xlink_type);
APPLY_PARSE_XML_ATTRIBUTES(odf::xlink_actuate);
APPLY_PARSE_XML_ATTRIBUTES(odf::xlink_show);

} // namespace cpdoccore

