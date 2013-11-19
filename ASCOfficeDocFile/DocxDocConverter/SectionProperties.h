#pragma once

#include "Sepx.h"
#include "Header.h"
#include "Footer.h"

namespace AVSDocFileFormat
{
  class SectionProperties
  {
    private:
	  Sepx sepx;
	  TextItem evenPageHeader;
	  TextItem oddPageHeader;
	  TextItem evenPageFooter;
	  TextItem oddPageFooter;
	  TextItem firstPageHeader;
	  TextItem firstPageFooter;

    public:
	  explicit SectionProperties( const Sepx& _sepx ):
	  sepx(_sepx)
	  {
	    this->SetEvenPageHeader( Header() );
		this->SetOddPageHeader( Header() );
		this->SetEvenPageFooter( Footer() );
		this->SetOddPageFooter( Footer() );
		this->SetFirstPageHeader( Header() );
		this->SetFirstPageFooter( Footer() );
	  }

	  SectionProperties( const SectionProperties& _sectionProperties ):
	  sepx(_sectionProperties.sepx)
	  {
	    if ( _sectionProperties.evenPageHeader.get() != NULL )
		{
		  this->SetEvenPageHeader( _sectionProperties.evenPageHeader.as<Header>() );
		}

		if ( _sectionProperties.oddPageHeader.get() != NULL )
		{
		  this->SetOddPageHeader( _sectionProperties.oddPageHeader.as<Header>() );
		}

	    if ( _sectionProperties.evenPageFooter.get() != NULL )
		{
		  this->SetEvenPageFooter( _sectionProperties.evenPageFooter.as<Footer>() );
		}

		if ( _sectionProperties.oddPageFooter.get() != NULL )
		{
		  this->SetOddPageFooter( _sectionProperties.oddPageFooter.as<Footer>() );
		}

		if ( _sectionProperties.firstPageHeader.get() != NULL )
		{
		  this->SetFirstPageHeader( _sectionProperties.firstPageHeader.as<Header>() );
		}

		if ( _sectionProperties.firstPageFooter.get() != NULL )
		{
		  this->SetFirstPageFooter( _sectionProperties.firstPageFooter.as<Footer>() );
		}
	  }

	  const Sepx& GetSepx() const
	  {
	    return this->sepx;
	  }

	  void SetEvenPageHeader( const Header& _header )
	  {
	    this->evenPageHeader = _header;
	  }

	  void SetOddPageHeader( const Header& _header )
	  {
	    this->oddPageHeader = _header;
	  }

	  void SetEvenPageFooter( const Footer& _footer )
	  {
	    this->evenPageFooter = _footer;
	  }

	  void SetOddPageFooter( const Footer& _footer )
	  {
	    this->oddPageFooter = _footer;
	  }

	  void SetFirstPageHeader( const Header& _header )
	  {
	    this->firstPageHeader = _header;
	  }

	  void SetFirstPageFooter( const Footer& _footer )
	  {
	    this->firstPageFooter = _footer;
	  }

	  const Header* GetEvenPageHeader() const
	  {
	    return static_cast<const Header*>(this->evenPageHeader.get());
	  }

	  const Header* GetOddPageHeader() const
	  {
	    return static_cast<const Header*>(this->oddPageHeader.get());
	  }

	  const Footer* GetEvenPageFooter() const
	  {
	    return static_cast<const Footer*>(this->evenPageFooter.get());
	  }

	  const Footer* GetOddPageFooter() const
	  {
	    return static_cast<const Footer*>(this->oddPageFooter.get());
	  }

	  const Header* GetFirstPageHeader() const
	  {
	    return static_cast<const Header*>(this->firstPageHeader.get());
	  }

	  const Footer* GetFirstPageFooter() const
	  {
	    return static_cast<const Footer*>(this->firstPageFooter.get());
	  }
  };
}