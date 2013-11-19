#pragma once

namespace DocFileFormat
{
  class LineSpacingDescriptor
  {
    friend class ParagraphPropertiesMapping;

    private: 
	  short dyaLine;
      bool fMultLinespace;

    public:
      /// Creates a new LineSpacingDescriptor with empty values
	  LineSpacingDescriptor():
      dyaLine(0), fMultLinespace(false)
      {
      }

      /// Parses the bytes to retrieve a LineSpacingDescriptor
      LineSpacingDescriptor( byte* bytes, int size ):
      dyaLine(0), fMultLinespace(false)
      {
        if ( size == 4 )
        {
		  this->dyaLine = FormatUtils::BytesToInt16( bytes, 0, size );

          if ( FormatUtils::BytesToInt16( bytes, 2, size ) == 1 )
          {
            this->fMultLinespace = true;
          }
        }
        else
        {
          //throw new ByteParseException("Cannot parse the struct LSPD, the length of the struct doesn't match");
        }
      }
  };
}