#pragma once
#include <glm/vec2.hpp>

struct SDL_Texture;
namespace diji
{
	/**
		* Simple RAII wrapper for an SDL_Texture
		*/
	class Texture2D final
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D() noexcept;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;
		glm::ivec2 GetSize() const;

	private:
		SDL_Texture* m_TexturePtr;
	};
}