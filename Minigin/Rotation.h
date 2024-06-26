#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace diji
{
    class Transform;
    class GameObject;

    class Rotation final : public Component
    {
        constexpr static int TWO_PI_DEGRESS{ 360 };

    public:
        Rotation(GameObject* ownerPtr);
        Rotation(GameObject* ownerPtr, float rotationSpeed, float distanceFromCenter);
        ~Rotation() noexcept override = default;

        Rotation(const Rotation& other) = delete;
        Rotation(Rotation&& other) = delete;
        Rotation& operator=(const Rotation& other) = delete;
        Rotation& operator=(Rotation&& other) = delete;

        void SetRotationAngle(float angle) { m_RotationAngle = angle; }
        float GetRotationAngle() const { return m_RotationAngle; }

        void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }
        float GetRotationSpeed() const { return m_RotationSpeed; }

        void SetCenter(const glm::vec3& center) { m_Center = center; }
        const glm::vec3& GetCenter() const { return m_Center; }

        void SetDistanceFromCenter(float distance) { m_DistanceFromCenter = distance; }
        float GetDistanceFromCenter() const { return m_DistanceFromCenter; }

        void Init() override;
        void Update() override;
        void FixedUpdate() override {};

    private:
        GameObject* m_OwnerPtr{};
        glm::vec3 m_Center;
        float m_RotationAngle;
        float m_RotationSpeed;
        float m_DistanceFromCenter;
    };
}
