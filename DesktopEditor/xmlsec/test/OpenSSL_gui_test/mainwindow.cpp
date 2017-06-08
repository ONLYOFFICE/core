#include "mainwindow.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QtNetwork/QSslSocket>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QRect rect = this->geometry();
    this->setGeometry(rect.x(), rect.y(), 600, 170);

    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing( 0 );
    mainGridLayout->setMargin( 0 );
    setLayout( mainGridLayout );

    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName( "centralWidget" );
    centralWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    this->setCentralWidget(centralWidget);

    m_pKeyFile = new QPlainTextEdit(centralWidget);
    m_pKeyPassword = new QPlainTextEdit(centralWidget);

    m_pCertFile = new QPlainTextEdit(centralWidget);
    m_pCertPassword = new QPlainTextEdit(centralWidget);

    m_pKeyDialog = new QPushButton(centralWidget);
    m_pCertDialog = new QPushButton(centralWidget);
    m_pOK = new QPushButton(centralWidget);

    m_pKeyDialog->setText("...");
    m_pCertDialog->setText("...");
    m_pOK->setText("OK");

    m_pKeyFile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pKeyPassword->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pCertFile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pCertPassword->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QObject::connect( m_pKeyDialog, SIGNAL( clicked() ), this, SLOT( pushButtonKeyClicked()) );
    QObject::connect( m_pCertDialog, SIGNAL( clicked() ), this, SLOT( pushButtonCertClicked()) );
    QObject::connect( m_pOK, SIGNAL( clicked() ), this, SLOT( pushButtonOKClicked()) );

    CalculatePlaces();
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    CalculatePlaces();
}

void MainWindow::CalculatePlaces()
{
    int nWidth = this->width();
    int nHeight = this->height();

    int nWidthBetween = 30;
    int nWidthButton = 40;
    int nWidthPassword = 40;
    int nHeightLine = 30;
    int nWidthPath = nWidth - 40 - 2 * nWidthBetween - nWidthButton - nWidthPassword;
    if (nWidthPath < 20)
        nWidthPath = 20;

    int nTop1 = 20;
    m_pKeyFile->setGeometry(20, nTop1, nWidthPath, nHeightLine);
    m_pKeyPassword->setGeometry(20 + nWidthPath + nWidthBetween, nTop1, nWidthPassword, nHeightLine);
    m_pKeyDialog->setGeometry(nWidth - 20 - nWidthButton, nTop1, nWidthButton, nHeightLine);

    nTop1 = 20 + nHeightLine + 20;
    m_pCertFile->setGeometry(20, nTop1, nWidthPath, nHeightLine);
    m_pCertPassword->setGeometry(20 + nWidthPath + nWidthBetween, nTop1, nWidthPassword, nHeightLine);
    m_pCertDialog->setGeometry(nWidth - 20 - nWidthButton, nTop1, nWidthButton, nHeightLine);

    m_pOK->setGeometry((nWidth - 40) >> 1, nTop1 + nHeightLine + 20, 40, nHeightLine);
}

void MainWindow::pushButtonKeyClicked()
{
    std::wstring sFile = QFileDialog::getOpenFileName(NULL, QString(), QString()).toStdWString();
    if (sFile.empty())
        return;

    m_pKeyFile->setPlainText(QString::fromStdWString(sFile));
}

void MainWindow::pushButtonCertClicked()
{
    std::wstring sFile = QFileDialog::getOpenFileName(NULL, QString(), QString()).toStdWString();
    if (sFile.empty())
        return;

    m_pCertFile->setPlainText(QString::fromStdWString(sFile));
}

void MainWindow::pushButtonOKClicked()
{
    COpenssl_library library;
    int nKeyValue = library.LoadKey(m_pKeyFile->toPlainText().toStdWString(), m_pKeyPassword->toPlainText().toStdString());
    int nCertValue = library.LoadCert(m_pCertFile->toPlainText().toStdWString(), m_pCertPassword->toPlainText().toStdString());

    int y = 0;
    y++;
}
