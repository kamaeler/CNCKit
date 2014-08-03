﻿/**
 * @file    wordring/gui/detail/native_window.h
 *
 * @brief   ウィンドウのpimplインターフェース
 *
 * @details
 *          このファイルには、pimplの公開されるインターフェースを宣言します。
 *          window関連クラスの実装に継承されます。
 *          
 *          
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_NATIVE_WINDOW_H
#define WORDRING_NATIVE_WINDOW_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>

#include <string>

namespace wordring
{
namespace gui
{

class window; // 前方宣言

namespace detail
{

// 
class native_window
{
private:
protected:
	wordring::gui::window* m_window;

public:
	native_window() { }
	virtual ~native_window() { }

	void set_window(wordring::gui::window* w) { m_window = w; }
	wordring::gui::window* get_window() { return m_window; }

	virtual void create_window(
		wordring::gui::window* parent, rect_int rc) = 0;
	virtual void close_window() = 0;
	virtual void destroy_window() = 0;

	virtual void show_window() = 0;
	virtual void hide_window() = 0;

	virtual void set_native_parent_window(native_window* parent) = 0;
	virtual native_window* get_native_parent_window() = 0;

	/// ウィンドウ全体を再描画します
	virtual void repaint_window() = 0;
	/// 指定の範囲を再描画します
	virtual void repaint_window(rect_int rc) = 0;

	virtual void set_window_size(size_int size) = 0;
	virtual size_int get_window_size() const = 0;

	virtual void set_window_position(point_int point) = 0;
	virtual point_int get_window_position() const = 0;
	/// ウィンドウの位置と大きさを設定します
	virtual void set_window_rect(rect_int rc) = 0;
	/// ウィンドウの位置と大きさを取得します
	virtual rect_int get_window_rect() const = 0;

	virtual void set_window_text(std::string text) = 0;
	virtual void set_window_text(std::wstring text) = 0;
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_NATIVE_WINDOW_H
