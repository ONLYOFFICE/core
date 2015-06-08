#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf_types { 

class style_family
{
public:
    enum type
    {
        None,
        Paragraph,
        Text,
        Section,
        Table,
        TableColumn,
        TableRow,
        TableCell,
        TablePage,
        Chart,
        Default,
        DrawingPage,
        Graphic,
        Presentation,
        Control,
        Ruby,
////////////////////////для генерации "красивых" имен и поиска
		Gradient,
		Hatch,
		Opacity,
		FillImage,
		Marker,
		StokeDash,
		PageLayout,
		MasterPage,
		HandoutMaster,
		LayerSet
    };

    style_family() : type_(Default) {}

    style_family(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    bool operator == (const style_family & rVal) const;
    static style_family parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const style_family & _Val);
} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::style_family);

}
