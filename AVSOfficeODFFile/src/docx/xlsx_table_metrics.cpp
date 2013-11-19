#include "precompiled_cpodf.h"
#include "xlsx_table_metrics.h"
#include <vector>
#include <boost/foreach.hpp>

namespace cpdoccore {
namespace oox 
{

struct region
{
    region(size_t sCell, double sPos, size_t c, double l) : start_cell(sCell),
        start_pos(sPos),
        count(c),
        length(l)
    {}

    size_t start_cell;
    double start_pos;
    size_t count;
    double length;
};

class regions
{
public:
    regions() : next_cell_(0), next_pos_(0)
    {}

public:
    void add(size_t count, double length_pt)
    {
        region_.push_back(region(next_cell_, next_pos_, count, length_pt));
        next_cell_ += count;
        next_pos_ += count * length_pt;
    }
    double search(size_t cell)
    {
		double length_pt =0;
        BOOST_FOREACH(region const & r, region_)
        {
            if (cell <= r.start_cell + r.count)
            {
				length_pt += (cell-r.start_cell)*r.length;
				break;
            }
			length_pt = r.start_pos + r.count * r.length;
        }

        if (region_.size() > 0)
        {
			if (region_.back().start_cell + region_.back().count < cell)
			{
				int offset = cell-region_.back().start_cell-region_.back().count/*-1*/;
				if (offset < 0)
				{
					offset =0;
				}
				length_pt+= offset * region_.back().length;
			}
		}

        return length_pt;
    }

    std::pair<size_t, double> search(size_t offset, double pos)
    {
		double skip_length =0;
        BOOST_FOREACH(region const & r, region_)
        {
			if (r.start_cell + r.count <= offset) 
			{
				skip_length = r.start_pos +r.count*r.length;
				continue;
			}
            if (pos+skip_length >= r.start_pos && pos+skip_length < (r.start_pos + r.count * r.length))
            {
				skip_length += (offset-r.start_cell) * r.length;
                const double diff = pos+skip_length - r.start_pos;
                const size_t cell =  diff / r.length ;
                double offset_cell = diff - cell * r.length;
				if (offset_cell < 0)
				{
					offset_cell =0;
				}
                return std::pair<size_t, double>(r.start_cell + cell , offset_cell);
            }
        }

        if (region_.size() > 0)
        {
 			if (region_.back().start_cell + region_.back().count < offset)
			{
				skip_length+= (offset-region_.back().start_cell-region_.back().count/*-1*/) * region_.back().length;
			}
			
			region const & last_r = region_[region_.size() - 1];
            const size_t last_cell = last_r.start_cell + last_r.count;
            const double last_pos = last_r.start_pos + last_r.count * last_r.length;
            double diff = pos+skip_length-last_r.start_pos-(last_r.count * last_r.length);
            const size_t cell =  diff / last_r.length;
            double offset_cell = diff - cell * last_r.length;
			if (offset_cell < 0)
			{
				offset_cell =0;
			}
            return std::pair<size_t, double>(last_cell + cell , offset_cell);                                    
        }
        else
            return std::pair<size_t, double>(offset, pos);
    }
    std::pair<size_t, double> search(double pos) 
    {
        BOOST_FOREACH(region const & r, region_)
        {
            if (pos >= r.start_pos && pos < (r.start_pos + r.count * r.length))
            {
                const double diff = pos - r.start_pos;
                const size_t cell =  diff / r.length;
                double offset = diff - cell * r.length;
				if (offset < 0)
				{
					offset =0;
				}
                return std::pair<size_t, double>(r.start_cell + cell, offset);
            }
        }

        if (region_.size() > 0)
        {
            region const & last_r = region_[region_.size() - 1];
            const size_t last_cell = last_r.start_cell + last_r.count;
            const double last_pos = last_r.start_pos + last_r.count * last_r.length;
            const double diff = pos - last_pos;
            const size_t cell =  diff / last_r.length;
            double offset = diff - cell * last_r.length;
			if (offset < 0)
			{
				offset =0;
			}
            return std::pair<size_t, double>(last_cell + cell, offset);                                    
        }
        else
            return std::pair<size_t, double>(0, pos);
    }
private:
    size_t next_cell_;
    double next_pos_;
    std::vector<region> region_;
};

class xlsx_table_metrics::Impl
{
public:
    xlsx_table_position calc(double x_pt, double y_pt)
    {
        const std::pair<size_t, double> c = cols_.search(x_pt);
        const std::pair<size_t, double> r = rows_.search(y_pt);
      
		xlsx_table_position res = {c.first, c.second, r.first, r.second};
        return res;
    }

     xlsx_table_position calc(size_t offset_col,size_t offset_row,double x_pt, double y_pt)
    {
		std::pair<size_t, double> c ;
		std::pair<size_t, double> r ; 
		
		c = cols_.search(offset_col,x_pt);

		r = rows_.search(offset_row,y_pt);
        
		xlsx_table_position res = {c.first, c.second, r.first, r.second};
      
		return res;
    }
      xlsx_table_position calc(size_t last_col,size_t last_row)
    {
		std::pair<size_t, double> c = cols_.search(last_col,0);
		std::pair<size_t, double> r = rows_.search(last_row,0); 
		
     
		xlsx_table_position res = {c.first, c.second, r.first, r.second};
      
		return res;
    }
	  void update_pt(size_t offset_col,size_t offset_row,double &x_pt, double &y_pt)
    {	
		x_pt += cols_.search(offset_col);
		y_pt += rows_.search(offset_row);
     }
	 
	 void add_cols(size_t count, double widht_pt)
    {
        return cols_.add(count, widht_pt);
    }

    void add_rows(size_t count, double height_pt)
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
    return impl_->calc(x_pt, y_pt);
}

xlsx_table_position xlsx_table_metrics::calc(size_t offset_col,size_t offset_row,double x_pt, double y_pt)
{
    return impl_->calc(offset_col,offset_row,x_pt, y_pt);
}
xlsx_table_position xlsx_table_metrics::calc(size_t last_col,size_t last_row)
{
    return impl_->calc(last_col,last_row);
}

void xlsx_table_metrics::update_pt(size_t offset_col,size_t offset_row,double &x_pt, double &y_pt)
{
    return impl_->update_pt(offset_col,offset_row,x_pt, y_pt);
}
void xlsx_table_metrics::add_cols(size_t count, double width_pt)
{
    return impl_->add_cols(count, width_pt);
}

void xlsx_table_metrics::add_rows(size_t count, double height_pt)
{
    return impl_->add_rows(count, height_pt);
}

}
}
