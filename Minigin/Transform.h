#pragma once
#include "GameObject.h"

namespace diji
{
	class Transform final : public Component
	{
	public:
		Transform(GameObject* ownerPtr) : Component(ownerPtr) { CheckRenderComponent(); SetPosition(0, 0, 0); };
		Transform(GameObject* ownerPtr, const float x, const float y, const float z) : Component(ownerPtr) { CheckRenderComponent(); SetPosition(x, y, z); };
		Transform(GameObject* ownerPtr, const float x, const float y) : Component(ownerPtr) { CheckRenderComponent(); SetPosition(x, y); };
		Transform(GameObject* ownerPtr, int x, int y, int z) : Component(ownerPtr) { CheckRenderComponent(); SetPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); };
		Transform(GameObject* ownerPtr, int x, int y) : Component(ownerPtr) { CheckRenderComponent(); SetPosition(static_cast<float>(x), static_cast<float>(y)); };
		~Transform() override = default;


		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		void Update() override { };

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const float x, const float y, const float z);
		void SetPosition(const float x, const float y);
		void SetPosition(glm::vec3 pos) { m_Position = pos; };

	private:
		glm::vec3 m_Position;
		void CheckRenderComponent();
	};
}
