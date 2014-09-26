﻿/**
* @file    wordring/gui/control.cpp
*
* @brief   GUIコントロールの実装ファイル
*
* @details
*          ボタン、スクロールバーなどのGUIコントロールです。
*
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

#include <wordring/wordring.h>

#include <wordring/gui/shape_int.h>
#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>
#include <wordring/gui/window.h>

#include <cassert>
#include <utility>
#include <string>
#include <atomic>
#include <cstdlib>

using namespace wordring::gui;

// control --------------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

control::control(rect_int rc)
	: m_parent(nullptr)
	, m_rc(rc)
{
}

control::control(rect_int rc, layout::store l)
	: m_parent(nullptr)
	, m_rc(rc)
	, m_layout(std::move(l))
{
}

control::~control()
{
	//m_storage.clear();
}

control::store control::create(rect_int rc)
{
	return store(new control(rc));
}

// 親子関係 -------------------------------------------------------------------

void control::attach_parent_internal(container *parent)
{
	assert(parent);

	m_parent = parent;

	// 自身はまだウィンドウが無い。
	// 親ウィンドウが作成済みであれば、自身のウィンドウを作成できる。
	// 親ウィンドウが未作成であれば、親ウィンドウが作成される時に
	// 子コントロールが再帰的にウィンドウ作成される。

	window *w = parent->find_window();
	if (w->get_native()->is_created()) { attach_window_internal(w); }
}

void control::detach_parent_internal()
{
	assert(m_parent);

	find_service()->erase_message(this);

	window *w = find_window();
	if (w->get_native()->is_created()) { detach_window_internal(); }

	m_parent = nullptr;
}

void control::attach_window_internal(window *pw)
{
	// もし自身がウィンドウを継承していれば、ウィンドウを作成する
	window *self = to_window();
	if (self)
	{
		// 位置の正規化
		point_int pt = get_position();

		control const *pc = get_parent();
		if (pc)
		{
			// 自身がウィンドウを持つため、ウィンドウ作成前の検索は必ず失敗する
			// そのため、親コンテナに検索させて差分を計算する
			pt += pc->query_offset_from_window();
		}
		self->get_native()->create_window(pw, rect_int(pt, get_size()));

		pw = self; // 子のために自身を親とする
	}

	// 子のウィンドウを処理する
	iterator it1 = begin(), it2 = end();
	while (it1 != it2)
	{
		(*it1)->attach_window_internal(pw);
		++it1;
	}
}

void control::detach_window_internal()
{
	// 子のウィンドウを処理する
	iterator it1 = begin(), it2 = end();
	while (it1 != it2)
	{
		(*it1)->detach_window_internal();
		++it1;
	}

	// もし自身がウィンドウを継承していれば、ウィンドウを破棄する
	window *w = to_window();
	if (w)
	{
		w->get_native()->destroy_window();
	}
}

container* control::get_parent()
{
	return m_parent;
}

container const* control::get_parent() const
{
	return m_parent;
}

control::iterator control::begin()
{
	return m_storage.begin();
}

control::iterator control::end()
{
	return m_storage.end();
}

control::const_iterator control::begin() const
{
	return m_storage.begin();
}

control::const_iterator control::end() const
{
	return m_storage.end();
}

control::reverse_iterator control::rbegin()
{
	return m_storage.rbegin();
}

control::reverse_iterator control::rend()
{
	return m_storage.rend();
}

control::const_reverse_iterator control::rbegin() const
{
	return m_storage.rbegin();
}

control::const_reverse_iterator control::rend() const
{
	return m_storage.rend();
}

// 情報 -----------------------------------------------------------------------

wchar_t const* control::get_control_name() const
{
	return L"control";
}

bool control::is_window() const
{
	return false;
}

bool control::is_container() const
{
	return false;
}

window* control::find_window()
{
	control *c = this;
	while (!c->to_window())
	{
		c = c->get_parent();
	}
	return c->to_window();
}

window* control::to_window()
{
	return nullptr;
}

/*
root_window* control::find_root_window()
{
	assert(get_parent());
	return get_parent()->find_root_window();
}
*/
window_service* control::find_service()
{
	assert(get_parent());
	return get_parent()->find_service();
}

control_style* control::get_style()
{
	return static_cast<control_style*>(m_style.get());
}

void control::set_style(style::store s)
{
	m_style = s;
}

// 表示 -----------------------------------------------------------------------

bool control::is_visible() const
{
	return true;
}

void control::show()
{
}

void control::hide()
{

}

// 描画 -----------------------------------------------------------------------

void control::repaint()
{
	repaint(get_rect());
}

void control::repaint(rect_int rc)
{
	window *w = find_window();

	point_int pt = query_offset_from_window();
	rc.pt = pt;

	w->get_native()->repaint_window(rc);
}

// 大きさ・位置 ---------------------------------------------------------------

void control::set_position(point_int pt)
{
	set_rect(rect_int(pt, m_rc.size));
}

point_int control::get_position() const
{
	return m_rc.pt;
}

void control::set_rect(rect_int rc)
{
	set_rect_internal(rc, true, true);
}

void control::set_rect_internal(rect_int rc, bool notify, bool paint)
{
	// ルート・コンテナはオーバーライドする必要があります
	assert(get_parent() != nullptr);
	/*
	rect_int old_rc = m_rc;

	if (is_container())
	{
		get_layout()->perform_layout(static_cast<container*>(this));
	}
	else if (is_window()) // ウィンドウは
	{
	}
	*/
	
	std::swap(m_rc, rc);
	
	if (is_container())
	{
		get_layout()->perform_layout(static_cast<container*>(this));
	}
	
	if (notify)
	{
		// rcは更新前の長方形と置き換わっている
		get_parent()->get_layout()->do_child_rect(this, rc);
	}

	if (paint) { repaint(); }
	
}

rect_int control::get_rect() const
{
	return m_rc;
}

void control::set_size(size_int size)
{
	set_rect(rect_int(m_rc.pt, size));
}

size_int control::get_size() const
{
	return is_visible() ? m_rc.size : size_int();
}

size_int control::get_preferred_size() const
{
	return get_size();
}

size_int control::get_min_size() const
{
	return get_size();
}

size_int control::get_max_size() const
{
	return get_size();
}

point_int control::query_offset_from_window() const
{
	point_int result(0, 0);
	control const *c = this;
	while (c)
	{
		if (c->is_window()) break;
		result += c->get_position();
		c = c->get_parent();
	}
	return result;
}

rect_int control::query_rect_from_window() const
{
	// ウィンドウからの相対でコントロールの長方形と親の長方形の重なる部分を
	// 求める
	point_int
		pt1 = get_parent()->query_offset_from_window(),
		pt2 = query_offset_from_window();

	rect_int
		rc1 = rect_int(pt1, get_parent()->get_size()),
		rc2 = rect_int(pt2, get_size());

	return rc1 & rc2; // 重なる長方形
}

point_int control::query_offset_from(container *c) const
{
	assert(c->is_ancestor_of(this));

	point_int result = get_position();
	container const *c0 = get_parent();

	do
	{
		if (c0 == c) { return result; }
		result += c0->get_position();
	}
	while ((c0 = c0->get_parent()) != nullptr);

	return point_int();
}

bool control::hit_test(point_int pt) const
{
	return true;
}

// レイアウト -----------------------------------------------------------------

void control::set_layout(layout::store l)
{
	m_layout = std::move(l);
}

/// レイアウトを取得します
layout* control::get_layout()
{
	return m_layout.get();
}

// タイマー -------------------------------------------------------------------

void control::set_timer(int32_t ms)
{
	find_service();
}

// ライブラリ定義のメッセージ・ハンドラ ---------------------------------------

void control::do_message_internal(message &m)
{
}

// マウス・メッセージ ---------------------------------------------------------
/*
bool control::do_click(mouse &m)
{
	return false;
}
*/
bool control::do_mouse_down(mouse &m)
{
	return false;
}

bool control::do_mouse_down_internal(mouse &m)
{
	return do_mouse_down(m);
}

void control::do_mouse_move(mouse &m)
{
}

void control::do_mouse_move_internal(mouse &m)
{
	window_service *service = find_service();

	service->get_mouse_service().process_bubble_up(this, m);
	service->get_mouse_service().process_bubble_top(this, m);

	//std::cout << m.pt.x << ", " << m.pt.y << std::endl;

	do_mouse_move(m);
}

void control::do_mouse_over(mouse &m)
{
}

void control::do_mouse_out(mouse &m)
{
}

bool control::do_mouse_up(mouse &m)
{
	return false;
}

bool control::do_mouse_up_internal(mouse &m)
{
	return do_mouse_up(m);
}

// キーボード・メッセージ -------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

void control::do_create()
{
}

void control::do_create_internal()
{
	do_create();
}

void control::do_destroy()
{
}

void control::do_paint(canvas &cv)
{
#ifdef WORDRING_DEBUG_CONTROL

	size_int size = get_size();
	rect_int rc(point_int(0, 0), size_int(size.cx - 1, size.cy - 1));

	color_rgb rgb(0x3F, 0x3F, 0x3F);

	int32_t w = 3;

	cv.fill_rect(rc, color_rgb(0xA0, 0xA0, 0xA0));

	cv.draw_line(rc.top_left(), rc.top_right(), w, rgb);
	cv.draw_line(rc.bottom_left(), rc.bottom_right(), w, rgb);
	cv.draw_line(rc.top_left(), rc.bottom_left(), w, rgb);
	cv.draw_line(rc.top_right(), rc.bottom_right(), w, rgb);
	cv.draw_line(rc.top_left(), rc.bottom_right(), w, rgb);
	cv.draw_line(rc.top_right(), rc.bottom_left(), w, rgb);
	
	cv.draw_string(
		get_control_name(), point_int(0, 0), rgb, nullptr);

#endif // WORDRING_DEBUG_CONTROL
}

void control::do_paint_internal(canvas& cv)
{
	//cv->set_viewport(query_rect_from_window());
	do_paint(cv);
}

void control::do_size(size_int size)
{
}

// test_control ---------------------------------------------------------------

test_control::test_control(rect_int rc, int32_t id)
	: m_id(id)
	, m_fg_color(0xFF, 0xFF, 0xFF)
	, control(rc)
{

}

test_control::~test_control()
{

}

control::store test_control::create(rect_int rc, int32_t id)
{
	return control::store(new test_control(rc, id));
}

bool test_control::do_mouse_down(mouse &m)
{
	//std::cout << "down " << m.pt.x << ", " << m.pt.y << std::endl;

	std::swap(m_fg_color, m_bg_color);
	repaint();
	return true;
}

void test_control::do_mouse_over(mouse &m)
{
	//std::cout << m.pt.x << ", " << m.pt.y << std::endl;
	//m_fg_color = color_rgb(0x33, 0, 0);
	std::swap(m_fg_color, m_bg_color);
	repaint();
}

void test_control::do_mouse_out(mouse &m)
{
	//std::cout << m.pt.x << ", " << m.pt.y << std::endl;
	std::swap(m_fg_color, m_bg_color);
	repaint();
}

bool test_control::do_mouse_up(mouse &m)
{
	//std::cout << "up " << m.pt.x << ", " << m.pt.y << std::endl;
	std::swap(m_fg_color, m_bg_color);
	repaint();
	return true;
}

void test_control::do_paint(canvas &cv)
{
	rect_int rc = rect_int(point_int(0, 0), get_size());

	point_int
		pt1(rc.left(), rc.top()), pt2(rc.right(), rc.top()),
		pt3(rc.left(), rc.bottom()), pt4(rc.right(), rc.bottom());

	color_rgb fg(0x3F, 0x3F, 0x3F);
	//color_rgb bg(0xA0, 0x0, 0xA0);

	int32_t w = 1;

	cv->fill_rect(rc, m_bg_color);

	cv->draw_rect(rc, w, m_fg_color);

	cv->draw_line(pt1, pt4, w, m_fg_color);
	cv->draw_line(pt2, pt3, w, m_fg_color);

	cv->draw_string(
		get_control_name(), point_int(0, 0), fg, nullptr);

	char str[128];
	std::string src("(");
	src += ::_itoa(get_rect().left(), str, 10);
	src += ", ";
	src += ::_itoa(get_rect().top(), str, 10);
	src += ") - (";
	src += ::_itoa(get_rect().right(), str, 10);
	src += ", ";
	src += ::_itoa(get_rect().bottom(), str, 10);
	src += ") ";
	src += ::_itoa(m_id, str, 10);
	cv->draw_string(
		src, point_int(0, 16), fg, nullptr);

	//std::cout << "paint: " << m_id << std::endl;
}
