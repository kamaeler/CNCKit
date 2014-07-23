/**
* @file    wordring/gui/container_window.h
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

#ifndef WORDRING_CONTAINER_WINDOW_H
#define WORDRING_CONTAINER_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/window.h>

namespace wordring
{
namespace gui
{

/**
* @brief �R���e�i�E�E�B���h�E
* @details
*    �R���e�i�Ƃ��Ďg�����E�B���h�E�ł��B
*    ��������h�����܂��B
*/
class container_window : public window
{
public:
	container_window();
	container_window(detail::native_window* nw);
	virtual ~container_window();
};

	


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTAINER_WINDOW_H
