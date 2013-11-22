namespace TestViewManager
{
    partial class ControlBrushProperties
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textContrast = new System.Windows.Forms.TextBox();
            this.trackContrast = new System.Windows.Forms.TrackBar();
            this.labelContast = new System.Windows.Forms.Label();
            this.textLimitBrightness = new System.Windows.Forms.TextBox();
            this.trackLimitBrightness = new System.Windows.Forms.TrackBar();
            this.labelLimitBrightness = new System.Windows.Forms.Label();
            this.textLevel = new System.Windows.Forms.TextBox();
            this.trackLevel = new System.Windows.Forms.TrackBar();
            this.labelLevel = new System.Windows.Forms.Label();
            this.textSize = new System.Windows.Forms.TextBox();
            this.trackSize = new System.Windows.Forms.TrackBar();
            this.labelSize = new System.Windows.Forms.Label();
            this.comboType = new System.Windows.Forms.ComboBox();
            this.labelType = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.trackContrast)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackLimitBrightness)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackLevel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackSize)).BeginInit();
            this.SuspendLayout();
            // 
            // textContrast
            // 
            this.textContrast.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textContrast.Enabled = false;
            this.textContrast.Location = new System.Drawing.Point(138, 205);
            this.textContrast.Name = "textContrast";
            this.textContrast.Size = new System.Drawing.Size(48, 20);
            this.textContrast.TabIndex = 29;
            this.textContrast.Visible = false;
            // 
            // trackContrast
            // 
            this.trackContrast.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackContrast.AutoSize = false;
            this.trackContrast.Location = new System.Drawing.Point(10, 204);
            this.trackContrast.Name = "trackContrast";
            this.trackContrast.Size = new System.Drawing.Size(122, 28);
            this.trackContrast.TabIndex = 28;
            this.trackContrast.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackContrast.Visible = false;
            this.trackContrast.ValueChanged += new System.EventHandler(this.trackContrast_ValueChanged);
            // 
            // labelContast
            // 
            this.labelContast.AutoSize = true;
            this.labelContast.Location = new System.Drawing.Point(12, 188);
            this.labelContast.Name = "labelContast";
            this.labelContast.Size = new System.Drawing.Size(46, 13);
            this.labelContast.TabIndex = 27;
            this.labelContast.Text = "Contrast";
            this.labelContast.Visible = false;
            // 
            // textLimitBrightness
            // 
            this.textLimitBrightness.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textLimitBrightness.Enabled = false;
            this.textLimitBrightness.Location = new System.Drawing.Point(138, 158);
            this.textLimitBrightness.Name = "textLimitBrightness";
            this.textLimitBrightness.Size = new System.Drawing.Size(48, 20);
            this.textLimitBrightness.TabIndex = 23;
            this.textLimitBrightness.Visible = false;
            // 
            // trackLimitBrightness
            // 
            this.trackLimitBrightness.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackLimitBrightness.AutoSize = false;
            this.trackLimitBrightness.Location = new System.Drawing.Point(10, 157);
            this.trackLimitBrightness.Name = "trackLimitBrightness";
            this.trackLimitBrightness.Size = new System.Drawing.Size(122, 28);
            this.trackLimitBrightness.TabIndex = 22;
            this.trackLimitBrightness.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackLimitBrightness.Visible = false;
            this.trackLimitBrightness.ValueChanged += new System.EventHandler(this.trackLimitBrightness_ValueChanged);
            // 
            // labelLimitBrightness
            // 
            this.labelLimitBrightness.AutoSize = true;
            this.labelLimitBrightness.Location = new System.Drawing.Point(12, 141);
            this.labelLimitBrightness.Name = "labelLimitBrightness";
            this.labelLimitBrightness.Size = new System.Drawing.Size(77, 13);
            this.labelLimitBrightness.TabIndex = 21;
            this.labelLimitBrightness.Text = "LimitBrightness";
            this.labelLimitBrightness.Visible = false;
            // 
            // textLevel
            // 
            this.textLevel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textLevel.Enabled = false;
            this.textLevel.Location = new System.Drawing.Point(138, 110);
            this.textLevel.Name = "textLevel";
            this.textLevel.Size = new System.Drawing.Size(48, 20);
            this.textLevel.TabIndex = 20;
            // 
            // trackLevel
            // 
            this.trackLevel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackLevel.AutoSize = false;
            this.trackLevel.Location = new System.Drawing.Point(10, 109);
            this.trackLevel.Name = "trackLevel";
            this.trackLevel.Size = new System.Drawing.Size(122, 28);
            this.trackLevel.TabIndex = 19;
            this.trackLevel.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackLevel.ValueChanged += new System.EventHandler(this.trackBrightness_ValueChanged);
            // 
            // labelLevel
            // 
            this.labelLevel.AutoSize = true;
            this.labelLevel.Location = new System.Drawing.Point(12, 93);
            this.labelLevel.Name = "labelLevel";
            this.labelLevel.Size = new System.Drawing.Size(33, 13);
            this.labelLevel.TabIndex = 18;
            this.labelLevel.Text = "Level";
            // 
            // textSize
            // 
            this.textSize.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textSize.Enabled = false;
            this.textSize.Location = new System.Drawing.Point(138, 25);
            this.textSize.Name = "textSize";
            this.textSize.Size = new System.Drawing.Size(46, 20);
            this.textSize.TabIndex = 17;
            // 
            // trackSize
            // 
            this.trackSize.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackSize.AutoSize = false;
            this.trackSize.Location = new System.Drawing.Point(10, 25);
            this.trackSize.Name = "trackSize";
            this.trackSize.Size = new System.Drawing.Size(122, 28);
            this.trackSize.TabIndex = 16;
            this.trackSize.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackSize.ValueChanged += new System.EventHandler(this.trackSize_ValueChanged);
            // 
            // labelSize
            // 
            this.labelSize.AutoSize = true;
            this.labelSize.Location = new System.Drawing.Point(10, 8);
            this.labelSize.Name = "labelSize";
            this.labelSize.Size = new System.Drawing.Size(27, 13);
            this.labelSize.TabIndex = 15;
            this.labelSize.Text = "Size";
            // 
            // comboType
            // 
            this.comboType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.comboType.FormattingEnabled = true;
            this.comboType.Items.AddRange(new object[] {
            "Brightness",
            "Contrast",
            "Blur",
            "Sharpen",
            "Saturation",
            "Posterize",
            "Glow",
            "Gamma"});
            this.comboType.Location = new System.Drawing.Point(65, 59);
            this.comboType.Name = "comboType";
            this.comboType.Size = new System.Drawing.Size(121, 21);
            this.comboType.TabIndex = 30;
            this.comboType.SelectedIndexChanged += new System.EventHandler(this.comboType_SelectedIndexChanged);
            // 
            // labelType
            // 
            this.labelType.AutoSize = true;
            this.labelType.Location = new System.Drawing.Point(12, 62);
            this.labelType.Name = "labelType";
            this.labelType.Size = new System.Drawing.Size(31, 13);
            this.labelType.TabIndex = 31;
            this.labelType.Text = "Type";
            // 
            // ControlBrushProperties
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.labelType);
            this.Controls.Add(this.comboType);
            this.Controls.Add(this.textContrast);
            this.Controls.Add(this.trackContrast);
            this.Controls.Add(this.labelContast);
            this.Controls.Add(this.textLimitBrightness);
            this.Controls.Add(this.trackLimitBrightness);
            this.Controls.Add(this.labelLimitBrightness);
            this.Controls.Add(this.textLevel);
            this.Controls.Add(this.trackLevel);
            this.Controls.Add(this.labelLevel);
            this.Controls.Add(this.textSize);
            this.Controls.Add(this.trackSize);
            this.Controls.Add(this.labelSize);
            this.Name = "ControlBrushProperties";
            this.Size = new System.Drawing.Size(200, 250);
            ((System.ComponentModel.ISupportInitialize)(this.trackContrast)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackLimitBrightness)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackLevel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackSize)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textContrast;
        private System.Windows.Forms.TrackBar trackContrast;
        private System.Windows.Forms.Label labelContast;
        private System.Windows.Forms.TextBox textLimitBrightness;
        private System.Windows.Forms.TrackBar trackLimitBrightness;
        private System.Windows.Forms.Label labelLimitBrightness;
        private System.Windows.Forms.TextBox textLevel;
        private System.Windows.Forms.TrackBar trackLevel;
        private System.Windows.Forms.Label labelLevel;
        private System.Windows.Forms.TextBox textSize;
        private System.Windows.Forms.TrackBar trackSize;
        private System.Windows.Forms.Label labelSize;
        private System.Windows.Forms.ComboBox comboType;
        private System.Windows.Forms.Label labelType;
    }
}
