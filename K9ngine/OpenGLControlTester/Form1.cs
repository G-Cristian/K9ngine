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

		private void openGLControl1_Render(OpenGLControl.OpenGLControl sender, OpenGLControl.OpenGLControl.OpenGLControlEventArgs eventArgs)
		{
			OpenGLControl.OpenGL openGLInstance = eventArgs.OpenGLInstanse;
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

		private void openGLControl1_OnError(object sender, EventArgs e)
		{
			OpenGLControl.OpenGLControl.ErrorEventArgs errorEventArgs = e as OpenGLControl.OpenGLControl.ErrorEventArgs;
			if(errorEventArgs != null)
			{
				Console.WriteLine(errorEventArgs.Msg);
				Console.WriteLine(errorEventArgs.StackTrace);
			}
		}

		private void openGLControl1_OnInitialized(OpenGLControl.OpenGLControl sender, OpenGLControl.OpenGLControl.OpenGLControlEventArgs eventArgs)
		{
			//openGLControl1.CreateAndBindVertexArrayObject();
		}
	}
}
