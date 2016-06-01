//#define NET_DLL

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace test
{
    public partial class MainForm : Form
    {
        public TabControl m_oTabControl = new TabControl();

        public MainForm()
        {
            InitializeComponent();

            this.Text = "Document Builder";
                        
            m_oTabControl.SetBounds(0, 0, this.ClientSize.Width, this.ClientSize.Height - 50);
            m_oTabControl.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.Controls.Add(m_oTabControl);

            AddTab("builder.SetTmpFolder(\"D:/BuilderTest\");\r\n" +
"#builder.CreateFile(\"docx\");\r\n" +
"builder.OpenFile(\"D:/TESTFILES/images.docx\", \"\");\r\n" +
"builder.SaveFile(\"pdf\", \"D:/TESTFILES/images.pdf\");\r\n" + 
"builder.CloseFile();");

            int nButtonsHeight = 30;
            int nButtonsTop = this.ClientSize.Height - ((50 + nButtonsHeight) / 2);
            int nButtonsWidth = 100;
            int nButtonsBetween = 10;
            int nButtonsRight = 10;
                        
            Button _buttonNew = new Button();
            _buttonNew.SetBounds(this.ClientSize.Width - nButtonsRight - 2 * nButtonsWidth - nButtonsBetween, nButtonsTop, nButtonsWidth, nButtonsHeight);
            _buttonNew.Anchor = AnchorStyles.Right | AnchorStyles.Bottom;
            _buttonNew.Text = "New Test";
            _buttonNew.Click += _buttonNew_Click;
            this.Controls.Add(_buttonNew);

            Button _buttonRun = new Button();
            _buttonRun.SetBounds(this.ClientSize.Width - nButtonsRight - nButtonsWidth, nButtonsTop, nButtonsWidth, nButtonsHeight);
            _buttonRun.Anchor = AnchorStyles.Right | AnchorStyles.Bottom;
            _buttonRun.Text = "Run";
            _buttonRun.Click += _buttonRun_Click;
            _buttonRun.BackColor = Color.Green;
            _buttonRun.ForeColor = Color.White;
            this.Controls.Add(_buttonRun);

#if (NET_DLL)
            docbuilder_net.CDocBuilder.Initialize();
#else
            ascdocbuilder.IASCDocBuilder oBuilder = new ascdocbuilder.CASCDocBuilder();
            oBuilder.Initialize();            
#endif

            this.Disposed += MainForm_Disposed;
        }

        void MainForm_Disposed(object sender, EventArgs e)
        {
#if (NET_DLL)
            docbuilder_net.CDocBuilder.Destroy();
#else
            ascdocbuilder.IASCDocBuilder oBuilder = new ascdocbuilder.CASCDocBuilder();
            oBuilder.Dispose();
#endif
        }

        void _buttonRun_Click(object sender, EventArgs e)
        {
#if (NET_DLL)
            docbuilder_net.CDocBuilder oBuilder = new docbuilder_net.CDocBuilder(true);
            oBuilder.RunText(this.m_oTabControl.SelectedTab.Controls[0].Text);
            oBuilder.Dispose();
#else
            ascdocbuilder.IASCDocBuilder oBuilder = new ascdocbuilder.CASCDocBuilder();
            oBuilder.CreateInstance(true);
            oBuilder.RunText(this.m_oTabControl.SelectedTab.Controls[0].Text);
            System.Runtime.InteropServices.Marshal.ReleaseComObject(oBuilder);
#endif
        }

        void _buttonNew_Click(object sender, EventArgs e)
        {
            AddTab("");
        }

        private void AddTab(string _code)
        {
            TabPage _page = new TabPage("Test №" + Convert.ToString(m_oTabControl.TabCount + 1));

            TextBox _text = new TextBox();
            _text.Multiline = true;
            _text.Dock = DockStyle.Fill;
            _text.Text = _code;
            _text.Font = new System.Drawing.Font(_text.Font.Name, 12);
            _page.Controls.Add(_text);

            m_oTabControl.Controls.Add(_page);
            m_oTabControl.Controls[m_oTabControl.TabCount - 1].Name = "Test №" + Convert.ToString(m_oTabControl.TabCount);

            this.OnResize(EventArgs.Empty);
        }

    }
}
