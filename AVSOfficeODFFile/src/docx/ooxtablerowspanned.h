#pragma once

#include <cpdoccore/CPSharedPtr.h>
#include <string>

namespace cpdoccore { 
namespace oox {

// контекст для обработки объединения столбцов

struct table_row_spanned
{
public:
    table_row_spanned() : num_(0), column_spanned_(0), style_(L""){}
    
    void num(unsigned int Val)  {num_ = Val; }
    unsigned int num() const { return num_; }
    void decrease() { --num_; }

    void set_style(const std::wstring & Val) {style_ = Val; }
    const std::wstring & style() const { return style_; }

    void column_spanned(unsigned int Val) {column_spanned_ = Val;}
    unsigned int column_spanned() const { return column_spanned_; }

private:
    unsigned int num_;
    unsigned int column_spanned_;
    std::wstring style_;
};

typedef shared_ptr<table_row_spanned>::Type table_row_spanned_ptr;

}
}
