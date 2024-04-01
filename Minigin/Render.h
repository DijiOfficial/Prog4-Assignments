#pragma once
#include "GameObject.h"

namespace diji
{
	class Texture2D;
	class Texture;
	class Text;
	class Transform;

	class Render final : public Component
	{
	public:
		Render(GameObject* ownerPtr);
		Render(GameObject* ownerPtr, int scale);
		~Render() override = default;

		Render(const Render& other) = delete;
		Render(Render&& other) = delete;
		Render& operator=(const Render& other) = delete;
		Render& operator=(Render&& other) = delete;

		void Update() override;
		void RenderFrame() const;
		void UpdateText();

	private:
		Texture2D* m_TexturePtr{};
		Texture* m_TextureCompPtr{};
		Text* m_TextCompPtr{};
		Transform* m_TransformCompPtr{};

		int m_Scale{ 1 };
	};
}