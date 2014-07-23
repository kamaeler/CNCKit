/**
* @file    wordring/gui/detail/win32/win32_container_window.h
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

#ifndef WORDRING_WIN32_CONTAINER_WINDOW_H
#define WORDRING_WIN32_CONTAINER_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/detail/win32/win32_window.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class dummy;

// native_container_window_impl -----------------------------------------------

class native_container_window_impl : public native_window_impl
{
public:
	native_container_window_impl();
	virtual ~native_container_window_impl();

	/// �E�B���h�E���쐬���܂�
	virtual void create(window* parent);

public:
	/// �E�B���h�E�E�v���V�[�W���̐��^�ł�
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// native_container_window_impl�p�ɃJ�X�^�}�C�Y���ꂽwin32�E�B���h�E�E�N���X�ł�
	struct window_class
		: public win32_window_class<window_class, native_container_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
};










} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_CONTAINER_WINDOW_H
