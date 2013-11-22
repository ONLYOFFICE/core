namespace TestViewManager
{
    partial class MainForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.m_vScrollBar = new System.Windows.Forms.VScrollBar();
            this.m_hScrollBar = new System.Windows.Forms.HScrollBar();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.openEmptyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openSpaceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openBadFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.m_oOpenImageFile = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.m_oZoomToWindow = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.m_oZoomButtonOut = new System.Windows.Forms.ToolStripButton();
            this.m_oZoomItems = new System.Windows.Forms.ToolStripComboBox();
            this.m_oZoomButtonIn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.m_oGridEnableButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.m_oRullersButton = new System.Windows.Forms.ToolStripButton();
            this.m_oComboBoxRullersType = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.ColorBackground = new System.Windows.Forms.ToolStripMenuItem();
            this.FullChess = new System.Windows.Forms.ToolStripMenuItem();
            this.ChessOnlyImage = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.m_oFastWindow = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.bBeginUpdate = new System.Windows.Forms.ToolStripButton();
            this.bEndpdate = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.m_oZoomTackBar = new System.Windows.Forms.TrackBar();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.m_oZoomStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_oImageSizesLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_oPreviewSizesLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_oScrollCenterLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_oControlSize = new System.Windows.Forms.ToolStripStatusLabel();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.m_oImageEditor = new AxAVSImageEditor.AxAVSImageEditor();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.UndoButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.UndoListBox = new System.Windows.Forms.ListBox();
            this.RedoButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.RedoListBox = new System.Windows.Forms.ListBox();
            this.UndoReset = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.controlBrightnessContrastProperties1 = new TestViewManager.ControlBrightnessContrastProperties();
            this.controlBrushProperties1 = new TestViewManager.ControlBrushProperties();
            this.controlCropProperties1 = new TestViewManager.CommandParams.ControlCropProperties();
            this.controlLevelsProperties1 = new TestViewManager.ControlLevelsProperties();
            this.controlToneProperties1 = new TestViewManager.ControlToneProperties();
            this.controlXMLProperties1 = new TestViewManager.ControlXMLProperties();
            this.label1 = new System.Windows.Forms.Label();
            this.CommandChoise_comboBox = new System.Windows.Forms.ComboBox();
            this.StartCommandBtn = new System.Windows.Forms.Button();
            this.StopCommandBtn = new System.Windows.Forms.Button();
            this.Size_label = new System.Windows.Forms.Label();
            this.Size_trackBar = new System.Windows.Forms.TrackBar();
            this.Type_label = new System.Windows.Forms.Label();
            this.Type_comboBox = new System.Windows.Forms.ComboBox();
            this.TimerSliderH = new System.Windows.Forms.Timer(this.components);
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_oZoomTackBar)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_oImageEditor)).BeginInit();
            this.tableLayoutPanel3.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Size_trackBar)).BeginInit();
            this.SuspendLayout();
            // 
            // m_vScrollBar
            // 
            this.m_vScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_vScrollBar.Location = new System.Drawing.Point(594, 0);
            this.m_vScrollBar.Name = "m_vScrollBar";
            this.m_vScrollBar.Size = new System.Drawing.Size(15, 449);
            this.m_vScrollBar.TabIndex = 1;
            this.m_vScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.VScrollBar_Scroll);
            // 
            // m_hScrollBar
            // 
            this.m_hScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_hScrollBar.Location = new System.Drawing.Point(0, 450);
            this.m_hScrollBar.Name = "m_hScrollBar";
            this.m_hScrollBar.Size = new System.Drawing.Size(594, 11);
            this.m_hScrollBar.TabIndex = 2;
            this.m_hScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.HScrollBar_Scroll);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(989, 24);
            this.menuStrip1.TabIndex = 3;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem,
            this.toolStripSeparator5,
            this.openEmptyToolStripMenuItem,
            this.openSpaceToolStripMenuItem,
            this.openBadFileToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openToolStripMenuItem.Text = "Open...";
            this.openToolStripMenuItem.ToolTipText = "Open...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(149, 6);
            // 
            // openEmptyToolStripMenuItem
            // 
            this.openEmptyToolStripMenuItem.Name = "openEmptyToolStripMenuItem";
            this.openEmptyToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openEmptyToolStripMenuItem.Text = "Open(\"\")";
            this.openEmptyToolStripMenuItem.Click += new System.EventHandler(this.openEmptyToolStripMenuItem_Click);
            // 
            // openSpaceToolStripMenuItem
            // 
            this.openSpaceToolStripMenuItem.Name = "openSpaceToolStripMenuItem";
            this.openSpaceToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openSpaceToolStripMenuItem.Text = "Open(\" \")";
            this.openSpaceToolStripMenuItem.Click += new System.EventHandler(this.openSpaceToolStripMenuItem_Click);
            // 
            // openBadFileToolStripMenuItem
            // 
            this.openBadFileToolStripMenuItem.Name = "openBadFileToolStripMenuItem";
            this.openBadFileToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openBadFileToolStripMenuItem.Text = "Open(\"asdfas\")";
            this.openBadFileToolStripMenuItem.Click += new System.EventHandler(this.openBadFileToolStripMenuItem_Click);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_oOpenImageFile,
            this.toolStripSeparator3,
            this.m_oZoomToWindow,
            this.toolStripSeparator1,
            this.m_oZoomButtonOut,
            this.m_oZoomItems,
            this.m_oZoomButtonIn,
            this.toolStripSeparator2,
            this.m_oGridEnableButton,
            this.toolStripSeparator4,
            this.m_oRullersButton,
            this.m_oComboBoxRullersType,
            this.toolStripSeparator6,
            this.toolStripDropDownButton1,
            this.toolStripSeparator7,
            this.m_oFastWindow,
            this.toolStripSeparator8,
            this.bBeginUpdate,
            this.bEndpdate,
            this.toolStripSeparator9});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.toolStrip1.Size = new System.Drawing.Size(989, 25);
            this.toolStrip1.TabIndex = 4;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // m_oOpenImageFile
            // 
            this.m_oOpenImageFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_oOpenImageFile.Image = global::TestViewManager.Properties.Resources.MenuFileOpenIcon;
            this.m_oOpenImageFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oOpenImageFile.Name = "m_oOpenImageFile";
            this.m_oOpenImageFile.Size = new System.Drawing.Size(23, 22);
            this.m_oOpenImageFile.Text = "Open Image File";
            this.m_oOpenImageFile.Click += new System.EventHandler(this.OpenImageFile_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // m_oZoomToWindow
            // 
            this.m_oZoomToWindow.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.m_oZoomToWindow.Image = ((System.Drawing.Image)(resources.GetObject("m_oZoomToWindow.Image")));
            this.m_oZoomToWindow.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oZoomToWindow.Name = "m_oZoomToWindow";
            this.m_oZoomToWindow.Size = new System.Drawing.Size(93, 22);
            this.m_oZoomToWindow.Text = "Zoom To Window";
            this.m_oZoomToWindow.Click += new System.EventHandler(this.ZoomToWindow_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // m_oZoomButtonOut
            // 
            this.m_oZoomButtonOut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_oZoomButtonOut.Image = global::TestViewManager.Properties.Resources.MenuViewZoomOutIcon;
            this.m_oZoomButtonOut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oZoomButtonOut.Name = "m_oZoomButtonOut";
            this.m_oZoomButtonOut.Size = new System.Drawing.Size(23, 22);
            this.m_oZoomButtonOut.Click += new System.EventHandler(this.ZoomButtonOut_Click);
            // 
            // m_oZoomItems
            // 
            this.m_oZoomItems.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_oZoomItems.FlatStyle = System.Windows.Forms.FlatStyle.Standard;
            this.m_oZoomItems.Items.AddRange(new object[] {
            "4000%",
            "2000%",
            "1600%",
            "1200%",
            "800%",
            "700%",
            "600%",
            "500%",
            "400%",
            "300%",
            "200%",
            "100%",
            "66%",
            "50%",
            "33%",
            "25%",
            "16%",
            "12%",
            "8%",
            "6%",
            "5%",
            "4%",
            "3%",
            "2%",
            "1%",
            "Window"});
            this.m_oZoomItems.MaxDropDownItems = 25;
            this.m_oZoomItems.Name = "m_oZoomItems";
            this.m_oZoomItems.Size = new System.Drawing.Size(100, 25);
            this.m_oZoomItems.SelectedIndexChanged += new System.EventHandler(this.ZoomItems_SelectedIndexChanged);
            // 
            // m_oZoomButtonIn
            // 
            this.m_oZoomButtonIn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_oZoomButtonIn.Image = global::TestViewManager.Properties.Resources.MenuViewZoomInIcon;
            this.m_oZoomButtonIn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oZoomButtonIn.Name = "m_oZoomButtonIn";
            this.m_oZoomButtonIn.Size = new System.Drawing.Size(23, 22);
            this.m_oZoomButtonIn.Click += new System.EventHandler(this.ZoomButtonIn_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // m_oGridEnableButton
            // 
            this.m_oGridEnableButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_oGridEnableButton.Image = global::TestViewManager.Properties.Resources.MenuViewGridIcon;
            this.m_oGridEnableButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oGridEnableButton.Name = "m_oGridEnableButton";
            this.m_oGridEnableButton.Size = new System.Drawing.Size(23, 22);
            this.m_oGridEnableButton.Text = "GridEnable";
            this.m_oGridEnableButton.Click += new System.EventHandler(this.GridEnableButton_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // m_oRullersButton
            // 
            this.m_oRullersButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_oRullersButton.Image = global::TestViewManager.Properties.Resources.MenuViewRulersIcon;
            this.m_oRullersButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oRullersButton.Name = "m_oRullersButton";
            this.m_oRullersButton.Size = new System.Drawing.Size(23, 22);
            this.m_oRullersButton.Text = "Rullers";
            this.m_oRullersButton.Click += new System.EventHandler(this.RullersButton_Click);
            // 
            // m_oComboBoxRullersType
            // 
            this.m_oComboBoxRullersType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_oComboBoxRullersType.FlatStyle = System.Windows.Forms.FlatStyle.Standard;
            this.m_oComboBoxRullersType.Items.AddRange(new object[] {
            "Pixels",
            "Centimeters",
            "Inches"});
            this.m_oComboBoxRullersType.Name = "m_oComboBoxRullersType";
            this.m_oComboBoxRullersType.Size = new System.Drawing.Size(85, 25);
            this.m_oComboBoxRullersType.SelectedIndexChanged += new System.EventHandler(this.ComboBoxRullersType_SelectedIndexChanged);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ColorBackground,
            this.FullChess,
            this.ChessOnlyImage});
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(76, 22);
            this.toolStripDropDownButton1.Text = "Background";
            // 
            // ColorBackground
            // 
            this.ColorBackground.Name = "ColorBackground";
            this.ColorBackground.Size = new System.Drawing.Size(155, 22);
            this.ColorBackground.Text = "Color";
            this.ColorBackground.Click += new System.EventHandler(this.ColorBackground_Click);
            // 
            // FullChess
            // 
            this.FullChess.Checked = true;
            this.FullChess.CheckState = System.Windows.Forms.CheckState.Checked;
            this.FullChess.Name = "FullChess";
            this.FullChess.Size = new System.Drawing.Size(155, 22);
            this.FullChess.Text = "FullChess";
            this.FullChess.Click += new System.EventHandler(this.FullChess_Click);
            // 
            // ChessOnlyImage
            // 
            this.ChessOnlyImage.Name = "ChessOnlyImage";
            this.ChessOnlyImage.Size = new System.Drawing.Size(155, 22);
            this.ChessOnlyImage.Text = "ChessOnlyImage";
            this.ChessOnlyImage.Click += new System.EventHandler(this.ChessOnlyImage_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
            // 
            // m_oFastWindow
            // 
            this.m_oFastWindow.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.m_oFastWindow.Image = ((System.Drawing.Image)(resources.GetObject("m_oFastWindow.Image")));
            this.m_oFastWindow.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_oFastWindow.Name = "m_oFastWindow";
            this.m_oFastWindow.Size = new System.Drawing.Size(70, 22);
            this.m_oFastWindow.Text = "FastWindow";
            this.m_oFastWindow.Click += new System.EventHandler(this.OnFastRenderWindow_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 25);
            // 
            // bBeginUpdate
            // 
            this.bBeginUpdate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.bBeginUpdate.Image = ((System.Drawing.Image)(resources.GetObject("bBeginUpdate.Image")));
            this.bBeginUpdate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bBeginUpdate.Name = "bBeginUpdate";
            this.bBeginUpdate.Size = new System.Drawing.Size(72, 22);
            this.bBeginUpdate.Text = "BeginUpdate";
            this.bBeginUpdate.Click += new System.EventHandler(this.bBeginUpdate_Click);
            // 
            // bEndpdate
            // 
            this.bEndpdate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.bEndpdate.Image = ((System.Drawing.Image)(resources.GetObject("bEndpdate.Image")));
            this.bEndpdate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bEndpdate.Name = "bEndpdate";
            this.bEndpdate.Size = new System.Drawing.Size(64, 22);
            this.bEndpdate.Text = "EndUpdate";
            this.bEndpdate.Click += new System.EventHandler(this.bEndpdate_Click);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(6, 25);
            // 
            // m_oZoomTackBar
            // 
            this.m_oZoomTackBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_oZoomTackBar.AutoSize = false;
            this.m_oZoomTackBar.Location = new System.Drawing.Point(3, 464);
            this.m_oZoomTackBar.Maximum = 20000;
            this.m_oZoomTackBar.Name = "m_oZoomTackBar";
            this.m_oZoomTackBar.Size = new System.Drawing.Size(588, 15);
            this.m_oZoomTackBar.TabIndex = 5;
            this.m_oZoomTackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.m_oZoomTackBar.Value = 500;
            this.m_oZoomTackBar.Scroll += new System.EventHandler(this.ZoomTackBar_Scroll);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_oZoomStatusLabel,
            this.m_oImageSizesLabel,
            this.m_oPreviewSizesLabel,
            this.m_oScrollCenterLabel,
            this.m_oControlSize});
            this.statusStrip1.Location = new System.Drawing.Point(0, 537);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(989, 22);
            this.statusStrip1.TabIndex = 6;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // m_oZoomStatusLabel
            // 
            this.m_oZoomStatusLabel.AutoSize = false;
            this.m_oZoomStatusLabel.Name = "m_oZoomStatusLabel";
            this.m_oZoomStatusLabel.Size = new System.Drawing.Size(120, 17);
            this.m_oZoomStatusLabel.Text = "Zoom : 100%";
            this.m_oZoomStatusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // m_oImageSizesLabel
            // 
            this.m_oImageSizesLabel.Name = "m_oImageSizesLabel";
            this.m_oImageSizesLabel.Size = new System.Drawing.Size(99, 17);
            this.m_oImageSizesLabel.Text = "ImageSize : [ 0: 0 ]";
            // 
            // m_oPreviewSizesLabel
            // 
            this.m_oPreviewSizesLabel.Name = "m_oPreviewSizesLabel";
            this.m_oPreviewSizesLabel.Size = new System.Drawing.Size(45, 17);
            this.m_oPreviewSizesLabel.Text = "Preview";
            // 
            // m_oScrollCenterLabel
            // 
            this.m_oScrollCenterLabel.Name = "m_oScrollCenterLabel";
            this.m_oScrollCenterLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // m_oControlSize
            // 
            this.m_oControlSize.Name = "m_oControlSize";
            this.m_oControlSize.Size = new System.Drawing.Size(0, 17);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "OpenFile";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.ColumnCount = 4;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 97.4026F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 2.597403F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 175F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 204F));
            this.tableLayoutPanel1.Controls.Add(this.m_vScrollBar, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.m_oImageEditor, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.m_oZoomTackBar, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.m_hScrollBar, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 3, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 2, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 52);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 97.39584F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 2.604167F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(989, 482);
            this.tableLayoutPanel1.TabIndex = 7;
            // 
            // m_oImageEditor
            // 
            this.m_oImageEditor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_oImageEditor.Enabled = true;
            this.m_oImageEditor.Location = new System.Drawing.Point(3, 3);
            this.m_oImageEditor.Name = "m_oImageEditor";
            this.m_oImageEditor.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("m_oImageEditor.OcxState")));
            this.m_oImageEditor.Size = new System.Drawing.Size(588, 443);
            this.m_oImageEditor.TabIndex = 0;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 1;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Controls.Add(this.UndoButton, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.UndoListBox, 0, 2);
            this.tableLayoutPanel3.Controls.Add(this.RedoButton, 0, 3);
            this.tableLayoutPanel3.Controls.Add(this.label3, 0, 4);
            this.tableLayoutPanel3.Controls.Add(this.RedoListBox, 0, 5);
            this.tableLayoutPanel3.Controls.Add(this.UndoReset, 0, 6);
            this.tableLayoutPanel3.Location = new System.Drawing.Point(787, 3);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 7;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 66.03773F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.96227F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 168F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 16F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 144F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(193, 440);
            this.tableLayoutPanel3.TabIndex = 7;
            // 
            // UndoButton
            // 
            this.UndoButton.Location = new System.Drawing.Point(3, 3);
            this.UndoButton.Name = "UndoButton";
            this.UndoButton.Size = new System.Drawing.Size(75, 23);
            this.UndoButton.TabIndex = 0;
            this.UndoButton.Text = "Undo";
            this.UndoButton.UseVisualStyleBackColor = true;
            this.UndoButton.Click += new System.EventHandler(this.UndoButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(61, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "UndoStack";
            // 
            // UndoListBox
            // 
            this.UndoListBox.FormattingEnabled = true;
            this.UndoListBox.Location = new System.Drawing.Point(3, 54);
            this.UndoListBox.Name = "UndoListBox";
            this.UndoListBox.Size = new System.Drawing.Size(187, 160);
            this.UndoListBox.TabIndex = 2;
            // 
            // RedoButton
            // 
            this.RedoButton.Location = new System.Drawing.Point(3, 222);
            this.RedoButton.Name = "RedoButton";
            this.RedoButton.Size = new System.Drawing.Size(75, 23);
            this.RedoButton.TabIndex = 3;
            this.RedoButton.Text = "Redo";
            this.RedoButton.UseVisualStyleBackColor = true;
            this.RedoButton.Click += new System.EventHandler(this.RedoButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 251);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "RedoStack";
            // 
            // RedoListBox
            // 
            this.RedoListBox.FormattingEnabled = true;
            this.RedoListBox.Location = new System.Drawing.Point(3, 270);
            this.RedoListBox.Name = "RedoListBox";
            this.RedoListBox.Size = new System.Drawing.Size(187, 134);
            this.RedoListBox.TabIndex = 5;
            // 
            // UndoReset
            // 
            this.UndoReset.Location = new System.Drawing.Point(3, 414);
            this.UndoReset.Name = "UndoReset";
            this.UndoReset.Size = new System.Drawing.Size(178, 23);
            this.UndoReset.TabIndex = 6;
            this.UndoReset.TabStop = false;
            this.UndoReset.Text = "Reset";
            this.UndoReset.UseVisualStyleBackColor = true;
            this.UndoReset.Click += new System.EventHandler(this.UndoReset_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.controlBrightnessContrastProperties1);
            this.panel1.Controls.Add(this.controlBrushProperties1);
            this.panel1.Controls.Add(this.controlCropProperties1);
            this.panel1.Controls.Add(this.controlLevelsProperties1);
            this.panel1.Controls.Add(this.controlToneProperties1);
            this.panel1.Controls.Add(this.controlXMLProperties1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.CommandChoise_comboBox);
            this.panel1.Controls.Add(this.StartCommandBtn);
            this.panel1.Controls.Add(this.StopCommandBtn);
            this.panel1.Controls.Add(this.Size_label);
            this.panel1.Controls.Add(this.Size_trackBar);
            this.panel1.Controls.Add(this.Type_label);
            this.panel1.Controls.Add(this.Type_comboBox);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(612, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(169, 443);
            this.panel1.TabIndex = 8;
            // 
            // controlBrightnessContrastProperties1
            // 
            this.controlBrightnessContrastProperties1.Location = new System.Drawing.Point(0, 85);
            this.controlBrightnessContrastProperties1.Name = "controlBrightnessContrastProperties1";
            this.controlBrightnessContrastProperties1.Size = new System.Drawing.Size(169, 250);
            this.controlBrightnessContrastProperties1.TabIndex = 18;
            // 
            // controlBrushProperties1
            // 
            this.controlBrushProperties1.Location = new System.Drawing.Point(0, 85);
            this.controlBrushProperties1.Name = "controlBrushProperties1";
            this.controlBrushProperties1.Size = new System.Drawing.Size(169, 250);
            this.controlBrushProperties1.TabIndex = 17;
            // 
            // controlCropProperties1
            // 
            this.controlCropProperties1.Location = new System.Drawing.Point(3, 79);
            this.controlCropProperties1.Name = "controlCropProperties1";
            this.controlCropProperties1.Size = new System.Drawing.Size(166, 291);
            this.controlCropProperties1.TabIndex = 16;
            // 
            // controlLevelsProperties1
            // 
            this.controlLevelsProperties1.Location = new System.Drawing.Point(3, 79);
            this.controlLevelsProperties1.Name = "controlLevelsProperties1";
            this.controlLevelsProperties1.Size = new System.Drawing.Size(166, 291);
            this.controlLevelsProperties1.TabIndex = 19;
            // 
            // controlToneProperties1
            // 
            this.controlToneProperties1.Location = new System.Drawing.Point(3, 79);
            this.controlToneProperties1.Name = "controlToneProperties1";
            this.controlToneProperties1.Size = new System.Drawing.Size(166, 291);
            this.controlToneProperties1.TabIndex = 20;
            // 
            // controlXMLProperties1
            // 
            this.controlXMLProperties1.Location = new System.Drawing.Point(3, 79);
            this.controlXMLProperties1.Name = "controlXMLProperties1";
            this.controlXMLProperties1.Size = new System.Drawing.Size(166, 291);
            this.controlXMLProperties1.TabIndex = 20;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Command";
            // 
            // CommandChoise_comboBox
            // 
            this.CommandChoise_comboBox.FormattingEnabled = true;
            this.CommandChoise_comboBox.Items.AddRange(new object[] {
            "Test ",
            "Stamp ",
            "Eraser ",
            "Crop ",
            "Histogramm ",
            "Brush ",
            "Red Eye ",
            "Brightness - Contrast",
            "Levels",
            "Tone",
            "XML"});
            this.CommandChoise_comboBox.Location = new System.Drawing.Point(3, 24);
            this.CommandChoise_comboBox.Name = "CommandChoise_comboBox";
            this.CommandChoise_comboBox.Size = new System.Drawing.Size(163, 21);
            this.CommandChoise_comboBox.TabIndex = 9;
            // 
            // StartCommandBtn
            // 
            this.StartCommandBtn.Location = new System.Drawing.Point(3, 50);
            this.StartCommandBtn.Name = "StartCommandBtn";
            this.StartCommandBtn.Size = new System.Drawing.Size(110, 23);
            this.StartCommandBtn.TabIndex = 10;
            this.StartCommandBtn.Text = "Start Command";
            this.StartCommandBtn.UseVisualStyleBackColor = true;
            this.StartCommandBtn.Click += new System.EventHandler(this.StartCommandBtn_Click);
            // 
            // StopCommandBtn
            // 
            this.StopCommandBtn.Location = new System.Drawing.Point(3, 417);
            this.StopCommandBtn.Name = "StopCommandBtn";
            this.StopCommandBtn.Size = new System.Drawing.Size(120, 23);
            this.StopCommandBtn.TabIndex = 11;
            this.StopCommandBtn.Text = "Stop Sommand";
            this.StopCommandBtn.UseVisualStyleBackColor = true;
            this.StopCommandBtn.Click += new System.EventHandler(this.StopCommandBtn_Click);
            // 
            // Size_label
            // 
            this.Size_label.AutoSize = true;
            this.Size_label.Location = new System.Drawing.Point(3, 85);
            this.Size_label.Name = "Size_label";
            this.Size_label.Size = new System.Drawing.Size(30, 13);
            this.Size_label.TabIndex = 12;
            this.Size_label.Text = "Size:";
            this.Size_label.Visible = false;
            // 
            // Size_trackBar
            // 
            this.Size_trackBar.Location = new System.Drawing.Point(3, 104);
            this.Size_trackBar.Name = "Size_trackBar";
            this.Size_trackBar.Size = new System.Drawing.Size(163, 45);
            this.Size_trackBar.TabIndex = 13;
            this.Size_trackBar.Visible = false;
            this.Size_trackBar.Scroll += new System.EventHandler(this.Size_trackBar_Scroll);
            // 
            // Type_label
            // 
            this.Type_label.AutoSize = true;
            this.Type_label.Location = new System.Drawing.Point(3, 164);
            this.Type_label.Name = "Type_label";
            this.Type_label.Size = new System.Drawing.Size(34, 13);
            this.Type_label.TabIndex = 14;
            this.Type_label.Text = "Type:";
            this.Type_label.Visible = false;
            // 
            // Type_comboBox
            // 
            this.Type_comboBox.FormattingEnabled = true;
            this.Type_comboBox.Items.AddRange(new object[] {
            "Round",
            "Rectangle",
            "Ellipse"});
            this.Type_comboBox.Location = new System.Drawing.Point(3, 183);
            this.Type_comboBox.Name = "Type_comboBox";
            this.Type_comboBox.Size = new System.Drawing.Size(163, 21);
            this.Type_comboBox.TabIndex = 15;
            this.Type_comboBox.Visible = false;
            this.Type_comboBox.SelectedIndexChanged += new System.EventHandler(this.Type_comboBox_SelectedIndexChanged);
            // 
            // TimerSliderH
            // 
            this.TimerSliderH.Enabled = true;
            this.TimerSliderH.Tick += new System.EventHandler(this.OnTimerSliderHTick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(989, 559);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ImageEdtor";
            this.SizeChanged += new System.EventHandler(this.MainForm_SizeChanged);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.Resize += new System.EventHandler(this.MainForm_Resize);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_oZoomTackBar)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_oImageEditor)).EndInit();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Size_trackBar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private AxAVSImageEditor.AxAVSImageEditor m_oImageEditor;
        private AVSImageEditor.IViewSettings m_oViewSettings;
        private AVSImageEditor.CHistoryStack m_oUndoStack;
        private AVSImageEditor.CHistoryStack m_oRedoStack;
        private System.Windows.Forms.VScrollBar m_vScrollBar;
        private System.Windows.Forms.HScrollBar m_hScrollBar;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.TrackBar m_oZoomTackBar;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel m_oZoomStatusLabel;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton m_oZoomToWindow;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton m_oZoomButtonOut;
        private System.Windows.Forms.ToolStripComboBox m_oZoomItems;
        private System.Windows.Forms.ToolStripButton m_oZoomButtonIn;
        private System.Windows.Forms.ToolStripStatusLabel m_oImageSizesLabel;
        private System.Windows.Forms.ToolStripStatusLabel m_oPreviewSizesLabel;
        private System.Windows.Forms.ToolStripStatusLabel m_oScrollCenterLabel;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton m_oGridEnableButton;
        private System.Windows.Forms.ToolStripButton m_oRullersButton;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Timer TimerSliderH;

        private System.Windows.Forms.ToolStripButton m_oOpenImageFile;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripStatusLabel m_oControlSize;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Button UndoButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ListBox UndoListBox;
        private System.Windows.Forms.Button RedoButton;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ListBox RedoListBox;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox CommandChoise_comboBox;
        private System.Windows.Forms.Button StartCommandBtn;
        private System.Windows.Forms.Button StopCommandBtn;
        private System.Windows.Forms.Label Size_label;
        private System.Windows.Forms.TrackBar Size_trackBar;
        private System.Windows.Forms.Label Type_label;
        private System.Windows.Forms.ComboBox Type_comboBox;
        private TestViewManager.CommandParams.ControlCropProperties controlCropProperties1;
        private ControlBrightnessContrastProperties controlBrightnessContrastProperties1;
        private ControlBrushProperties controlBrushProperties1;
        private ControlLevelsProperties controlLevelsProperties1;
        private ControlToneProperties controlToneProperties1;
        private ControlXMLProperties controlXMLProperties1;
        private System.Windows.Forms.ToolStripComboBox m_oComboBoxRullersType;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.Button UndoReset;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem openEmptyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openSpaceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openBadFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripButton m_oFastWindow;
        private System.Windows.Forms.ToolStripMenuItem FullChess;
        private System.Windows.Forms.ToolStripMenuItem ColorBackground;
        private System.Windows.Forms.ToolStripMenuItem ChessOnlyImage;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripButton bBeginUpdate;
        private System.Windows.Forms.ToolStripButton bEndpdate;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;

    }
}

