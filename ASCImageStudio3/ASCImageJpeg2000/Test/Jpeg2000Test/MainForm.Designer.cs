namespace Jpeg2000Test
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
            this.btSave = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.axImageView31 = new AxAVSImageView3.AxImageView3();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.btOptions = new System.Windows.Forms.Button();
            this.btClose = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).BeginInit();
            this.SuspendLayout();
            // 
            // btOpen
            // 
            this.btOpen.Location = new System.Drawing.Point(336, 12);
            this.btOpen.Name = "btOpen";
            this.btOpen.Size = new System.Drawing.Size(66, 32);
            this.btOpen.TabIndex = 0;
            this.btOpen.Text = "Open";
            this.btOpen.UseVisualStyleBackColor = true;
            this.btOpen.Click += new System.EventHandler(this.btOpen_Click);
            // 
            // btSave
            // 
            this.btSave.Location = new System.Drawing.Point(336, 50);
            this.btSave.Name = "btSave";
            this.btSave.Size = new System.Drawing.Size(66, 33);
            this.btSave.TabIndex = 1;
            this.btSave.Text = "Save";
            this.btSave.UseVisualStyleBackColor = true;
            this.btSave.Click += new System.EventHandler(this.btSave_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // axImageView31
            // 
            this.axImageView31.Enabled = true;
            this.axImageView31.Location = new System.Drawing.Point(12, 12);
            this.axImageView31.Name = "axImageView31";
            this.axImageView31.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axImageView31.OcxState")));
            this.axImageView31.Size = new System.Drawing.Size(318, 412);
            this.axImageView31.TabIndex = 2;
            // 
            // btOptions
            // 
            this.btOptions.Location = new System.Drawing.Point(336, 89);
            this.btOptions.Name = "btOptions";
            this.btOptions.Size = new System.Drawing.Size(66, 35);
            this.btOptions.TabIndex = 3;
            this.btOptions.Text = "Options";
            this.btOptions.UseVisualStyleBackColor = true;
            this.btOptions.Click += new System.EventHandler(this.btOptions_Click);
            // 
            // btClose
            // 
            this.btClose.Location = new System.Drawing.Point(336, 130);
            this.btClose.Name = "btClose";
            this.btClose.Size = new System.Drawing.Size(66, 35);
            this.btClose.TabIndex = 4;
            this.btClose.Text = "Close";
            this.btClose.UseVisualStyleBackColor = true;
            this.btClose.Click += new System.EventHandler(this.btClose_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(409, 436);
            this.Controls.Add(this.btClose);
            this.Controls.Add(this.btOptions);
            this.Controls.Add(this.axImageView31);
            this.Controls.Add(this.btSave);
            this.Controls.Add(this.btOpen);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "Тестируем JPEG2000";
            ((System.ComponentModel.ISupportInitialize)(this.axImageView31)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btOpen;
        private System.Windows.Forms.Button btSave;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private AxAVSImageView3.AxImageView3 axImageView31;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button btOptions;
        private System.Windows.Forms.Button btClose;
    }
}

