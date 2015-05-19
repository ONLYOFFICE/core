#pragma once
	
// - For East Asian languages such as Chinese, Japanese, Korean, and other languages with complex characters. 
// - Input Method Editors (IME) 

// http://msdn.microsoft.com/en-us/library/ee419002(VS.85).aspx
// http://msdn.microsoft.com/en-us/library/dd318641(VS.85).aspx

#include <Imm.h>

#pragma comment ( lib, "imm32.lib" )

class CIMEUIController
{
public:
	
	CIMEUIController ()
	{
		m_bInitialized = false;
	}

	~CIMEUIController ()
	{
		Uninitialize ();
	}

	
	bool Initialize( HWND hWND )
	{
		if ( m_bInitialized )
			return true;

		m_hWND = hWND;
	
		return true;
	}

	void Uninitialize()
	{
		if ( false == m_bInitialized )
			return;
		
		m_bInitialized = false;
	}

	
	LPARAM ProcessMessage ( UINT uMsg, WPARAM wParam, LPARAM& lParam, WCHAR** ppIMEStr, LONG& IMEBytes )
	{
		switch ( uMsg )
		{
		case WM_IME_STARTCOMPOSITION:
			{
			
			}
			break;
		
		// 
		case WM_IME_ENDCOMPOSITION:
			{
				*ppIMEStr	=	NULL;
				IMEBytes	=	0;

				HIMC hIMC = ImmGetContext ( m_hWND );
				if ( hIMC )
				{
					LONG IMECursorBytes	= ::ImmGetCompositionStringW ( hIMC, 
						GCS_RESULTSTR,
						//GCS_RESULTREADSTR
						NULL, 0 );

					DWORD Bytes = 0;
					WCHAR* pIMEStr = new WCHAR [ IMECursorBytes + 1 ];
					::ZeroMemory ( pIMEStr, sizeof ( WCHAR ) * IMECursorBytes + 1 );
					
					if ( pIMEStr )
					{
						if ( 0 != ::ImmGetCompositionStringW ( hIMC, 
							GCS_RESULTSTR,
							//GCS_RESULTREADSTR,
							pIMEStr, IMECursorBytes + 1 ) ) 
						{
							*ppIMEStr	=	pIMEStr;
							IMEBytes	=	IMECursorBytes + 1;
					
							ImmReleaseContext ( m_hWND, hIMC );

							return TRUE;

						}
					}

					ImmReleaseContext ( m_hWND, hIMC );
				}
			}
			break;

		case WM_IME_NOTIFY:
			{
				switch ( wParam )
				{
				case IMN_SETCONVERSIONMODE:
				case IMN_SETOPENSTATUS:
					break;

				case IMN_OPENCANDIDATE:
				case IMN_CHANGECANDIDATE:
					break;
				}
			}
			break;
		}

		return FALSE;
	}


private:

	bool	m_bInitialized;

	HWND	m_hWND;
};