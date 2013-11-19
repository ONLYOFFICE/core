namespace TestPPTtoPPTX
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
            this.button1 = new System.Windows.Forms.Button();
            this.m_oDoTransform = new System.Windows.Forms.Button();
            this.m_oSrcFilePPT = new System.Windows.Forms.TextBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.button3 = new System.Windows.Forms.Button();
            this.m_oDstPath = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(511, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Open File";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.OpenSourceFile);
            // 
            // m_oDoTransform
            // 
            this.m_oDoTransform.Location = new System.Drawing.Point(511, 74);
            this.m_oDoTransform.Name = "m_oDoTransform";
            this.m_oDoTransform.Size = new System.Drawing.Size(75, 23);
            this.m_oDoTransform.TabIndex = 1;
            this.m_oDoTransform.Text = "Transforn";
            this.m_oDoTransform.UseVisualStyleBackColor = true;
            this.m_oDoTransform.Click += new System.EventHandler(this.DoTransform);
            // 
            // m_oSrcFilePPT
            // 
            this.m_oSrcFilePPT.Location = new System.Drawing.Point(18, 12);
            this.m_oSrcFilePPT.Name = "m_oSrcFilePPT";
            this.m_oSrcFilePPT.Size = new System.Drawing.Size(487, 20);
            this.m_oSrcFilePPT.TabIndex = 2;
            this.m_oSrcFilePPT.Text = "E:\\\\OfficeTests\\\\PPTtoPPTX\\\\Empty.ppt";
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(18, 74);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(487, 23);
            this.progressBar1.TabIndex = 3;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(511, 41);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 4;
            this.button3.Text = "Destination";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // m_oDstPath
            // 
            this.m_oDstPath.Location = new System.Drawing.Point(18, 41);
            this.m_oDstPath.Name = "m_oDstPath";
            this.m_oDstPath.Size = new System.Drawing.Size(487, 20);
            this.m_oDstPath.TabIndex = 5;
            this.m_oDstPath.Text = "E:\\\\OfficeTests\\\\PPTtoPPTX";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(598, 120);
            this.Controls.Add(this.m_oDstPath);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.m_oSrcFilePPT);
            this.Controls.Add(this.m_oDoTransform);
            this.Controls.Add(this.button1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "PPT to PPTX";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button m_oDoTransform;
        private System.Windows.Forms.TextBox m_oSrcFilePPT;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox m_oDstPath;
    }
}

