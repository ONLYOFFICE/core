#pragma once

namespace DocFileFormat
{
  class CharacterRange
  {
    private: 
	  int CharacterPosition;
      int CharacterCount;

    public:
	  CharacterRange():
      CharacterPosition(0), CharacterCount(0)
	  {
	  }

	  CharacterRange( int cp, int ccp ):
      CharacterPosition(0), CharacterCount(0)
      {
        this->CharacterPosition = cp;
        this->CharacterCount = ccp;
      }

	  ~CharacterRange()
	  {
	  }

	  int GetCharacterPosition() const
	  {
	    return this->CharacterPosition;
	  }

      int GetCharacterCount() const
	  {
	    return this->CharacterCount;
	  }
  };
}