#pragma once
#include "Component.h"
#include <string>

namespace diji
{
	class Texture2D;
	class GameObject;

	class Texture final : public Component
	{
	public:
		Texture(GameObject* ownerPtr);
		Texture(GameObject* ownerPtr, const std::string& filename);
		Texture(GameObject* ownerPtr, const std::string& filename, int width, int height);
		Texture(GameObject* ownerPtr, const std::string& filename, int width, int height, int frames);
		~Texture() noexcept override = default;

		Texture(const Texture& other) = delete;
		Texture(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override {};

		void SetTexture(const std::string& filename);
		Texture2D* GetTexturePtr() const { return m_TexturePtr; };

		void SetWidth(int width) { m_Width = width; }
		void SetHeight(int height) { m_Height = height; }
		void SetAnimated() { m_IsAnimated = true; }
		void SetNrOfFrames(int nrOfFrames) { if (nrOfFrames >= 0) m_NrOfFrames = nrOfFrames; }
		void SetRotation(bool canRotate) { m_CanRotate = canRotate; }
		void SetCurrentFrame(int frame) { if (frame >= 0) m_Frame = frame; }
		void SetRotationAngle(float angle) { m_RotationAngle = angle; }
		void PauseAnimation() { m_IsAnimationPaused = true; }
		void ResumeAnimation() { m_IsAnimationPaused = false; }
		void SetStartingFrame(int frame);

		bool IsAnimated() const { return m_IsAnimated; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetFrame() const { return m_Frame; }
		bool CanRotate() const { return m_CanRotate; }
		float GetRotationAngle() const { return m_RotationAngle; }

	private:
		Texture2D* m_TexturePtr;
		std::string m_FilePath;
		int m_Width;
		int m_Height;
		int m_NrOfFrames = 0;
		int m_Frame = 0;
		int m_StartingFrame = 0;
		float m_FrameTime = 0;
		float m_RotationAngle = 0;

		bool m_IsAnimated = false;
		bool m_CanRotate = false;
		bool m_IsAnimationPaused = false;
	};
}

