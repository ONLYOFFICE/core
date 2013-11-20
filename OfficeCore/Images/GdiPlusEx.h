#pragma once

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

/*
	По простому (когда инициализация в конструкторе CGdiPlusInit()) сделать 
	не получилось, поскольку конструктор вызывается до InitInstance приложения
	а GDI+ создает свое какое-то окно прятанное, и DDE сообщения валятся туда
	вместо того чтобы дождаться пока создадим MainFrame. И через это если 
	открывали чертеж из проводника двойным кликом, а ни одного экзмпляра 
	приложения еще не было загружено, проводник орал и файл не открывал.
	Поэтому пришлось добавить процедуру Init() и вызывать ее после того как
	MainFrame уже создан.

	ВАЖНО!!!! Не надо переделывать не подумав, этот глюк отлавливался 3 часа.
*/

class CGdiPlusInit  
{
public:

	CGdiPlusInit()
	{
		m_bPresent = FALSE;
		m_nToken = 0;
	}
	virtual ~CGdiPlusInit()
	{
		if (m_bPresent) 
			Gdiplus::GdiplusShutdown(m_nToken);
	}
	BOOL Init()
	{
		Gdiplus::GdiplusStartupInput pInput;

		try
		{
			Gdiplus::GdiplusStartup(&m_nToken, &pInput, 0);

			m_bPresent = TRUE;
		}
		catch (...)
		{
			m_bPresent = FALSE;
		}

		return m_bPresent;
	}
	BOOL Good()
	{
		return m_bPresent;
	}
	BOOL IsValid()
	{
		return m_bPresent;
	}

private:

	BOOL m_bPresent;
	ULONG_PTR m_nToken;
};
