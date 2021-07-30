#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_nPosX = 0;
    m_nPosY = 0;
    m_dScale = 1.0;
    m_pReader = NULL;
    m_pImage = NULL;

    m_pLabel = new QLabel(ui->mainView);
    m_pLabel->setScaledContents(true);

    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;
    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    m_pFonts = oWorker.Check();

    m_pFontManager = m_pFonts->GenerateFontManager();
    NSFonts::IFontsCache* pFontsCache = NSFonts::NSFontCache::Create();
    pFontsCache->SetStreams(m_pFonts->GetStreams());
    pFontsCache->SetCacheSize(16);
    m_pFontManager->SetOwnerCache(pFontsCache);
    m_pImageCache = NSImages::NSFilesCache::Create(m_pFonts);

#ifdef QT_DEBUG
    ui->PageLineEdit->setText("1");
    ui->FileNameLineEdit->setText("/home/danya/Downloads/Telegram Desktop/PDF 1-7 (756p)-750.pdf");
    m_sFile = "/home/danya/Downloads/Telegram Desktop/PDF 1-7 (756p)-750.pdf";
    OpenFile();
#endif
}

MainWindow::~MainWindow()
{
    if (m_pFontManager) m_pFontManager->Release();
    if (m_pFonts) m_pFonts->Release();
    if (m_pImageCache) m_pImageCache->Release();
    if (m_pReader) delete m_pReader;
    delete ui;
}

void MainWindow::OpenFile()
{
    if (m_pReader)
        delete m_pReader;

    m_pReader = new PdfReader::CPdfReader(m_pFonts);
//    std::wstring sTempDirectory = NSFile::GetProcessDirectory() + L"/file_tmp";
//    if (!NSDirectory::Exists(sTempDirectory))
//        NSDirectory::CreateDirectory(sTempDirectory);
//    m_pReader->SetTempDirectory(sTempDirectory);
    if (!m_pReader->LoadFromFile(m_sFile.toStdWString()))
    {
        delete m_pReader;
        m_pReader = NULL;
    }
    on_RenderButton_clicked();
}

bool MainWindow::RenderOnByteData(int nPage, BYTE*& pBgraData, int& w, int& h)
{
    if (!m_pReader)
        return false;

    double dWidth, dHeight;
    double dDpiX, dDpiY;
    m_pReader->GetPageInfo(nPage, &dWidth, &dHeight, &dDpiX, &dDpiY);

    int nWidth  = (int)dWidth;
    int nHeight = (int)dHeight;

    nWidth = (int)(m_dScale * nWidth);
    nHeight = (int)(m_dScale * nHeight);

    w = nWidth;
    h = nHeight;

    pBgraData = new BYTE[nWidth * nHeight * 4];
    if (!pBgraData)
       return false;

    memset(pBgraData, 0xff, nWidth * nHeight * 4);
    CBgraFrame oFrame;
    oFrame.put_Data(pBgraData);
    oFrame.put_Width(nWidth);
    oFrame.put_Height(nHeight);
    oFrame.put_Stride(4 * nWidth);

    NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
    pRenderer->SetFontManager(m_pFontManager);
    pRenderer->CreateFromBgraFrame(&oFrame);
    pRenderer->SetSwapRGB(true);
    pRenderer->put_Width(dWidth * 25.4 / dDpiX);
    pRenderer->put_Height(dHeight * 25.4 / dDpiY);

    m_pReader->DrawPageOnRenderer(pRenderer, nPage, NULL);
    pRenderer->Release();

    // иначе удалится память при деструктопе CBgraFrame
    oFrame.put_Data(NULL);
    return true;
}

void MainWindow::SetImage()
{
    int nX = 0;
    int nY = 0;
    int nW = m_pImage->width();
    int nH = m_pImage->height();

    int nBaseW = ui->mainView->width();
    int nBaseH = ui->mainView->height();
    if (nW <= nBaseW)
    {
        nX = (int)((nBaseW - nW) / 2);
    }
    else
    {
        if (nX > 0)
            nX = 0;
        if (nX + nW < nBaseW)
            nX = nBaseW - nW;
    }

    if (nH <= nBaseH)
    {
        nY = (int)((nBaseH - nH) / 2);
    }
    else
    {
        if (nY > 0)
            nY = 0;
        if (nY + nH < nBaseH)
            nY = nBaseH - nH;
    }

    ui->verticalScrollBar->setMaximum((nH > nBaseH) ? (nH - nBaseH) : 0);
    ui->horizontalScrollBar->setMaximum((nW > nBaseW) ? (nW - nBaseW) : 0);

    m_pLabel->setGeometry(nX, nY, nW, nH);
    m_pLabel->setPixmap(QPixmap::fromImage(*m_pImage));

    m_nPosX = nX;
    m_nPosY = nY;
    ui->verticalScrollBar->setValue(0);
    ui->horizontalScrollBar->setValue(0);
}

void MainWindow::on_OpenFileButton_clicked()
{
    m_sFile = QFileDialog::getOpenFileName(this, tr("Open PDF"), "./", tr("PDF Files (*.pdf)"));
    ui->FileNameLineEdit->setText(m_sFile);
    OpenFile();
}

void cleanUpPixels(void* pixels)
{
    BYTE* arrPixels = (BYTE*)pixels;
    delete [] arrPixels;
}
void MainWindow::on_RenderButton_clicked()
{
    if (!m_pReader)
        return;

    int page = ui->PageLineEdit->text().toInt();
    int w, h;
    BYTE *data;
    if (!RenderOnByteData(page - 1, data, w, h))
        return;

    if (m_pImage)
        delete m_pImage;

    m_pImage = new QImage(data, w, h, 4 * w, QImage::Format_RGBA8888, &cleanUpPixels, data);
    SetImage();
}

void MainWindow::on_ScaleSlider_sliderMoved(int position)
{
    if (!m_pReader)
        return;

    m_dScale = (double)position / 100;
    on_RenderButton_clicked();
}

void MainWindow::on_LeftButton_clicked()
{
    int page = ui->PageLineEdit->text().toUInt();
    page--;
    ui->PageLineEdit->setText(std::to_string(page).c_str());
    on_RenderButton_clicked();
}

void MainWindow::on_RightButton_clicked()
{
    int page = ui->PageLineEdit->text().toUInt();
    page++;
    ui->PageLineEdit->setText(std::to_string(page).c_str());
    on_RenderButton_clicked();
}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    if (!m_pReader)
        return;
    m_nPosY = value;
    m_pLabel->move(m_pLabel->x(), -m_nPosY);
}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    if (!m_pReader)
        return;
    m_nPosX = value;
    m_pLabel->move(-m_nPosX, m_pLabel->y());
}
