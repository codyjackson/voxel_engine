#pragma once

#include "camera.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	static void clear_screen()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	template<typename T>
	static void render(const Camera& c, const T& objectOfInterest)
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);

		setup_matrices(c.get_projection_matrix(), c.get_view_matrix(), objectOfInterest.get_model_matrix());
		objectOfInterest.get_mesh().draw_with_color();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	template<typename T>
	static void render_wireframe(const Camera& c, const T& objectOfInterest)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1);
		glColor3f(0, 0, 0);

		setup_matrices(c.get_projection_matrix(), c.get_view_matrix(), objectOfInterest.get_model_matrix());
		objectOfInterest.get_mesh().draw_without_color();
	}

private:
	static inline void setup_matrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(projection));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(view));
		glMultMatrixf(glm::value_ptr(model));
	}
};