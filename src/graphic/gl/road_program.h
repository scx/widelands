/*
 * Copyright (C) 2006-2014 by the Widelands Development Team
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

#ifndef WL_GRAPHIC_GL_ROAD_PROGRAM_H
#define WL_GRAPHIC_GL_ROAD_PROGRAM_H

#include <memory>
#include <vector>

#include "base/macros.h"
#include "graphic/gl/fields_to_draw.h"
#include "graphic/gl/utils.h"
#include "logic/roadtype.h"

class Texture;
class Surface;

class RoadProgram {
public:
	// Compiles the program. Throws on error.
	RoadProgram();
	~RoadProgram();

	// Draws the roads. The 'surface' is needed to convert from pixel space to
	// GL space.
	void draw(const Surface& surface, const FieldsToDraw& fields_to_draw, float z_value);

private:
	struct PerVertexData {
		float gl_x;
		float gl_y;
		float texture_x;
		float texture_y;
		float brightness;

		// This is a hack: we want to draw busy and normal roads in the same
		// run, but since samplers (apparently?) cannot be passed through
		// attribute arrays, we instead sample twice (busy and normal) and mix
		// them together with 'texture_mix' which is either 1 or 0.
		float texture_mix;
	};
	static_assert(sizeof(PerVertexData) == 24, "Wrong padding.");

	// Adds a road from 'start' to 'end' to be rendered in this frame using the
	// correct texture for 'road_type'.
	void add_road(const Surface& surface,
	              const FieldsToDraw::Field& start,
	              const FieldsToDraw::Field& end,
	              const Widelands::RoadType road_type);

	// The buffer that will contain 'vertices_' for rendering.
	Gl::Buffer gl_array_buffer_;

	// The program used for drawing the roads.
	Gl::Program gl_program_;

	// Attributes.
	GLint attr_position_;
	GLint attr_texture_position_;
	GLint attr_brightness_;
	GLint attr_texture_mix_;

	// Uniforms.
	GLint u_normal_road_texture_;
	GLint u_busy_road_texture_;
	GLint u_z_value_;

	// All vertices that get rendered this frame.
	std::vector<PerVertexData> vertices_;

	// The road textures.
	std::unique_ptr<Texture> normal_road_texture_;
	std::unique_ptr<Texture> busy_road_texture_;

	DISALLOW_COPY_AND_ASSIGN(RoadProgram);
};

#endif  // end of include guard: WL_GRAPHIC_GL_ROAD_PROGRAM_H
