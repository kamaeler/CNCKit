﻿/**
 * @file    wordring/gui/root_window.cpp
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
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#include <wordring/debug.h>

#include <wordring/gui/root_window.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_child_window.h>
#endif // _WIN32

#include <algorithm>

using namespace wordring::gui;

// root_container -------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

root_container::root_container()
	: base_type(
		  rect_int() // ルート・ウィンドウによって決まるので空
		, full_layout::create()
		, std::move(detail::native_control_window_impl::create()))
{

}

root_container::~root_container()
{
	
}

root_container::store root_container::create()
{
	return store(new root_container());
}

void root_container::attach_root_window(root_window *parent)
{
	m_root_window = parent;
	attach_window();
}

void root_container::detach_root_window()
{
	find_service()->erase_message(this);
	detach_window();
}

void root_container::attach_parent(container *c)
{
	assert(false); // ルート・コンテナに親コンテナはありません
}

void root_container::detach_parent()
{
	assert(false); // ルート・コンテナに親コンテナはありません
}

void root_container::attach_window()
{
	// 自分のウィンドウを作成する
	get_native()->create_window(m_root_window, get_rect());

	// 子のウィンドウを処理する
	for (control::store &s : m_children)
	{
		s->attach_window();
	}
}

// 情報 -----------------------------------------------------------------------

wchar_t const* root_container::get_control_name() const
{
	return L"wordring::gui::root_container";
}

void root_container::set_root_window(root_window *rw)
{
	m_root_window = rw;
}

root_window* root_container::find_root_window()
{
	return m_root_window;
}


window_service* root_container::find_service()
{
	return m_root_window->get_service();
}

// root_window ----------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

root_window::root_window(rect_int rc)
	: m_client(root_container::create())
	, m_pt(rc.pt)
	, m_size(rc.size)
{
}

root_window::root_window(rect_int rc, detail::native_window::store nw)
	: window(std::move(nw))
	, m_client(root_container::create())
	, m_pt(rc.pt)
	, m_size(rc.size)
{
}

root_window::~root_window()
{
	m_client.reset();
}

root_window::store root_window::create(rect_int rc)
{
	return store(new root_window(rc));
}

void root_window::attach_service(window_service *ws)
{
	assert(ws);

	m_service = ws;
	get_native()->create_window(nullptr, get_rect());

	m_client->attach_root_window(this);
}

void root_window::detach_service()
{
	assert(m_service);

	m_client->detach_root_window();
	m_service = nullptr;
}

void root_window::set_client(root_container::store s)
{
	m_client->detach_root_window();
	m_client = std::move(s);
	m_client->attach_root_window(this);
}

container* root_window::get_client()
{
	return m_client.get();
}

control* root_window::assign(control::store s)
{
	control *c = static_cast<control*>(s.get());
	m_client->push_back(std::move(s));

	return c;
}

// 情報 -------------------------------------------------------------------

void root_window::set_service(window_service *ws)
{
	m_service = ws;
}

window_service* root_window::get_service()
{
	return m_service;
}

// 表示 -----------------------------------------------------------------------

void root_window::show()
{
	get_native()->show_window();
}

void root_window::hide()
{
	get_native()->hide_window();
}

// 大きさ・位置 ---------------------------------------------------------------

void root_window::set_rect(rect_int rc)
{
	get_native()->set_window_rect(rc);
}

rect_int root_window::get_rect() const
{
	return rect_int(m_pt, m_size);
}

// 一般メッセージ -------------------------------------------------------------

void root_window::do_destroy_window()
{
	m_service->quit();
}

void root_window::do_paint_window(canvas &cv)
{
	// 基底実装のwindow::do_paint_window()呼び出しを抑制するためにオーバーライド
}

void root_window::do_size_window(size_int size)
{
	if (m_client) { m_client->set_size(size); }
}

