namespace TestViewManager
{
    partial class ControlBrightnessContrastProperties
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
            this.textBrightness = new System.Windows.Forms.TextBox();
            this.trackBrightness = new System.Windows.Forms.TrackBar();
            this.labelBrightness = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.trackContrast)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBrightness)).BeginInit();
            this.SuspendLayout();
            // 
            // textContrast
            // 
            this.textContrast.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textContrast.Enabled = false;
            this.textContrast.Location = new System.Drawing.Point(131, 69);
            this.textContrast.Name = "textContrast";
            this.textContrast.Size = new System.Drawing.Size(48, 20);
            this.textContrast.TabIndex = 35;
            // 
            // trackContrast
            // 
            this.trackContrast.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackContrast.AutoSize = false;
            this.trackContrast.Location = new System.Drawing.Point(3, 68);
            this.trackContrast.Name = "trackContrast";
            this.trackContrast.Size = new System.Drawing.Size(122, 28);
            this.trackContrast.TabIndex = 34;
            this.trackContrast.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackContrast.ValueChanged += new System.EventHandler(this.trackContrast_ValueChanged);
            // 
            // labelContast
            // 
            this.labelContast.AutoSize = true;
            this.labelContast.Location = new System.Drawing.Point(5, 52);
            this.labelContast.Name = "labelContast";
            this.labelContast.Size = new System.Drawing.Size(46, 13);
            this.labelContast.TabIndex = 33;
            this.labelContast.Text = "Contrast";
            // 
            // textBrightness
            // 
            this.textBrightness.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBrightness.Enabled = false;
            this.textBrightness.Location = new System.Drawing.Point(131, 25);
            this.textBrightness.Name = "textBrightness";
            this.textBrightness.Size = new System.Drawing.Size(48, 20);
            this.textBrightness.TabIndex = 32;
            // 
            // trackBrightness
            // 
            this.trackBrightness.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBrightness.AutoSize = false;
            this.trackBrightness.Location = new System.Drawing.Point(3, 24);
            this.trackBrightness.Name = "trackBrightness";
            this.trackBrightness.Size = new System.Drawing.Size(122, 28);
            this.trackBrightness.TabIndex = 31;
            this.trackBrightness.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBrightness.ValueChanged += new System.EventHandler(this.trackBrightness_ValueChanged);
            // 
            // labelBrightness
            // 
            this.labelBrightness.AutoSize = true;
            this.labelBrightness.Location = new System.Drawing.Point(5, 8);
            this.labelBrightness.Name = "labelBrightness";
            this.labelBrightness.Size = new System.Drawing.Size(56, 13);
            this.labelBrightness.TabIndex = 30;
            this.labelBrightness.Text = "Brightness";
            // 
            // ControlBrightnessContrastProperties
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.textContrast);
            this.Controls.Add(this.trackContrast);
            this.Controls.Add(this.labelContast);
            this.Controls.Add(this.textBrightness);
            this.Controls.Add(this.trackBrightness);
            this.Controls.Add(this.labelBrightness);
            this.Name = "ControlBrightnessContrastProperties";
            this.Size = new System.Drawing.Size(183, 112);
            ((System.ComponentModel.ISupportInitialize)(this.trackContrast)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBrightness)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textContrast;
        private System.Windows.Forms.TrackBar trackContrast;
        private System.Windows.Forms.Label labelContast;
        private System.Windows.Forms.TextBox textBrightness;
        private System.Windows.Forms.TrackBar trackBrightness;
        private System.Windows.Forms.Label labelBrightness;
    }
}
