#pragma once

#define NOMINMAX

#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <functional>
#include <algorithm>

#include <Windows.h>
#include <windowsx.h>

#include <usp10.h>

class render_item;

struct point
{
	uint32_t m_x = 0;
	uint32_t m_y = 0;

	point(uint32_t x, uint32_t y) : m_x(x), m_y(y) { }
};

struct rect
{
	uint32_t m_x;
	uint32_t m_y;

	uint32_t m_cx;
	uint32_t m_cy;

	rect();

	rect(uint32_t x, uint32_t y, uint32_t cx, uint32_t cy);
};

struct point_caret
{
	enum
	{
		over = 0xFFFFFFFF,
	};

	uint32_t m_x;
	uint32_t m_y;

	point_caret(uint32_t x, uint32_t y) : m_x(x), m_y(y) { }
};

class font
{
public:
	typedef std::shared_ptr<font> store;

public:
	SCRIPT_CACHE m_cache;

	LOGFONT      m_logfont;
	HFONT        m_hfont;

	COLORREF     m_color;

	uint32_t     m_height;

public:
	font();

	font(uint32_t height, std::wstring face);

	~font();

	HFONT create(uint32_t height, std::wstring face);
};

class font_span
{
public:
	font::store  m_font;
	uint32_t     m_length;

public:
	font_span(font::store f, uint32_t l);
};

class font_storage
{
protected:
	std::vector<font_span> m_storage;

public:
};

class item
{
public:
	enum : uint16_t
	{
		none = 0,
		line_end,
		string,
		image,
	};

private:
	SCRIPT_ANALYSIS m_analysis;
	
	wchar_t    *m_data;
	uint32_t m_type    : 8;
	uint32_t m_slength : 24;

public:
	item(uint32_t type = none);

	item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength);

	item(item const &ri);

	uint32_t get_type() const;

	bool is_empty() const;

	SCRIPT_ANALYSIS* get_analysis(); // shape,place�ŕK�v

	SCRIPT_ANALYSIS const* get_analysis() const;

	void* get_data();

	uint32_t get_slength() const;

	item trim_front_by_slength(uint32_t slength);
};

class render_item : public item
{
protected:
	std::vector<WORD>    m_glyphs;    // �O���t�ɑ΂��Ĉ�̗v�f
	std::vector<int>     m_advances;  // �O���t�ɑ΂��Ĉ�̗v�f
	std::vector<int>     m_justifies; // �O���t�ɑ΂��Ĉ�̗v�f
	std::vector<GOFFSET> m_goffsets;  // �O���t�ɑ΂��Ĉ�̗v�f

	std::vector<WORD>    m_log_cluster; // �L�����N�^�ɑ΂��Ĉ�̗v�f

	ABC m_abc; // �A�C�e���ɑ΂��Ĉ��

	std::vector<SCRIPT_VISATTR> m_visattr; // �O���t�ɑ΂��Ĉ�̗v�f
	std::vector<SCRIPT_LOGATTR> m_logattr; // �L�����N�^�ɑ΂��Ĉ�̗v�f

	font::store m_font;

	uint32_t m_xpt, m_ypt;
	uint16_t m_line_height;

	uint16_t m_ch_offset; // �`��A�C�e���̐擪����̕����I�t�Z�b�g
	uint16_t m_ct_offset; // �`��A�C�e���̐擪����̃L�����b�g�I�t�Z�b�g

public:
	render_item(uint32_t type = item::none);

	// �A�C�e���񂩂畜������Ƃ��g��
	render_item(item const &rhs);

	// �����񂩂番�����鎞�g��
	render_item(SCRIPT_ANALYSIS sa, wchar_t *data, uint32_t slength);

	// �R���e�i�ɑ}������Ƃ��g��
	render_item(render_item &&rhs);

	void set_position(uint32_t xpt, uint32_t ypt);

	void set_line_height(uint32_t cy);

	void set_ch_offset(uint32_t ich);

	uint32_t get_ch_offset() const;

	void set_ct_offset(uint32_t ict);

	uint32_t get_ct_offset() const;

	void set_glength(size_t nglyph);

	// �O���t����Ԃ�
	uint32_t get_glength() const;

	// �N���X�^����Ԃ�
	uint32_t get_clength() const;

	// �L�����b�g�̈ړ�����Ԃ�
	uint32_t get_ctlength() const;

	uint32_t get_height() const;

	uint32_t get_width() const;

	void set_font(font::store f);

	font::store get_font() { return m_font; }

	render_item trim_front_by_slength(HDC hdc, uint32_t slength);

	// �L�����b�g�ړ������當������Ԃ�
	uint32_t query_slength_from_ctlength(uint32_t ctlength) const;

	// ����������L�����b�g����Ԃ�
	uint32_t query_ctlength_from_slength(uint32_t slength) const;

	// �����ł���ŏ���������Ԃ�
	// �C���[�W��0��Ԃ�
	// �����ł��Ȃ��ꍇ��0��Ԃ�
	uint32_t query_softbreak_slength_by_minimum() const;

	// ��cx�ȓ��ŕ����ł���ő啶������Ԃ�
	// �C���[�W��0��Ԃ�
	// �����ł��Ȃ��ꍇ��0��Ԃ�
	uint32_t query_softbreak_slength_by_width(uint32_t cx) const;

	//rect query_rect_from_
	void break_();

	void shape(HDC hdc);

	void place(HDC hdc);

	// �O���t�Ԋu�̒����ɂ���ĕ\������dx�ɂ���
	void justify(uint32_t dx);

	void paint(HDC hdc, int32_t x, int32_t y);

	void paint_under_line_if(HDC hdc, uint32_t offset, uint32_t length);

	// x�ʒu�܂ł̕���������������
	uint32_t find_xcp_from_xpt(uint32_t x) const;

	// x�ʒu�܂ł̃L�����b�g������������
	uint32_t find_xct_from_xpt(uint32_t x) const;

	// �A�C�e���̐擪����̃L�����b�g�ʒuxct��x�s�N�Z���ʒu����������
	uint32_t find_xpt_from_nct(uint32_t nct) const;
};

/*
class item_breaker
{
public:
	typedef std::function<void(item::store)> function_type;

protected:
	SCRIPT_DIGITSUBSTITUTE &m_sds;
	function_type m_output;

public:
	item_breaker(SCRIPT_DIGITSUBSTITUTE &sds, function_type output);

	void operator()(std::wstring s);
};

template <typename T>
struct style_breaker
{
	typedef std::deque<style> storage_type;

	T &m_output; // �o�͊֐�
	storage_type m_storage; // �X�^�C���L���[

	style_breaker(T &output) : m_output(output)
	{

	}

	render_item split(render_item &ri, uint32_t len) const
	{
		ri.m_logattr[len].fCharStop;
		return ri; // todo
	}

	void operator()(item i)
	{
		render_item ri(std::forward<item>(i));
		ri.break_(); // �����\�ʒu�̏���t����

		bool flg = true;
		while (flg)
		{
			style &s = m_storage.front();
			int32_t len = ri.get_slength() - get_front_size();
			if (len <= 0) // �X�^�C���̂ق��������A���邢�͓�������
			{
				s.m_length -= ri.get_slength();
				ri.set_style(s.m_font, s.m_color);
				m_output(std::move(ri));
				flg = false;
			}
			else // �X�^�C���̂ق����Z���i�A�C�e���̕������K�v�j
			{
				render_item ri1 = split(ri, get_front_size());
				ri1.set_style(s.m_font, s.m_color);
				m_output(std::move(ri1));
			}

			// �X�^�C�����Z���A���邢�͓���
			// �Ō�̃X�^�C���́A�؂蕪���ʒu�ُ̈�ɔ����Ďc���Ă���
			if (0 <= len && 1 < m_storage.size()) m_storage.pop_front();
		}
	}

	// �X�^�C�����v�b�V��
	void push(font::store f, COLORREF c, uint32_t len)
	{
		if (!m_storage.empty())
		{
			style &back = m_storage.back();
			if (back.m_font->hfont == f->hfont && back.m_color == c)
			{
				back.m_length += len;
				return;
			}
		}
		m_storage.push_back(style(f, c, len));
	}

	// �擪�̃X�^�C���̒���
	uint32_t get_front_size() const
	{
		return m_storage.empty() ? 0 : m_storage.front().m_length;
	}
};

/// ��s���̃o�b�t�@�����̂ŁA�Ō��flush()���Ȃ���΂Ȃ�Ȃ�
struct line_breaker
{
	typedef std::function<void(item_line)> function_type;

	HDC           m_hdc;

	function_type m_output;
	uint32_t      m_cx;
	uint32_t      m_x;
	item_line     m_line_buffer; // ��s���̃o�b�t�@

	line_breaker(HDC hdc, uint32_t cx, function_type output)
		: m_hdc(hdc)
		, m_output(output)
		, m_cx(cx)
		, m_x(0)
	{

	}

	void flush()
	{
		if (!m_line_buffer.empty())
		{
			m_output(std::move(m_line_buffer));
			m_line_buffer.clear();
		}
	}

	void operator()(render_item ri)
	{
		while (!ri.empty())
		{
			ri.shape(m_hdc);
			ri.place(m_hdc);
			uint32_t ax = ri.get_width(); // �A�C�e���̕�
			uint32_t dx = m_cx - m_x; // �c��̕�

			if (ax <= dx) // ���C�����炠�ӂ�Ă��Ȃ��̂ŏ[�U
			{
				m_line_buffer.push_back(std::move(ri.m_item));
				m_x += ax;

				if (ax == dx)
				{
					m_x = 0;
					m_output(std::move(m_line_buffer));
					m_line_buffer.clear();
				}
			}
			else // ���C�����炠�ӂꂽ
			{
				item i = ri.trim_front_by_width(dx);
				if (!i.empty())
				{
					m_line_buffer.push_back(i);
				}
				m_x = 0;
				m_output(std::move(m_line_buffer));
				m_line_buffer.clear();
			}

		}
	}
};

*/