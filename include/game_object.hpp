#pragma once

#include "matrix_stack.hpp"
#include "xplor_types.hpp"
#include <stb_image.h>
#include <string>
#include <vector>
#include <array>
#include <memory>

struct ImageData
{
	int width;
	int height;
	int channels;
	unsigned char* data;
};

namespace Xplor {

	// A Game Object should contain also relevant information about where to place an object in the game world
	// and how to render it

	template <typename T>
	class GameObject
	{
	public:
		void Init();
		
		void AddTexture(std::string imagePath, ImageFormat format)
		{
			imgData imageBox;
			stbi_set_flip_vertically_on_load(true); // Align the coordinates
			std::string fullPath = resources + imagePath;
			// Fill Variables with image data
			imageBox.data = stbi_load(fullPath.c_str(), &imageBox.width, &imageBox.height, &imageBox.channels, 0);
			if (!imageBox.data)
			{
				std::cout << "Error: Image failed to load" << std::endl;
			}
			//--- Texture generation
			uint32_t texture1;
			// Generate one texture and store it
			glGenTextures(1, &texture1);
			// Bind the texture
			glBindTexture(GL_TEXTURE_2D, texture1);
			// Set the texture filtering and wrapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			// Generate the bound texture
			if (format == ImageFormat::jpg)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageBox.width, imageBox.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBox.data);
			else if (format == ImageFormat::png)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBox.width, imageBox.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBox.data);

			
			// Generate mipmaps for the bound texture
			glGenerateMipmap(GL_TEXTURE_2D);
			// Free the image data once the texture has been created
			stbi_image_free(imageBox.data);

			m_textures.push_back(texture1);
		}

		void AddShader(std::string vertexShaderPath, std::string fragmentShaderPath)
		{
			std::string fullVertexPath = resources + vertexShaderPath;
			std::string fullFragmentPath = resources + fragmentShaderPath;

			//m_shader = std::make_shared<Shader>(Xplor::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str()) );
			m_shader = new Xplor::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str());
		}

		void Update();

		void Render();

		void Delete();

		std::vector<uint32_t> GetTextures()
		{
			return m_textures;
		};

		Shader* GetShader()
		{
			return m_shader;
		}
		

	protected:
		const std::string resources = "..//resources//";
		ImageData tempImg;
		std::vector<uint32_t> m_textures{};
		Shader* m_shader; // having this not set as a pointer forces me to make a default constructor
		//std::shared_ptr<Shader> m_shader;

	private:

	}; // end class


	class PropObject : public GameObject<PropObject>
	{

	};

}; // end namespace

