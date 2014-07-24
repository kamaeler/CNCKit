/**
 * @file    wordring/opengl/detail/win32/win32_gl_context.h
 *
 * @brief   
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

#ifndef WORDRING_WIN32_GL_CONTEXT_H
#define WORDRING_WIN32_GL_CONTEXT_H

#include <wordring/gui/canvas.h>
#include <wordring/gui/window.h>
#include <wordring/opengl/detail/native_gl_context.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <atomic>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_context_impl : public native_gl_context
{
public:
	HGLRC m_hglrc;

public:
	native_gl_context_impl();

	native_gl_context_impl(
		wordring::gui::window& w, int flg, int depth, int bpp);

	native_gl_context_impl(
		wordring::gui::canvas& cv, int flg, int depth, int bpp);

	virtual ~native_gl_context_impl();

	virtual void assign(wordring::gui::window& w, int flg, int depth, int bpp);
	virtual void assign(wordring::gui::canvas& cv, int flg, int depth, int bpp);

private:
	virtual void create(wordring::gui::window& w, int flg, int depth, int bpp);
	virtual void create(wordring::gui::canvas& cv, int flg, int depth, int bpp);
	virtual void create(HDC hdc, int flg, int depth, int bpp);

private:
	virtual void make_current(HDC hdc);
public:
	virtual void make_current(wordring::gui::canvas& cv);
	virtual void unmake_current(wordring::gui::canvas& cv);
	virtual void unmake_current();

private:
	/// glewInit()�����Ăяo���ł���΁A�Ăяo��
	virtual void initialize(HDC hdc);

private:
	/// glewInit()���Ăяo���ς݂ł����true�A�Ăяo���O��false
	static std::atomic_bool g_initialized;
};

} // namespace detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_WIN32_GL_CONTEXT_H
