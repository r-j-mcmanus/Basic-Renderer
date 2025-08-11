
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>

#include "Errors.h"


void loadTexture(std::string filename, bool flip = true) {
	stbi_set_flip_vertically_on_load(flip);
	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	if (!data) {
		std::cerr << "failed to open file " << filename;
		return;
	}


	unsigned int textureRef;

	const int howManyTextures = 1;
	GLCall(glGenTextures(howManyTextures, &textureRef));

	// register which texture buffer we are using
	GLCall(glBindTexture(GL_TEXTURE_2D, textureRef));

	// set the texture wrapping/filtering options (on the currently bound texture object)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	
	// Map channel count to GL format
	GLenum format = channelsToGLType(channels);
	if (format == 0) {
		std::cerr << "Unsupported channel count: " << channels << "\n";
		stbi_image_free(data);
		return;
	}

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	
	// make textures of various sizes based on the current texture
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	// free image after it has been put into a buffer
	stbi_image_free(data);
}

int channelsToGLType(int chanels) {
	switch (chanels) {
	case 1: // Greyscale
		return GL_RED;
	case 2: // Greyscale + Alpha
		return GL_RG;
	case 3: // RGB
		return GL_RGB;
	case 4: // RGBA
		return GL_RGBA;
	default:
		return 0;
	}
}

// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit compnents per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data);