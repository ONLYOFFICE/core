namespace TestImageStudio
{
    partial class FormMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.buttonRun = new System.Windows.Forms.Button();
            this.textBoxXml = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.textBoxPath = new System.Windows.Forms.TextBox();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.numericDuration = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.checkBoxUpdatePreview = new System.Windows.Forms.CheckBox();
            this.checkBoxAutoReload = new System.Windows.Forms.CheckBox();
            this.treeViewPresets = new System.Windows.Forms.TreeView();
            this.buttonRun3 = new System.Windows.Forms.Button();
            this.axImageView31 = new AxAVSImageView3.AxImageView3();
            ((System.ComponentModel.ISupportInitialize)(this.numericDuration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonRun
            // 
            this.buttonRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRun.Location = new System.Drawing.Point(936, 12);
            this.buttonRun.Name = "buttonRun";
            this.buttonRun.Size = new System.Drawing.Size(86, 23);
            this.buttonRun.TabIndex = 0;
            this.buttonRun.Text = "Run";
            this.buttonRun.UseVisualStyleBackColor = true;
            this.buttonRun.Click += new System.EventHandler(this.buttonRun_Click);
            // 
            // textBoxXml
            // 
            this.textBoxXml.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)));
            this.textBoxXml.HideSelection = false;
            this.textBoxXml.Location = new System.Drawing.Point(106, 228);
            this.textBoxXml.Multiline = true;
            this.textBoxXml.Name = "textBoxXml";
            this.textBoxXml.Size = new System.Drawing.Size(345, 207);
            this.textBoxXml.TabIndex = 1;
            this.textBoxXml.WordWrap = false;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "All Files|*.*";
            this.openFileDialog1.Title = "Open Files";
            // 
            // textBoxPath
            // 
            this.textBoxPath.HideSelection = false;
            this.textBoxPath.Location = new System.Drawing.Point(106, 12);
            this.textBoxPath.Name = "textBoxPath";
            this.textBoxPath.ReadOnly = true;
            this.textBoxPath.Size = new System.Drawing.Size(307, 20);
            this.textBoxPath.TabIndex = 3;
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Location = new System.Drawing.Point(419, 10);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(32, 22);
            this.buttonBrowse.TabIndex = 4;
            this.buttonBrowse.Text = "...";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(76, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Image file path";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 447);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Duration in frames";
            // 
            // numericDuration
            // 
            this.numericDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.numericDuration.Location = new System.Drawing.Point(106, 445);
            this.numericDuration.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.numericDuration.Name = "numericDuration";
            this.numericDuration.Size = new System.Drawing.Size(123, 20);
            this.numericDuration.TabIndex = 7;
            this.numericDuration.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 41);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(72, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Transform xml";
            // 
            // checkBoxUpdatePreview
            // 
            this.checkBoxUpdatePreview.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxUpdatePreview.AutoSize = true;
            this.checkBoxUpdatePreview.Location = new System.Drawing.Point(461, 447);
            this.checkBoxUpdatePreview.Name = "checkBoxUpdatePreview";
            this.checkBoxUpdatePreview.Size = new System.Drawing.Size(101, 17);
            this.checkBoxUpdatePreview.TabIndex = 14;
            this.checkBoxUpdatePreview.Text = "Update preview";
            this.checkBoxUpdatePreview.UseVisualStyleBackColor = true;
            // 
            // checkBoxAutoReload
            // 
            this.checkBoxAutoReload.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxAutoReload.AutoSize = true;
            this.checkBoxAutoReload.Location = new System.Drawing.Point(568, 447);
            this.checkBoxAutoReload.Name = "checkBoxAutoReload";
            this.checkBoxAutoReload.Size = new System.Drawing.Size(141, 17);
            this.checkBoxAutoReload.TabIndex = 13;
            this.checkBoxAutoReload.Text = "Reload image each step";
            this.checkBoxAutoReload.UseVisualStyleBackColor = true;
            // 
            // treeViewPresets
            // 
            this.treeViewPresets.HideSelection = false;
            this.treeViewPresets.Location = new System.Drawing.Point(106, 41);
            this.treeViewPresets.Name = "treeViewPresets";
            this.treeViewPresets.Size = new System.Drawing.Size(345, 181);
            this.treeViewPresets.TabIndex = 15;
            this.treeViewPresets.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewPresets_AfterSelect);
            // 
            // buttonRun3
            // 
            this.buttonRun3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRun3.Location = new System.Drawing.Point(936, 41);
            this.buttonRun3.Name = "buttonRun3";
            this.buttonRun3.Size = new System.Drawing.Size(86, 23);
            this.buttonRun3.TabIndex = 16;
            this.buttonRun3.Text = "Run Compose";
            this.buttonRun3.UseVisualStyleBackColor = true;
            this.buttonRun3.Click += new System.EventHandler(this.buttonRun3_Click);
            // 
            // axImageView31
            // 
            this.axImageView31.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.axImageView31.Enabled = true;
            this.axImageView31.Location = new System.Drawing.Point(461, 41);
            this.axImageView31.Name = "axImageView31";
            this.axImageView31.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axImageView31.OcxState")));
            this.axImageView31.Size = new System.Drawing.Size(469, 394);
            this.axImageView31.TabIndex = 17;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1034, 481);
            this.Controls.Add(this.axImageView31);
            this.Controls.Add(this.buttonRun3);
            this.Controls.Add(this.treeViewPresets);
            this.Controls.Add(this.checkBoxUpdatePreview);
            this.Controls.Add(this.checkBoxAutoReload);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numericDuration);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonBrowse);
            this.Controls.Add(this.textBoxPath);
            this.Controls.Add(this.textBoxXml);
            this.Controls.Add(this.buttonRun);
            this.Name = "FormMain";
            this.Text = "TestImageStudio";
            this.Load += new System.EventHandler(this.FormMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numericDuration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonRun;
        private System.Windows.Forms.TextBox textBoxXml;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.TextBox textBoxPath;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numericDuration;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox checkBoxUpdatePreview;
        private System.Windows.Forms.CheckBox checkBoxAutoReload;
        private System.Windows.Forms.TreeView treeViewPresets;
        private System.Windows.Forms.Button buttonRun3;
        private AxAVSImageView3.AxImageView3 axImageView31;
    }
}

