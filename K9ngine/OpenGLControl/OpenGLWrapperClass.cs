using System;
using System.Collections.Generic;
using System.Linq;
//using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OpenGLControl
{
	public static class OpenGLWrapperClass
	{
		#region Members

		public static IntPtr GLLibraryHandle { get; private set; }
		public static IntPtr GlewLibraryHandle { get; private set; }
		public static IntPtr OpenGLWrapperLibraryHandle { get; private set; }

		#endregion

		static OpenGLWrapperClass()
		{
			GLLibraryHandle = LoadLibrary(opengl_dll);
			//GlewLibraryHandle = LoadLibrary(glew32_dll);
			//int error = Marshal.GetLastWin32Error();

			//OpenGLWrapperLibraryHandle = LoadLibrary(OpenGLWrapper_dll);
			//error = Marshal.GetLastWin32Error();
		}

		#region kernel32.dll

		public const string kernel32_dll = "kernel32.dll";

		[DllImport(kernel32_dll, SetLastError = true)]
		public static extern IntPtr LoadLibrary(string fileName);

		[DllImport(kernel32_dll, SetLastError = true)]
		public static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

		[DllImport("Kernel32", SetLastError = true)]
		public extern static Boolean CloseHandle(IntPtr handle);

		#endregion

		#region user32.dll

		public const string user32_dll = "user32.dll";

		[DllImport(user32_dll, SetLastError = true)]
		public static extern IntPtr GetDC(IntPtr hWnd);

		[DllImport(user32_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

		#endregion

		#region gdi32.dll
		public const string gdi32_dll = "gdi32.dll";

		[DllImport(gdi32_dll, SetLastError = true)]
		public static extern int SwapBuffers(IntPtr hDC);

		[DllImport(gdi32_dll, SetLastError = true)]
		public unsafe static extern int ChoosePixelFormat(IntPtr hDC, [In, MarshalAs(UnmanagedType.LPStruct)] PIXELFORMATDESCRIPTION pfd);

		[DllImport(gdi32_dll, SetLastError = true)]
		public unsafe static extern bool SetPixelFormat(IntPtr hDC, int format, [In, MarshalAs(UnmanagedType.LPStruct)] PIXELFORMATDESCRIPTION pfd);

		#endregion

		#region glew32.dll

		public const string glew32_dll = "C:\\Users\\lococ\\Documents\\GitHub\\K9ngine\\K9ngine\\Debug\\glew32.dll";

		#endregion

		#region OpenGLWrapper.dll
#if DEBUG
		public const string OpenGLWrapper_dll = "..\\..\\..\\..\\Debug\\OpenGLWrapper.dll";
#else
		public const string OpenGLWrapper_dll = "..\\..\\..\\..\\Release\\OpenGLWrapper.dll";
#endif
		
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern int getOpenGLError(int[] errorsBuffer, int bufferSize, bool[] moreErrors);
		

		/// <summary>
		/// Gets OpenGL errors. Empty string if there were no errors.
		/// </summary>
		/// <param name="numberOfErrorsToShow">Number of errors to show per call (0 for no limit)</param>
		/// <returns>std::string containing OpenGL errors (empty string if there were no errors).</returns>
		/// 
		//[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		//public static extern string getOpenGLError(int numberOfErrorsToShow);
		/*
		public static string GetOpenGLError(int numberOfErrorsToShow)
		{
			//Type type = typeof(Func<int, string>);
			//var tmp = Marshal.GetDelegateForFunctionPointer(GetProcAddress(OpenGLWrapperLibraryHandle, "getOpenGLError"), type) as Func<int, string>;

			//return tmp(numberOfErrorsToShow);
			//var tmp = Marshal.GetDelegateForFunctionPointer(getOpenGLError, typeof(Func<int, string>)) as Func<int, string>;
			//getOpenGLError(numberOfErrorsToShow);

			return "Hola";
		}
		*/
		/*
		/// <summary>
		/// Gets shader log corresponding to 'shader'.
		/// </summary>
		/// <param name="shader">Shader for which to get the log.</param>
		/// <param name="logBuffers">Buffers for holding log lines. Each line will go to one position of 'buffers'.</param>
		/// <param name="logSize">The size of each element in 'buffers' (i.e. The max size that can be read per line)</param>
		/// <param name="numberOfBuffers">Number of buffers</param>
		/// <param name="moreToRead">True if there are more values to read.</param>
		/// <returns>Number of logs read (or error code)</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern int getShaderLog(GLuint shader, char** logBuffers, int logSize, int numberOfBuffers, bool* moreToRead);
		*/

		/// <summary>
		/// Gets shader log corresponding to 'shader'.
		/// </summary>
		/// <param name="shader">Shader for which to get the log.</param>
		/// <returns>std::string containing shader log.</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern string getShaderLog(uint shader);

		/// <summary>
		/// Gets program log corresponding to 'program'.
		/// </summary>
		/// <param name="program">Program to get the log for.</param>
		/// <returns>std::string containing program log.</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern string getProgramLog(uint program);

		/// <summary>
		/// Reads a text file containing a shader source and gets its content.
		/// </summary>
		/// <param name="shaderSourcePath">Path of the text file.</param>
		/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
		/// <returns>std::string with source file content</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern string readShaderSource(string shaderSourcePath, ref int returnCode);

		/// <summary>
		/// Creates and compiles a shader from a text file containing its content.
		/// </summary>
		/// <param name="shaderPath">Path of shader file.</param>
		/// <param name="shaderType">OpenGL enum indicating the type of shader to create (examples: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)</param>
		/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
		/// <param name="returnMsg">Out string for setting a return message.</param>
		/// <returns>GLuint for a shader</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern uint createAndCompileShader(string shaderPath, int shaderType, ref int returnCode, ref string returnMsg);

		/// <summary>
		/// Creates and compiles a vertex shader from a text file containing its content.
		/// </summary>
		/// <param name="shaderPath">Path of shader file.</param>
		/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
		/// <param name="returnMsg">Out string for setting a return message.</param>
		/// <returns>GLuint for a vertex shader</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern uint createAndCompileVertexShader(string shaderPath, ref int returnCode, ref string returnMsg);

		/// <summary>
		/// Creates and compiles a fragment shader from a text file containing its content.
		/// </summary>
		/// <param name="shaderPath">Path of shader file.</param>
		/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
		/// <param name="returnMsg">Out string for setting a return message.</param>
		/// <returns>GLuint for a fragment shader</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern uint createAndCompileFragmentShader(string shaderPath, ref int returnCode, ref string returnMsg);

		/// <summary>
		/// Creates a rendering program using a vertex and fragment shader files.
		/// </summary>
		/// <param name="vertexShaderFilePath">Vertex shader file path.</param>
		/// <param name="fragmentShaderFilePath">Fragment shader file path.</param>
		/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
		/// <param name="returnMsg">Out string for setting a return message.</param>
		/// <returns>GLuint for rendering program</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.StdCall)]
		public static extern uint createRenderingProgram(string vertexShaderFilePath, string fragmentShaderFilePath, ref int returnCode, ref string returnMsg);

		/// <summary>
		/// Inits glew (calls glewInit())
		/// Returns GLEW_OK if success
		/// </summary>
		/// <returns>Code returned by glewInit()</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern int initGlew();

		/// <summary>
		/// Clears color buffer (GL_COLOR_BUFFER_BIT)
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void clearColorBuffer();

		/// <summary>
		/// Clears depth buffer (GL_DEPTH_BUFFER_BIT)
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void clearDepthBuffer();

		/// <summary>
		/// Wrapper for glEnable
		/// </summary>
		/// <param name="attribute">Attribute to enable with glEnable (e.g. GL_CULL_FACE, GL_DEPTH_TEST)</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void enableGLAttribute(int attribute);

		/// <summary>
		/// Enables cull face (glEnable(GL_CULL_FACE))
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void enableCullFace();

		/// <summary>
		/// Enables depth test (glEnable(GL_DEPTH_TEST))
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void enableDepthTest();

		/// <summary>
		/// Sets depth function to use (calls glDepthFunc)
		/// </summary>
		/// <param name="depthFunction">Depth function to use (e.g. GL_LEQUAL)</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void setDepthFunction(int depthFunction);

		/// <summary>
		/// Sets depth function to GL_LEQUAL (glDepthFunc(GL_LEQUAL))
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void setDepthFunctionLessEqual();

		/// <summary>
		/// Sets clockwise (GL_CW) or counterclockwise (GL_CCW) by calling glFrontFace
		/// </summary>
		/// <param name="frontFace">GLenum for glFrontFace (GL_CW or GL_CCW)</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void setFrontFace(int frontFace);

		/// <summary>
		/// Sets front face to clockwise (glFrontFace(GL_CW))
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void setFrontFaceClockwise();

		/// <summary>
		/// Sets front face to counter clockwise (glFrontFace(GL_CCW))
		/// </summary>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void setFrontFaceCounterClockwise();

		/// <summary>
		/// Creates and binds a vertex array object.
		/// </summary>
		/// <returns>The vertex array object (VAO).</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern uint createAndBindVAO();

		/// <summary>
		/// Creates an array buffer (GL_ARRAY_BUFFER)
		/// </summary>
		/// <param name="attributes">Array of attributes.</param>
		/// <param name="numberOfAttributes">Number of attributes.</param>
		/// <returns>The vertex buffer object (VBO)</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern uint createArrayBuffer(float[] attributes, int numberOfAttributes);

		/// <summary>
		/// Creates an element array buffer (GL_ELEMENT_ARRAY_BUFFER)
		/// </summary>
		/// <param name="attributes">Array of attributes.</param>
		/// <param name="numberOfAttributes">Number of attributes.</param>
		/// <returns>The vertex buffer object (VBO)</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern uint createElementArrayBuffer(int[] attributes, int numberOfAttributes);

		/// <summary>
		/// Loads a texture and creates the buffer object
		/// if there were no errors *returnCode equals 1
		/// </summary>
		/// <param name="textureImagePath">Path of the texture to load</param>
		/// <param name="returnCode">Out variable used for setting a return value (errors, etc.)</param>
		/// <returns>Buffer object for the texture</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern uint loadAndCreateTexture(string textureImagePath, int[] returnCode);

		/// <summary>
		/// Gets the location for a uniform.
		/// </summary>
		/// <param name="renderingProgram">Renderign program</param>
		/// <param name="attributeName">The name of the attribute in the shader</param>
		/// <returns>GLint with the location.</returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern int getUniformLocation(uint renderingProgram, string attributeName);

		/// <summary>
		/// Sends a matrix to a shader
		/// </summary>
		/// <param name="matrixLocation">Location in the shader</param>
		/// <param name="matrix">Pointer of matrix to send</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void setUniformMatrix4fv(int matrixLocation, float[] matrix);

		/// <summary>
		/// Sends value to the rendering program in the specified location
		/// </summary>
		/// <param name="renderingProgram">Rendering program</param>
		/// <param name="location">Location</param>
		/// <param name="value">Value to send</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void programUniform1f(uint renderingProgram, int location, float value);

		/// <summary>
		/// Sends an array of three elements to the rendering program in the specified location
		/// </summary>
		/// <param name="renderingProgram">Rendering program</param>
		/// <param name="location">Location</param>
		/// <param name="value">Array to send</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void programUniform3fv(uint renderingProgram, int location, float[] value);

		/// <summary>
		/// Sends an array of four elements to the rendering program in the specified location
		/// </summary>
		/// <param name="renderingProgram">Rendering program</param>
		/// <param name="location">Location</param>
		/// <param name="value">Array to send</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void programUniform4fv(uint renderingProgram, int location, float[] value);

		/// <summary>
		/// Binds a vertex array and enables it in the shader
		/// (The attribute size is 2)
		/// </summary>
		/// <param name="index">Location in the shader</param>
		/// <param name="vbo">Vertex array to bind</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void activateArrayBuffer2f(uint index, uint vbo);

		/// <summary>
		/// Binds a vertex array and enables it in the shader
		/// (The attribute size is 3)
		/// </summary>
		/// <param name="index">Location in the shader</param>
		/// <param name="vbo">Vertex array to bind</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void activateArrayBuffer3f(uint index, uint vbo);

		/// <summary>
		/// Activates a 2D texture and binds the vbo
		/// </summary>
		/// <param name="texture">Texture to activate (for example: GL_TEXTURE0)</param>
		/// <param name="textureVBO">VBO to bind, corresponding to a texture.</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void activateTexture2D(int texture, uint textureVBO);

		/// <summary>
		/// Activates texture 0 (GL_TEXTURE0) and binds the vbo which corresponds to a 2D texture (GL_TEXTURE_2D)
		/// </summary>
		/// <param name="textureVBO">VBO to bind, corresponding to a texture.</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void activateTexture02D(uint textureVBO);

		/// <summary>
		/// Draws triangles using vertices (uses glDrawArrays)
		/// </summary>
		/// <param name="numberOfVertices">Total number of vertices</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void drawTrianglesUsingVertices(int numberOfVertices);

		/// <summary>
		/// Binds an element array containing indices and draws triangles using them (uses glDrawElements)
		/// </summary>
		/// <param name="indices">VBO of the indices</param>
		/// <param name="numberOfIndices">Total number of indices</param>
		/// <returns></returns>
		[DllImport(OpenGLWrapper_dll, SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
		public static extern void drawTrianglesUsingIndices(uint indices, int numberOfIndices);

		#endregion

		#region opengl.dll

		public const string opengl_dll = "opengl32.dll";

		//[DllImport(opengl_dll, EntryPoint = "wglGetProcAddress", SetLastError = true)]
		[DllImport(opengl_dll, SetLastError = true)]
		private static extern IntPtr wglGetProcAddress(string lpszProc);

		[DllImport(opengl_dll, SetLastError = true)]
		public static extern void glClearColor(float red, float green, float blue, float alpha);

		[DllImport(opengl_dll, SetLastError = true)]
		public static extern int wglMakeCurrent(IntPtr hdc, IntPtr hrc);

		[DllImport(opengl_dll, SetLastError = true)]
		public static extern IntPtr wglCreateContext(IntPtr hdc);

		[DllImport(opengl_dll, SetLastError = true)]
		public static extern int wglDeleteContext(IntPtr hrc);

		private delegate IntPtr Delegate_glGetString(int name);

		public static string glGetString(int name)
		{
			string returnVal = string.Empty;
			IntPtr val = IntPtr.Zero;

			//IntPtr addr = wglGetProcAddress("glGetString");
			IntPtr addr = GetProcAddress(GLLibraryHandle, "glGetString");
			if (addr != IntPtr.Zero)
			{
				var delegateFunction = (Delegate_glGetString)Marshal.GetDelegateForFunctionPointer(addr, typeof(Delegate_glGetString));
				val = delegateFunction(name);
			}

			if(val != IntPtr.Zero)
			{
				returnVal = Marshal.PtrToStringAnsi(val);
			}

			return returnVal;
		}

		public const int GL_VERSION = 0x1F02;

		#endregion

		#region PIXELFORMATDESCRIPTION

		[StructLayout(LayoutKind.Explicit)]
		public class PIXELFORMATDESCRIPTION
		{
			[FieldOffset(0)]
			public UInt16 nSize;
			[FieldOffset(2)]
			public UInt16 nVersion;
			[FieldOffset(4)]
			public UInt32 dwFlags;
			[FieldOffset(8)]
			public Byte iPixelType;
			[FieldOffset(9)]
			public Byte cColorBits;
			[FieldOffset(10)]
			public Byte cRedBits;
			[FieldOffset(11)]
			public Byte cRedShift;
			[FieldOffset(12)]
			public Byte cGreenBits;
			[FieldOffset(13)]
			public Byte cGreenShift;
			[FieldOffset(14)]
			public Byte cBlueBits;
			[FieldOffset(15)]
			public Byte cBlueShift;
			[FieldOffset(16)]
			public Byte cAlphaBits;
			[FieldOffset(17)]
			public Byte cAlphaShift;
			[FieldOffset(18)]
			public Byte cAccumBits;
			[FieldOffset(19)]
			public Byte cAccumRedBits;
			[FieldOffset(20)]
			public Byte cAccumGreenBits;
			[FieldOffset(21)]
			public Byte cAccumBlueBits;
			[FieldOffset(22)]
			public Byte cAccumAlphaBits;
			[FieldOffset(23)]
			public Byte cDepthBits;
			[FieldOffset(24)]
			public Byte cStencilBits;
			[FieldOffset(25)]
			public Byte cAuxBuffers;
			[FieldOffset(26)]
			public Byte iLayerType;
			[FieldOffset(27)]
			public Byte bReserved;
			[FieldOffset(28)]
			public UInt32 dwLayerMask;
			[FieldOffset(32)]
			public UInt32 dwVisibleMask;
			[FieldOffset(36)]
			public UInt32 dwDamageMask;
		}

		public const Byte PFD_TYPE_RGBA = 0;

		public const UInt32 PFD_DOUBLEBUFFER = 1;
		public const UInt32 PFD_DRAW_TO_WINDOW = 4;
		public const UInt32 PFD_SUPPORT_OPENGL = 32;

		public const Byte PFD_MAIN_PLANE = 0;

		public static PIXELFORMATDESCRIPTION GetDefaultPixelFormatDescriptor()
		{
			PIXELFORMATDESCRIPTION pfd = new PIXELFORMATDESCRIPTION();
			pfd.nSize = (UInt16)Marshal.SizeOf(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cRedBits = 0;
			pfd.cRedShift = 0;
			pfd.cGreenBits = 0;
			pfd.cGreenShift = 0;
			pfd.cBlueBits = 0;
			pfd.cBlueShift = 0;
			pfd.cAlphaBits = 0;
			pfd.cAlphaShift = 0;
			pfd.cAccumBits = 0;
			pfd.cAccumRedBits = 0;
			pfd.cAccumGreenBits = 0;
			pfd.cAccumBlueBits = 0;
			pfd.cAccumAlphaBits = 0;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
			pfd.cAuxBuffers = 0;
			pfd.iLayerType = PFD_MAIN_PLANE;
			pfd.bReserved = 0;
			pfd.dwLayerMask = 0;
			pfd.dwVisibleMask = 0;
			pfd.dwDamageMask = 0;

			return pfd;
		}

#endregion

#region OpenGL Extensions

		// values taken from https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
		public const int WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
		public const int WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
		public const int WGL_CONTEXT_LAYER_PLANE_ARB = 0x2093;
		public const int WGL_CONTEXT_FLAGS_ARB = 0x2094;
		public const int WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;

		public const int WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;

		private delegate IntPtr Delegate_wglCreateContextAttribsARB(IntPtr hDC, IntPtr hShareContext, int[] attribList);

		public static IntPtr wglCreateContextAttribsARB(IntPtr hDC, IntPtr hShareContext, int[] attribList)
		{
			IntPtr hRC = IntPtr.Zero;

			IntPtr addr = wglGetProcAddress("wglCreateContextAttribsARB");
			if (addr != IntPtr.Zero)
			{
				var delegateFunction = (Delegate_wglCreateContextAttribsARB)Marshal.GetDelegateForFunctionPointer(addr, typeof(Delegate_wglCreateContextAttribsARB));
				hRC = delegateFunction(hDC, hShareContext, attribList);
			}

			return hRC;
		}

#endregion
	}
}
