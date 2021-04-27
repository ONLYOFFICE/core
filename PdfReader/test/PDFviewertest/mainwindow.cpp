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

#ifdef QT_DEBUG
    ui->PageLineEdit->setText("748");
    ui->FileNameLineEdit->setText("./test.pdf");
    filename = "./test.pdf";
#endif
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

    PDFREADER.LoadFromFile(filename.toStdWString());
    int page = ui->PageLineEdit->text().toInt();
    PDFREADER.ConvertToRaster(page - 1, L"testpdf.bmp", 1);
    QImage pm("testpdf.bmp");

    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width(),
                             ui->PDFLabel->size().height());

    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
     //ui->PDFLabel->resize((pm.size()));
}

void MainWindow::on_OpenFileButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,
        tr("Open PDF"), "./", tr("PDF Files (*.pdf)"));

    ui->FileNameLineEdit->setText(filename);
}

void MainWindow::on_verticalScrollBar_sliderMoved(int position)
{
    QImage pm("testpdf.bmp");
    pos_y = position;
    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width(),
                             ui->PDFLabel->size().height());
    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    QImage pm("testpdf.bmp");
    pos_x = position;
    QImage to_draw = pm.copy((pm.size().width() - ui->PDFLabel->size().width()) * (pos_x / 99.),
                             (pm.size().height() - ui->PDFLabel->size().height()) * (pos_y / 99.),
                             ui->PDFLabel->size().width(),
                             ui->PDFLabel->size().height());
    ui->PDFLabel->setPixmap(QPixmap::fromImage(to_draw));
    ui->PDFLabel->setScaledContents(true);
}
