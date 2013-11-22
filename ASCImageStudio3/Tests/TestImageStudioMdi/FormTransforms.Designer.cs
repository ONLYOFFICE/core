namespace TestImageStudioMdi
{
    partial class FormTransforms
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.treeViewPresets = new System.Windows.Forms.TreeView();
            this.textBoxXml = new System.Windows.Forms.TextBox();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.menuFormat = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.treeViewPresets);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.textBoxXml);
            this.splitContainer1.Size = new System.Drawing.Size(672, 456);
            this.splitContainer1.SplitterDistance = 224;
            this.splitContainer1.TabIndex = 0;
            // 
            // treeViewPresets
            // 
            this.treeViewPresets.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewPresets.HideSelection = false;
            this.treeViewPresets.Location = new System.Drawing.Point(0, 0);
            this.treeViewPresets.Name = "treeViewPresets";
            this.treeViewPresets.Size = new System.Drawing.Size(224, 456);
            this.treeViewPresets.TabIndex = 17;
            this.treeViewPresets.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewPresets_AfterSelect);
            // 
            // textBoxXml
            // 
            this.textBoxXml.ContextMenuStrip = this.contextMenuStrip1;
            this.textBoxXml.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxXml.HideSelection = false;
            this.textBoxXml.Location = new System.Drawing.Point(0, 0);
            this.textBoxXml.Multiline = true;
            this.textBoxXml.Name = "textBoxXml";
            this.textBoxXml.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxXml.Size = new System.Drawing.Size(444, 456);
            this.textBoxXml.TabIndex = 2;
            this.textBoxXml.WordWrap = false;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFormat});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(120, 26);
            // 
            // menuFormat
            // 
            this.menuFormat.Name = "menuFormat";
            this.menuFormat.Size = new System.Drawing.Size(119, 22);
            this.menuFormat.Text = "Format";
            this.menuFormat.Click += new System.EventHandler(this.menuFormat_Click);
            // 
            // FormTransforms
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(672, 456);
            this.Controls.Add(this.splitContainer1);
            this.Name = "FormTransforms";
            this.Text = "Available Transforms";
            this.Load += new System.EventHandler(this.FormTransforms_Load);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TreeView treeViewPresets;
        private System.Windows.Forms.TextBox textBoxXml;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuFormat;


    }
}