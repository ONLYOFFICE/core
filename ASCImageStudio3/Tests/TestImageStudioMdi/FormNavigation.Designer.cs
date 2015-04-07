namespace TestImageStudioMdi
{
    partial class FormNavigation
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
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.numericStartTime = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.numericStep = new System.Windows.Forms.NumericUpDown();
            this.buttonRun = new System.Windows.Forms.Button();
            this.checkBoxUpdatePreview = new System.Windows.Forms.CheckBox();
            this.numericCurrent = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.numericEndTime = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxPath2 = new System.Windows.Forms.TextBox();
            this.buttonBrowse2 = new System.Windows.Forms.Button();
            this.textBoxPath1 = new System.Windows.Forms.TextBox();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.textBoxPath3 = new System.Windows.Forms.TextBox();
            this.buttonBrowse3 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.numericStartTime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericStep)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericCurrent)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericEndTime)).BeginInit();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "All Files|*.*";
            this.openFileDialog1.Title = "Open Files";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 128);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Start time";
            // 
            // numericStartTime
            // 
            this.numericStartTime.Location = new System.Drawing.Point(12, 144);
            this.numericStartTime.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.numericStartTime.Name = "numericStartTime";
            this.numericStartTime.Size = new System.Drawing.Size(89, 20);
            this.numericStartTime.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(199, 128);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Time step";
            // 
            // numericStep
            // 
            this.numericStep.Location = new System.Drawing.Point(202, 144);
            this.numericStep.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.numericStep.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericStep.Name = "numericStep";
            this.numericStep.Size = new System.Drawing.Size(89, 20);
            this.numericStep.TabIndex = 7;
            this.numericStep.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // buttonRun
            // 
            this.buttonRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRun.Location = new System.Drawing.Point(299, 196);
            this.buttonRun.Name = "buttonRun";
            this.buttonRun.Size = new System.Drawing.Size(86, 23);
            this.buttonRun.TabIndex = 11;
            this.buttonRun.Text = "Run";
            this.buttonRun.UseVisualStyleBackColor = true;
            this.buttonRun.Click += new System.EventHandler(this.buttonRun_Click);
            // 
            // checkBoxUpdatePreview
            // 
            this.checkBoxUpdatePreview.AutoSize = true;
            this.checkBoxUpdatePreview.Checked = true;
            this.checkBoxUpdatePreview.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxUpdatePreview.Location = new System.Drawing.Point(12, 202);
            this.checkBoxUpdatePreview.Name = "checkBoxUpdatePreview";
            this.checkBoxUpdatePreview.Size = new System.Drawing.Size(101, 17);
            this.checkBoxUpdatePreview.TabIndex = 10;
            this.checkBoxUpdatePreview.Text = "Update preview";
            this.checkBoxUpdatePreview.UseVisualStyleBackColor = true;
            // 
            // numericCurrent
            // 
            this.numericCurrent.Location = new System.Drawing.Point(297, 144);
            this.numericCurrent.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.numericCurrent.Name = "numericCurrent";
            this.numericCurrent.Size = new System.Drawing.Size(89, 20);
            this.numericCurrent.TabIndex = 9;
            this.numericCurrent.ValueChanged += new System.EventHandler(this.numericCurrent_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(294, 128);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(63, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Current time";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 10;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // numericEndTime
            // 
            this.numericEndTime.Location = new System.Drawing.Point(107, 144);
            this.numericEndTime.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.numericEndTime.Name = "numericEndTime";
            this.numericEndTime.Size = new System.Drawing.Size(89, 20);
            this.numericEndTime.TabIndex = 5;
            this.numericEndTime.Value = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.numericEndTime.ValueChanged += new System.EventHandler(this.numericEndTime_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(104, 128);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(48, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "End time";
            // 
            // textBoxPath2
            // 
            this.textBoxPath2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPath2.HideSelection = false;
            this.textBoxPath2.Location = new System.Drawing.Point(12, 38);
            this.textBoxPath2.Name = "textBoxPath2";
            this.textBoxPath2.ReadOnly = true;
            this.textBoxPath2.Size = new System.Drawing.Size(336, 20);
            this.textBoxPath2.TabIndex = 28;
            // 
            // buttonBrowse2
            // 
            this.buttonBrowse2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonBrowse2.Location = new System.Drawing.Point(354, 38);
            this.buttonBrowse2.Name = "buttonBrowse2";
            this.buttonBrowse2.Size = new System.Drawing.Size(31, 20);
            this.buttonBrowse2.TabIndex = 29;
            this.buttonBrowse2.Text = "...";
            this.buttonBrowse2.UseVisualStyleBackColor = true;
            this.buttonBrowse2.Click += new System.EventHandler(this.buttonBrowse2_Click);
            // 
            // textBoxPath1
            // 
            this.textBoxPath1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPath1.HideSelection = false;
            this.textBoxPath1.Location = new System.Drawing.Point(12, 12);
            this.textBoxPath1.Name = "textBoxPath1";
            this.textBoxPath1.ReadOnly = true;
            this.textBoxPath1.Size = new System.Drawing.Size(336, 20);
            this.textBoxPath1.TabIndex = 26;
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonBrowse.Location = new System.Drawing.Point(354, 12);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(31, 20);
            this.buttonBrowse.TabIndex = 27;
            this.buttonBrowse.Text = "...";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click_1);
            // 
            // textBoxPath3
            // 
            this.textBoxPath3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPath3.HideSelection = false;
            this.textBoxPath3.Location = new System.Drawing.Point(12, 64);
            this.textBoxPath3.Name = "textBoxPath3";
            this.textBoxPath3.ReadOnly = true;
            this.textBoxPath3.Size = new System.Drawing.Size(336, 20);
            this.textBoxPath3.TabIndex = 30;
            // 
            // buttonBrowse3
            // 
            this.buttonBrowse3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonBrowse3.Location = new System.Drawing.Point(354, 64);
            this.buttonBrowse3.Name = "buttonBrowse3";
            this.buttonBrowse3.Size = new System.Drawing.Size(31, 20);
            this.buttonBrowse3.TabIndex = 31;
            this.buttonBrowse3.Text = "...";
            this.buttonBrowse3.UseVisualStyleBackColor = true;
            this.buttonBrowse3.Click += new System.EventHandler(this.buttonBrowse3_Click);
            // 
            // FormNavigation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(397, 237);
            this.ControlBox = false;
            this.Controls.Add(this.textBoxPath3);
            this.Controls.Add(this.buttonBrowse3);
            this.Controls.Add(this.textBoxPath2);
            this.Controls.Add(this.buttonBrowse2);
            this.Controls.Add(this.textBoxPath1);
            this.Controls.Add(this.buttonBrowse);
            this.Controls.Add(this.numericEndTime);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.numericCurrent);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.checkBoxUpdatePreview);
            this.Controls.Add(this.buttonRun);
            this.Controls.Add(this.numericStep);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numericStartTime);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "FormNavigation";
            this.Text = "Preview Navigation";
            ((System.ComponentModel.ISupportInitialize)(this.numericStartTime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericStep)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericCurrent)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericEndTime)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericStartTime;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericStep;
        private System.Windows.Forms.Button buttonRun;
        private System.Windows.Forms.CheckBox checkBoxUpdatePreview;
        private System.Windows.Forms.NumericUpDown numericCurrent;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.NumericUpDown numericEndTime;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxPath2;
        private System.Windows.Forms.Button buttonBrowse2;
        private System.Windows.Forms.TextBox textBoxPath1;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.TextBox textBoxPath3;
        private System.Windows.Forms.Button buttonBrowse3;
    }
}