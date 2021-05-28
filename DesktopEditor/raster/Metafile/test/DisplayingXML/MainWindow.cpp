#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QResizeEvent>
#include <QFileDialog>
#include <QTextEdit>

#include "../../../../graphics/pro/Fonts.h"
#include "../../../../graphics/pro/Graphics.h"
#include "../../../../fontengine/ApplicationFontsWorker.h"

#include "../../../../raster/BgraFrame.h"
#include "../../../../common/Directory.h"

#include "CMetafileTreeView.h"
#include "CTextEditDelegate.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CTextEditDelegate *pTextEditDelegate = new CTextEditDelegate(this);
    ui->treeView->setItemDelegate(pTextEditDelegate);

    QFont *pFont = new QFont;
    pFont->setPointSize(13);

    pTextEditDelegate->SetFont(pFont);
    ui->treeView->setFont(*pFont);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ReadFile()
{
    ui->treeView->SetMetafile(m_wsPathToXmlFile);

    return true;
}

void MainWindow::ConvertToRaster()
{
    if (m_wsPathToFile.empty())
        return;

    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
    pMetafile->LoadFromFile(m_wsPathToFile.c_str());
    std::wstring wsPathToRasterFile = NSFile::GetProcessDirectory() + L"/test.png";
    m_wsPathToXmlFile = NSFile::GetProcessDirectory() + L"/test.xml";

    pMetafile->ConvertToRaster(wsPathToRasterFile.c_str(), 4, ui->customView->GetWidthMetafile(),  ui->customView->GetHeightMetafile());

    pMetafile->Release();
    pFonts->Release();

    QImage oImage(QString::fromStdWString(wsPathToRasterFile));
    QGraphicsScene *pGraphicsScene = new QGraphicsScene;
    pGraphicsScene->addPixmap(QPixmap::fromImage(QImage(oImage)));
    ui->graphicsView->setScene(pGraphicsScene);
}

void MainWindow::resizeEvent(QResizeEvent *pResizeEvent)
{
    int h = pResizeEvent->size().height();
    int w = pResizeEvent->size().width();

    ui->horizontalLayoutWidget->setGeometry(QRect(0, 0, w, h));
}

void MainWindow::on_ChangeButton_clicked()
{
    m_wsPathToFile = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Metafile (*.emf *.wmf)")).toStdWString();
    ui->customView->DrawMetafile(m_wsPathToFile);
    ConvertToRaster();
    ReadFile();
}


void MainWindow::on_expandButton_clicked()
{
    if (ui->treeView->IsClear())
        return;

    if (ui->expandButton->text() == "Expand All")
    {
            ui->treeView->expandAll();
            ui->expandButton->setText("Collapse All");
    }
    else if (ui->expandButton->text() == "Collapse All")
    {
            ui->treeView->collapseAll();
            ui->expandButton->setText("Expand All");
    }
}


void MainWindow::on_ModButton_clicked()
{
    if (ui->treeView->IsClear())
        return;

    if (ui->ModButton->text() == "Use a light Mod")
    {
            ui->treeView->SetMode(true);
            ui->ModButton->setText("Use a hard Mod");
    }
    else if (ui->ModButton->text() == "Use a hard Mod")
    {
            ui->treeView->SetMode(false);
            ui->ModButton->setText("Use a light Mod");
    }}

