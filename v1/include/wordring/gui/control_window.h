/**
 * @file    wordring/gui/control_window.h
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

#ifndef WORDRING_CONTROL_WINDOW_H
#define WORDRING_CONTROL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/window.h>

namespace wordring
{
namespace gui
{

/**
* @brief �R���g���[���E�E�B���h�E
* @details
*    �R���g���[���Ƃ��Ďg�����E�B���h�E�ł��B
*    ��������h�����܂��B
*/
class control_window : public window
{
public:
	control_window();
	control_window(detail::native_window* nw);
	virtual ~control_window();
};


} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_WINDOW_H
