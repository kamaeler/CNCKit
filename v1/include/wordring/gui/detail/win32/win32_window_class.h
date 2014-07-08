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

// ���b�Z�[�W�E�}�b�v ---------------------------------------------------------

/**
* @brief ���b�Z�[�W�E�}�b�v
* @details
*    �E�B���h�E�Y�̃E�B���h�E�E�n���h����C++�̃E�B���h�E�E�I�u�W�F�N�g��
*    ���т���}�b�v�ł��B
*    ���݂̓X���b�h�ɑΉ����Ă��Ȃ����߁A�v���Z�X���Ɉ�����L��܂���B
*/
class win32_message_map
{
	/// �v���O�������B��̃}�b�v
	static std::map<HWND, window*> g_map;

public:
	/// �n���h���ƃE�B���h�E�E�I�u�W�F�N�g�̃Z�b�g���}�b�v�ɒǉ����܂�
	static void assign(HWND hwnd, window* pwin)
	{
		g_map[hwnd] = pwin;
	}

	/// �n���h������E�B���h�E�E�I�u�W�F�N�g���������܂�
	static window* find(HWND hwnd)
	{
		std::map<HWND, window*>::iterator it = g_map.find(hwnd);
		if (it == g_map.end()) { return nullptr; }

		return it->second;
	}

	/// �n���h�����w�肵�ăE�B���h�E�E�I�u�W�F�N�g�Ƃ̃Z�b�g���폜���܂�
	static void remove_window(HWND hwnd)
	{
		size_t n = g_map.erase(hwnd);
		assert(n == 1);
	}
};

// �E�B���h�E�E�N���X ---------------------------------------------------------

/**
* @brief ���E�B���h�E�E�N���X
* @details
*    win32�̃E�B���h�E�E�N���X�����ړI�̊��N���X�ł��B
*    C++�̃N���X��win32�̃E�B���h�E�E�N���X�����т��܂��B
*    ���z�֐����g�킸�Ƀ��b�Z�[�W��z������\�͂�����܂��B
*/
template <typename ClsT, typename WinT, typename ImplT>
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

	/// win32�̃E�B���h�E�E�I�u�W�F�N�g���������܂�
	virtual ~win32_window_class()
	{
		assert(m_atom != (ATOM)NULL);
		::UnregisterClass((LPCTSTR)(DWORD)m_atom, m_hinstance);
	}

	/// �E�B���h�E�E�v���V�[�W���̐��^�ł�
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//ImplT* impl = static_cast<ImplT*>(win32_message_map::find_window(hWnd));

		switch (uMsg)
		{
		// �}�E�X�E���b�Z�[�W -------------------------------------------------
		
		HANDLE_MSG(hwnd, WM_DROPFILES, onDropFiles);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, onLButtonDblClk);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, onLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, onLButtonUp);
		HANDLE_MSG(hwnd, WM_MOUSEACTIVATE, onMouseActivate);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, onMouseMove);
		HANDLE_MSG(hwnd, WM_MOUSEWHEEL, onMouseWheel);
		HANDLE_MSG(hwnd, WM_RBUTTONDBLCLK, onRButtonDblClk);
		HANDLE_MSG(hwnd, WM_RBUTTONDOWN, onRButtonDown);
		HANDLE_MSG(hwnd, WM_RBUTTONUP, onRButtonUp);
		
		// �L�[�{�[�h ---------------------------------------------------------

		HANDLE_MSG(hwnd, WM_CHAR, onChar);
		HANDLE_MSG(hwnd, WM_DEADCHAR, onDeadChar);
		HANDLE_MSG(hwnd, WM_HOTKEY, onHotKey);
		HANDLE_MSG(hwnd, WM_KEYDOWN, onKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, onKeyUp);
		HANDLE_MSG(hwnd, WM_SYSCHAR, onSysChar);
		HANDLE_MSG(hwnd, WM_SYSDEADCHAR, onSysDeadChar);
		HANDLE_MSG(hwnd, WM_SYSKEYDOWN, onSysKeyDown);
		HANDLE_MSG(hwnd, WM_SYSKEYUP, onSysKeyUp);

		// ��ʃ��b�Z�[�W -----------------------------------------------------
		
		HANDLE_MSG(hwnd, WM_ACTIVATE, onActivate);
		HANDLE_MSG(hwnd, WM_ACTIVATEAPP, onActivateApp);
		HANDLE_MSG(hwnd, WM_CLOSE, onClose);
		HANDLE_MSG(hwnd, WM_COMMAND, onCommand);
		HANDLE_MSG(hwnd, WM_COMPACTING, onCompacting); // �������[�s��
		HANDLE_MSG(hwnd, WM_COPYDATA, onCopyData);
		HANDLE_MSG(hwnd, WM_CREATE, onCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, onDestroy);
		HANDLE_MSG(hwnd, WM_ERASEBKGND, onEraseBkgnd);
		HANDLE_MSG(hwnd, WM_KILLFOCUS, onKillFocus);
		HANDLE_MSG(hwnd, WM_MOVE, onMove);
		HANDLE_MSG(hwnd, WM_PAINT, onPaint);
		HANDLE_MSG(hwnd, WM_POWER, onPower); // �V�X�e�����f
		HANDLE_MSG(hwnd, WM_QUERYENDSESSION, onQueryEndSession);
		HANDLE_MSG(hwnd, WM_QUIT, onQuit);
		HANDLE_MSG(hwnd, WM_SETFOCUS, onSetFocus);
		HANDLE_MSG(hwnd, WM_SHOWWINDOW, onShowWindow);
		HANDLE_MSG(hwnd, WM_SIZE, onSize);
		//HANDLE_MSG(hwnd, WM_SYSCOMMAND, onSysCommand);
		HANDLE_MSG(hwnd, WM_TIMER, onTimer);
		HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGED, onWindowPosChanged);
		HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, onWindowPosChanging);
		}

		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);


		// case WM_GETMINMAXINFO:
		// case WM_NCDESTROY:
	}


	// �}�E�X�E���b�Z�[�W -----------------------------------------------------

	/// �t�@�C�����h���b�O�E�A���h�E�h���b�v���ꂽ
	static void onDropFiles(HWND hwnd, HDROP hdrop)
	{
		WinT* pT = static_cast<WinT*>(win32_message_map::find(hwnd));
		assert(pT);

		UINT n = ::DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0); // �t�@�C����
		assert(n);

		UINT j = MAX_PATH;
		std::unique_ptr<TCHAR[]> sz(new TCHAR[j]);
		for (UINT i = 0; i < n; i++)
		{
			UINT k = ::DragQueryFile(hdrop, i, NULL, 0); // ������
			if (j < k)
			{
				sz.reset(new TCHAR[k]);
				j = k;
			}
			k = ::DragQueryFile(hdrop, i, sz.get(), j);

			std::basic_string<TCHAR> str(sz.get(), sz.get() + k);
			// todo: Window�R�[���o�b�N
		}
	}

	static void onLButtonDblClk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
	{

	}

	static void onLButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
	{
	}

	static void onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags){}

	static int onMouseActivate(
		HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
	{
		return MA_ACTIVATE;
	}

	static void onMouseMove(HWND hwnd, int x, int y, UINT keyFlags){}

	static void onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys){}

	static void onRButtonDblClk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){}

	static void onRButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){}

	static void onRButtonUp(HWND hwnd, int x, int y, UINT flags){}

	// �L�[�{�[�h�E���b�Z�[�W -------------------------------------------------

	static void onChar(HWND hwnd, TCHAR ch, int cRepeat){}

	static void onDeadChar(HWND hwnd, TCHAR ch, int cRepeat){}

	static void onHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk){}

	static void onKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

	static void onKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

	static void onSysChar(HWND hwnd, TCHAR ch, int cRepeat){}

	static void onSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat){}

	static void onSysKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

	static void onSysKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

	// ��ʃ��b�Z�[�W ---------------------------------------------------------

	static void onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized){}

	static void onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId){}

	static void onClose(HWND hwnd){}

	static void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify){}

	static void onCompacting(HWND hwnd, UINT compactRatio){}

	static BOOL onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
	{
		return 0;
	}

	static BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
	{
		WinT* pT = (WinT*)lpCreateStruct->lpCreateParams;
		
		win32_message_map::assign(hwnd, pT);
		static_cast<ImplT*>(pT->get_native_window())->m_hwnd = hwnd;

		pT->onCreate();

		return 0;
	}

	static void onDestroy(HWND hwnd){}

	static BOOL onEraseBkgnd(HWND hwnd, HDC hdc)
	{
		return 0;
	}

	static void onKillFocus(HWND hwnd, HWND hwndNewFocus){}

	static void onMove(HWND hwnd, int x, int y){}

	static void onPaint(HWND hwnd){}

	static void onPower(HWND hwnd, int code){}

	static BOOL onQueryEndSession(HWND hwnd)
	{
		return 0;
	}

	static void onQuit(HWND hwnd, int exitCode){}

	static void onSetFocus(HWND hwnd, HWND hwndOldFocus){}

	static void onShowWindow(HWND hwnd, BOOL fShow, UINT status){}

	static void onSize(HWND hwnd, UINT state, int cx, int cy){}

	static void onSysCommand(HWND hwnd, UINT cmd, int x, int y){}

	static void onTimer(HWND hwnd, UINT id){}

	static void onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos){}

	static BOOL onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
	{
		return 0;
	}

};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_CLASS_H
