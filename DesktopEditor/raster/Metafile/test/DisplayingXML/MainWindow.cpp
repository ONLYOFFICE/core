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

        ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
        ui->treeView->SetMainWindow(this);

        QFont *pFont = new QFont;
        pFont->setPointSize(13);

        ui->treeView->setFont(*pFont);

        pTextEditDelegate->SetFont(pFont);
        ui->treeView->setFont(*pFont);
}

MainWindow::~MainWindow()
{
        delete ui;

        NSFile::CFileBinary oFileBinary;

        oFileBinary.Remove(L"Temp.png");
        oFileBinary.Remove(L"Temp.xml");
        oFileBinary.Remove(L"TempFile.emf");
}

bool MainWindow::ReadXmlFile(const std::wstring& wsPathToXmlFile)
{
        ui->treeView->SetMetafile(wsPathToXmlFile);

        return true;
}

bool MainWindow::ConvertToRaster(const std::wstring& wsPathToFile, bool bWithXmlFile)
{
        if (wsPathToFile.empty())
                return false;

        CApplicationFontsWorker oWorker;
        oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
        oWorker.m_bIsNeedThumbnails = false;

        if (!NSDirectory::Exists(oWorker.m_sDirectory))
                NSDirectory::CreateDirectory(oWorker.m_sDirectory);

        NSFonts::IApplicationFonts* pFonts = oWorker.Check();

        MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
        if (!pMetafile->LoadFromFile(wsPathToFile.c_str()))
                return false;

        std::wstring wsPathToRasterFile = NSFile::GetProcessDirectory() + L"/Temp.png";

        if (bWithXmlFile)
        {
                std::wstring wsPathToXmlFile    = NSFile::GetProcessDirectory() + L"/Temp.xml";
                pMetafile->ConvertToXmlAndRaster(wsPathToXmlFile.c_str(), wsPathToRasterFile.c_str(), 4, ui->customView->GetWidthMetafile(),  ui->customView->GetHeightMetafile());
                ReadXmlFile(wsPathToXmlFile);
        }
        else
        {
                pMetafile->ConvertToRaster(wsPathToRasterFile.c_str(), 4, ui->customView->GetWidthMetafile(),  ui->customView->GetHeightMetafile());
        }

        pMetafile->Release();
        pFonts->Release();

        QImage oImage(QString::fromStdWString(wsPathToRasterFile));
        QGraphicsScene *pGraphicsScene = new QGraphicsScene;
        pGraphicsScene->addPixmap(QPixmap::fromImage(QImage(oImage)));
        ui->graphicsView->setScene(pGraphicsScene);

        return true;
}

bool MainWindow::ConvertToEmf(const std::wstring &wsPathToXmlFile, const std::wstring& wsPathToNewEmfFile)
{
        if (wsPathToXmlFile.empty())
                return false;

        CApplicationFontsWorker oWorker;
        oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
        oWorker.m_bIsNeedThumbnails = false;

        if (!NSDirectory::Exists(oWorker.m_sDirectory))
                NSDirectory::CreateDirectory(oWorker.m_sDirectory);

        NSFonts::IApplicationFonts* pFonts = oWorker.Check();

        MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
        if (!pMetafile->LoadFromXmlFile(wsPathToXmlFile.c_str()))
                return false;

        std::wstring wsPathToEmfFile = (wsPathToNewEmfFile.empty())
                                        ?  NSFile::GetProcessDirectory() + L"/TempFile.emf"
                                        :  wsPathToNewEmfFile;

        pMetafile->ConvertToEmf(wsPathToEmfFile.c_str());

        pMetafile->Release();
        pFonts->Release();

        return true;
}

bool MainWindow::SaveInXmlFile(const std::wstring &wsPathToFile)
{
        return ui->treeView->SaveInXmlFile(wsPathToFile);
}

void MainWindow::DisplayingFile(const std::wstring &wsPathToFile, bool bWithXmlFile)
{
        if (!ui->customView->DrawMetafile(wsPathToFile))
        {
                Clear();
                QMessageBox::warning(this, "Warning", "Couldn't open EMF file");
                return;
        }

        if (!ConvertToRaster(wsPathToFile, bWithXmlFile))
        {
                Clear();
                QMessageBox::warning(this, "Warning", "Couldn't open EMF file");
                return;
        }
}

void MainWindow::Clear()
{
        ui->actionStatistics->setEnabled(false);
        ui->actionSave_XML_as->setEnabled(false);
        ui->customView->Clear();

        QGraphicsScene *pGVScene = ui->graphicsView->scene();
        if (NULL != pGVScene)
        {
                delete pGVScene;
                ui->graphicsView->setScene(NULL);
        }

        ui->treeView->ClearTree();
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
        if (ui->treeView->IsClearTree())
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
        if (ui->treeView->IsClearTree())
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
        std::wstring wsPathToFile = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Metafile (*.emf *.wmf)")).toStdWString();

        if (wsPathToFile.empty())
                return;

        DisplayingFile(wsPathToFile);

        ui->actionStatistics->setEnabled(true);
        ui->actionSave_XML_as->setEnabled(true);
        ui->actionSave_EMF_as->setEnabled(true);
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
        SaveInXmlFile(sSaveFilePath.toStdWString());
}

void MainWindow::on_actionSave_EMF_as_triggered()
{
        QString qsSaveFilePath = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("EMF file (*.emf)"));

        ConvertToEmf(NSFile::GetProcessDirectory() + L"/Temp.xml", qsSaveFilePath.toStdWString());
}

