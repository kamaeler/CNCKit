/**
 * @file    wordring/opengl/detail/win32/win32_gl_context.cpp
 *
 * @brief   gl_context�̊��ˑ������t�@�C��
 *
 * @details
 *          
 *          
 *          
 *          
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     �z�[��
 *          https://github.com/wordring/
 * @par     ���C�Z���X
 *          PDS
 */

#ifdef _WIN32

#ifdef _MSC_VER // TODO: 
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#endif // _MSCVER

#include <wordring/gui/canvas.h>
#include <wordring/gui/detail/win32/win32_canvas.h>

#include <wordring/gui/window.h>
#include <wordring/gui/detail/win32/win32_window.h>

#include <wordring/opengl/detail/win32/win32_gl_context.h>

#include <wordring/opengl/gl_context.h>

#include <wordring/exception.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <Windows.h>

#include <stdexcept>
#include <atomic>

using namespace wordring::opengl::detail;

// �\�z�E�j�� -----------------------------------------------------------------

native_gl_context_impl::native_gl_context_impl() : m_hglrc(nullptr)
{

}

native_gl_context_impl::native_gl_context_impl(
	wordring::gui::window& w, int flg, int depth, int bpp)
	: m_hglrc(nullptr)
{
	create(w, flg, depth, bpp);
}

native_gl_context_impl::native_gl_context_impl(
	wordring::gui::canvas& cv, int flg, int depth, int bpp) : m_hglrc(nullptr)
{
	create(cv, flg, depth, bpp);
}

native_gl_context_impl::~native_gl_context_impl()
{
	if (m_hglrc) { ::wglDeleteContext(m_hglrc); }
}

// OpenGL�R���e�L�X�g�̍쐬 ---------------------------------------------------

void native_gl_context_impl::assign(
	wordring::gui::window& w, int flg, int depth, int bpp)
{
	assert(m_hglrc == nullptr);
	create(w, flg, depth, bpp);
}

void native_gl_context_impl::assign(
	wordring::gui::canvas& cv, int flg, int depth, int bpp)
{
	assert(m_hglrc == nullptr);
	create(cv, flg, depth, bpp);
}

void native_gl_context_impl::create(
	wordring::gui::window& w, int flg, int depth, int bpp)
{
	using namespace wordring::gui::detail;

	native_window_impl* nwi =
		static_cast<native_window_impl*>(w.get_native_window());
	assert(nwi); // �E�B���h�E���쐬����Ă��邩�H
	HWND hwnd = nwi->m_hwnd;
	assert(hwnd);
	HDC hdc = ::GetDC(hwnd);
	wordring::check_assertion(hdc != NULL);

	create(hdc, flg, depth, bpp);
	initialize(hdc); // �����ϐ������ď��������s��

		wordring::check_assertion(
	::ReleaseDC(hwnd, hdc) == 1);
}

void native_gl_context_impl::create(
	wordring::gui::canvas& cv, int flg, int depth, int bpp)
{
	using namespace wordring::gui::detail;

	native_canvas_impl& nci =
		static_cast<native_canvas_impl&>(cv.get_native_canvas());
	HDC hdc = nci.get_dc();
	assert(hdc);

	create(hdc, flg, depth, bpp);
}

void native_gl_context_impl::create(HDC hdc, int flg, int depth, int bpp)
{
	using namespace wordring::gui::detail;

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = bpp;
	pfd.cDepthBits = depth;
	pfd.iLayerType = PFD_MAIN_PLANE;

	if (flg == gl_context::flag::WINDOW) { pfd.dwFlags |= PFD_DRAW_TO_WINDOW; }
	if (flg == gl_context::flag::MEMORY) { pfd.dwFlags |= PFD_DRAW_TO_BITMAP; }

	int npf = ::ChoosePixelFormat(hdc, &pfd);
	wordring::check_assertion(npf != 0);

		wordring::check_assertion(
	::SetPixelFormat(hdc, npf, &pfd) != 0);

	m_hglrc = ::wglCreateContext(hdc);
	wordring::check_assertion(m_hglrc != NULL);	
}

// �R���e�L�X�g�̃J�����g���E��J�����g�� -------------------------------------

void native_gl_context_impl::make_current(wordring::gui::canvas& cv)
{
	using namespace wordring::gui::detail;

	native_canvas_impl& nci =
		static_cast<native_canvas_impl&>(cv.get_native_canvas());
	HDC hdc = nci.get_dc();
	assert(hdc);

	make_current(hdc);
}

void native_gl_context_impl::make_current(HDC hdc)
{
		wordring::check_assertion(
	::wglMakeCurrent(hdc, m_hglrc) != FALSE);
}

void native_gl_context_impl::unmake_current()
{
	wordring::check_assertion(
		::wglMakeCurrent(NULL, NULL) != FALSE);
}

void native_gl_context_impl::unmake_current(wordring::gui::canvas& cv)
{
	using namespace wordring::gui::detail;

	native_canvas_impl& nci =
		static_cast<native_canvas_impl&>(cv.get_native_canvas());
	HDC hdc = nci.get_dc();

	::SwapBuffers(hdc);

	wordring::check_assertion(
		::wglMakeCurrent(NULL, NULL) != FALSE);
}

// GLEW���C�u���������� -------------------------------------------------------

void native_gl_context_impl::initialize(HDC hdc)
{
	if (g_initialized == false)
	{
		make_current(hdc);
		GLenum e = ::glewInit();
		unmake_current();

		if (e != GLEW_OK)
		{
			throw std::runtime_error((char*)::glewGetErrorString(e));
		}
		else { g_initialized = true; }
	}
}

// �������t���O ---------------------------------------------------------------

std::atomic_bool native_gl_context_impl::g_initialized = { false };

#endif // _WIN32
