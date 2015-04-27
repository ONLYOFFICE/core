#ifndef _METAFILE_OBJECTS_H
#define _METAFILE_OBJECTS_H

namespace MetaFile
{
	class CFont
	{
	public:
		CFont(){}
		virtual ~CFont(){}

		virtual int          GetHeight() = 0;
		virtual std::wstring GetFaceName() = 0;
		virtual int          GetWeight() = 0;
		virtual bool         IsItalic() = 0;
		virtual bool         IsStrikeOut() = 0;
		virtual bool         IsUnderline() = 0;
		virtual int          GetEscapement() = 0;
	};

	class CBrush
	{
	public:
		CBrush(){}
		virtual ~CBrush(){}

		virtual int          GetColor() = 0;
		virtual unsigned int GetStyle() = 0;
		virtual unsigned int GetHatch() = 0;
		virtual unsigned int GetAlpha() = 0;
		virtual std::wstring GetDibPatterPath() = 0;
	};

	class CPen
	{
	public:
		CPen(){}
		virtual ~CPen(){}

		virtual int          GetColor() = 0;
		virtual unsigned int GetStyle() = 0;
		virtual unsigned int GetWidth() = 0;
	};
}

#endif //_METAFILE_OBJECTS_H