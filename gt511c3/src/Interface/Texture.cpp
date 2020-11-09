#include "Texture.h"
#include "Core/Utils.h"
#include <iostream>
#include "glad/glad.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


	Texture::Texture(
		const std::string& filename,
		bool mipmaps,
		bool flipped,
		ChannelType type,
		bool free_after_load
	) :
		filename(filename),
		mipmaps(mipmaps),
		lodBias(0.0f),
		flipped(flipped),
		id(0),
		data(nullptr),
		channel_type(type),
		free_after_load(free_after_load),
		min_filter(Filter::LINEAR),
		mag_filter(Filter::LINEAR),
		wrap_s(WrapType::REPEAT),
		wrap_t(WrapType::REPEAT),
		load_from_memory(true)
	{
		glGenTextures(1, &id);
	}

	Texture* Texture::Texture::load(unsigned char* buffer)
	{
		stbi_set_flip_vertically_on_load(flipped);

		if (load_from_memory && buffer != nullptr) {
			data = stbi_load_from_memory(buffer, sizeof(buffer), &width, &height, &components_count, channel_type);
			std::cout << "Texture components count: " << std::dec << components_count << std::endl;
		}
		else {
			data = stbi_load(filename.c_str(), &width, &height, &components_count, channel_type);
		}
		
		if (data == NULL)
		{
			std::cout << "Texture loading failed: " << filename.c_str() << std::endl;
			return nullptr;
		}

		glBindTexture(GL_TEXTURE_2D, id);

		GLenum format;
		if (components_count == 1)
			format = GL_RED;
		else if (components_count == 3)
			format = GL_RGB;
		else if (components_count == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		std::string size = std::string("(" + std::to_string(width) + "x" + std::to_string(height) + ")");
		std::cout << "Loaded texture: " << filename << " " << size << " " << Utils::bytesToSize(Utils::getFileSize(filename)) << std::endl;
	
		if(free_after_load)
			stbi_image_free(data);

		return this;
	}

	void Texture::bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}

	Texture::~Texture()
	{
		if(!free_after_load)
			stbi_image_free(data);

		glDeleteTextures(1, &id);
	}

