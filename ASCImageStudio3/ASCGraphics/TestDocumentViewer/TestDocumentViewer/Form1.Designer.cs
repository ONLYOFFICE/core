namespace WindowsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.toolMenu = new System.Windows.Forms.ToolStrip();
            this.toolBottonOpen = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolButtonPrevPage = new System.Windows.Forms.ToolStripButton();
            this.toolButtonNextPage = new System.Windows.Forms.ToolStripButton();
            this.toolTextCurPage = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolTextPagesCount = new System.Windows.Forms.ToolStripTextBox();
            this.toolComboPages = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolComboViewMode = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.cont_one = new System.Windows.Forms.ToolStripButton();
            this.break_one = new System.Windows.Forms.ToolStripButton();
            this.cont_two = new System.Windows.Forms.ToolStripButton();
            this.break_two = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripWater = new System.Windows.Forms.ToolStripButton();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panel1 = new System.Windows.Forms.Panel();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.labelZoom2 = new System.Windows.Forms.Label();
            this.hScrollBar2 = new System.Windows.Forms.HScrollBar();
            this.vScrollBar2 = new System.Windows.Forms.VScrollBar();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.hScrollBar1 = new System.Windows.Forms.HScrollBar();
            this.labelZoom = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.axCAVSThumbnailsViewer1 = new AxAVSGraphics.AxCAVSThumbnailsViewer();
            this.axCAVSDocumentViewer1 = new AxAVSGraphics.AxCAVSDocumentViewer();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonEpub = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonFb2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonMobi = new System.Windows.Forms.ToolStripButton();
            this.toolMenu.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.axCAVSThumbnailsViewer1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.axCAVSDocumentViewer1)).BeginInit();
            this.SuspendLayout();
            // 
            // toolMenu
            // 
            this.toolMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolBottonOpen,
            this.toolStripSeparator1,
            this.toolButtonPrevPage,
            this.toolButtonNextPage,
            this.toolTextCurPage,
            this.toolStripLabel1,
            this.toolTextPagesCount,
            this.toolComboPages,
            this.toolStripSeparator2,
            this.toolComboViewMode,
            this.toolStripSeparator3,
            this.cont_one,
            this.break_one,
            this.cont_two,
            this.break_two,
            this.toolStripSeparator4,
            this.toolStripWater,
            this.toolStripSeparator5,
            this.toolStripSeparator6,
            this.toolStripSeparator7,
            this.toolStripButtonEpub,
            this.toolStripButtonFb2,
            this.toolStripButtonMobi});
            this.toolMenu.Location = new System.Drawing.Point(0, 0);
            this.toolMenu.Name = "toolMenu";
            this.toolMenu.Size = new System.Drawing.Size(949, 25);
            this.toolMenu.TabIndex = 1;
            this.toolMenu.Text = "toolStrip1";
            // 
            // toolBottonOpen
            // 
            this.toolBottonOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolBottonOpen.Image = global::WindowsApplication1.Properties.Resources.open;
            this.toolBottonOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolBottonOpen.Name = "toolBottonOpen";
            this.toolBottonOpen.Size = new System.Drawing.Size(23, 22);
            this.toolBottonOpen.Text = "Open...";
            this.toolBottonOpen.Click += new System.EventHandler(this.toolBottonOpen_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolButtonPrevPage
            // 
            this.toolButtonPrevPage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolButtonPrevPage.Image = global::WindowsApplication1.Properties.Resources.prev;
            this.toolButtonPrevPage.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolButtonPrevPage.Name = "toolButtonPrevPage";
            this.toolButtonPrevPage.Size = new System.Drawing.Size(23, 22);
            this.toolButtonPrevPage.Text = "toolStripButton1";
            this.toolButtonPrevPage.Click += new System.EventHandler(this.toolButtonPrevPage_Click);
            // 
            // toolButtonNextPage
            // 
            this.toolButtonNextPage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolButtonNextPage.Image = global::WindowsApplication1.Properties.Resources.next;
            this.toolButtonNextPage.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolButtonNextPage.Name = "toolButtonNextPage";
            this.toolButtonNextPage.Size = new System.Drawing.Size(23, 22);
            this.toolButtonNextPage.Text = "toolStripButton1";
            this.toolButtonNextPage.Click += new System.EventHandler(this.toolButtonNextPage_Click);
            // 
            // toolTextCurPage
            // 
            this.toolTextCurPage.Name = "toolTextCurPage";
            this.toolTextCurPage.Size = new System.Drawing.Size(30, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(13, 22);
            this.toolStripLabel1.Text = "/";
            // 
            // toolTextPagesCount
            // 
            this.toolTextPagesCount.Name = "toolTextPagesCount";
            this.toolTextPagesCount.Size = new System.Drawing.Size(30, 25);
            // 
            // toolComboPages
            // 
            this.toolComboPages.Name = "toolComboPages";
            this.toolComboPages.Size = new System.Drawing.Size(75, 25);
            this.toolComboPages.SelectedIndexChanged += new System.EventHandler(this.toolComboPages_SelectedIndexChanged);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolComboViewMode
            // 
            this.toolComboViewMode.Items.AddRange(new object[] {
            "Custom",
            "Fit Width",
            "Fit Height",
            "Fit Page"});
            this.toolComboViewMode.Name = "toolComboViewMode";
            this.toolComboViewMode.Size = new System.Drawing.Size(121, 25);
            this.toolComboViewMode.SelectedIndexChanged += new System.EventHandler(this.toolComboViewMode_SelectedIndexChanged);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // cont_one
            // 
            this.cont_one.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.cont_one.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cont_one.Image = global::WindowsApplication1.Properties.Resources.cont_one;
            this.cont_one.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cont_one.Name = "cont_one";
            this.cont_one.Size = new System.Drawing.Size(23, 22);
            this.cont_one.Text = "toolStripButton1";
            this.cont_one.Click += new System.EventHandler(this.cont_one_Click);
            // 
            // break_one
            // 
            this.break_one.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.break_one.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.break_one.Image = global::WindowsApplication1.Properties.Resources.break_one;
            this.break_one.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.break_one.Name = "break_one";
            this.break_one.Size = new System.Drawing.Size(23, 22);
            this.break_one.Text = "toolStripButton1";
            this.break_one.Click += new System.EventHandler(this.break_one_Click);
            // 
            // cont_two
            // 
            this.cont_two.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.cont_two.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cont_two.Image = global::WindowsApplication1.Properties.Resources.cont_two;
            this.cont_two.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cont_two.Name = "cont_two";
            this.cont_two.Size = new System.Drawing.Size(23, 22);
            this.cont_two.Text = "toolStripButton1";
            this.cont_two.Click += new System.EventHandler(this.cont_two_Click);
            // 
            // break_two
            // 
            this.break_two.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.break_two.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.break_two.Image = global::WindowsApplication1.Properties.Resources.break_two;
            this.break_two.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.break_two.Name = "break_two";
            this.break_two.Size = new System.Drawing.Size(23, 22);
            this.break_two.Text = "toolStripButton1";
            this.break_two.Click += new System.EventHandler(this.break_two_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripWater
            // 
            this.toolStripWater.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripWater.Image = ((System.Drawing.Image)(resources.GetObject("toolStripWater.Image")));
            this.toolStripWater.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripWater.Name = "toolStripWater";
            this.toolStripWater.Size = new System.Drawing.Size(76, 22);
            this.toolStripWater.Text = "Watermark";
            this.toolStripWater.Click += new System.EventHandler(this.toolStripWater_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.panel1);
            this.splitContainer1.Panel1.Controls.Add(this.trackBar2);
            this.splitContainer1.Panel1.Controls.Add(this.labelZoom2);
            this.splitContainer1.Panel1.Controls.Add(this.hScrollBar2);
            this.splitContainer1.Panel1.Controls.Add(this.vScrollBar2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
            this.splitContainer1.Panel2.Controls.Add(this.axCAVSDocumentViewer1);
            this.splitContainer1.Panel2.Controls.Add(this.progressBar1);
            this.splitContainer1.Panel2.Controls.Add(this.vScrollBar1);
            this.splitContainer1.Panel2.Controls.Add(this.hScrollBar1);
            this.splitContainer1.Panel2.Controls.Add(this.labelZoom);
            this.splitContainer1.Panel2.Controls.Add(this.trackBar1);
            this.splitContainer1.Size = new System.Drawing.Size(949, 575);
            this.splitContainer1.SplitterDistance = 173;
            this.splitContainer1.TabIndex = 2;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.panel1.Controls.Add(this.axCAVSThumbnailsViewer1);
            this.panel1.Location = new System.Drawing.Point(3, 26);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(145, 475);
            this.panel1.TabIndex = 18;
            // 
            // trackBar2
            // 
            this.trackBar2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.trackBar2.AutoSize = false;
            this.trackBar2.Location = new System.Drawing.Point(12, 544);
            this.trackBar2.Maximum = 500;
            this.trackBar2.Minimum = 10;
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(139, 26);
            this.trackBar2.TabIndex = 17;
            this.trackBar2.Value = 100;
            this.trackBar2.ValueChanged += new System.EventHandler(this.trackBar2_ValueChanged);
            // 
            // labelZoom2
            // 
            this.labelZoom2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.labelZoom2.AutoSize = true;
            this.labelZoom2.Location = new System.Drawing.Point(12, 528);
            this.labelZoom2.Name = "labelZoom2";
            this.labelZoom2.Size = new System.Drawing.Size(72, 13);
            this.labelZoom2.TabIndex = 16;
            this.labelZoom2.Text = "Zoom = 100%";
            // 
            // hScrollBar2
            // 
            this.hScrollBar2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.hScrollBar2.Location = new System.Drawing.Point(12, 504);
            this.hScrollBar2.Name = "hScrollBar2";
            this.hScrollBar2.Size = new System.Drawing.Size(139, 20);
            this.hScrollBar2.TabIndex = 15;
            this.hScrollBar2.ValueChanged += new System.EventHandler(this.hScrollBar2_ValueChanged);
            // 
            // vScrollBar2
            // 
            this.vScrollBar2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.vScrollBar2.Location = new System.Drawing.Point(151, 26);
            this.vScrollBar2.Name = "vScrollBar2";
            this.vScrollBar2.Size = new System.Drawing.Size(19, 478);
            this.vScrollBar2.TabIndex = 14;
            this.vScrollBar2.ValueChanged += new System.EventHandler(this.vScrollBar2_ValueChanged);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Location = new System.Drawing.Point(3, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(757, 19);
            this.tabControl1.TabIndex = 16;
            this.tabControl1.SelectedIndexChanged += new System.EventHandler(this.tabControl1_SelectedIndexChanged);
            // 
            // progressBar1
            // 
            this.progressBar1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar1.Location = new System.Drawing.Point(576, 529);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(187, 23);
            this.progressBar1.TabIndex = 14;
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.vScrollBar1.Location = new System.Drawing.Point(744, 26);
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(19, 478);
            this.vScrollBar1.TabIndex = 13;
            this.vScrollBar1.ValueChanged += new System.EventHandler(this.vScrollBar1_ValueChanged);
            // 
            // hScrollBar1
            // 
            this.hScrollBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.hScrollBar1.Location = new System.Drawing.Point(3, 504);
            this.hScrollBar1.Name = "hScrollBar1";
            this.hScrollBar1.Size = new System.Drawing.Size(738, 20);
            this.hScrollBar1.TabIndex = 12;
            this.hScrollBar1.ValueChanged += new System.EventHandler(this.hScrollBar1_ValueChanged);
            // 
            // labelZoom
            // 
            this.labelZoom.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.labelZoom.AutoSize = true;
            this.labelZoom.Location = new System.Drawing.Point(6, 533);
            this.labelZoom.Name = "labelZoom";
            this.labelZoom.Size = new System.Drawing.Size(72, 13);
            this.labelZoom.TabIndex = 11;
            this.labelZoom.Text = "Zoom = 100%";
            // 
            // trackBar1
            // 
            this.trackBar1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.trackBar1.AutoSize = false;
            this.trackBar1.Location = new System.Drawing.Point(97, 526);
            this.trackBar1.Maximum = 1000;
            this.trackBar1.Minimum = 10;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(158, 26);
            this.trackBar1.TabIndex = 10;
            this.trackBar1.Value = 100;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // axCAVSThumbnailsViewer1
            // 
            this.axCAVSThumbnailsViewer1.Enabled = true;
            this.axCAVSThumbnailsViewer1.Location = new System.Drawing.Point(44, 113);
            this.axCAVSThumbnailsViewer1.Name = "axCAVSThumbnailsViewer1";
            this.axCAVSThumbnailsViewer1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axCAVSThumbnailsViewer1.OcxState")));
            this.axCAVSThumbnailsViewer1.Size = new System.Drawing.Size(192, 192);
            this.axCAVSThumbnailsViewer1.TabIndex = 0;
            // 
            // axCAVSDocumentViewer1
            // 
            this.axCAVSDocumentViewer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.axCAVSDocumentViewer1.Enabled = true;
            this.axCAVSDocumentViewer1.Location = new System.Drawing.Point(3, 26);
            this.axCAVSDocumentViewer1.Name = "axCAVSDocumentViewer1";
            this.axCAVSDocumentViewer1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axCAVSDocumentViewer1.OcxState")));
            this.axCAVSDocumentViewer1.Size = new System.Drawing.Size(738, 475);
            this.axCAVSDocumentViewer1.TabIndex = 15;
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonEpub
            // 
            this.toolStripButtonEpub.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonEpub.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonEpub.Image")));
            this.toolStripButtonEpub.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonEpub.Name = "toolStripButtonEpub";
            this.toolStripButtonEpub.Size = new System.Drawing.Size(59, 22);
            this.toolStripButtonEpub.Text = "To Epup";
            this.toolStripButtonEpub.Click += new System.EventHandler(this.toolStripButtonEpub_Click);
            // 
            // toolStripButtonFb2
            // 
            this.toolStripButtonFb2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonFb2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonFb2.Image")));
            this.toolStripButtonFb2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonFb2.Name = "toolStripButtonFb2";
            this.toolStripButtonFb2.Size = new System.Drawing.Size(52, 22);
            this.toolStripButtonFb2.Text = "To Fb2";
            this.toolStripButtonFb2.Click += new System.EventHandler(this.toolStripButtonFb2_Click);
            // 
            // toolStripButtonMobi
            // 
            this.toolStripButtonMobi.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonMobi.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonMobi.Image")));
            this.toolStripButtonMobi.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonMobi.Name = "toolStripButtonMobi";
            this.toolStripButtonMobi.Size = new System.Drawing.Size(58, 22);
            this.toolStripButtonMobi.Text = "To Mobi";
            this.toolStripButtonMobi.Click += new System.EventHandler(this.toolStripButtonMobi_Click);
            // 
            // Form1
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(949, 600);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.toolMenu);
            this.Name = "Form1";
            this.Text = "DocumentViewer - NoDocument";
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.Form1_DragDrop);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.Form1_DragEnter);
            this.toolMenu.ResumeLayout(false);
            this.toolMenu.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.axCAVSThumbnailsViewer1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.axCAVSDocumentViewer1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolMenu;
        private System.Windows.Forms.ToolStripButton toolBottonOpen;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolButtonPrevPage;
        private System.Windows.Forms.ToolStripButton toolButtonNextPage;
        private System.Windows.Forms.ToolStripComboBox toolComboPages;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripComboBox toolComboViewMode;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ToolStripTextBox toolTextCurPage;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripTextBox toolTextPagesCount;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton cont_one;
        private System.Windows.Forms.ToolStripButton break_one;
        private System.Windows.Forms.ToolStripButton cont_two;
        private System.Windows.Forms.ToolStripButton break_two;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabControl1;
        private AxAVSGraphics.AxCAVSDocumentViewer axCAVSDocumentViewer1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.VScrollBar vScrollBar1;
        private System.Windows.Forms.HScrollBar hScrollBar1;
        private System.Windows.Forms.Label labelZoom;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.Label labelZoom2;
        private System.Windows.Forms.HScrollBar hScrollBar2;
        private System.Windows.Forms.VScrollBar vScrollBar2;
        private System.Windows.Forms.Panel panel1;
        private AxAVSGraphics.AxCAVSThumbnailsViewer axCAVSThumbnailsViewer1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton toolStripWater;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripButton toolStripButtonEpub;
        private System.Windows.Forms.ToolStripButton toolStripButtonFb2;
        private System.Windows.Forms.ToolStripButton toolStripButtonMobi;
    }
}

