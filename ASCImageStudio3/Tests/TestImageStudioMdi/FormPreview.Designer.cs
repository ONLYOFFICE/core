namespace TestImageStudioMdi
{
    partial class FormPreview
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormPreview));
            this.axImageView31 = new AxAVSImageView3.AxImageView3();
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).BeginInit();
            this.SuspendLayout();
            // 
            // axImageView31
            // 
            this.axImageView31.Dock = System.Windows.Forms.DockStyle.Fill;
            this.axImageView31.Enabled = true;
            this.axImageView31.Location = new System.Drawing.Point(0, 0);
            this.axImageView31.Name = "axImageView31";
            this.axImageView31.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axImageView31.OcxState")));
            this.axImageView31.Size = new System.Drawing.Size(428, 362);
            this.axImageView31.TabIndex = 0;
            // 
            // FormPreview
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(428, 362);
            this.Controls.Add(this.axImageView31);
            this.Name = "FormPreview";
            this.Text = "Preview";
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private AxAVSImageView3.AxImageView3 axImageView31;
    }
}