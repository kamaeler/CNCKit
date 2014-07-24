/**
 * @file    wordring/opengl/gl_context.cpp
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
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#include <wordring/opengl/gl_context.h>
#include <wordring/gui/canvas.h>
#include <wordring/gui/window.h>

#ifdef _WIN32
#include <wordring/opengl/detail/win32/win32_gl_context.h>
#endif // _WIN32

using namespace wordring::opengl;

gl_context::gl_context() : m_native(new detail::native_gl_context_impl())
{

}

gl_context::gl_context(wordring::gui::window& w, int flg, int depth, int bpp)
	: m_native(new detail::native_gl_context_impl(w, flg, depth, bpp))
{

}

gl_context::gl_context(wordring::gui::canvas& cv, int flg, int depth, int bpp)
	: m_native(new detail::native_gl_context_impl(cv, flg, depth, bpp))
{

}

gl_context::~gl_context()
{

}

void gl_context::assign(wordring::gui::window& w, int flg, int depth, int bpp)
{
	m_native->assign(w, flg, depth, bpp);
}

void gl_context::assign(wordring::gui::canvas& cv, int flg, int depth, int bpp)
{
	m_native->assign(cv, flg, depth, bpp);
}

void gl_context::make_current(wordring::gui::canvas& cv)
{
	m_native->make_current(cv);
}

void gl_context::unmake_current(wordring::gui::canvas& cv)
{
	m_native->unmake_current(cv);
}

