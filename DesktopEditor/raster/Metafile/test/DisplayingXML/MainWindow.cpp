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

#include "CStatisticsWidget.h"

#include "CMetafileTreeView.h"
#include "CTextEditDelegate.h"

#include <QStandardItemModel>
#include <QMessageBox>

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

bool MainWindow::ConvertToRaster()
{
    if (m_wsPathToFile.empty())
        return false;

    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
    if (!pMetafile->LoadFromFile(m_wsPathToFile.c_str()))
        return false;

    std::wstring wsPathToRasterFile = NSFile::GetProcessDirectory() + L"/test.png";
    std::wstring wsPathToXmlFile    = NSFile::GetProcessDirectory() + L"/test.xml";

    m_wsPathToXmlFile = NSFile::GetProcessDirectory() + L"/test.xml";

    pMetafile->ConvertToXmlAndRaster(wsPathToXmlFile.c_str(), wsPathToRasterFile.c_str(), 4, ui->customView->GetWidthMetafile(),  ui->customView->GetHeightMetafile());

    pMetafile->Release();
    pFonts->Release();

    QImage oImage(QString::fromStdWString(wsPathToRasterFile));
    QGraphicsScene *pGraphicsScene = new QGraphicsScene;
    pGraphicsScene->addPixmap(QPixmap::fromImage(QImage(oImage)));
    ui->graphicsView->setScene(pGraphicsScene);

    return true;
}

void MainWindow::Clear()
{
    m_wsPathToFile.clear();
    m_wsPathToXmlFile.clear();

    ui->actionStatistics->setEnabled(false);
    ui->customView->Clear();

    QGraphicsScene *pGVScene = ui->graphicsView->scene();
    if (NULL != pGVScene)
    {
        delete pGVScene;
        ui->graphicsView->setScene(NULL);
    }

    ui->treeView->Clear();
}

void MainWindow::resizeEvent(QResizeEvent *pResizeEvent)
{
    unsigned int unManubarHeight        = ui->menubar->height();
    unsigned int unWorkingPartHeight    = pResizeEvent->size().height();
    unsigned int unWorkingPartWidth     = pResizeEvent->size().width();

    ui->horizontalLayoutWidget->setGeometry(QRect(0, 0, unWorkingPartWidth, unWorkingPartHeight - unManubarHeight));
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
    }
}



void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionChange_File_triggered()
{
    m_wsPathToFile = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Metafile (*.emf *.wmf)")).toStdWString();

    if (m_wsPathToFile.empty())
        return;

    if (!ui->customView->DrawMetafile(m_wsPathToFile))
    {
        Clear();
        QMessageBox::warning(this, "Warning", "Couldn't open EMF file");
        return;
    }

    if (!ConvertToRaster())
    {
        Clear();
        QMessageBox::warning(this, "Warning", "Couldn't open EMF file");
        return;
    }

    ReadFile();

    ui->actionStatistics->setEnabled(true);
    ui->actionSave_XML_as->setEnabled(true);
}


void MainWindow::on_actionStatistics_triggered()
{
    CStatisticsWidget *pStaticsWidget = new CStatisticsWidget;
    pStaticsWidget->SetMainWindow(this);
    pStaticsWidget->SetStatistics(ui->treeView->GetStatistics());
    pStaticsWidget->show();
}


void MainWindow::on_actionSave_XML_as_triggered()
{
        QString sSaveFilePath = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("XML file (*.xml)"));
        ui->treeView->SaveInFile(sSaveFilePath);
}

