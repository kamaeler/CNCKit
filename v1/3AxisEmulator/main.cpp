﻿/**
 * @file    3AxisEmulator/main.cpp
 *
 * @brief   3軸CNCエミュレータ
 *
 * @details
 *          CNCソフトウェア開発用のツールとして仮想的なCNCマシンとして
 *          エミュレータを準備します。
 *          開発の段階が進むにつれて、シミュレータとしての機能が追加されると
 *          良いと思います。
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

//#include <wordring/gui/window.h>
#include <wordring/gui/window_service.h>
//#include <wordring/gui/container.h>

#include <wordring/gui/root_window.h>

#include <wordring/gui/button.h>
#include <wordring/gui/text.h>
#include <wordring/gui/style.h>

#include "cnc_3axis_emulator_control.h"


//#include <Windows.h>

//#include <cstdint>
#include <iostream>
#include <memory>

#ifdef WORDRING_CC_VC // TODO: 
#pragma comment (lib, "Library.lib")
#endif // WORDRING_CC_VC


#pragma comment(lib, "usp10.lib")
#pragma comment(lib, "imm32.lib")
int main()
{
	::setlocale(LC_ALL, "japanese");

	using namespace wordring::gui;
	using namespace wordring::opengl;

	wordring::debug::debug_memory_leak();

	rect_int rc(0, 0, 640, 480);

	window_service  ws;
	font_service   &fs = ws.get_font_service();
	gl_context gl;

	std::cout << sizeof(control) << std::endl;
//	root_window* rw1 = ws.push_back(root_window::create(rc));


//	gl.assign(rw1, gl_context::flag::WINDOW, 24, 24);
/*	control* e = rw1->assign(cnc_3axis_emulator_control::create(rect_int(), gl));
	cnc_3axis_emulator_control* emu =
		static_cast<cnc_3axis_emulator_control*>(e);
	gl.assign(*static_cast<window*>(emu), gl_context::flag::WINDOW, 24, 24);
	//a3e->set_position(point_int(0, 50));
	//a3e->set_size(size_int(100, 100));
	//a3e->show();

	rw1->show();
	*/

	//test_control::store tc0 = test_control::create(
	//	rect_int(point_int(0, 0), size_int(100, 100)), 11);

	// スタイル ---------------------------------------------------------------

	style::store s = control_style::create(style::state::normal);

	// ------------------------------------------------------------------------
	control_style *cs = static_cast<control_style*>(s.get());

	cs->background_color = color_rgb(0x40, 0x40, 0x40);
	cs->margin_top = 5;
	cs->margin_bottom = 12;

	cs->font = ws.get_font_service().create(16, 0, 400, false, L"Tahoma");

	cs = static_cast<control_style*>(
		s->push_back(control_style::create(style::state::active)));

	cs->background_color = color_rgb(0xFF, 0xFF, 0, 0xFF);


	//s = color_style::create(style::hover);
	//cs = static_cast<color_style*>(s.get());
	cs->color = color_rgb(0xFF, 0xFF, 0xFF);
	//b->set_style(s);




	root_window *rw = ws.push_back(root_window::create(rc));

	rw->set_title_string(L"三軸エミュレータ");

	container *c = static_cast<container*>(
		rw->get_client()->push_back(test_container::create(rc)));
	c->set_layout(flow_layout::create());

	text_control *tc = static_cast<text_control*>(
		c->push_back(std::make_unique<text_control>()));
	tc->set_string(L"あいうえお");

	/*
	for (int i = 0; i < 200; i++)
	{
		button *b = static_cast<button*>(
			c->push_back(button::create(0, 0, 200, 16)));

		b->set_style(s);
		b->on_click = [](mouse &m)->bool{
			std::cout << "click" << m.pt.x << ", " << m.pt.y << std::endl;
			return true;
		};
	}
	*/

	//c->push_back(std::move(tc0));

	//std::wcout << rw2->get_title_string() << std::endl;

	//uint32_t fc = fs.create(
	//	10, font::sans_serif, 400, false, L"Meiryo UI")->get_code();

	// ボタンのスタイル

	//ss.insert(typeid(button));
	//*s = button::create_default_style(ws);

	rw->show();

	//size_int s1 = f.get_size(),
	//	s2 = a3e->get_size();




	/*
	a3e_control* ctrl = f.assign(new a3e_control);

	ctrl->on_size = [&](size_int size)
	{
		size_int sz = size;
	};
	ctrl->set_size(size_int(100, 100));

	gl_context ctx;

		f.on_create = [&]()
		{
			ctx.assign(f, gl_context::flag::WINDOW, 24, 24);
			//ctx.make_current();
			//ctx.unmake_current();
		};

		f.on_size = [&](size_int size)
		{
			int i = 0;
		};

		f.on_paint = [&](canvas& cv)
		{
			size_int size = f.get_size();
			double dx = double(size.cx) / size.cy;
			ctx.make_current(cv);

			glViewport(0, 0, size.cx, size.cy);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-dx, dx, -1, 1, 0, 3);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
				1.0, 1.0, 1.0,
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBegin(GL_LINES);

			glPushMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 3.0f);

			//glColor3f(1.0f, 0.0f, 0.0f); // 赤
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 3.0f, 0.0f);

			//glColor3f(0.0f, 1.0f, 0.0f); // 緑
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(3.0f, 0.0f, 0.0f);
			glPopMatrix();

			// 座標線
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 0.0f); // 黄

			// xz
			glVertex3f(0.5f, 0.0f, 0.3f);
			glVertex3f(0.5f, 0.5f, 0.3f);
			// yz
			glVertex3f(0.0f, 0.5f, 0.3f);
			glVertex3f(0.5f, 0.5f, 0.3f);
			// xy
			glVertex3f(0.5f, 0.5f, 0.0f);
			glVertex3f(0.5f, 0.5f, 0.3f);


			glPopMatrix();

			glEnd();

			glFlush();

			GLUquadricObj* obj = gluNewQuadric();
			glPushMatrix();
			glTranslatef(0.5f, 0.5f, 0.3f);
			glRotatef(270.0f, 1.0f, 0.0f, 0.0f);

			glColor3f(0.5f, 0.5f, 0.5f);
			gluQuadricDrawStyle(obj, GLU_FILL);
			gluQuadricNormals(obj, GLU_SMOOTH);
			gluQuadricTexture(obj, GLU_FALSE);
			gluQuadricOrientation(obj, GLU_OUTSIDE);
			gluCylinder(obj, 0.02, 0.02, 0.2, 8, 4);
			glPopMatrix();

			gluDeleteQuadric(obj);
			
			//glVertex2d(0.9, -0.9);
			//glVertex2d(0.9, 0.9);
			//glVertex2d(-0.9, 0.9);


			ctx.unmake_current(cv);
			cv.draw_string(L"テスト文字列", point_int(30, 30));
		};
		f.on_destroy = [&]() { ws.quit(); };
		//f.set_parent(nullptr);

		f.set_size(size_int(640, 480));
		f.set_position(point_int(0, 0));
		f.set_title(std::wstring(L"3AxisEmulator"));

		//control* c = &f;
		//c->set_size(size_int(100, 100));

		f.show();
		*/
		ws.run();

	try
	{	}
	catch (std::runtime_error const& e)
	{
		std::cout << e.what() << std::endl;
	}



	return 0;
}
