#ifndef _CPDOCCORE_XLSX_ROW_SPANNED_H_INCLUDED_4e93cd8f
#define _CPDOCCORE_XLSX_ROW_SPANNED_H_INCLUDED_4e93cd8f

#ifdef _MSC_VER
#pragma once
#endif

namespace cpdoccore {
namespace oox {

struct xlsx_row_spanned
{
public:
    xlsx_row_spanned(): num_(0), column_spanned_(0), styleId_(-1)
    {}
    void num(unsigned int Val)  {num_ = Val; }

    unsigned int num() const { return num_; }
    void decrease() { --num_; }

    void set_style_id(int id)
    {
        styleId_ = id;
    }

    int get_style_id()
    {
        return styleId_;   
    }

    void column_spanned(unsigned int Val) {column_spanned_ = Val;}
    unsigned int column_spanned() const { return column_spanned_; }

private:
    unsigned int num_;
    unsigned int column_spanned_;
    int styleId_;

};

}
}

#endif
