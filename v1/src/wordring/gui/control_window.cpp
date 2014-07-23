/**
* @file    wordring/gui/control_window.cpp
*
* @brief   �E�B���h�E�̎����t�@�C��
*
* @details
*          pimpl�̊��ŗL�w�b�_�����̃t�@�C���ɂ̂݃C���N���[�h���Ă��������B
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

#include <wordring/debug.h>

#include <wordring/gui/control_window.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_control_window.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

using namespace wordring::gui;

// control_window -------------------------------------------------------------

control_window::control_window()
	: window(new detail::native_control_window_impl)
{
}

control_window::control_window(detail::native_window* nw) : window(nw)
{
}

control_window::~control_window()
{

}
