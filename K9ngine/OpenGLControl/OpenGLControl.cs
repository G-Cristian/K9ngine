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
		#region Members

		private OpenGL _openGLInstance= null;
		private int _majorVersion = 0;
		private int _minorVersion = 0;

		private bool _allowDebug = true;

		#endregion

		#region Delegates/Events
		public delegate void RenderDelegate(OpenGL openGLInstance);

		public event RenderDelegate PreRender;
		public event RenderDelegate Render;
		public event RenderDelegate PostRender;

		#endregion

		#region Properties

		public int MajorVersion {
			get
			{
				return _majorVersion;
			}
			set
			{
				_majorVersion = (value < 0 ? 0 : value);
			}
		}
		public int MinorVersion
		{
			get
			{
				return _minorVersion;
			}
			set
			{
				_minorVersion = (value < 0 ? 0 : value);
			}
		}
		public bool AllowDebug
		{
			get
			{
				return _allowDebug;
			}
			set
			{
				_allowDebug = value;
			}
		}

		#endregion

			#region Control Methods

		public OpenGLControl()
        {
            InitializeComponent();
        }

		bool red = true;
		private void OpenGLControl_Load(object sender, EventArgs e)
		{
			if (this.DesignMode) return;

			//OpenGLWrapperClass.LoadLibrary(OpenGLWrapperClass.opengl_dll);
			//OpenGLWrapperClass.LoadLibrary(OpenGLWrapperClass.glew32_dll);
			_openGLInstance = new OpenGL(this.Handle, _allowDebug);
			OpenGL.Settings settings = new OpenGL.Settings();
			//TODO: Get versions from properties
			settings.majorVersion = MajorVersion;
			settings.minorVersion = MinorVersion;

			_openGLInstance.Init(settings);


			//OpenGLWrapperClass.initGlew();
			//error = Marshal.GetLastWin32Error();

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
			
			//IntPtr hdc = e.Graphics.GetHdc();
			//error = Marshal.GetLastWin32Error();

			//OpenGLWrapperClass.clearColorBuffer();
			//error = Marshal.GetLastWin32Error();

			//OpenGLWrapperClass.SwapBuffers(hdc);
			//error = Marshal.GetLastWin32Error();

			//e.Graphics.ReleaseHdc(hdc);
			//error = Marshal.GetLastWin32Error();
		}

		#endregion

		#region Custom Methods

		public void DrawScene()
		{
			if(PreRender != null)
			{
				PreRender(_openGLInstance);
			}

			if(Render != null)
			{
				Render(_openGLInstance);
			}

			if (PostRender != null)
			{
				PostRender(_openGLInstance);
			}

			try
			{
				//TODO: This is inside a try because for some reason SwapBuffers sets last win32 error to 2
				// so I throw an exception
				_openGLInstance.SwapBuffers();
			}
			catch(Exception e)
			{
				if (_allowDebug)
				{
					Console.WriteLine(e.Message);
				}
			}
		}

		

		#endregion

		#region Helper Methods
		
		#endregion
	}
}
