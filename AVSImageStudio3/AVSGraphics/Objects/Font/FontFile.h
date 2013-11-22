#ifndef _FONT_FILE_H
#define _FONT_FILE_H

class CFontEngine;
class CFont;

//-------------------------------------------------------------------------------------------------------------------------------
// CFontFile
//-------------------------------------------------------------------------------------------------------------------------------

class CFontFile 
{
public:

	virtual ~CFontFile();

	// Создаем новый объект CFont, для данного FontFile.
	virtual CFont *MakeFont() = 0;

	// Счетчик ссылок. Удаляем объект если счетчик равен 0.
	void AddRef();
	void Release();

	CStringW GetFontPath() const
	{
		return m_wsFileName;
	}

	long GetFontIndex() const
	{
		return m_lIndex;
	}

protected:

	CFontFile(wchar_t *wsFileName, long lIndex);

private:

	CStringW     m_wsFileName; // Путь к файлу
	long         m_lIndex;     // Номер шрифта в файле

	int          m_nRef;       // Счеткчик ссылок

	friend class CFontEngine;
};

#endif /* _FONT_FILE_H */
