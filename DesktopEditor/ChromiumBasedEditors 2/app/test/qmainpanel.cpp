#include <windows.h>

#include "./qmainpanel.h"

QMainPanel::QMainPanel( HWND hWnd, CAscApplicationManager* pManager ) : QWinWidget( hWnd )
{
    windowHandle = hWnd;

    // Central widget
    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName( "centralWidget" );
    centralWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    centralWidget->setStyleSheet("background-color:#313437");

    m_pPanel = new QAscMainPanel(this, pManager, true);
    m_pPanel->SetMainButtonsController(this);

    resizeEvent(NULL);

    show();
}

void QMainPanel::resizeEvent(QResizeEvent* event)
{
    if (NULL != event)
        QWinWidget::resizeEvent(event);

    int nWindowW = this->width();
    int nWindowH = this->height();

    m_pPanel->setGeometry(0, 0, nWindowW, nWindowH);
}

bool QMainPanel::nativeEvent( const QByteArray &, void *msg, long * result)
{
    Q_UNUSED(result);
    MSG *message = ( MSG * )msg;
    switch( message->message )
    {
    case WM_SYSKEYDOWN:
    {
        if ( message->wParam == VK_SPACE )
        {
            RECT winrect;
            GetWindowRect( windowHandle, &winrect );
            TrackPopupMenu( GetSystemMenu( windowHandle, false ), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, windowHandle, NULL);
            break;
        }
    }
    case WM_KEYDOWN:
    {
        if ( message->wParam == VK_F5 || message->wParam == VK_F6 || message->wParam == VK_F7)
        {
            SendMessage( windowHandle, WM_KEYDOWN, message->wParam, message->lParam );
            break;
        }
    }
    }

    return false;
}

void QMainPanel::mousePressEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        ReleaseCapture();
        SendMessage( windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
    }

    if ( event->type() == QEvent::MouseButtonDblClick )
    {
        if (event -> button() == Qt::LeftButton)
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( parentWindow(), &wp );
            if ( wp.showCmd == SW_MAXIMIZE )
            {
                ShowWindow( parentWindow(), SW_RESTORE );
            }
            else
            {
                ShowWindow( parentWindow(), SW_MAXIMIZE );
            }
        }
    }
}

void QMainPanel::pushButtonMinimizeClicked()
{
    ShowWindow( parentWindow(), SW_MINIMIZE );
}

void QMainPanel::pushButtonMaximizeClicked()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( parentWindow(), &wp );
    if ( wp.showCmd == SW_MAXIMIZE )
    {
        ShowWindow( parentWindow(), SW_RESTORE );
    }
    else
    {
        ShowWindow( parentWindow(), SW_MAXIMIZE );
    }
}

void QMainPanel::pushButtonCloseClicked()
{
    PostQuitMessage( 0 );
}
