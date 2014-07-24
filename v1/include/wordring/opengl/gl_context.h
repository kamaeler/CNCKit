/**
 * @file    wordring/opengl/gl_context.h
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

#ifndef WORDRING_GL_CONTEXT_H
#define WORDRING_GL_CONTEXT_H

#include <wordring/gui/canvas.h>

#include <wordring/gui/window.h>
#include <wordring/opengl/detail/native_gl_context.h>

#include <memory>

namespace wordring
{
namespace opengl
{

class dummy;

class gl_context
{
public:
	enum flag
	{
		WINDOW, MEMORY,
	};

protected:
	std::unique_ptr<detail::native_gl_context> m_native;

public:
	gl_context();
	gl_context(wordring::gui::window& w, int flg, int depth, int bpp);
	gl_context(wordring::gui::canvas& cv, int flg, int depth, int bpp);
	virtual ~gl_context();

	/**
	 * �E�B���h�E�ƌ݊��������R���e�L�X�g���쐬���܂�
	 * 
	 * �����ɏo���邾���߂��R���e�L�X�g���쐬����܂��B
	 * 
	 * @param w �E�B���h�E
	 * @param flg �`��Ώۂ�\���t���O�iWINDOW���邢��MEMORY�j
	 * @param depth z�o�b�t�@�̐[�x�i16�A24�A32���j
	 * @param bpp �s�N�Z��������̃r�b�g���i24�A32���j
	 */
	virtual void assign(wordring::gui::window& w, int flg, int depth, int bpp);

	/**
	 * �L�����o�X�ƌ݊��������R���e�L�X�g���쐬���܂�
	 *
	 * �����ɏo���邾���߂��R���e�L�X�g���쐬����܂��B
	 *
	 * @param cv �L�����o�X
	 * @param flg �`��Ώۂ�\���t���O�iWINDOW���邢��MEMORY�j
	 * @param depth z�o�b�t�@�̐[�x�i16�A24�A32���j
	 * @param bpp �s�N�Z��������̃r�b�g���i24�A32���j
	 */
	virtual void assign(wordring::gui::canvas& cv, int flg, int depth, int bpp);

	/**
	 * �X���b�h�̃J�����g�E�R���e�L�X�g�ɑI�����܂�
	 * 
	 * �J�����g�E�R���e�L�X�g�ɑI������Ă���ԁAOpenGL�̕`��͂��̃R���e�L�X�g
	 * �̐ݒ肳�ꂽ�L�����o�X�ɑ΂��čs���܂��B
	 * 
	 * @param cv �`��ΏۂƂȂ�canvas
	 */
	virtual void make_current(wordring::gui::canvas& cv);
	/**
	 * �X���b�h�̃J�����g�E�R���e�L�X�g����O���܂�
	 */
	virtual void unmake_current(wordring::gui::canvas& cv);
};

} // namespace opengl
} // namespace wordring

#endif // WORDRING_GL_CONTEXT_H
