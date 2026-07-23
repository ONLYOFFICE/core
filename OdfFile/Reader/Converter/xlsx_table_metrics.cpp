/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "xlsx_table_metrics.h"

#include <vector>

namespace cpdoccore {
namespace oox 
{

struct region
{
    region(int sCell, double sPos, int c, double l) : start_cell(sCell),
        start_pos(sPos),
        count(c),
        length(l)
    {}

    int start_cell;
    double start_pos;
    int count;
    double length;
};

class regions
{
public:
    regions() : next_cell_(0), next_pos_(0)
    {}

public:
    void add(int count, double length_pt)
    {
        region_.push_back(region(next_cell_, next_pos_, count, length_pt));
        next_cell_ += count;
        next_pos_ += count * length_pt;
    }
    double search(int cell)
    {
		double length_pt =0;
 		
		for (size_t i = 0; i < region_.size(); i++)
        {
			region & r = region_[i];
            if (cell <= r.start_cell + r.count)
            {
				length_pt += (cell-r.start_cell)*r.length;
				break;
            }
			length_pt = r.start_pos + r.count * r.length;
        }

        if (false == region_.empty())
        {
			if (region_.back().start_cell + region_.back().count < cell)
			{
				int offset = cell-region_.back().start_cell-region_.back().count/*-1*/;
				if (offset < 0)
				{
					offset =0;
				}
				length_pt += offset * region_.back().length;
			}
		}

        return length_pt;
    }

    std::pair<int, double> search(int offset, double pos)
    {
		double skip_length =0;

		if (pos < 0)//cs102.ods
		{
			int c_skip = 0, i = 0;
			for (i = 0; i < (int)region_.size(); i++)
			{
				if (region_[i].count + c_skip > offset)
					break;
				c_skip += region_[i].count;
			}
			for (; i >= 0 && pos < 0; i--)
			{
				pos += region_[i].length * region_[i].count;
				offset--;
			}

			if (offset	< 0) offset	= 0;
			if (pos		< 0) pos	= 0;

		}
        
		for (size_t i = 0; i < region_.size(); i++)
        {
			region & r = region_[i];
			if (r.start_cell + r.count <= offset) 
			{
				skip_length = r.start_pos + r.count*r.length;
				continue;
			}
            if (pos + skip_length >= r.start_pos && pos + skip_length < (r.start_pos + r.count * r.length))
            {
				skip_length += (offset - r.start_cell) * r.length;
                const double diff = pos + skip_length - r.start_pos;
                
				int cell =  diff / r.length ;
                double offset_cell = diff - cell * r.length;
				if (offset_cell < 0)
				{
					offset_cell =0;
				}
                return std::pair<int, double>(r.start_cell + cell, offset_cell);
            }
        }

        if (region_.size() > 0)
        {
 			if (region_.back().start_cell + region_.back().count < offset)
			{
				skip_length += (offset - region_.back().start_cell - region_.back().count /*- 1*/) * region_.back().length;
			}
			
			region const & last_r	= region_[region_.size() - 1];
            const int last_cell	= last_r.start_cell + last_r.count;
           
			const double last_pos	= last_r.start_pos + last_r.count * last_r.length;
            double diff = pos + skip_length - last_r.start_pos - (last_r.count * last_r.length);
           
			if (diff < 0)
				diff = 0;

			const int cell =  diff / last_r.length;
            double offset_cell	= diff - cell * last_r.length;
			
			if (offset_cell < 0)
			{
				offset_cell = 0;
			}
            return std::pair<int, double>(last_cell + cell , offset_cell);                                    
        }
        else
            return std::pair<int, double>(offset, pos);
    }
    std::pair<int, double> search(double pos) 
    {
		for (size_t i = 0; i < region_.size(); i++)
        {
			region & r = region_[i];

            if (pos >= r.start_pos && pos < (r.start_pos + r.count * r.length))
            {
                const double diff	= pos - r.start_pos;
                const int	cell	=  diff / r.length;
                double		offset	= diff - cell * r.length;
				
				if (offset < 0)
				{
					offset =0;
				}
                return std::pair<int, double>(r.start_cell + cell, offset);
            }
        }

        if (region_.size() > 0)
        {
            region const & last_r = region_[region_.size() - 1];
            const size_t last_cell = last_r.start_cell + last_r.count;
            const double last_pos = last_r.start_pos + last_r.count * last_r.length;
            
			const double	diff	= pos - last_pos;
            const int		cell	= diff / last_r.length;
            double			offset	= diff - cell * last_r.length;
			if (offset < 0)
			{
				offset =0;
			}
            return std::pair<int, double>(last_cell + cell, offset);                                    
        }
        else
            return std::pair<int, double>(0, pos);
    }
private:
    int next_cell_;
    double next_pos_;
    std::vector<region> region_;
};

class xlsx_table_metrics::Impl
{
public:
    xlsx_table_position calc(double x_pt, double y_pt)
    {
        const std::pair<int, double> c = cols_.search (x_pt);
        const std::pair<int, double> r = rows_.search (y_pt);
      
		xlsx_table_position res = {(size_t)c.first, c.second, (size_t)r.first, r.second};
        return res;
    }

     xlsx_table_position calc(int offset_col, int offset_row, double x_pt, double y_pt)
    {
		std::pair<int, double> c ;
		std::pair<int, double> r ; 
		
		c = cols_.search(offset_col, x_pt);

		r = rows_.search(offset_row, y_pt);
        
		xlsx_table_position res = {(size_t)c.first, c.second, (size_t)r.first, r.second};
      
		return res;
    }
      xlsx_table_position calc(int last_col,int last_row)
    {
		std::pair<int, double> c = cols_.search (last_col, 0);
		std::pair<int, double> r = rows_.search (last_row, 0); 
		
     
		xlsx_table_position res = {(size_t)c.first, c.second, (size_t)r.first, r.second};
      
		return res;
    }
	  void update_pt(int offset_col,int offset_row,double &x_pt, double &y_pt)
    {	
		x_pt += cols_.search (offset_col);
		y_pt += rows_.search (offset_row);
     }
	 
	 void add_cols(int count, double widht_pt)
    {
        return cols_.add(count, widht_pt);
    }

    void add_rows(int count, double height_pt)
    {
        return rows_.add(count, height_pt);
    }

private:
    regions cols_;
    regions rows_;    

};

xlsx_table_metrics::xlsx_table_metrics() : impl_(new xlsx_table_metrics::Impl() )
{
}

xlsx_table_metrics::~xlsx_table_metrics()
{
}

xlsx_table_position xlsx_table_metrics::calc(double x_pt, double y_pt)
{
    return impl_->calc (x_pt, y_pt);
}

xlsx_table_position xlsx_table_metrics::calc(int offset_col,int offset_row,double x_pt, double y_pt)
{
    return impl_->calc(offset_col, offset_row, x_pt, y_pt);
}
xlsx_table_position xlsx_table_metrics::calc(int last_col,int last_row)
{
    return impl_->calc (last_col, last_row);
}

void xlsx_table_metrics::update_pt(int offset_col,int offset_row,double &x_pt, double &y_pt)
{
    return impl_->update_pt (offset_col, offset_row, x_pt, y_pt);
}
void xlsx_table_metrics::add_cols(int count, double width_pt)
{
    return impl_->add_cols (count, width_pt);
}

void xlsx_table_metrics::add_rows(int count, double height_pt)
{
    return impl_->add_rows (count, height_pt);
}

}
}
