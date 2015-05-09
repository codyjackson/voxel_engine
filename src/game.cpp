#include "game.h"
#include "window.h"

Game::Game(Window& window)
	:_player(), 
	_camera(Transform::make_transform(glm::vec3(), Orientation(), _player.get_transform())), 
	_chunkVault(meters(2.0f / 13.0f), glm::vec3(0, 0, 0)),
	_resolution(1024, 768),
	_editor(_chunkVault, _camera)
{
	_player.move_forward(-meters(2.0));

	//ui.update_resolution(resolution);

	window.update_resolution(_resolution);
	window.update_title("Voxel Engine");

	/*window.on_key_event(std::bind(&UI::forward_key_event, &ui, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	window.on_mouse_move_event(std::bind(&UI::forward_mouse_move_event, &ui, std::placeholders::_1, std::placeholders::_2));
	window.on_mouse_button_event(std::bind(&UI::forward_mouse_button_event, &ui, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	window.on_mouse_wheel_event(std::bind(&UI::forward_mouse_wheel_event, &ui, std::placeholders::_1, std::placeholders::_2));*/


	JSValue::Object api;
	api["player"] = _player.create_ui_api();
	api["input"] = window.input().create_ui_api();
	api["editor"] = _editor.create_ui_api();
	//ui.register_api(api);

	//window.input().on(Input::MoveableCombo(Input::MoveableTerminal::MOUSE), [&](Input& in){
	//	const auto delta = in.mouse().get_position_delta();
	//	player.rotate_yaw(delta.x * 0.15f);
	//	player.rotate_pitch(delta.y * 0.2f);
	//});

	//window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_2, Input::PressableEvent::RELEASED), [&](Input& in){
	//	Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
	//	if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
	//		chunkVault.delete_voxel(intersection->get_object_of_interest());
	//	}
	//});

	//window.input().on(Input::PressableTerminal(Input::Pressable::MOUSE_BUTTON_1, Input::PressableEvent::RELEASED), [&](Input& in){
	//	Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
	//	if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
	//		chunkVault.add_adjacent_voxel(intersection->get_object_of_interest(), Color(0x00, 0x85, 0xAD, 255));
	//	}
	//});

	//window.input().mouse().lock_movement();

	std::string vertexPath = "C:/Users/sxenog/Documents/Projects/voxel_engine/src/shaders/test/test.vert";
	std::string fragmentPath = "C:/Users/sxenog/Documents/Projects/voxel_engine/src/shaders/test/test.frag";
	std::vector<glm::vec3> verts = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};
	vao = VertexArrayObject::make<Vertex>();
	buffer = std::make_shared<VertexBufferObject>(Buffer::Usage::STATIC_DRAW, verts, vao);
	pro = std::make_shared<Program>(vertexPath, fragmentPath);
}

void Game::onIteration(Window& window, float timeStepInSeconds)
{
	//ui.tick();
	/*player.tick(timeStepInSeconds);

	Ray r(camera.get_transform()->position(), camera.get_transform()->orientation().forward());
	Renderer::clear_screen();
	if (const auto intersection = chunkVault.find_nearest_intersection(r)) {
	const auto modelMatrix = chunkVault.get_voxel_model_matrix(intersection->get_object_of_interest());
	const auto mesh = chunkVault.get_mesh_of_voxel(intersection->get_object_of_interest());
	Renderer::render_wireframe(camera, modelMatrix, Color(0xFF, 0xFF, 0xFF, 255), mesh);
	}
	chunkVault.render(camera);*/
	//ui.render();
	/*Binder<Program> programBinder(*pro);
	Binder<VertexArrayObject> vaoBinder(*vao);
	Binder<Buffer> bufferBinder(*buffer);*/

	pro->bind();
	buffer->bind();

	Renderer::clear_screen();
	glDrawArrays(GL_TRIANGLES, 0, 3); gl_error_check();
}

Game::~Game()
{
	pro.reset();
	vao.reset();
	buffer.reset();
}