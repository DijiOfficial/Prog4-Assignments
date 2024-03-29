#include "Texture.h"
#include "Time.h"

diji::Texture::Texture(GameObject* ownerPtr)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_Width{ 0 }
	, m_Height{ 0 }
	, m_NrOfFrames{ 0 }
	, m_Frame{ 0 }
	, m_FrameTime{ 0 }
	, m_IsAnimated{ false }
{
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_Width{ 0 }
	, m_Height{ 0 }
	, m_NrOfFrames{ 0 }
	, m_Frame{ 0 }
	, m_FrameTime{ 0 }
	, m_IsAnimated{ false }
{
		SetTexture(filename);
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename, int width, int height)
	: Component(ownerPtr)
	, m_TexturePtr{ nullptr }
	, m_Width{ width }
	, m_Height{ height }
	, m_NrOfFrames{ 0 }
	, m_Frame{ 0 }
	, m_FrameTime{ 0 }
	, m_IsAnimated{ false }
{
	SetTexture(filename);
}

diji::Texture::Texture(GameObject* ownerPtr, const std::string& filename, int width, int height, int frames)
	: Texture(ownerPtr, filename, width, height)
{
	m_NrOfFrames = frames;
	m_IsAnimated = true;
}

void diji::Texture::SetTexture(const std::string& filename)
{
	m_TexturePtr = ResourceManager::GetInstance().LoadTexture(filename);
}

void diji::Texture::Update()
{
	if (not m_IsAnimated)
		return;

	m_FrameTime += Time::GetInstance().GetDeltaTime();
	if (m_FrameTime >= 0.3f / m_NrOfFrames)
	{
		++m_Frame;
		m_FrameTime = 0;
		if (m_Frame >= m_NrOfFrames)
			m_Frame = 0;
	}
}
