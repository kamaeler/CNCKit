/**
 * @file    wordring/gui/detail/win32/win32_window_class.h
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

#ifndef WORDRING_WIN32_WINDOW_CLASS_H
#define WORDRING_WIN32_WINDOW_CLASS_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>

#include <Windows.h>
#include <windowsx.h>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cassert>

namespace wordring
{
namespace gui
{
namespace detail
{

// �E�B���h�E�E�N���X ---------------------------------------------------------

/**
* @brief ���E�B���h�E�E�N���X
* @details
*    win32�̃E�B���h�E�E�N���X�����ړI�̊��N���X�ł��B
*    C++�̃N���X��win32�̃E�B���h�E�E�N���X�����т��܂��B
*    ���z�֐����g�킸�Ƀ��b�Z�[�W��z������\�͂�����܂��B
*/
template <typename ClsT, typename ImplT>
struct win32_window_class
{
	HINSTANCE m_hinstance;
	ATOM m_atom;

	/// win32�̃E�B���h�E�E�N���X��o�^���܂�
	win32_window_class() : m_hinstance((HINSTANCE)NULL), m_atom((ATOM)NULL)
	{
		WNDCLASSEX wcex = ClsT::create();
		m_atom = ::RegisterClassEx(&wcex);
		m_hinstance = wcex.hInstance;
	}

	/// win32�̃E�B���h�E�E�N���X��o�^�������܂�
	virtual ~win32_window_class()
	{
		assert(m_atom != (ATOM)NULL);
		::UnregisterClass((LPCTSTR)(DWORD)m_atom, m_hinstance);
	}

	/// �E�B���h�E�E�v���V�[�W���̐��^�ł�
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
			native_window_impl* nw =
				static_cast<native_window_impl*>(cs->lpCreateParams);
			assert(nw);

			win32_window_service_impl::assign(hwnd, nw);
		}

		LRESULT result = 0;
		bool handled = false;

		ImplT* w = win32_window_service_impl::find(hwnd);
		if(w) // WM_NCCREATE�ȑO�͓o�^����Ă��Ȃ�
		{
			result = w->WindowProc(hwnd, uMsg, wParam, lParam);
			handled = w->get_message_handled();
		}
		else
		{
			// TODO: �E�B���h�E�ɔz���ł��Ȃ����b�Z�[�W�̏���
		}

		if (uMsg == WM_NCDESTROY)
		{
			win32_window_service_impl::remove(hwnd);
			assert(w); // �o�^����Ă��Ȃ��E�B���h�E���������Ƃ͂ł��Ȃ�
			w->m_hwnd = nullptr;
		}

		if (!handled) { result = ::DefWindowProc(hwnd, uMsg, wParam, lParam); }

		return result;
	}
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_CLASS_H
