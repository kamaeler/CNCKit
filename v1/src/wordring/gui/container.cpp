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

form::form() : m_window(new form_window<form>())
{
	m_window->set_control(this);
	m_window->create(NULL);
}

form* form::get_form()
{
	return this;
}

window* form::get_window()
{
	return m_window.get();
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
/*
bool form::on_click()
{
	return true;
}
*/