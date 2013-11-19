namespace AVSOfficeFB2FileTest
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
            this.input_textbox = new System.Windows.Forms.TextBox();
            this.output_textbox = new System.Windows.Forms.TextBox();
            this.input_button = new System.Windows.Forms.Button();
            this.output_button = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.convert = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // input_textbox
            // 
            this.input_textbox.Location = new System.Drawing.Point(116, 15);
            this.input_textbox.Name = "input_textbox";
            this.input_textbox.Size = new System.Drawing.Size(624, 20);
            this.input_textbox.TabIndex = 0;
            // 
            // output_textbox
            // 
            this.output_textbox.Location = new System.Drawing.Point(116, 63);
            this.output_textbox.Name = "output_textbox";
            this.output_textbox.Size = new System.Drawing.Size(624, 20);
            this.output_textbox.TabIndex = 1;
            // 
            // input_button
            // 
            this.input_button.Location = new System.Drawing.Point(757, 15);
            this.input_button.Name = "input_button";
            this.input_button.Size = new System.Drawing.Size(52, 20);
            this.input_button.TabIndex = 2;
            this.input_button.Text = "input";
            this.input_button.UseVisualStyleBackColor = true;
            this.input_button.Click += new System.EventHandler(this.click_input);
            // 
            // output_button
            // 
            this.output_button.Location = new System.Drawing.Point(757, 63);
            this.output_button.Name = "output_button";
            this.output_button.Size = new System.Drawing.Size(52, 20);
            this.output_button.TabIndex = 3;
            this.output_button.Text = "output";
            this.output_button.UseVisualStyleBackColor = true;
            this.output_button.Click += new System.EventHandler(this.click_output);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Choose input file";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 66);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Choose output file";
            // 
            // convert
            // 
            this.convert.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.convert.ForeColor = System.Drawing.Color.Red;
            this.convert.Location = new System.Drawing.Point(834, 30);
            this.convert.Name = "convert";
            this.convert.Size = new System.Drawing.Size(119, 40);
            this.convert.TabIndex = 6;
            this.convert.Text = "FB2 to Docx";
            this.convert.UseVisualStyleBackColor = true;
            this.convert.Click += new System.EventHandler(this.click_convert);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(959, 102);
            this.Controls.Add(this.convert);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.output_button);
            this.Controls.Add(this.input_button);
            this.Controls.Add(this.output_textbox);
            this.Controls.Add(this.input_textbox);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox input_textbox;
        private System.Windows.Forms.TextBox output_textbox;
        private System.Windows.Forms.Button input_button;
        private System.Windows.Forms.Button output_button;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button convert;
    }
}

