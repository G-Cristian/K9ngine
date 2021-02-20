using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OpenGLControlTester
{
	public partial class Form1 : Form
	{
		bool isRed = true; //TODO: Remove. For testing
		public Form1()
		{
			InitializeComponent();
		}

		private void openGLControl1_Load(object sender, EventArgs e)
		{

		}

		private void openGLControl1_DoubleClick(object sender, EventArgs e)
		{
			OpenGLControl.OpenGLControl control = sender as OpenGLControl.OpenGLControl;

			if(control != null)
			{
				control.DrawScene();
			}
		}

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{

		}

		private void openGLControl1_Render(OpenGLControl.OpenGL openGLInstance)
		{
			//For testing
			if (isRed)
			{
				openGLInstance.ClearColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				openGLInstance.ClearColorBuffer(0.0f, 1.0f, 0.0f, 1.0f);
			}

			isRed = !isRed;
		}
	}
}
