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
	public partial class OpenGLControl : UserControl
	{
		#region Members

		private OpenGL _openGLInstance = null;
		private int _majorVersion = 0;
		private int _minorVersion = 0;

		private bool _allowDebug = true;

		#endregion

		#region Delegates/Events

		public class OpenGLControlEventArgs
		{
			public OpenGLControlEventArgs(OpenGL openGLInstance)
			{
				OpenGLInstanse = openGLInstance;
			}
			public OpenGL OpenGLInstanse { get; private set; }
		}

		public class ErrorEventArgs : EventArgs
		{
			public int LastWin32Error { get; private set; }
			public List<int> OpenGLErrors { get; private set; }
			public string Msg { get; private set; }
			public string StackTrace { get; private set; }
			public ErrorEventArgs(int lastWin32Error, List<int> openGLErrors, string msg, string stackTrace = ""):
				base()
			{
				LastWin32Error = lastWin32Error;
				OpenGLErrors = openGLErrors;
				Msg = msg;
				StackTrace = stackTrace;
			}
		}

		public delegate void OpenGLControlDelegate(OpenGLControl sender, OpenGLControlEventArgs eventArgs);

		public event OpenGLControlDelegate OnInitialized;

		public event OpenGLControlDelegate PreRender;
		public event OpenGLControlDelegate Render;
		public event OpenGLControlDelegate PostRender;

		public event EventHandler OnError;

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

			try
			{
				//OpenGLWrapperClass.LoadLibrary(OpenGLWrapperClass.opengl_dll);
				//OpenGLWrapperClass.LoadLibrary(OpenGLWrapperClass.glew32_dll);
				_openGLInstance = new OpenGL(this.Handle, _allowDebug);
				OpenGL.Settings settings = new OpenGL.Settings();
				//TODO: Get versions from properties
				settings.majorVersion = MajorVersion;
				settings.minorVersion = MinorVersion;

				_openGLInstance.Init(settings);


				RiseOpenGLEvent(OnInitialized);
			}
			catch(OpenGL.OpenGLException openGLException)
			{
				RiseErrorEvent(openGLException);
			}

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

		public uint CreateAndBindVertexArrayObject()
		{
			try
			{
				return _openGLInstance.CreateAndBindVertexArrayObject();
			}
			catch(OpenGL.OpenGLException e)
			{
				RiseErrorEvent(e);

				return 0;
			}
		}

		public uint CreateArrayBuffer(float[] attributes)
		{
			try
			{
				return _openGLInstance.CreateArrayBuffer(attributes);
			}
			catch (OpenGL.OpenGLException e)
			{
				RiseErrorEvent(e);

				return 0;
			}
		}

		public uint CreateElementArrayBuffer(int[] attributes)
		{
			try
			{
				return _openGLInstance.CreateElementArrayBuffer(attributes);
			}
			catch (OpenGL.OpenGLException e)
			{
				RiseErrorEvent(e);

				return 0;
			}
		}

		public void DrawScene()
		{
			RiseOpenGLEvent(PreRender);
			RiseOpenGLEvent(Render);
			RiseOpenGLEvent(PostRender);

			try
			{
				//TODO: This is inside a try because for some reason SwapBuffers sets last win32 error to 2
				// so I throw an exception
				_openGLInstance.SwapBuffers();
			}
			catch (OpenGL.OpenGLException e)
			{
				RiseErrorEvent(e);
			}
		}

		#endregion

		#region Helper Methods

		private void RiseOpenGLEvent(OpenGLControlDelegate openGLEvent)
		{
			OpenGLControlEventArgs eventArgs = new OpenGLControlEventArgs(_openGLInstance);
			if (openGLEvent != null)
			{
				Invoke(openGLEvent, this, eventArgs);
			}
		}

		private void RiseErrorEvent(ErrorEventArgs eventArgs)
		{
			if (OnError != null)
			{
				Invoke(OnError, this, eventArgs);
			}
		}

		private void RiseErrorEvent(OpenGL.OpenGLException throwingException)
		{
			ErrorEventArgs eventArgs = new ErrorEventArgs(	throwingException.LastWin32Error,
															throwingException.OpenGLErrors,
															throwingException.Message,
															throwingException.StackTrace);
			RiseErrorEvent(eventArgs);
		}

		#endregion
	}
}
