/**
 * @file    wordring/gui/render.h
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

#include <wordring/wordring.h>

#include <memory>

#ifndef WORDRING_RENDER_H
#define WORDRING_RENDER_H

namespace wordring
{
namespace gui
{

class dummy;

/**
 * @brief   �R���g���[���̕`����s���܂�
 *
 * @details
 *          �R���g���[���̕`��́A�R���g���[���̋@�\�ƕ����ł��܂��B
 *          �Ⴆ�΃{�^���́A�o�c��A���A������A�摜�Ȃǂ�\�����܂����A
 *          �����ꂽ��n���h�������s����Ƃ����{�^���{���̋@�\���ƁA�`����e��
 *          �֌W������܂���B
 *          �\�����e�ɍ��킹�ăN���X�����͖̂��ʂȂ̂ŁA�`�敔����������
 *          ���܂��B
 */
class render
{
public:
	typedef std::shared_ptr<render> store;

};



} // namespace gui
} // namespace wordring

#endif // WORDRING_RENDER_H
