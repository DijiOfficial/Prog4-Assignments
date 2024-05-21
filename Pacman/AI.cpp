#include "AI.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "PickUp.h"
#include "ScoreCounter.h"

#include "Observers.h"
#include "TimeSingleton.h"
#include <iostream>
#include "Render.h"

pacman::AI::AI(diji::GameObject* ownerPtr)
	: Component(ownerPtr)
{
	m_TextureCompPtr = ownerPtr->GetComponent<diji::Texture>();
	m_TransformCompPtr = ownerPtr->GetComponent<diji::Transform>();
	m_ColliderCompPtr = ownerPtr->GetComponent<diji::Collider>();
	assert(m_TextureCompPtr and "AI Component needs to be initialized aftera Texture");
	assert(m_TransformCompPtr and "AI Component needs to be initialized aftera Transform");
	assert(m_ColliderCompPtr and "AI Component needs to be initialized aftera Collider");

	m_TransformCompPtr->SetMovement(m_PreviousMovement);
	//m_CurrentStateUPtr = std::make_unique<Idle>();
	//m_CurrentStateUPtr->OnEnter();
}

void pacman::AI::Update()
{

	if (m_IsPoweredUp)
	{
		PowerUpTimer += diji::TimeSingleton::GetInstance().GetDeltaTime();
		if (PowerUpTimer >= 10.f)
		{
			m_IsPoweredUp = false;
			PowerUpTimer = 0.f;
		}
	}
}

void pacman::AI::FixedUpdate()
{
	const auto& currentMovement = m_TransformCompPtr->GetMovement();

	const auto& shape = CalculateNewPosition(currentMovement);
	if (not diji::Collision::GetInstance().IsCollidingWithWorld(shape))
	{
		m_TransformCompPtr->SetPosition(shape.left, shape.bottom);
		if (m_PreviousMovement != currentMovement)
		{
			if (m_PreviousMovement == diji::Movement::Idle)
				m_TextureCompPtr->ResumeAnimation();
			if (currentMovement == diji::Movement::Idle)
				m_TextureCompPtr->PauseAnimation();
			else
				m_TextureCompPtr->SetRotationAngle(static_cast<int>(currentMovement) * 90.f);

			m_PreviousMovement = currentMovement;
		}
	}
	else
	{
		const auto& oldShape = CalculateNewPosition(m_PreviousMovement);

		if (not diji::Collision::GetInstance().IsCollidingWithWorld(oldShape))
			m_TransformCompPtr->SetPosition(oldShape.left, oldShape.bottom);
		else
		{
			m_TransformCompPtr->SetMovement(diji::Movement::Idle);
			// Smooth out collision (testing)
			//SmoothOutCollision(shape, m_PreviousMovement);
		}
	}

	// late update stuff
	if (currentMovement == diji::Movement::Left)
		if (shape.left < 0 - shape.width)
			m_TransformCompPtr->SetPosition(TOTAL_WIDTH, shape.bottom);
	if (currentMovement == diji::Movement::Right)
		if (shape.left > TOTAL_WIDTH)
			m_TransformCompPtr->SetPosition(0 - shape.width, shape.bottom);

	//std::cout << "AI: " << shape.left << " " << shape.bottom << std::endl;

	//if (Collision::GetInstance().IsCollidingWithIntersection(shape))
	//{
	//	std::cout << "AI: Intersection Collision" << std::endl;
	//}

}

void pacman::AI::OnNotify(diji::MessageTypes message, [[maybe_unused]] diji::Subject* subject)
{
	auto msg = static_cast<MessageTypesDerived>(message);
	switch (msg)
	{
	case MessageTypesDerived::LEVEL_COLLISION:
	{
		std::cout << "AI: Level Collision" << std::endl;
		break;
	}
	case MessageTypesDerived::PICKUP_COLLISION:
	{
		PickUp* pickUp = dynamic_cast<PickUp*>(subject);

		const int value = pickUp->GetValue();
		GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);
		break;
	}
	case MessageTypesDerived::POWERUP_COLLISION:
	{
		PickUp* pickUp = dynamic_cast<PickUp*>(subject);

		const int value = pickUp->GetValue();
		GetOwner()->GetComponent<ScoreCounter>()->IncreaseScore(value);

		m_IsPoweredUp = true;
		PowerUpTimer = 0.f;
		break;
	}
	default:
		break;
	}
}

const diji::Rectf pacman::AI::CalculateNewPosition(diji::Movement movement)
{
	auto shape = m_ColliderCompPtr->GetCollisionBox();
	//const auto& deltaTime = TimeSingleton::GetInstance().GetDeltaTime();
	switch (movement)
	{
	case diji::Movement::Up:
		//shape.bottom -= m_Speed.y * deltaTime;
		//--shape.bottom;
		shape.bottom -= 2;
		break;
	case diji::Movement::Down:
		//shape.bottom += m_Speed.y * deltaTime;
		//++shape.bottom;
		shape.bottom += 2;
		break;
	case diji::Movement::Left:
		//shape.left -= m_Speed.x * deltaTime;
		//--shape.left;
		shape.left -= 2;
		break;
	case diji::Movement::Right:
		//shape.left += m_Speed.x * deltaTime;
		//++shape.left;
		shape.left += 2;
		break;
	}

	if (diji::Collision::GetInstance().IsCollidingWithWorld(shape))
	{
		switch (movement)
		{
		case diji::Movement::Up:
			++shape.bottom;
			break;
		case diji::Movement::Down:
			--shape.bottom;
			break;
		case diji::Movement::Left:
			++shape.left;
			break;
		case diji::Movement::Right:
			--shape.left;
			break;
		}
	}

	return shape;
}
