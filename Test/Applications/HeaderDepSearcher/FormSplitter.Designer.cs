namespace HeaderDepSearcher
{
    partial class FormSplitter
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
            this.textFilePath = new System.Windows.Forms.TextBox();
            this.labelFilePath = new System.Windows.Forms.Label();
            this.buttonSplit = new System.Windows.Forms.Button();
            this.labelFileProcess = new System.Windows.Forms.Label();
            this.labelInfo = new System.Windows.Forms.Label();
            this.buttonDir = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textFilePath
            // 
            this.textFilePath.Location = new System.Drawing.Point(28, 35);
            this.textFilePath.Name = "textFilePath";
            this.textFilePath.Size = new System.Drawing.Size(523, 20);
            this.textFilePath.TabIndex = 3;
            // 
            // labelFilePath
            // 
            this.labelFilePath.AutoSize = true;
            this.labelFilePath.Location = new System.Drawing.Point(25, 19);
            this.labelFilePath.Name = "labelFilePath";
            this.labelFilePath.Size = new System.Drawing.Size(76, 13);
            this.labelFilePath.TabIndex = 2;
            this.labelFilePath.Text = "Class File Path";
            // 
            // buttonSplit
            // 
            this.buttonSplit.Location = new System.Drawing.Point(270, 74);
            this.buttonSplit.Name = "buttonSplit";
            this.buttonSplit.Size = new System.Drawing.Size(75, 23);
            this.buttonSplit.TabIndex = 4;
            this.buttonSplit.Text = "Split";
            this.buttonSplit.UseVisualStyleBackColor = true;
            this.buttonSplit.Click += new System.EventHandler(this.buttonSplit_Click);
            // 
            // labelFileProcess
            // 
            this.labelFileProcess.AutoSize = true;
            this.labelFileProcess.Location = new System.Drawing.Point(25, 79);
            this.labelFileProcess.Name = "labelFileProcess";
            this.labelFileProcess.Size = new System.Drawing.Size(0, 13);
            this.labelFileProcess.TabIndex = 10;
            // 
            // labelInfo
            // 
            this.labelInfo.Location = new System.Drawing.Point(25, 109);
            this.labelInfo.Name = "labelInfo";
            this.labelInfo.Size = new System.Drawing.Size(562, 46);
            this.labelInfo.TabIndex = 11;
            this.labelInfo.Text = "* Supported: ComplexTypes.h, SimpleTypes_Base.h, SimpleTypes_Drawing.h, SimpleTyp" +
    "es_OMath.h, SimpleTypes_Shared.h, SimpleTypes_Vml.h, SimpleTypes_Word.h";
            // 
            // buttonDir
            // 
            this.buttonDir.Location = new System.Drawing.Point(557, 34);
            this.buttonDir.Name = "buttonDir";
            this.buttonDir.Size = new System.Drawing.Size(30, 22);
            this.buttonDir.TabIndex = 12;
            this.buttonDir.Text = "...";
            this.buttonDir.UseVisualStyleBackColor = true;
            this.buttonDir.Click += new System.EventHandler(this.buttonDir_Click);
            // 
            // FormSplitter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(615, 164);
            this.Controls.Add(this.buttonDir);
            this.Controls.Add(this.labelInfo);
            this.Controls.Add(this.labelFileProcess);
            this.Controls.Add(this.buttonSplit);
            this.Controls.Add(this.textFilePath);
            this.Controls.Add(this.labelFilePath);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormSplitter";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Class Splitter";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textFilePath;
        private System.Windows.Forms.Label labelFilePath;
        private System.Windows.Forms.Button buttonSplit;
        private System.Windows.Forms.Label labelFileProcess;
        private System.Windows.Forms.Label labelInfo;
        private System.Windows.Forms.Button buttonDir;
    }
}