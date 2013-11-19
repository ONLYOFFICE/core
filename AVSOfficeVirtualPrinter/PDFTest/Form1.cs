using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PDFTest
{
    public partial class MainForm : Form
    {
        readonly AVSOfficeVirtualPrinter.CVirtualPrinterClass m_Printer   = new AVSOfficeVirtualPrinter.CVirtualPrinterClass();
        readonly AVSOfficeImageFile.CImageFile m_ImageFile = new AVSOfficeImageFile.CImageFile();
        readonly AVSOfficePDFFile.CPDFFile m_PDFFile = new AVSOfficePDFFile.CPDFFile();

        CompleteForm formComplete = new CompleteForm();
        ConvertingForm formConvert = new ConvertingForm();

        int m_nConvertingStatus = 0;
        //bool m_bHTML = false;

        public MainForm()
        {
            InitializeComponent();
            this.m_Printer.StartConverting += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_StartConvertingEventHandler(m_Printer_StartConverting);
            this.m_Printer.EndConverting += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_EndConvertingEventHandler(m_Printer_EndConverting);
            this.m_Printer.HTMLFile += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_HTMLFileEventHandler(m_Printer_HTMLFile);
            this.m_Printer.OpenHTMLinIE += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_OpenHTMLinIEEventHandler(m_Printer_OpenHTMLinIE);
            this.m_Printer.PreparingDataForPrinting += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_PreparingDataForPrintingEventHandler(m_Printer_PreparingDataForPrinting);
            this.m_Printer.Printing += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_PrintingEventHandler(m_Printer_Printing);
            this.m_Printer.Cancel += new AVSOfficeVirtualPrinter._IVirtualPrinterEvents_CancelEventHandler(m_Printer_Cancel);
            
            saveFileDialog1.FileName = "";
            openFileDialog1.FileName = "";
            saveFileDialog1.Filter = "Portable Document Format (*.pdf)|*.pdf|Portable Network Graphics (*.png)|*.png|(*.jpg)|*.jpg";
            openFileDialog1.Filter = "Post Scripts (*.ps)|*.ps|HTML files (*.htm);(*.html)|*.htm;*.html|All files (*.*)|*.*";
            m_nConvertingStatus = 0;
       }

        void m_Printer_Cancel()
        {
            m_nConvertingStatus = 31;
            //throw new NotImplementedException();
        }

        void m_Printer_Printing()
        {
            m_nConvertingStatus = 15;
            //throw new NotImplementedException();
        }

        void m_Printer_PreparingDataForPrinting()
        {
            m_nConvertingStatus = 14;
            //throw new NotImplementedException();
        }

        void m_Printer_OpenHTMLinIE()
        {
            m_nConvertingStatus = 13;
            //throw new NotImplementedException();
        }

        void m_Printer_HTMLFile()
        {
            //m_bHTML = true;
            //throw new NotImplementedException();
        }

        void m_Printer_StartConverting()
        {
            m_nConvertingStatus = 10;
            //throw new NotImplementedException();
        }

        void m_Printer_EndConverting()
        {
            m_nConvertingStatus = 11;
            //throw new NotImplementedException();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            saveFileDialog1.FileName = ""; 
            
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            string sDstPath = saveFileDialog1.FileName;
            int nFormat = saveFileDialog1.FilterIndex - 1;
            string sSrcPath = openFileDialog1.FileName;

            m_Printer.PsTo(sSrcPath, sDstPath, nFormat);
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            m_Printer.InstallPrinter();
            formComplete.ShowDialog();
        }

        private void btUnInstallPrinter_Click(object sender, EventArgs e)
        {
            m_Printer.UnInstallPrinter();
            formComplete.ShowDialog();
        }

        private void btPrint_Click(object sender, EventArgs e)
        {
            
            m_nConvertingStatus = 0;
            //m_bHTML = false;
            saveFileDialog1.FileName = "";

            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            string sDstPath = saveFileDialog1.FileName;
            string sSrcPath = openFileDialog1.FileName;

            Refresh();
            int nFormat = saveFileDialog1.FilterIndex - 1;

            m_Printer.SaveAs(sSrcPath, sDstPath, nFormat);
            
        }


        private void timer_Tick(object sender, EventArgs e)
        {
            uint nStatus = 0;
            m_ImageFile.GetStatus(out nStatus);

            if (nStatus != 0 && nStatus != 12)
                m_nConvertingStatus = (int)nStatus;

            m_PDFFile.GetStatus(out nStatus);
            if (nStatus != 0 && nStatus != 12)
                m_nConvertingStatus = (int)nStatus;

            switch (m_nConvertingStatus)
            {
                default:
                    formConvert.labelEvents.Text = "";
                    break;
                case 10:
                    formConvert.labelEvents.Text = "Начало конвертации";
                    formConvert.btCancel.Enabled = true;
                    formConvert.btFinish.Enabled = false;

                    if (!formConvert.Visible)
                        formConvert.ShowDialog();
                    break;
                case 11:
                    formConvert.progressBar.Style = ProgressBarStyle.Blocks;
                    formConvert.progressBar.Value = formConvert.progressBar.Maximum;
                    formConvert.labelEvents.Text = "Конвертация завершена";
                    formConvert.btCancel.Enabled = false;
                    formConvert.btFinish.Enabled = true;
                    break;
                case 13:
                    formConvert.labelEvents.Text = "Открываем HTML в IE";
                    break;
                case 14:
                    formConvert.labelEvents.Text = "Подготовка к печати";
                    break;
                case 15:
                    formConvert.labelEvents.Text = "Печать";
                    formConvert.btCancel.Enabled = false;
                    break;
                case 31:
                    formConvert.progressBar.Style = ProgressBarStyle.Blocks;
                    formConvert.progressBar.Value = formConvert.progressBar.Minimum;
                    formConvert.labelEvents.Text = "Конвертация отменена";
                    formConvert.btCancel.Enabled = false;
                    formConvert.btFinish.Enabled = true;
                    formConvert.btFinish.Text = "Закрыть";
                    if (!formConvert.Visible)
                        formConvert.ShowDialog();
                    m_nConvertingStatus = 0;
                    break;
            };

            if (formConvert.m_bCancelConverting)
                m_Printer.CancelConverting();

        }

        private void btSimplePrint_Click(object sender, EventArgs e)
        {
            saveFileDialog1.FileName = "";

            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            string sDstPath = saveFileDialog1.FileName;
            string sSrcPath = openFileDialog1.FileName;

            Refresh();
            int nFormat = saveFileDialog1.FilterIndex - 1;

            if ( 0 != nFormat )
                m_ImageFile.SaveToFile(sDstPath, sSrcPath, null);
            else 
                m_PDFFile.SaveToFile(sDstPath, sSrcPath, null);

        }
    }
}
