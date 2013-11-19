#ifndef _SFONT_FILE_ID_H
#define _SFONT_FILE_ID_H


//-------------------------------------------------------------------------------------------------------------------------------
// SFontFileID
//-------------------------------------------------------------------------------------------------------------------------------

class SFontFileID 
{
public:

	SFontFileID();
	virtual ~SFontFileID();

	virtual BOOL Matches(SFontFileID *pFontFileID) = 0;
};

#endif /* _SFONT_FILE_ID_H */
