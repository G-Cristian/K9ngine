using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLControl
{
	public class OpenGL : IDisposable
	{

		#region Structs & classes

		public struct Settings
		{
			public int majorVersion;
			public int minorVersion;
		}

		public class OpenGLException : Exception
		{
			public int LastWin32Error { get; private set; }
			public List<int> OpenGLErrors { get; private set; }
			public OpenGLException(int lastWin32Error, List<int> openGLErrors, string msg) :
				base(msg)
			{
				LastWin32Error = lastWin32Error;
				OpenGLErrors = openGLErrors;
			}
		}

		#endregion

		#region Members

		bool _disposed = false;
		private IntPtr _hWnd;
		private IntPtr _hDC;
		private IntPtr _hRC;

		#endregion

		#region Properties

		public bool AllowDebug { get; private set; }

		#endregion

		#region Constructor, Destructor, Dispose

		public OpenGL(IntPtr hWnd, bool allowDebug)
		{
			_hWnd = hWnd;
			AllowDebug = allowDebug;
		}

		~OpenGL() => Dispose(false);

		public void Dispose()
		{
			Dispose(true);

			GC.SuppressFinalize(true);
		}
		private void Dispose(bool disposing)
		{
			if (_disposed)
			{
				return;
			}

			if (disposing)
			{
				//TODO: Release hdc, hrc
				OpenGLWrapperClass.wglDeleteContext(_hRC);
				OpenGLWrapperClass.ReleaseDC(_hWnd, _hDC);
			}

			//OpenGLWrapperClass.CloseHandle(_hRC);
			//OpenGLWrapperClass.CloseHandle(_hDC);
			_hRC = IntPtr.Zero;
			_hDC = IntPtr.Zero;

			_disposed = true;
		}

		#endregion

		#region Initialization

		public void Init(Settings settings)
		{
			CreateDummyContext();

			if (settings.majorVersion == 0)
			{
				CreateLatestAvailableVersion();
			}
			else
			{
				CreateSpecificVersion(settings.majorVersion, settings.minorVersion);
			}
		}

		private void CreateSpecificVersion(int majorVersion, int minorVersion)
		{
			//TODO: If I'm not wrong this only work for OpenGL version >= 3.2
			if (majorVersion > 3 || (majorVersion == 3 && minorVersion >= 2))
			{
				int[] attribs = {
								OpenGLWrapperClass.WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
								OpenGLWrapperClass.WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
								OpenGLWrapperClass.WGL_CONTEXT_PROFILE_MASK_ARB, OpenGLWrapperClass.WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
								//OpenGLWrapperClass.WGL_CONTEXT_FLAGS_ARB, OpenGLWrapperClass.GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT,
								0
				};

				IntPtr newHRC = OpenGLWrapperClass.WGLCreateContextAttribsARB(_hDC, IntPtr.Zero, attribs);
				ThrowIfError();
				if (newHRC != IntPtr.Zero)
				{
					OpenGLWrapperClass.wglDeleteContext(_hRC);
					ThrowLatsWin32Error();
					OpenGLWrapperClass.wglMakeCurrent(_hDC, newHRC);
					ThrowIfError();
					_hRC = newHRC;
				}
			}
			else
			{
				throw new Exception("Cannot create OpenGL context <= 3.2");
			}
		}

		private void CreateLatestAvailableVersion()
		{
			int error = 0;
			int majorVersion = 0;
			int minorVersion = 0;

			GetLatestAvailableVersion(out majorVersion, out minorVersion);
			CreateSpecificVersion(majorVersion, minorVersion);
		}

		private void GetLatestAvailableVersion(out int majorVersion, out int minorVersion)
		{
			try
			{
				string version = OpenGLWrapperClass.glGetString(OpenGLWrapperClass.GL_VERSION);

				string[] majorMinorVersions = version.Split('.', ' ');

				int.TryParse(majorMinorVersions[0], out majorVersion);
				int.TryParse(majorMinorVersions[1], out minorVersion);
			}
			catch(Exception e)
			{
				majorVersion = minorVersion = 0;
			}

		}

		private void CreateDummyContext()
		{
			InitHDC();
			InitPixelFormat();
			InitRenderingContext();
		}

		private void InitHDC()
		{
			_hDC = OpenGLWrapperClass.GetDC(_hWnd);
			ThrowLatsWin32Error();
		}

		private void InitPixelFormat()
		{
			OpenGLWrapperClass.PIXELFORMATDESCRIPTION pfd = OpenGLWrapperClass.GetDefaultPixelFormatDescriptor();
			int format = OpenGLWrapperClass.ChoosePixelFormat(_hDC, pfd);
			ThrowLatsWin32Error();

			OpenGLWrapperClass.SetPixelFormat(_hDC, format, pfd);
			ThrowLatsWin32Error();
		}

		private void InitRenderingContext()
		{
			_hRC = OpenGLWrapperClass.wglCreateContext(_hDC);
			ThrowLatsWin32Error();

			OpenGLWrapperClass.wglMakeCurrent(_hDC, _hRC);
			ThrowIfError();
		}

		#endregion

		#region Helper Methods

		private void ThrowLatsWin32Error()
		{
			if (AllowDebug)
			{
				string msg = string.Empty;
				int error = Marshal.GetLastWin32Error();
				if (error != 0)
				{
					msg = error.ToString();
				}

				if (!string.IsNullOrEmpty(msg))
				{
					throw new Exception(msg);
				}
			}
		}

		private void ThrowOpenGLError()
		{
			if (AllowDebug)
			{
				string msg = string.Empty;

				int[] errors = new int[1];
				bool[] moreErrors = new bool[1];
				do
				{
					int numberOfErrors = OpenGLWrapperClass.getOpenGLError(errors, 1, moreErrors);

					for (int i = 0; i < numberOfErrors; ++i)
					{
						int intError = 0;
						msg += errors[i];
						if(i < (numberOfErrors - 1))
						{
							msg += ",";
						}
					}
				} while (moreErrors[0]);

				if (!string.IsNullOrEmpty(msg))
				{
					throw new Exception(msg);
				}
			}
		}
		private void ThrowIfError()
		{
			if (AllowDebug)
			{
				string msg = string.Empty;
				int lastWin32Error = 0;
				List<int> openGLErrors = new List<int>();
				try
				{
					ThrowLatsWin32Error();
				}
				catch(Exception e)
				{
					int.TryParse(e.Message, out lastWin32Error);
					msg += "LastWin32Error: " + e.Message;
				}

				try
				{
					ThrowOpenGLError();
				}
				catch (Exception e)
				{
					msg += "OpenGLErrors: " + e.Message;
					string eMsg = e.Message;
					string[] openGLErrorsStr = eMsg.Split(',');
					foreach (string openGLErrorStr in openGLErrorsStr)
					{
						int openGLError = 0;
						int.TryParse(openGLErrorStr, out openGLError);
						openGLErrors.Add(openGLError);
					}
				}

				if (!string.IsNullOrEmpty(msg))
				{
					throw new OpenGLException(lastWin32Error, openGLErrors, msg);
				}
			}
		}

		private void MakeCurrent()
		{
			OpenGLWrapperClass.wglMakeCurrent(_hDC, _hRC);
			ThrowIfError();
		}

		#endregion

		#region Buffers Creation

		/// <summary>
		/// Creates and binds a vertex array object.
		/// </summary>
		/// <returns>The vertex array object (VAO).</returns>
		public uint CreateAndBindVertexArrayObject()
		{
			throw new NotImplementedException();
		}

		/// <summary>
		/// Creates an array buffer (GL_ARRAY_BUFFER)
		/// </summary>
		/// <param name="attributes">Array of attributes.</param>
		/// <returns>The vertex buffer object (VBO)</returns>
		public uint CreateArrayBuffer(float[] attributes)
		{
			throw new NotImplementedException();
		}

		/// <summary>
		/// Creates an element array buffer (GL_ELEMENT_ARRAY_BUFFER)
		/// </summary>
		/// <param name="attributes">Array of attributes.</param>
		/// <returns>The vertex buffer object (VBO)</returns>
		public uint CreateElementArrayBuffer(int[] attributes)
		{
			throw new NotImplementedException();
		}

		#endregion

		#region Rendering Program Creation

		public uint CreateReneringProgram(string vertexShaderFilePath, string fragmentShaderFilePath)
		{
			int errorCode = 0;
			string errorMsg = string.Empty;
			uint renderingProgram = OpenGLWrapperClass.createRenderingProgram(vertexShaderFilePath, fragmentShaderFilePath, ref errorCode, ref errorMsg);
			if (AllowDebug && errorCode != 1)
			{
				throw new Exception(errorMsg);
			}
			ThrowIfError();

			return renderingProgram;
		}

		#endregion

		#region Rendering

		public void SetClearColor(float red, float green, float blue, float alpha)
		{
			MakeCurrent();
			OpenGLWrapperClass.glClearColor(red, green, blue, alpha);
			ThrowIfError();
		}

		public void ClearColorBuffer()
		{
			MakeCurrent();
			OpenGLWrapperClass.clearColorBuffer();
			ThrowIfError();
		}

		public void ClearColorBuffer(float red, float green, float blue, float alpha)
		{
			MakeCurrent();
			OpenGLWrapperClass.glClearColor(red, green, blue, alpha);
			ThrowIfError();
			OpenGLWrapperClass.clearColorBuffer();
			ThrowIfError();
		}

		public void ClearDepthBuffer()
		{
			MakeCurrent();
			OpenGLWrapperClass.clearDepthBuffer();
			ThrowIfError();
		}

		public void SwapBuffers()
		{
			MakeCurrent();
			OpenGLWrapperClass.SwapBuffers(_hDC);
			ThrowIfError();
		}

		#endregion
	}
}
