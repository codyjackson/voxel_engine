#pragma once

#include "color.h"
#include "pixel_data_type.h"
#include "pixel_format.h"
#include "../spatial/offset.h"
#include "../spatial/rect_size.h"

#include <boost/noncopyable.hpp>
#include <vector>

class Texture
{
	public:
		class Binder;

	public:
		Texture(const Texture& rhs);
		Texture(const RectSize& size, PixelFormat pixelFormat, PixelDataType pixelDataType, const void* pixels);
		Texture(const RectSize& size, PixelFormat pixelFormat, PixelDataType pixelDataType, const Color& defaultColor);
		~Texture();

		Texture& operator=(const Texture& rhs);

		void resize(const RectSize& size, const void* pixels);
		void update_pixel_rect(Offset offset, RectSize size, const void* pixels);

		Binder bind() const;

		const RectSize& get_size() const;

	private:
		void generate_new_texture();

		std::vector<int> get_pixels_raw() const;

		void update_all_pixels(const void* pixels);

		std::vector<int> get_default_color_pixels(const Color& defaultColor) const;

		RectSize _size;
		PixelFormat _pixelFormat;
		PixelDataType _pixelDataType;
		GLuint _handle;
};

class Texture::Binder : boost::noncopyable
{
public:
	Binder(GLuint handle);
	~Binder();

private:
	GLenum _target;
	GLuint _previousHandle;
};