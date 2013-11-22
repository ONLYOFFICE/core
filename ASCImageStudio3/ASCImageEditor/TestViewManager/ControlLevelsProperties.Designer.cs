namespace TestViewManager
{
    partial class ControlLevelsProperties
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
            this.textRed = new System.Windows.Forms.TextBox();
            this.trackRed = new System.Windows.Forms.TrackBar();
            this.labelRed = new System.Windows.Forms.Label();
            this.textGreen = new System.Windows.Forms.TextBox();
            this.trackGreen = new System.Windows.Forms.TrackBar();
            this.labelGreen = new System.Windows.Forms.Label();
            this.textBlue = new System.Windows.Forms.TextBox();
            this.trackBlue = new System.Windows.Forms.TrackBar();
            this.Blue = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.trackRed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBlue)).BeginInit();
            this.SuspendLayout();
            // 
            // textRed
            // 
            this.textRed.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textRed.Enabled = false;
            this.textRed.Location = new System.Drawing.Point(131, 25);
            this.textRed.Name = "textRed";
            this.textRed.Size = new System.Drawing.Size(48, 20);
            this.textRed.TabIndex = 35;
            // 
            // trackRed
            // 
            this.trackRed.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackRed.AutoSize = false;
            this.trackRed.Location = new System.Drawing.Point(3, 24);
            this.trackRed.Name = "trackRed";
            this.trackRed.Size = new System.Drawing.Size(122, 28);
            this.trackRed.TabIndex = 34;
            this.trackRed.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackRed.ValueChanged += new System.EventHandler(this.trackRed_ValueChanged);
            // 
            // labelRed
            // 
            this.labelRed.AutoSize = true;
            this.labelRed.Location = new System.Drawing.Point(5, 8);
            this.labelRed.Name = "labelRed";
            this.labelRed.Size = new System.Drawing.Size(27, 13);
            this.labelRed.TabIndex = 33;
            this.labelRed.Text = "Red";
            // 
            // textGreen
            // 
            this.textGreen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textGreen.Enabled = false;
            this.textGreen.Location = new System.Drawing.Point(131, 68);
            this.textGreen.Name = "textGreen";
            this.textGreen.Size = new System.Drawing.Size(48, 20);
            this.textGreen.TabIndex = 38;
            // 
            // trackGreen
            // 
            this.trackGreen.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackGreen.AutoSize = false;
            this.trackGreen.Location = new System.Drawing.Point(3, 67);
            this.trackGreen.Name = "trackGreen";
            this.trackGreen.Size = new System.Drawing.Size(122, 28);
            this.trackGreen.TabIndex = 37;
            this.trackGreen.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackGreen.ValueChanged += new System.EventHandler(this.trackGreen_ValueChanged);
            // 
            // labelGreen
            // 
            this.labelGreen.AutoSize = true;
            this.labelGreen.Location = new System.Drawing.Point(5, 51);
            this.labelGreen.Name = "labelGreen";
            this.labelGreen.Size = new System.Drawing.Size(36, 13);
            this.labelGreen.TabIndex = 36;
            this.labelGreen.Text = "Green";
            // 
            // textBlue
            // 
            this.textBlue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBlue.Enabled = false;
            this.textBlue.Location = new System.Drawing.Point(131, 112);
            this.textBlue.Name = "textBlue";
            this.textBlue.Size = new System.Drawing.Size(48, 20);
            this.textBlue.TabIndex = 41;
            // 
            // trackBlue
            // 
            this.trackBlue.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBlue.AutoSize = false;
            this.trackBlue.Location = new System.Drawing.Point(3, 111);
            this.trackBlue.Name = "trackBlue";
            this.trackBlue.Size = new System.Drawing.Size(122, 28);
            this.trackBlue.TabIndex = 40;
            this.trackBlue.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBlue.ValueChanged += new System.EventHandler(this.trackBlue_ValueChanged);
            // 
            // Blue
            // 
            this.Blue.AutoSize = true;
            this.Blue.Location = new System.Drawing.Point(5, 95);
            this.Blue.Name = "Blue";
            this.Blue.Size = new System.Drawing.Size(28, 13);
            this.Blue.TabIndex = 39;
            this.Blue.Text = "Blue";
            // 
            // ControlLevelsProperties
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.textBlue);
            this.Controls.Add(this.trackBlue);
            this.Controls.Add(this.Blue);
            this.Controls.Add(this.textGreen);
            this.Controls.Add(this.trackGreen);
            this.Controls.Add(this.labelGreen);
            this.Controls.Add(this.textRed);
            this.Controls.Add(this.trackRed);
            this.Controls.Add(this.labelRed);
            this.Name = "ControlLevelsProperties";
            this.Size = new System.Drawing.Size(181, 148);
            ((System.ComponentModel.ISupportInitialize)(this.trackRed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBlue)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textRed;
        private System.Windows.Forms.TrackBar trackRed;
        private System.Windows.Forms.Label labelRed;
        private System.Windows.Forms.TextBox textGreen;
        private System.Windows.Forms.TrackBar trackGreen;
        private System.Windows.Forms.Label labelGreen;
        private System.Windows.Forms.TextBox textBlue;
        private System.Windows.Forms.TrackBar trackBlue;
        private System.Windows.Forms.Label Blue;
    }
}
