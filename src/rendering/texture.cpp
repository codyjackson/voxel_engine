#include "texture.h"

Texture::Texture(const Texture& rhs)
:_handle(NULL), _size(rhs._size), _pixelFormat(rhs._pixelFormat), _pixelDataType(rhs._pixelDataType)
{
	generate_new_texture();
	update_all_pixels(rhs.get_pixels_raw().data());
}

Texture::Texture(const RectSize& size, PixelFormat pixelFormat, PixelDataType pixelDataType, const void* pixels)
:_handle(NULL), _size(size), _pixelFormat(pixelFormat), _pixelDataType(pixelDataType)
{
	generate_new_texture();
	update_all_pixels(pixels);
}

Texture::Texture(const RectSize& size, PixelFormat pixelFormat, PixelDataType pixelDataType, const Color& defaultColor)
:_handle(NULL), _size(size), _pixelFormat(pixelFormat), _pixelDataType(pixelDataType)
{
	generate_new_texture();
	update_all_pixels(get_default_color_pixels(defaultColor).data());
}

Texture::~Texture()
{
	glDeleteTextures(1, &_handle);
}

void Texture::generate_new_texture()
{
	if (_handle)  {
		glDeleteTextures(1, &_handle);
	}

	glGenTextures(1, &_handle);
	if (auto error = glGetError()) {
		throw std::runtime_error("Failed to create texture. Make sure this is called after the gl context has been created.");
	}

	{
		Binder binder = bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

std::vector<int> Texture::get_pixels_raw() const
{
	std::vector<int> data;
	data.reserve(_size.get_area());

	Binder binder(_handle);
	glGetTexImage(GL_TEXTURE_2D, 0, static_cast<GLenum>(_pixelFormat), static_cast<GLenum>(_pixelDataType), data.data());
	return data;
}

void Texture::update_all_pixels(const void* pixels)
{
	Binder binder(_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.width, _size.height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, _size.width);
}

std::vector<int> Texture::get_default_color_pixels(const Color& defaultColor) const
{
	if (_pixelFormat == PixelFormat::RGBA) {
		auto test = std::vector<int>(_size.get_area(), defaultColor.to_raw());
		return test;
	} else {
		auto test = std::vector<int>(_size.get_area(), defaultColor.to_bgra_raw());
		return test;
	}
}

Texture& Texture::operator=(const Texture& rhs)
{
	_size = rhs._size;
	_pixelFormat = rhs._pixelFormat;
	_pixelDataType = rhs._pixelDataType;

	generate_new_texture();
	update_all_pixels(rhs.get_pixels_raw().data());

	return *this;
}

void Texture::resize(const RectSize& size, const void* pixels)
{
	_size = size;
	update_all_pixels(pixels);
}

void Texture::update_pixel_rect(Offset offset, RectSize size, const void* pixels)
{
	Binder binder(_handle);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, offset.y);
	glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size.width, size.height, static_cast<GLenum>(_pixelFormat), static_cast<GLenum>(_pixelDataType), pixels);
}

Texture::Binder Texture::bind() const
{
	return Binder(_handle);
}

const RectSize& Texture::get_size() const
{
	return _size;
}

Texture::Binder::Binder(GLuint handle)
{
	glGetIntegerv(GL_TEXTURE_2D, reinterpret_cast<GLint*>(&_previousHandle));
	glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::Binder::~Binder()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}