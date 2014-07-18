/*
 * Copyright (C) 2002-2004, 2006, 2008 by the Widelands Development Team
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef WL_GRAPHIC_COLORMAP_H
#define WL_GRAPHIC_COLORMAP_H

#include <SDL_video.h>

#include "graphic/color.h"

/**
 * Colormap contains a palette and lookup table for use with ground textures.
*/
class Colormap {
public:
	Colormap (const SDL_Color &, const SDL_PixelFormat & fmt);
	~Colormap ();

	// Returns the palette of this colormap (256 entries of RGB Colors);
	SDL_Color * get_palette() {return palette;}

	// Returns the internally calculated colormap used in the renderer.
	void * get_colormap () const {return colormap;}

private:
	SDL_Color palette[256];

	/// maps 8 bit color and brightness value to the shaded color.
	/// \note Brightness is currently 8 bits. Restricting brightness to 64 or
	/// less shades would greatly reduce the size of this table, and thus
	/// improve memory cache impact inside the renderer.
	void * colormap;
};

#endif  // end of include guard: WL_GRAPHIC_COLORMAP_H
