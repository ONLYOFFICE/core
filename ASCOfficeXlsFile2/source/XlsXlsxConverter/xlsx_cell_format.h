#pragma once

namespace oox {

struct XlsxCellType
{
    enum type { null, b, n, e, s, str, inlineStr };    
};

int odf_string_to_build_in(const int value_type);

class xlsx_cell_format
{
public:
    xlsx_cell_format();
public:
    XlsxCellType::type get_cell_type() const;
    
	void set_cell_type(XlsxCellType::type type);

    int		get_num_format() const;
    void	set_num_format(int numFmt);

private:
    XlsxCellType::type	cell_type_;
    int					num_format_;

};

}

