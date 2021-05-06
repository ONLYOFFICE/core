#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PDFLabel->setStyleSheet("QLabel { background-color : white;}");
    pos_x = 0;
    pos_y = 0;
    scale_factor = 1;

#ifdef QT_DEBUG
    ui->PageLineEdit->setText("750");
    ui->FileNameLineEdit->setText("./test.pdf");
    filename = "./test.pdf";
#endif


    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;
    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    pFonts = oWorker.Check();
    PDFREADER = new PdfReader::CPdfReader(pFonts);
}



MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_RenderButton_clicked()
{


    int page = ui->PageLineEdit->text().toInt();
    PDFREADER->ConvertToRaster(page - 1, L"testpdf.bmp", 1);
    int w, h;
    BYTE *data;
    RenderOnByteData(page - 1, &data, &w, &h);
    pm = QImage(w, h, QImage::Format_RGBA8888);

    for (int i = 0 ; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            BYTE *p = (data + 4 * (i * w + j));
            QColor c(*(p + 2), *(p + 1), *(p + 0), *(p + 3));
            pm.setPixelColor(j, i, c);

        }
    }
    pm = pm.mirrored();
    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width() * scale_factor,
                             ui->PDFLabel->size().height() * scale_factor);

    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
     //ui->PDFLabel->resize((pm.size()));
}

void MainWindow::on_OpenFileButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,
        tr("Open PDF"), "./", tr("PDF Files (*.pdf)"));

    ui->FileNameLineEdit->setText(filename);
    PDFREADER->LoadFromFile(filename.toStdWString());
}

void MainWindow::on_verticalScrollBar_sliderMoved(int position)
{
    pos_y = position;
    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width() * scale_factor,
                             ui->PDFLabel->size().height() * scale_factor);
    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    pos_x = position;
    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width() * scale_factor,
                             ui->PDFLabel->size().height() * scale_factor);
    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
}

int MainWindow::RenderOnByteData(int nPage, BYTE **pBgraData, int *w, int *h)
{
    NSFonts::IFontManager *pFontManager = pFonts->GenerateFontManager();
    NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
    pFontCache->SetStreams(pFonts->GetStreams());
    pFontManager->SetOwnerCache(pFontCache);

    NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();

    pRenderer->SetFontManager(pFontManager);


    double dWidth, dHeight;
    double dDpiX, dDpiY;
    PDFREADER->GetPageInfo(nPage, &dWidth, &dHeight, &dDpiX, &dDpiY);

    int nWidth  = (int)dWidth  * 72 / 25.4;
    int nHeight = (int)dHeight * 72 / 25.4;

    *w = nWidth;
    *h = nHeight;

    *pBgraData = new BYTE[nWidth * nHeight * 4];
    if (!*pBgraData)
       return -1;

    memset(*pBgraData, 0xff, nWidth * nHeight * 4);
    CBgraFrame oFrame;
    oFrame.put_Data(*pBgraData);
    oFrame.put_Width(nWidth);
    oFrame.put_Height(nHeight);
    oFrame.put_Stride(-4 * nWidth);

    pRenderer->CreateFromBgraFrame(&oFrame);
    pRenderer->SetSwapRGB(false);

    dWidth  *= 25.4 / dDpiX;
    dHeight *= 25.4 / dDpiY;

    pRenderer->put_Width(dWidth);
    pRenderer->put_Height(dHeight);

    bool bBreak = false;
    PDFREADER->DrawPageOnRenderer(pRenderer, nPage, &bBreak);
}

void MainWindow::on_ScaleSlider_sliderMoved(int position)
{
    scale_factor = pow(2 , -position / 50.);
    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width() * scale_factor,
                             ui->PDFLabel->size().height() * scale_factor);
    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
}
