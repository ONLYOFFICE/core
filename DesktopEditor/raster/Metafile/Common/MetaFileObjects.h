#ifndef _METAFILE_COMMON_METAFILEOBJECTS_H
#define _METAFILE_COMMON_METAFILEOBJECTS_H

namespace MetaFile
{
	class IFont
	{
	public:
		IFont(){}
		virtual ~IFont(){}

		virtual int          GetHeight() = 0;
		virtual std::wstring GetFaceName() = 0;
		virtual int          GetWeight() = 0;
		virtual bool         IsItalic() = 0;
		virtual bool         IsStrikeOut() = 0;
		virtual bool         IsUnderline() = 0;
		virtual int          GetEscapement() = 0;
	};

	class IBrush
	{
	public:
		IBrush(){}
		virtual ~IBrush(){}

		virtual int          GetColor() = 0;
		virtual unsigned int GetStyle() = 0;
		virtual unsigned int GetHatch() = 0;
		virtual unsigned int GetAlpha() = 0;
		virtual std::wstring GetDibPatterPath() = 0;
	};

	class IPen
	{
	public:
		IPen(){}
		virtual ~IPen(){}

		virtual int          GetColor() = 0;
		virtual unsigned int GetStyle() = 0;
		virtual unsigned int GetWidth() = 0;
	};
}

#endif //_METAFILE_COMMON_METAFILEOBJECTS_H