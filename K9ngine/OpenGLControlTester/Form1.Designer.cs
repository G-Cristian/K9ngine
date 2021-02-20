
namespace OpenGLControlTester
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
			this.openGLControl1 = new OpenGLControl.OpenGLControl();
			this.SuspendLayout();
			// 
			// openGLControl1
			// 
			this.openGLControl1.AllowDebug = true;
			this.openGLControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.openGLControl1.Location = new System.Drawing.Point(0, 0);
			this.openGLControl1.MajorVersion = 0;
			this.openGLControl1.MinorVersion = 0;
			this.openGLControl1.Name = "openGLControl1";
			this.openGLControl1.Size = new System.Drawing.Size(800, 450);
			this.openGLControl1.TabIndex = 0;
			this.openGLControl1.Render += new OpenGLControl.OpenGLControl.RenderDelegate(this.openGLControl1_Render);
			this.openGLControl1.Load += new System.EventHandler(this.openGLControl1_Load);
			this.openGLControl1.DoubleClick += new System.EventHandler(this.openGLControl1_DoubleClick);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(800, 450);
			this.Controls.Add(this.openGLControl1);
			this.Name = "Form1";
			this.Text = "Form1";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
			this.ResumeLayout(false);

		}

		#endregion

		private OpenGLControl.OpenGLControl openGLControl1;
	}
}

