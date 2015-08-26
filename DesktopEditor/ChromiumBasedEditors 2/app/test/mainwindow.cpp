#include "MainWindow.h"

#include <dwmapi.h>
#include <windowsx.h>
#include <windows.h>
#include <stdexcept>

#include <QFile>
#include <QPixmap>

#include <QDebug>

HWND winId = 0;

CMainWindow::CMainWindow( QApplication *app, HBRUSH windowBackground, const int x, const int y, const int width, const int height, CAscApplicationManager* pManager ) :
    hWnd(0),
    hInstance( GetModuleHandle( NULL ) ),
    borderless( false ),
    borderlessResizeable( true ),
    aeroShadow( true ),
    closed( false ),
    visible( false )
{
    mainPanel = NULL;

    m_pManager = pManager;

    WNDCLASSEXW wcx = { 0 };
    wcx.cbSize = sizeof( WNDCLASSEX );
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.hInstance = hInstance;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra	= 0;
    wcx.cbWndExtra	= 0;
    wcx.lpszClassName = L"WindowClass";
    wcx.hbrBackground = windowBackground;
    wcx.hCursor = LoadCursor( hInstance, IDC_ARROW );

    std::wstring sPath = NSFile::GetProcessDirectory() + L"/asceditor_64.ico";

    wcx.hIcon = (HICON) LoadImageW( // returns a HANDLE so we have to cast to HICON
      NULL,             // hInstance must be NULL when loading from a file
      sPath.c_str(),   // the icon file name
      IMAGE_ICON,       // specifies that the file is an icon
      0,                // width of the image (we'll specify default later on)
      0,                // height of the image
      LR_LOADFROMFILE|  // we want to load a file (as opposed to a resource)
      LR_DEFAULTSIZE|   // default metrics based on the type (IMAGE_ICON, 32x32)
      LR_SHARED         // let the system release the handle when it's no longer used
    );

    RegisterClassExW( &wcx );
    if ( FAILED( RegisterClassExW( &wcx ) ) )
        throw std::runtime_error( "Couldn't register window class" );

    hWnd = CreateWindowW( L"WindowClass", L"CMainWindow", static_cast<DWORD>( Style::windowed ) , x, y, width, height, 0, 0, hInstance, nullptr );

    if ( !hWnd )
        throw std::runtime_error( "couldn't create window because of reasons" );

    SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );

    mainPanel = new QMainPanel( hWnd, m_pManager );
    winId = ( HWND )mainPanel->winId();

    SetWindowPos(winId, NULL, 0, 0, width, height, SWP_FRAMECHANGED);

    //show();
    visible = true;
    toggleBorderless();

    a = app;
}

CMainWindow::~CMainWindow()
{
    hide();
    DestroyWindow( hWnd );
}

LRESULT CALLBACK CMainWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    CMainWindow *window = reinterpret_cast<CMainWindow*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    if ( !window )
        return DefWindowProc( hWnd, message, wParam, lParam );

    switch ( message )
    {
    case WM_KEYDOWN:
    {
        switch ( wParam )
        {
            case VK_F5:
            {
                window->borderlessResizeable = !window->borderlessResizeable;
                break;
            }
            case VK_F6:
            {
                window->toggleShadow();
                window->toggleBorderless();
                SetFocus( winId );
                break;
            }
            case VK_F7:
            {
                window->toggleShadow();
                break;
            }
        }

        if ( wParam != VK_TAB )
            return DefWindowProc( hWnd, message, wParam, lParam );

        SetFocus( winId );
        break;
    }

    // ALT + SPACE or F10 system menu
    case WM_SYSCOMMAND:
    {
        if ( wParam == SC_KEYMENU )
        {
            return 0;
            RECT winrect;
            GetWindowRect( hWnd, &winrect );
            TrackPopupMenu( GetSystemMenu( hWnd, false ), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, hWnd, NULL);
            break;
        }
        else
        {
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
    }

    case WM_SETFOCUS:
    {
        QString str( "Got focus" );
        QWidget *widget = QWidget::find( ( WId )HWND( wParam ) );
        if ( widget )
            str += QString( " from %1 (%2)" ).arg( widget->objectName() ).arg(widget->metaObject()->className() );
        str += "\n";
        OutputDebugStringA( str.toLocal8Bit().data() );
        break;
    }

    case WM_NCCALCSIZE:
    {
        //this kills the window frame and title bar we added with
        //WS_THICKFRAME and WS_CAPTION
        if (window->borderless)
        {
            return 0;
        }
        break;
    }

    case WM_KILLFOCUS:
    {        
        QString str( "Lost focus" );
        QWidget *widget = QWidget::find( (WId)HWND( wParam ) );
        if ( widget )
            str += QString( " to %1 (%2)" ).arg( widget->objectName() ).arg(widget->metaObject()->className() );
        str += "\n";

        OutputDebugStringA( str.toLocal8Bit().data() );
        break;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }

    case WM_NCHITTEST:
    {
        if ( window->borderless )
        {
            const LONG borderWidth = 8; //in pixels
            RECT winrect;
            GetWindowRect( hWnd, &winrect );
            long x = GET_X_LPARAM( lParam );
            long y = GET_Y_LPARAM( lParam );
            if ( window->borderlessResizeable )
            {
                //bottom left corner
                if ( x >= winrect.left && x < winrect.left + borderWidth &&
                    y < winrect.bottom && y >= winrect.bottom - borderWidth )
                {
                    return HTBOTTOMLEFT;
                }
                //bottom right corner
                if ( x < winrect.right && x >= winrect.right - borderWidth &&
                    y < winrect.bottom && y >= winrect.bottom - borderWidth )
                {
                    return HTBOTTOMRIGHT;
                }
                //top left corner
                if ( x >= winrect.left && x < winrect.left + borderWidth &&
                    y >= winrect.top && y < winrect.top + borderWidth )
                {
                    return HTTOPLEFT;
                }
                //top right corner
                if ( x < winrect.right && x >= winrect.right - borderWidth &&
                    y >= winrect.top && y < winrect.top + borderWidth )
                {
                    return HTTOPRIGHT;
                }
                //left border
                if ( x >= winrect.left && x < winrect.left + borderWidth )
                {
                    return HTLEFT;
                }
                //right border
                if ( x < winrect.right && x >= winrect.right - borderWidth )
                {
                    return HTRIGHT;
                }
                //bottom border
                if ( y < winrect.bottom && y >= winrect.bottom - borderWidth )
                {
                    return HTBOTTOM;
                }
                //top border
                if ( y >= winrect.top && y < winrect.top + borderWidth )
                {
                    return HTTOP;
                }
            }

            return HTCAPTION;
        }
        break;
    }

    case WM_SIZE:
    {
        RECT lpWindowRect;
        GetWindowRect(hWnd, &lpWindowRect);

        bool bIsMaximize = false;

        int nMaxOffsetX = 0;
        int nMaxOffsetY = 0;
        int nMaxOffsetR = 0;
        int nMaxOffsetB = 0;

        if (window->mainPanel)
        {
            RECT winrect;
            GetClientRect( hWnd, &winrect );

            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( hWnd, &wp );

            int nSizeBorder_normal = 3;
            int nSizeBorder_max = nSizeBorder_normal;

            if ( wp.showCmd == SW_MAXIMIZE )
            {
                QPushButton* pushButtonMaximize = window->mainPanel->findChild<QPushButton*>( "pushButtonMaximize" );
                pushButtonMaximize->setStyleSheet("QPushButton {background-image:url(:/Icons/new_deploy_normal.png);border:none;margin:0;padding:0;}\
QPushButton::hover {background-image:url(:/Icons/new_deploy_hover.png);border:none;margin:0;padding:0;}\
QPushButton::pressed {background-image:url(:/Icons/new_deploy_hover.png);border:none;margin:0;padding:0;}");

                LONG lTestW = 640;
                LONG lTestH = 480;
                RECT wrect;
                wrect.left = 0;
                wrect.top = 0;
                wrect.right = lTestW;
                wrect.bottom = lTestH;
                AdjustWindowRectEx(&wrect, (GetWindowStyle(hWnd) & ~WS_DLGFRAME), FALSE, 0);

                if (0 > wrect.left)
                    nMaxOffsetX = -wrect.left;
                if (0 > wrect.top)
                    nMaxOffsetY = -wrect.top;
                if (wrect.right > lTestW)
                    nMaxOffsetR = (wrect.right - lTestW);
                if (wrect.bottom > lTestH)
                    nMaxOffsetB = (wrect.bottom - lTestH);

                // TODO: вот тут бордер!!!
                window->mainPanel->setGeometry( nMaxOffsetX + nSizeBorder_max,
                                                nMaxOffsetY + nSizeBorder_max,
                                                winrect.right - (nMaxOffsetX + nMaxOffsetR + 2 * nSizeBorder_max),
                                                winrect.bottom - (nMaxOffsetY + nMaxOffsetB + 2 * nSizeBorder_max));

                bIsMaximize = true;
            }
            else
            {
                QPushButton* pushButtonMaximize = window->mainPanel->findChild<QPushButton*>( "pushButtonMaximize" );
                pushButtonMaximize->setStyleSheet("QPushButton {background-image:url(:/Icons/new_deploy2_normal.png);border:none;margin:0;padding:0;}\
            QPushButton::hover {background-image:url(:/Icons/new_deploy2_hover.png);border:none;margin:0;padding:0;}\
            QPushButton::pressed {background-image:url(:/Icons/new_deploy2_hover.png);border:none;margin:0;padding:0;}");

                // TODO: вот тут бордер!!!
                window->mainPanel->setGeometry( nSizeBorder_normal, nSizeBorder_normal,
                                        winrect.right - 2 * nSizeBorder_normal,
                                        winrect.bottom - 2 * nSizeBorder_normal);
            }
        }

        HRGN hRgn = CreateRectRgn(nMaxOffsetX,
                                  nMaxOffsetY,
                                  lpWindowRect.right - lpWindowRect.left - nMaxOffsetX,
                                  lpWindowRect.bottom - lpWindowRect.top - nMaxOffsetY);

        SetWindowRgn(hWnd, hRgn, TRUE);

        break;
    }

    case WM_ERASEBKGND:
    {
#if 1
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);

        LONG lMargin = 0;
        rect.left -= lMargin;
        rect.top -= lMargin;
        rect.right += lMargin;
        rect.bottom += lMargin;
        // тут цвет бекграунда!
        HBRUSH hBrush = CreateSolidBrush(RGB( 49, 52, 55 ));
        FillRect(hDC, &rect, (HBRUSH)hBrush);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
#else
        HDC hDC = GetWindowDC(hWnd);

        RECT rect;
        //GetClientRect(hWnd, &rect);
        GetWindowRect(hWnd, &rect);

        int nW = (int)(rect.right-rect.left);
        int nH = (int)(rect.bottom-rect.top);

        HDC hCDC = ::CreateCompatibleDC(hDC);
        HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, nW, nH);
        HBITMAP hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
        HBRUSH hBrush = ::CreateSolidBrush(RGB( 49, 52, 55 ));
        HBRUSH hBrushOld = (HBRUSH )::SelectObject(hCDC, hBrush);

        ::PatBlt(hCDC, 0,0, nW, nH, PATCOPY);
        ::BitBlt(hDC, rect.left, rect.top, nW, nH,  hCDC, 0,0, SRCCOPY);

        ::SelectObject(hCDC, hBrushOld);
        ::SelectObject(hCDC, hBmpOld);
        ::DeleteObject(hBrush);
        ::DeleteObject(hBmp);

        ::ReleaseDC(hWnd, hDC);
#endif
        return 0;
    }

    case WM_NCPAINT:
    {
        return 0;
#if 0
        if (true)
        {
            //if (QSysInfo::windowsVersion() != QSysInfo::WV_XP)
            //    DefWindowProc(hWnd, message, wParam, lParam);

            HDC hdc;
            //hdc = GetDCEx(hWnd, (HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN);
            hdc = GetWindowDC(hWnd);
            // Paint into this DC

            RECT rect;
            GetWindowRect(hWnd, &rect);
            rect.left = 0;
            rect.top = 0;

            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            //FillRect(hdc, &rect, (HBRUSH)hBrush);
            FillRgn(hdc, (HRGN)wParam, hBrush);
            DeleteObject(hBrush);

            ReleaseDC(hWnd, hdc);
            return 0;
        }
        break;
#endif
    }

    case WM_NCACTIVATE:
    {
        return 1;
    }

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = ( MINMAXINFO* )lParam;
        if ( window->minimumSize.required )
        {
            minMaxInfo->ptMinTrackSize.x = window->getMinimumWidth();
            minMaxInfo->ptMinTrackSize.y = window->getMinimumHeight();
        }

        if ( window->maximumSize.required )
        {
            minMaxInfo->ptMaxTrackSize.x = window->getMaximumWidth();
            minMaxInfo->ptMaxTrackSize.y = window->getMaximumHeight();
        }
        return 0;
    }
    case WM_QUERYENDSESSION:
    {
        break;
    }
    case WM_ENDSESSION:
    {
        window->m_pManager->CloseApplication();
        break;
    }
#if 0
    case WM_INPUTLANGCHANGE:
    case WM_INPUTLANGCHANGEREQUEST:
    {        
        int _lang = LOWORD(lParam);
        m_oLanguage.Check(_lang);
    }
#endif
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void CMainWindow::toggleBorderless()
{
    if ( visible )
    {
        LONG newStyle = (LONG)Style::windowed;
        // чтобы не было мерцания. перерисовку при "неактивном окне" - перекроем
        //newStyle &= (~WS_CAPTION);
        newStyle &= (~WS_SYSMENU);
        if (borderless)
        {
            newStyle = (LONG)Style::aero_borderless;
        }

        SetWindowLongPtr( hWnd, GWL_STYLE, newStyle );

        borderless = !borderless;
        if ( !borderless )
        {
            toggleShadow();
        }

        //redraw frame
        SetWindowPos( hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE /*| SWP_NOZORDER | SWP_NOOWNERZORDER */);
        show();
    }
}

void CMainWindow::toggleShadow()
{
    if ( borderless )
    {
        aeroShadow = !aeroShadow;
        const MARGINS shadow_on = { 1, 1, 1, 1 };
        const MARGINS shadow_off = { 0, 0, 0, 0 };
        //DwmExtendFrameIntoClientArea( hWnd, ( aeroShadow ) ? ( &shadow_on ) : ( &shadow_off ) );
    }
}

void CMainWindow::toggleResizeable()
{
    borderlessResizeable = borderlessResizeable ? false : true;
}

bool CMainWindow::isResizeable()
{
    return borderlessResizeable ? true : false;
}

void CMainWindow::show()
{
    ShowWindow( hWnd, SW_SHOW );
    visible = true;
}

void CMainWindow::hide()
{
    ShowWindow( hWnd, SW_HIDE );
    visible = false;
}

bool CMainWindow::isVisible()
{
    return visible ? true : false;
}

// Minimum size
void CMainWindow::setMinimumSize( const int width, const int height )
{
    this->minimumSize.required = true;
    this->minimumSize.width = width;
    this->minimumSize.height = height;
}

bool CMainWindow::isSetMinimumSize()
{
    return this->minimumSize.required;
}
void CMainWindow::removeMinimumSize()
{
    this->minimumSize.required = false;
    this->minimumSize.width = 0;
    this->minimumSize.height = 0;
}

int CMainWindow::getMinimumWidth()
{
    return minimumSize.width;
}

int CMainWindow::getMinimumHeight()
{
    return minimumSize.height;
}

// Maximum size
void CMainWindow::setMaximumSize( const int width, const int height )
{
    this->maximumSize.required = true;
    this->maximumSize.width = width;
    this->maximumSize.height = height;
}
bool CMainWindow::isSetMaximumSize()
{
    return this->maximumSize.required;
}

void CMainWindow::removeMaximumSize()
{
    this->maximumSize.required = false;
    this->maximumSize.width = 0;
    this->maximumSize.height = 0;
}
int CMainWindow::getMaximumWidth()
{
    return maximumSize.width;
}
int CMainWindow::getMaximumHeight()
{
    return maximumSize.height;
}

