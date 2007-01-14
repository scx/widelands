/*
 * Copyright (C) 2003, 2006-2007 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <cassert>
#include "graphic.h"
#include "rendertarget.h"
#include "types.h"
#include "ui_tabpanel.h"


namespace UI {
#define TP_BUTTON_WIDTH     34
#define TP_BUTTON_HEIGHT    34

//  height of the bar separating buttons and tab contents
#define TP_SEPARATOR_HEIGHT  4


/**
Initialize an empty Tab_Panel
*/
Tab_Panel::Tab_Panel(Panel* parent, int x, int y, uint background)
	: Panel(parent, x, y, 0, 0)
{
	m_active = 0;
	m_snapparent = false;

	switch(background) {
	default:
	case 0: m_pic_background = g_gr->get_picture( PicMod_UI,  "pics/but0.png" ); break;
	case 1: m_pic_background = g_gr->get_picture( PicMod_UI,  "pics/but1.png" ); break;
	case 2: m_pic_background = g_gr->get_picture( PicMod_UI,  "pics/but2.png" ); break;
	case 3: m_pic_background = g_gr->get_picture( PicMod_UI,  "pics/but3.png" ); break;
	}

	m_highlight = -1;
}


/**
Resize according to number of tabs and the size of the currently visible
contents.
Resize the parent if snapparent is enabled.
*/
void Tab_Panel::resize()
{
	int w;
	int h;

	// size of button row
	w = TP_BUTTON_WIDTH * m_tabs.size();
	h = TP_BUTTON_HEIGHT + TP_SEPARATOR_HEIGHT;

	// size of contents
	if (m_active < m_tabs.size())
		{
		Panel* panel = m_tabs[m_active].panel;

		if (panel->get_w() > w)
			w = panel->get_w();
		h += panel->get_h();
		}

	set_size(w, h);

	// adjust parent's size, if necessary
	if (m_snapparent) {
		get_parent()->set_inner_size(w, h);
		get_parent()->move_inside_parent();
	}
}


/**
If snapparent is enabled, the parent window will be adjusted to the size of the
Tab_Panel whenever the Tab_Panel's size changes.
The default for snapparent behaviour is off.
*/
void Tab_Panel::set_snapparent(bool snapparent)
{
	m_snapparent = snapparent;
}


/**
Add a new tab
*/
uint Tab_Panel::add(uint picid, Panel* panel, const std::string & tooltip_text)
{
	assert(panel);
	assert(panel->get_parent() == this);

	Tab t;
	uint id;

	t.picid = picid;
	t.tooltip = tooltip_text;
	t.panel = panel;

	m_tabs.push_back(t);
	id = m_tabs.size() - 1;

	panel->set_pos(Point(0, TP_BUTTON_HEIGHT + TP_SEPARATOR_HEIGHT));
	panel->set_visible(id == m_active);

	return id;
}


/**
Make a different tab the currently active tab.
*/
void Tab_Panel::activate(uint idx)
{
	if (m_active < m_tabs.size())
		m_tabs[m_active].panel->set_visible(false);
	if (idx < m_tabs.size())
		m_tabs[idx].panel->set_visible(true);

	m_active = idx;

	resize();
}


/**
Draw the buttons and the tab
*/
void Tab_Panel::draw(RenderTarget* dst)
{
	uint idx;
	int x;

	// draw the background
	compile_assert(2 < TP_BUTTON_WIDTH);
	compile_assert(4 < TP_BUTTON_HEIGHT);
	dst->tile
		(Rect(Point(0, 0), m_tabs.size() * TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT - 2),
		 m_pic_background, Point(get_x(), get_y()));
	assert(TP_BUTTON_HEIGHT - 2 <= get_h());
	dst->tile
		(Rect
		 (Point(0, TP_BUTTON_HEIGHT - 2),
		  get_w(), get_h() - TP_BUTTON_HEIGHT + 2),
		 m_pic_background,
		 Point(get_x(), get_y() + TP_BUTTON_HEIGHT - 2));


	// draw the buttons
	for(idx = 0, x = 0; idx < m_tabs.size(); idx++, x += TP_BUTTON_WIDTH) {
		if (m_highlight == static_cast<const int>(idx)) dst->brighten_rect
			(Rect(Point(x, 0), TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT),
			 MOUSE_OVER_BRIGHT_FACTOR);

		// Draw the icon
		uint cpw, cph;
		g_gr->get_picture_size(m_tabs[idx].picid, cpw, cph);

		dst->blit
			(Point(x + (TP_BUTTON_WIDTH - cpw)/2, (TP_BUTTON_HEIGHT - cph) / 2),
			 m_tabs[idx].picid);

		// Draw top part of border
		RGBColor black(0,0,0);

		dst->brighten_rect
			(Rect(Point(x, 0), TP_BUTTON_WIDTH, 2), BUTTON_EDGE_BRIGHT_FACTOR);
		dst->brighten_rect
			(Rect(Point(x, 2), 2, TP_BUTTON_HEIGHT - 4),
			 BUTTON_EDGE_BRIGHT_FACTOR);
		dst->fill_rect
			(Rect(Point(x + TP_BUTTON_WIDTH - 2, 2), 1, TP_BUTTON_HEIGHT - 4),
			 black);
		dst->fill_rect
			(Rect(Point(x + TP_BUTTON_WIDTH - 1, 1), 1, TP_BUTTON_HEIGHT - 3),
			 black);

		// Draw bottom part
		if (m_active != idx) dst->brighten_rect
			(Rect(Point(x, TP_BUTTON_HEIGHT - 2), TP_BUTTON_WIDTH, 2),
			 2 * BUTTON_EDGE_BRIGHT_FACTOR);
		else {
			dst->brighten_rect
				(Rect(Point(x, TP_BUTTON_HEIGHT - 2), 2, 2),
				 BUTTON_EDGE_BRIGHT_FACTOR);

			dst->brighten_rect
				(Rect(Point(x + TP_BUTTON_WIDTH - 2, TP_BUTTON_HEIGHT - 2), 2, 2),
				 BUTTON_EDGE_BRIGHT_FACTOR);
			dst->fill_rect
				(Rect(Point(x + TP_BUTTON_WIDTH - 1, TP_BUTTON_HEIGHT - 2), 1, 1),
				 black);
			dst->fill_rect
				(Rect(Point(x + TP_BUTTON_WIDTH - 2, TP_BUTTON_HEIGHT - 1), 2, 1),
				 black);
			}
		}

	// draw the remaining separator
	assert(x <= get_w());
	dst->brighten_rect
		(Rect(Point(x, TP_BUTTON_HEIGHT - 2), get_w() - x, 2),
		 2 * BUTTON_EDGE_BRIGHT_FACTOR);
}


/**
Cancel all highlights when the mouse leaves the panel
*/
void Tab_Panel::handle_mousein(bool inside)
{
	if (!inside && m_highlight >= 0)
		{
		update(m_highlight*TP_BUTTON_WIDTH, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT);

		m_highlight = -1;
		}
}


/**
Update highlighting
*/
void Tab_Panel::handle_mousemove(int x, int y, int, int) {
	int hl;

	if (y < 0 || y >= TP_BUTTON_HEIGHT)
		hl = -1;
	else
		{
		hl = x / TP_BUTTON_WIDTH;

		if (hl < 0 || hl >= (int)m_tabs.size())
			hl = -1;
		}

	if (hl != m_highlight)
		{
		{
			const char * t = 0;
			if (hl >= 0) {
				const std::string & str = m_tabs[hl].tooltip;
				if (str.size()) t = str.c_str();
			}
			set_tooltip(t);
		}
		if (m_highlight >= 0)
			update(m_highlight*TP_BUTTON_WIDTH, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT);
		if (hl >= 0)
			update(hl*TP_BUTTON_WIDTH, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT);

		m_highlight = hl;
		}
}


/**
Change the active tab if a tab button has been clicked
*/
bool Tab_Panel::handle_mousepress(const Uint8 btn, int x, int y) {
	if (btn == SDL_BUTTON_LEFT) {
		int id;

		if (y >= TP_BUTTON_HEIGHT)
			return false;

		id = x / TP_BUTTON_WIDTH;

		if (id >= 0 && id < (int)m_tabs.size())
			{
			activate(id);

			return true;
			}
		}

	return false;
}
bool Tab_Panel::handle_mouserelease(const Uint8, int, int)
{return false;}
};
