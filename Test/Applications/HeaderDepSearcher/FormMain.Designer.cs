namespace HeaderDepSearcher
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
            this.labelDir = new System.Windows.Forms.Label();
            this.textBoxDir = new System.Windows.Forms.TextBox();
            this.buttonSearch = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.labelIncludeLimit = new System.Windows.Forms.Label();
            this.textBoxIncludeLimit = new System.Windows.Forms.TextBox();
            this.textBoxIncludeLimitStd = new System.Windows.Forms.TextBox();
            this.labelIncludeLimitStd = new System.Windows.Forms.Label();
            this.labelFileProcess = new System.Windows.Forms.Label();
            this.buttonClassSplitter = new System.Windows.Forms.Button();
            this.buttonDir = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // labelDir
            // 
            this.labelDir.AutoSize = true;
            this.labelDir.Location = new System.Drawing.Point(25, 18);
            this.labelDir.Name = "labelDir";
            this.labelDir.Size = new System.Drawing.Size(122, 13);
            this.labelDir.TabIndex = 0;
            this.labelDir.Text = "Project Source Directory";
            // 
            // textBoxDir
            // 
            this.textBoxDir.Location = new System.Drawing.Point(28, 34);
            this.textBoxDir.Name = "textBoxDir";
            this.textBoxDir.Size = new System.Drawing.Size(523, 20);
            this.textBoxDir.TabIndex = 1;
            // 
            // buttonSearch
            // 
            this.buttonSearch.Location = new System.Drawing.Point(270, 107);
            this.buttonSearch.Name = "buttonSearch";
            this.buttonSearch.Size = new System.Drawing.Size(75, 23);
            this.buttonSearch.TabIndex = 2;
            this.buttonSearch.Text = "Search";
            this.buttonSearch.UseVisualStyleBackColor = true;
            this.buttonSearch.Click += new System.EventHandler(this.buttonSearch_Click);
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(370, 112);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(217, 11);
            this.progressBar.TabIndex = 3;
            this.progressBar.Visible = false;
            // 
            // labelIncludeLimit
            // 
            this.labelIncludeLimit.AutoSize = true;
            this.labelIncludeLimit.Location = new System.Drawing.Point(25, 74);
            this.labelIncludeLimit.Name = "labelIncludeLimit";
            this.labelIncludeLimit.Size = new System.Drawing.Size(99, 13);
            this.labelIncludeLimit.TabIndex = 4;
            this.labelIncludeLimit.Text = "Header include limit";
            // 
            // textBoxIncludeLimit
            // 
            this.textBoxIncludeLimit.Location = new System.Drawing.Point(128, 71);
            this.textBoxIncludeLimit.Name = "textBoxIncludeLimit";
            this.textBoxIncludeLimit.Size = new System.Drawing.Size(42, 20);
            this.textBoxIncludeLimit.TabIndex = 5;
            this.textBoxIncludeLimit.Text = "5";
            // 
            // textBoxIncludeLimitStd
            // 
            this.textBoxIncludeLimitStd.Location = new System.Drawing.Point(304, 71);
            this.textBoxIncludeLimitStd.Name = "textBoxIncludeLimitStd";
            this.textBoxIncludeLimitStd.Size = new System.Drawing.Size(42, 20);
            this.textBoxIncludeLimitStd.TabIndex = 8;
            this.textBoxIncludeLimitStd.Text = "2";
            // 
            // labelIncludeLimitStd
            // 
            this.labelIncludeLimitStd.AutoSize = true;
            this.labelIncludeLimitStd.Location = new System.Drawing.Point(184, 74);
            this.labelIncludeLimitStd.Name = "labelIncludeLimitStd";
            this.labelIncludeLimitStd.Size = new System.Drawing.Size(118, 13);
            this.labelIncludeLimitStd.TabIndex = 7;
            this.labelIncludeLimitStd.Text = "Header Std include limit";
            // 
            // labelFileProcess
            // 
            this.labelFileProcess.AutoSize = true;
            this.labelFileProcess.Location = new System.Drawing.Point(25, 112);
            this.labelFileProcess.Name = "labelFileProcess";
            this.labelFileProcess.Size = new System.Drawing.Size(0, 13);
            this.labelFileProcess.TabIndex = 9;
            // 
            // buttonClassSplitter
            // 
            this.buttonClassSplitter.Location = new System.Drawing.Point(271, 136);
            this.buttonClassSplitter.Name = "buttonClassSplitter";
            this.buttonClassSplitter.Size = new System.Drawing.Size(75, 23);
            this.buttonClassSplitter.TabIndex = 10;
            this.buttonClassSplitter.Text = "Class Splitter";
            this.buttonClassSplitter.UseVisualStyleBackColor = true;
            this.buttonClassSplitter.Click += new System.EventHandler(this.buttonClassSplitter_Click);
            // 
            // buttonDir
            // 
            this.buttonDir.Location = new System.Drawing.Point(557, 33);
            this.buttonDir.Name = "buttonDir";
            this.buttonDir.Size = new System.Drawing.Size(30, 22);
            this.buttonDir.TabIndex = 11;
            this.buttonDir.Text = "...";
            this.buttonDir.UseVisualStyleBackColor = true;
            this.buttonDir.Click += new System.EventHandler(this.buttonDir_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(615, 166);
            this.Controls.Add(this.buttonDir);
            this.Controls.Add(this.buttonClassSplitter);
            this.Controls.Add(this.labelFileProcess);
            this.Controls.Add(this.textBoxIncludeLimitStd);
            this.Controls.Add(this.labelIncludeLimitStd);
            this.Controls.Add(this.textBoxIncludeLimit);
            this.Controls.Add(this.labelIncludeLimit);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.buttonSearch);
            this.Controls.Add(this.textBoxDir);
            this.Controls.Add(this.labelDir);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Header Dependency Searcher [core]";
            this.Shown += new System.EventHandler(this.FormMain_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelDir;
        private System.Windows.Forms.TextBox textBoxDir;
        private System.Windows.Forms.Button buttonSearch;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Label labelIncludeLimit;
        private System.Windows.Forms.TextBox textBoxIncludeLimit;
        private System.Windows.Forms.TextBox textBoxIncludeLimitStd;
        private System.Windows.Forms.Label labelIncludeLimitStd;
        private System.Windows.Forms.Label labelFileProcess;
        private System.Windows.Forms.Button buttonClassSplitter;
        private System.Windows.Forms.Button buttonDir;
    }
}

