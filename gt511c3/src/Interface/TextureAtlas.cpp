#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(const std::string& filename, bool mipmaps, bool flipped) :
	Texture(filename, mipmaps, flipped),
	offset({ 3.0f, 3.0f }),
	size({ 60.0f, 60.0f })
{
	this->load();
}

TextureAtlas::~TextureAtlas()
{
}
