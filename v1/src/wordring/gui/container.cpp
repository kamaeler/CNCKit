/**
* @file    wordring/gui/container.cpp
*
* @brief   GUI�R���e�i�̎����t�@�C��
*
* @details
*          �t�H�[���Ȃǂ�GUI�R���e�i�ł��B
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

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>

using namespace wordring::gui;

form::form() : m_window(new window)
{
}

void form::set_size(size_int size)
{
	m_window->set_size(size);

}

size_int form::get_size() const
{
	return m_window->get_size();
}

void form::set_position(point_int point)
{
	m_window->set_position(point);
}

point_int form::get_position() const
{
	return m_window->get_position();
}