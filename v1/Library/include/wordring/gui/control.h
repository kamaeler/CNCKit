﻿/**
 * @file    wordring/gui/control.h
 *
 * @brief   GUIコントロール
 *
 * @details
 *          ボタン、スクロールバーなどのGUIコントロールの基礎を宣言します。
 *          controlクラスはインタフェースとして宣言されます。
 *          共通の機能が実装されます。
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

#ifndef WORDRING_CONTROL_H
#define WORDRING_CONTROL_H

#include <wordring/debug.h>

#include <wordring/gui/shape_int.h>
#include <wordring/gui/canvas.h>

#include <wordring/gui/message.h>

#include <wordring/gui/mouse.h>

#include <cstdint>
#include <memory>
#include <functional>

namespace wordring
{
namespace gui
{

class window_service; // 前方宣言
class window;         // 前方宣言
class root_window;    // 前方宣言
class container;      // 前方宣言
class root_container; // 前方宣言

/**
 * @brief   GUIコントロール基本クラス
 *
 * @details 全てのGUIコントロールの祖先です。
 *          コントロールに必要な基本機能とインターフェースを備えています。
 */
class control
{
public:
	typedef std::unique_ptr<control> store;

protected:
	container* m_parent; ///< 親コンテナ
	rect_int m_rc; ///< コントロールの長方形

	// 構築・破棄 -------------------------------------------------------------
protected:
	/// コントロールを作成します
	explicit control(rect_int rc);

public:
	/// コントロールを破棄します
	virtual ~control();

	/**
	 * @brief   コントロールを作成します
	 *
	 * @details 
	 *          オブジェクトのメモリー管理を行うため、オブジェクトの生成は必ず
	 *          この関数を使います。
	 */
	static store create(rect_int rc);

	// 親子関係 ---------------------------------------------------------------

	/**
	 * @brief   親コンテナを取り付けます
	 *
	 * @details 
	 *          親コンテナから呼び出されます。
	 */
	virtual void attach_parent(container *parent);

	/// 親コンテナを取り外します
	virtual void detach_parent();

	virtual void attach_window();

	virtual void detach_window();

	/**
	 * @brief   親コンテナを取得します
	 *
	 * @details
	 *          最上位のコンテナは親を持たないため、nullptrを返します。
	 */
	virtual container* get_parent();

	/**
	 * @brief   親コンテナを取得します
	 *
	 * @details
	 *          最上位のコンテナは親を持たないため、nullptrを返します。
	 */
	virtual container const* get_parent() const;

	// 情報 -------------------------------------------------------------------

	/// コントロール名を返します
	virtual wchar_t const* get_control_name() const;

	/// コントロール・アトムを返します
	virtual int32_t find_control_atom();

	/// コントロールがウィンドウの場合、trueを返します
	virtual bool is_window() const;

	/// コントロールがコンテナの場合、trueを返します
	virtual bool is_container() const;

	/**
	 * @brief   コントロールが配置されているウィンドウを検索します
	 * 
	 * @details コントロール自身がウィンドウの場合もあり得ます。
	 *          コントロール自身がウィンドウの場合は、このメンバをオーバーライド
	 *          してください。
	 */
	virtual window* find_window();

	/**
	 * @brief   一番近いコンテナを返します
	 *
	 * @details thisがコンテナの場合、thisを返します。
	 */
	virtual container* find_container();

	/**
	 * @brief   ルート・ウィンドウを設定します
	 *
	 * @details
	 *          ルート・ウィンドウはデスクトップ直下のウィンドウです。
	 *          このメンバは、ルート・コンテナのためにあります。
	 */
	virtual void set_root_window(root_window &rw);

	/**
	 * @brief   ルート・ウィンドウを検索します
	 *
	 * @details 
	 *          ルート・ウィンドウはデスクトップ直下のウィンドウです。
	 */
	virtual root_window* find_root_window();

	/**
	 * @brief   スレッドのウィンドウ・サービスを検索します
	 *
	 * @return  ウィンドウ・サービス
	 */
	virtual window_service* find_service();

	// 表示 -------------------------------------------------------------------

	/// コントロールの表示状態を返します
	virtual bool is_visible() const;

	/// コントロールを表示します
	virtual void show();

	/// コントロールを非表示にします
	virtual void hide();

	// 描画 -------------------------------------------------------------------

	/// コントロール全体を再描画します
	virtual void repaint();

	/// コントロールの指定部分を再描画します
	virtual void repaint(rect_int rc);

	// 大きさ・位置 -----------------------------------------------------------

	/// コントロールの大きさを設定する
	virtual void set_size(size_int size);

	/// コントロールの大きさを取得する
	virtual size_int get_size() const;

	/// コントロールの位置を設定する
	virtual void set_position(point_int pt);

	/// コントロールの位置を取得する
	virtual point_int get_position() const;

	/**
	 * @brief   コントロールの長方形を設定する
	 * 
	 * @details 
	 *          コントロールの長方形を変更すると、結果として祖先の変更を引き
	 *          起こす場合があります。
	 *          祖先の変更は、結果として子孫の変更を引き起こします。
	 *          従って、無限ループが起こる可能性があるため、このメンバを
	 *          オーバーライドする場合、ループ・ガードが必要です。
	 *          
	 *          長方形の変更によってdo_size()メッセージ・ハンドラが引き起こされ
	 *          その中から更に長方形の変更が引き起こされる場合があります。
	 *          無駄な変更を避けるため、レイアウトの実施はウィンドウ・サービス
	 *          に委譲します。
	 *          委譲は親コンテナのrequest_layout()呼び出しによって適切に
	 *          取り扱われます。
	 *
	 * @param   rc 長方形
	 */
	virtual void set_rect(rect_int rc);

public:
	/// コントロールの長方形を取得する
	virtual rect_int get_rect() const;

	/// 推奨される大きさを取得する
	virtual size_int get_preferred_size() const;
	
	/// 最少の大きさを取得する
	virtual size_int get_min_size() const;
	
	/// 最大の大きさを取得する
	virtual size_int get_max_size() const;

	/// ウィンドウからの相対位置を取得する
	virtual point_int query_offset_from_window() const;
	
	/// ウィンドウからの相対長方形を取得する
	virtual rect_int query_rect_from_window() const;

	/// cからコントロールまでのオフセットを取得する
	point_int query_offset_from(container *c) const;

	// タイマー ---------------------------------------------------------------

	/// msミリ秒後に発火するタイマーを設定します
	virtual void set_timer(int32_t ms);

	// ライブラリ定義のメッセージ・ハンドラ -----------------------------------

	/// ライブラリが実装するメッセージ機構のハンドラです
	virtual void do_message_internal(message &m);

	// マウス・メッセージ -----------------------------------------------------

	virtual bool do_click(mouse &m);

	virtual bool do_mouse_down(mouse &m);

	virtual bool do_mouse_down_internal(mouse &m);

	/*
	 * @brief   マウスの移動で呼び出されます
	 *
	 * @details
	 *          マウスの移動に反応するには、このメンバをオーバーライドします。
	 */
	virtual void do_mouse_move(mouse &m);

	/*
	 * @brief   [内部用] マウスの移動で呼び出されます
	 *
	 * @details
	 *          containerはこのメンバを実装しています。
	 *          その中で、メッセージの配送を処理しています。
	 */
	virtual void do_mouse_move_internal(mouse &m);

	virtual void do_mouse_over(mouse &m);

	virtual void do_mouse_out(mouse &m);

	virtual bool do_mouse_up(mouse &m);

	virtual bool do_mouse_up_internal(mouse &m);

	//virtual 

	// キーボード・メッセージ -------------------------------------------------

	// 一般メッセージ ---------------------------------------------------------

	virtual void do_create();

	virtual void do_create_internal();

	virtual void do_destroy();

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);

	/// 内部用: 再描画要求で呼び出されます
	virtual void do_paint_internal(canvas &cv);

	virtual void do_size(size_int size);
};

class test_control : public control
{
protected:
	rgb_color m_fg_color, m_bg_color;
	int32_t m_id;

protected:
	test_control(rect_int rc, int32_t id);

public:
	virtual ~test_control();

	static control::store create(rect_int rc, int32_t id);

	virtual bool do_mouse_down(mouse &m);

	virtual void do_mouse_over(mouse &m);

	virtual void do_mouse_out(mouse &m);

	virtual bool do_mouse_up(mouse &m);

	/// 再描画要求で呼び出されます
	virtual void do_paint(canvas& cv);
};

} // namespace gui
} // namespace wordring

#endif // WORDRING_CONTROL_H