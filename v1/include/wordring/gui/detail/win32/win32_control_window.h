/**
 * @file    wordring/gui/detail/win32/win32_control_window.h
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

#ifndef WORDRING_WIN32_CONTROL_WINDOW_H
#define WORDRING_WIN32_CONTROL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/detail/win32/win32_window_class.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class dummy;

// native_control_window_impl -----------------------------------------------

class native_control_window_impl : public native_window_impl
{
public:
	native_control_window_impl();
	virtual ~native_control_window_impl();

public:
	/// �E�B���h�E�E�v���V�[�W���̐��^�ł�
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// native_control_window_impl�p�ɃJ�X�^�}�C�Y���ꂽwin32�E�B���h�E�E�N���X�ł�
	struct window_class
		: public win32_window_class<window_class, native_control_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
};

















} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_CONTROL_WINDOW_H
