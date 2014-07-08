/**
* @file    wordring/gui/control.cpp
*
* @brief   GUI�R���g���[���̎����t�@�C��
*
* @details
*          �{�^���A�X�N���[���o�[�Ȃǂ�GUI�R���g���[���ł��B
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

#include <cassert>

using namespace wordring::gui;

/*
window_service* control::get_service() const
{
	return m_parent->get_service();
}
*/

control::control() : m_parent(nullptr)
{

}

container* control::get_parent()
{
	assert(m_parent);
	return m_parent;
}

form* control::get_form()
{
	assert(m_parent);
	return m_parent->get_form();
}