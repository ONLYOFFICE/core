namespace TestMetaData
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.btOpen = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.axImageView31 = new AxAVSImageView3.AxImageView3();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.btMetaData = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).BeginInit();
            this.SuspendLayout();
            // 
            // btOpen
            // 
            this.btOpen.Location = new System.Drawing.Point(12, 3);
            this.btOpen.Name = "btOpen";
            this.btOpen.Size = new System.Drawing.Size(73, 32);
            this.btOpen.TabIndex = 0;
            this.btOpen.Text = "Open";
            this.btOpen.UseVisualStyleBackColor = true;
            this.btOpen.Click += new System.EventHandler(this.btOpen_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(91, 10);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(440, 20);
            this.textBox1.TabIndex = 1;
            // 
            // axImageView31
            // 
            this.axImageView31.Enabled = true;
            this.axImageView31.Location = new System.Drawing.Point(12, 41);
            this.axImageView31.Name = "axImageView31";
            this.axImageView31.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axImageView31.OcxState")));
            this.axImageView31.Size = new System.Drawing.Size(283, 362);
            this.axImageView31.TabIndex = 4;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(301, 41);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(316, 362);
            this.richTextBox1.TabIndex = 3;
            this.richTextBox1.Text = "";
            // 
            // btMetaData
            // 
            this.btMetaData.Location = new System.Drawing.Point(537, 3);
            this.btMetaData.Name = "btMetaData";
            this.btMetaData.Size = new System.Drawing.Size(79, 32);
            this.btMetaData.TabIndex = 5;
            this.btMetaData.Text = "MetaData";
            this.btMetaData.UseVisualStyleBackColor = true;
            this.btMetaData.Click += new System.EventHandler(this.btMetaData_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(628, 415);
            this.Controls.Add(this.btMetaData);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.axImageView31);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.btOpen);
            this.Name = "MainForm";
            this.Text = "TestMetaData";
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btOpen;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.TextBox textBox1;
        public AxAVSImageView3.AxImageView3 axImageView31;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button btMetaData;
        //public object oImage;
        //readonly AVSImageFile3.ImageFile3Class m_ImageFile = new AVSImageFile3.ImageFile3Class();
    }
}

