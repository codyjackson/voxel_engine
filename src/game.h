#pragma once
#include <GL/glew.h>

#include "editor/editor.h"
#include "player.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "spatial/chunk_vault.h"
#include "ui.h"
#include "units.h"

#include <glm/glm.hpp>
#include <memory>


#include "rendering/gl_rendering/vertex_array_object.h"
#include "rendering/gl_rendering/vertex_buffer_object.h"
#include "rendering/gl_rendering/program.h"
#include "rendering/gl_rendering/buffer.h"
#include "rendering/gl_rendering/error.h"

#include "utility/filesystem.h"

class Window;

class Game
{
public:
	Game(Window& window);
	void onIteration(Window& window, float timeStepInSeconds);
	~Game();

private:
	Player _player;
	Camera _camera;

	ChunkVault _chunkVault;
	RectSize _resolution;
	Editor _editor;

	std::shared_ptr<VertexArrayObject> vao;
	std::shared_ptr<VertexBufferObject> buffer;
	std::shared_ptr<ShaderProgram> pro;
};