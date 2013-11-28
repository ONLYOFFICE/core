#pragma once

#include "Sepx.h"
#include "Header.h"
#include "Footer.h"

namespace AVSDocFileFormat
{
	class SectionProperties
	{
	public:
		explicit SectionProperties(const Sepx& _sepx) : sepx(_sepx)
		{
			SetEvenPageHeader(Header());
			SetOddPageHeader(Header());
			SetEvenPageFooter(Footer());
			SetOddPageFooter(Footer());
			SetFirstPageHeader(Header());
			SetFirstPageFooter(Footer());
		}

		SectionProperties(const SectionProperties& sp) : sepx(sp.sepx)
		{
			if (sp.evenPageHeader.IsInit())		SetEvenPageHeader(sp.evenPageHeader.as<Header>());
			if (sp.oddPageHeader.IsInit())		SetOddPageHeader(sp.oddPageHeader.as<Header>());
			if (sp.evenPageFooter.IsInit())		SetEvenPageFooter(sp.evenPageFooter.as<Footer>());
			if (sp.oddPageFooter.IsInit())		SetOddPageFooter(sp.oddPageFooter.as<Footer>());
			if (sp.firstPageHeader.IsInit())	SetFirstPageHeader(sp.firstPageHeader.as<Header>());
			if (sp.firstPageFooter.IsInit())	SetFirstPageFooter(sp.firstPageFooter.as<Footer>());
		}

		inline const Sepx& GetSepx() const
		{
			return sepx;
		}

		inline void SetEvenPageHeader(const Header& _header)
		{
			evenPageHeader = _header;
		}

		inline void SetOddPageHeader(const Header& _header)
		{
			oddPageHeader = _header;
		}

		inline void SetEvenPageFooter(const Footer& _footer)
		{
			evenPageFooter = _footer;
		}

		inline void SetOddPageFooter(const Footer& _footer)
		{
			oddPageFooter = _footer;
		}

		inline void SetFirstPageHeader(const Header& _header)
		{
			firstPageHeader = _header;
		}

		inline void SetFirstPageFooter(const Footer& _footer)
		{
			firstPageFooter = _footer;
		}

		inline const Header* GetEvenPageHeader() const
		{
			return static_cast<const Header*>(evenPageHeader.Get());
		}

		inline const Header* GetOddPageHeader() const
		{
			return static_cast<const Header*>(oddPageHeader.Get());
		}

		inline const Footer* GetEvenPageFooter() const
		{
			return static_cast<const Footer*>(evenPageFooter.Get());
		}

		inline const Footer* GetOddPageFooter() const
		{
			return static_cast<const Footer*>(oddPageFooter.Get());
		}

		inline const Header* GetFirstPageHeader() const
		{
			return static_cast<const Header*>(firstPageHeader.Get());
		}

		inline const Footer* GetFirstPageFooter() const
		{
			return static_cast<const Footer*>(firstPageFooter.Get());
		}

	private:
		
		Sepx		sepx;
		TextItem	evenPageHeader;
		TextItem	oddPageHeader;
		TextItem	evenPageFooter;
		TextItem	oddPageFooter;
		TextItem	firstPageHeader;
		TextItem	firstPageFooter;
	};
}