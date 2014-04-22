#pragma once

#include <vector>
#include <string>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

namespace cpdoccore
{

	class oox_shape;
	typedef _CP_PTR(oox_shape) oox_shape_ptr;

	class oox_shape
	{
	public:
		static oox_shape_ptr create(int ooxPrstGeomType);

		struct _equation
		{
			std::wstring name;
			std::wstring formula;

		};
		struct _handle
		{		
			_CP_OPT(std::wstring) position;	

			_CP_OPT(std::wstring) y_minimum;
			_CP_OPT(std::wstring) y_maximum;
			_CP_OPT(std::wstring) x_minimum;
			_CP_OPT(std::wstring) x_maximum;
		};

		void add(std::wstring name,std::wstring frmla)
		{
			_equation q = {name,frmla};
			equations.push_back(q);
		}

		std::vector<_equation> equations;
		std::vector<_handle> handles;		
		
		std::wstring enhanced_path;
		std::wstring modifiers;
		std::wstring text_areas;
		std::wstring view_box;

		_CP_OPT(std::wstring) glue_points;
	};
};