namespace PDFTest
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
            this.components = new System.ComponentModel.Container();
            this.btSave = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.btOpen = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btInstallPrinter = new System.Windows.Forms.Button();
            this.btUnInstallPrinter = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btPrint = new System.Windows.Forms.Button();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.btSimplePrint = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btSave
            // 
            this.btSave.Location = new System.Drawing.Point(6, 59);
            this.btSave.Name = "btSave";
            this.btSave.Size = new System.Drawing.Size(79, 37);
            this.btSave.TabIndex = 0;
            this.btSave.Text = "Save";
            this.btSave.UseVisualStyleBackColor = true;
            this.btSave.Click += new System.EventHandler(this.button1_Click);
            // 
            // btOpen
            // 
            this.btOpen.Location = new System.Drawing.Point(6, 19);
            this.btOpen.Name = "btOpen";
            this.btOpen.Size = new System.Drawing.Size(79, 36);
            this.btOpen.TabIndex = 1;
            this.btOpen.Text = "Open";
            this.btOpen.UseVisualStyleBackColor = true;
            this.btOpen.Click += new System.EventHandler(this.button2_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // btInstallPrinter
            // 
            this.btInstallPrinter.Location = new System.Drawing.Point(6, 13);
            this.btInstallPrinter.Name = "btInstallPrinter";
            this.btInstallPrinter.Size = new System.Drawing.Size(91, 40);
            this.btInstallPrinter.TabIndex = 2;
            this.btInstallPrinter.Text = "InstallPrinter";
            this.btInstallPrinter.UseVisualStyleBackColor = true;
            this.btInstallPrinter.Click += new System.EventHandler(this.button3_Click);
            // 
            // btUnInstallPrinter
            // 
            this.btUnInstallPrinter.Location = new System.Drawing.Point(6, 59);
            this.btUnInstallPrinter.Name = "btUnInstallPrinter";
            this.btUnInstallPrinter.Size = new System.Drawing.Size(91, 40);
            this.btUnInstallPrinter.TabIndex = 3;
            this.btUnInstallPrinter.Text = "UnInstallPrinter";
            this.btUnInstallPrinter.UseVisualStyleBackColor = true;
            this.btUnInstallPrinter.Click += new System.EventHandler(this.btUnInstallPrinter_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btOpen);
            this.groupBox1.Controls.Add(this.btSave);
            this.groupBox1.Location = new System.Drawing.Point(6, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(100, 102);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "PS->PDF";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btInstallPrinter);
            this.groupBox2.Controls.Add(this.btUnInstallPrinter);
            this.groupBox2.Location = new System.Drawing.Point(112, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(104, 101);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Printer";
            // 
            // btPrint
            // 
            this.btPrint.Location = new System.Drawing.Point(16, 118);
            this.btPrint.Name = "btPrint";
            this.btPrint.Size = new System.Drawing.Size(74, 35);
            this.btPrint.TabIndex = 6;
            this.btPrint.Text = "Print";
            this.btPrint.UseVisualStyleBackColor = true;
            this.btPrint.Click += new System.EventHandler(this.btPrint_Click);
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Interval = 10;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // btSimplePrint
            // 
            this.btSimplePrint.Location = new System.Drawing.Point(121, 120);
            this.btSimplePrint.Name = "btSimplePrint";
            this.btSimplePrint.Size = new System.Drawing.Size(87, 32);
            this.btSimplePrint.TabIndex = 7;
            this.btSimplePrint.Text = "SimplePrint";
            this.btSimplePrint.UseVisualStyleBackColor = true;
            this.btSimplePrint.Click += new System.EventHandler(this.btSimplePrint_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(218, 159);
            this.Controls.Add(this.btSimplePrint);
            this.Controls.Add(this.btPrint);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "MainForm";
            this.Text = "Тестим PDF";
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btSave;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button btOpen;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btInstallPrinter;
        private System.Windows.Forms.Button btUnInstallPrinter;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btPrint;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Button btSimplePrint;

    }
}

