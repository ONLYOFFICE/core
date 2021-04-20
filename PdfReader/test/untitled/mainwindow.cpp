#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PDFLabel->setStyleSheet("QLabel { background-color : white;}");
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
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;
    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();
    PdfReader::CPdfReader PDFREADER(pFonts);
    PDFREADER.LoadFromFile(L"test.pdf");
    int page = ui->PageLineEdit->text().toInt();
    PDFREADER.ConvertToRaster(page + 1, L"testpdf.bmp", 1);
      QImage pm("testpdf.bmp");
   // QImage pm(400, 400, QImage::Format_RGB888);
   // GenerateImg(pm,  points, info);
    //setColor2(pm, 0x0000FF);
   //pm.invertPixels();
    ui->PDFLabel->setPixmap(QPixmap::fromImage(pm));
    ui->PDFLabel->setScaledContents(true);
   // ui->lable_test->resize(pm.size());
    //pm.save("test.bmp");

   // pFonts->Release();
}
