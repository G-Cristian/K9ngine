using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OpenGLControl
{
    public partial class OpenGLControl: UserControl
    {
        public OpenGLControl()
        {
            InitializeComponent();
        }
		IntPtr hDC;
		IntPtr hRC;
		bool red = true;
		private void OpenGLControl_Load(object sender, EventArgs e)
		{
			if (this.DesignMode) return;

			int error = 0;

			//OpenGLWrapperClass.LoadLibrary(OpenGLWrapperClass.opengl_dll);
			//OpenGLWrapperClass.LoadLibrary(OpenGLWrapperClass.glew32_dll);
			IntPtr hWnd = this.Handle;
			error = Marshal.GetLastWin32Error();

			hDC = OpenGLWrapperClass.GetDC(hWnd);
			error = Marshal.GetLastWin32Error();

			OpenGLWrapperClass.PIXELFORMATDESCRIPTION pfd = OpenGLWrapperClass.GetDefaultPixelFormatDescriptor();
			int format = OpenGLWrapperClass.ChoosePixelFormat(hDC, pfd);
			error = Marshal.GetLastWin32Error();
			OpenGLWrapperClass.SetPixelFormat(hDC, format, pfd);
			error = Marshal.GetLastWin32Error();

			hRC = OpenGLWrapperClass.wglCreateContext(hDC);
			error = Marshal.GetLastWin32Error();
			
			OpenGLWrapperClass.wglMakeCurrent(hDC, hRC);
			error = Marshal.GetLastWin32Error();

			//OpenGLWrapperClass.initGlew();
			//error = Marshal.GetLastWin32Error();
			OpenGLWrapperClass.glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
			error = Marshal.GetLastWin32Error();
			//OpenGLWrapperClass.clearColorBuffer();
			//error = Marshal.GetLastWin32Error();

			//OpenGLWrapperClass.SwapBuffers(hDC);
			//error = Marshal.GetLastWin32Error();

			//OpenGLWrapperClass.setFrontFaceClockwise();

			//string error = OpenGLWrapperClass.GetOpenGLError(1);

			//MessageBox.Show(error);





		}

		private void OpenGLControl_Paint(object sender, PaintEventArgs e)
		{
			if (DesignMode) {
				return;
			}
			int error;

			OpenGLWrapperClass.wglMakeCurrent(hDC, hRC);
			error = Marshal.GetLastWin32Error();

			if (red)
			{
				OpenGLWrapperClass.glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
				red = false;
			}
			else
			{
				OpenGLWrapperClass.glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
				red = true;
			}
			IntPtr hdc = e.Graphics.GetHdc();
			error = Marshal.GetLastWin32Error();

			OpenGLWrapperClass.clearColorBuffer();
			error = Marshal.GetLastWin32Error();

			OpenGLWrapperClass.SwapBuffers(hdc);
			error = Marshal.GetLastWin32Error();

			e.Graphics.ReleaseHdc(hdc);
			error = Marshal.GetLastWin32Error();
		}
	}
}
