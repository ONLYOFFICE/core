#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>

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
	m_eZoomType = ZoomType::pageHeight;

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

	QSettings settings("OnlyOffice (R)", "PDF Reader Test");
	restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	restoreState(settings.value("mainWindowState").toByteArray());
	ui->FileNameLineEdit->setText(settings.value("mainWindowFilePath").toString());
	m_sFile = ui->FileNameLineEdit->text();
	ui->PageLineEdit->setText("1");

#ifdef QT_DEBUG
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
	if (!m_pReader)
		m_pReader = new PdfReader::CPdfReader(m_pFonts);

	m_pReader->Close();

	if (!m_pReader->LoadFromFile(m_sFile.toStdWString()))
		return;

	ui->PageLineEdit->setText("1");
	RenderPage();
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

	double dScale = GetPageScaleByZoomType(dWidth * dDpiX / 72.0, dHeight * dDpiY / 72.0);
	nWidth = (int)(dScale * nWidth);
	nHeight = (int)(dScale * nHeight);

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
	m_sFile = QFileDialog::getOpenFileName(this, tr("Open PDF"), ui->FileNameLineEdit->text(), tr("PDF Files (*.pdf)"));	
	if (!m_sFile.isEmpty())
	{
		ui->FileNameLineEdit->setText(m_sFile);
		OpenFile();
	}
}

void cleanUpPixels(void* pixels)
{
    BYTE* arrPixels = (BYTE*)pixels;
    delete [] arrPixels;
}
void MainWindow::on_RenderButton_clicked()
{
	RenderPage();
}

void MainWindow::on_ScaleSlider_sliderMoved(int position)
{
	if (!m_pReader)
		return;

	m_eZoomType = ZoomType::custom;
	m_dScale = (double)position / 100;
	RenderPage();
}

void MainWindow::on_LeftButton_clicked()
{
	if (!IsFileOpened())
		return ui->PageLineEdit->setText("");

	int nPage = ui->PageLineEdit->text().toUInt();
	nPage = std::max(1, std::min(m_pReader->GetPagesCount(), nPage - 1));

	ui->PageLineEdit->setText(std::to_string(nPage).c_str());
	RenderPage();
}

void MainWindow::on_RightButton_clicked()
{
	if (!IsFileOpened())
		return ui->PageLineEdit->setText("");

	int nPage = ui->PageLineEdit->text().toUInt();
	nPage = std::max(1, std::min(m_pReader->GetPagesCount(), nPage + 1));

	ui->PageLineEdit->setText(std::to_string(nPage).c_str());
	RenderPage();
}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
	if (!IsFileOpened())
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
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   int nW = this->width();
   int nH = this->height();

   ui->LeftButton->move(nW / 2 - 120, nH - 67);
   ui->RightButton->move(nW / 2 + 40, nH - 67);
   ui->RenderButton->move(nW / 2 - 40, nH - 67);

   ui->OpenFileButton->move(nW - 91, 10);
   ui->FileNameLineEdit->setMaximumWidth(nW - 110);
   ui->FileNameLineEdit->setMinimumWidth(nW - 110);

   ui->ScaleSlider->move(nW - 200, nH - 57);
   ui->label->move(nW - 200 + ui->ScaleSlider->width() / 2 - ui->label->width() / 2, nH - 87);

   ui->PageLineEdit->move(nW / 2 - ui->PageLineEdit->width() / 2, nH - 97);

   ui->verticalScrollBar->move(nW - 31, 40);
   ui->verticalScrollBar->setMinimumHeight(nH - 176);
   ui->verticalScrollBar->setMaximumHeight(nH - 176);

   ui->horizontalScrollBar->move(10, nH - 137);
   ui->horizontalScrollBar->setMaximumWidth(nW - 40);
   ui->horizontalScrollBar->setMinimumWidth(nW - 40);

   ui->mainView->move(10, 40);
   ui->mainView->setMinimumWidth(nW - 40);
   ui->mainView->setMaximumWidth(nW - 40);
   ui->mainView->setMinimumHeight(nH - 176);
   ui->mainView->setMaximumHeight(nH - 176);
   RenderPage();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings("OnlyOffice (R)", "PDF Reader Test");
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
	settings.setValue("mainWindowFilePath", ui->FileNameLineEdit->text());
}
bool MainWindow::IsFileOpened()
{
	return m_pReader && !m_pReader->GetError();
}
double MainWindow::GetPageScaleByZoomType(const double& dPageWidth, const double& dPageHeight)
{
	double dScale = m_dScale;

	if (ZoomType::pageWidth == m_eZoomType)
	{
		int nViewWidth = ui->mainView->width();
		dScale = nViewWidth / dPageWidth;
	}
	else if (ZoomType::pageHeight == m_eZoomType)
	{
		int nViewHeight = ui->mainView->height();
		dScale = nViewHeight / dPageHeight;
	}

	return dScale;
}
void MainWindow::RenderPage()
{
	if (!IsFileOpened())
		return;

	int page = ui->PageLineEdit->text().toInt();

	int nPagesCount = m_pReader->GetPagesCount();
	if (nPagesCount <= 0)
		return;

	page = std::max(1, std::min(nPagesCount, page));

	int w, h;
	BYTE *data;
	if (!RenderOnByteData(page - 1, data, w, h))
		return;

	if (m_pImage)
		delete m_pImage;

	m_pImage = new QImage(data, w, h, 4 * w, QImage::Format_RGBA8888, &cleanUpPixels, data);
	SetImage();
}
