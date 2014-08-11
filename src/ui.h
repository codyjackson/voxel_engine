#pragma once

#include "browser/browser.h"
#include "rendering/texture.h"

#include <functional>
#include <memory>
#include <vector>


typedef unsigned int GLuint;

class UI
{
public:
	UI();

	void update_resolution(const RectSize& resolution);
	const RectSize& get_resolution() const;


	void register_api(const JSValue& api);

	~UI();

	void tick();
	void render();

private:
	void update_texture(const RectSize& fullSize, const CefRenderHandler::RectList& dirtyRects, const void* buffer);

	std::shared_ptr<Texture> _texture;
	CefRefPtr<Browser> _browser;
};