#include "pch.h"

#include "OrthographicCameraController.h"

#include "Hazel/Input.h"
#include "Hazel/KeyCode.h"

namespace Hazel
{
	OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation)
		: m_AspectRatio{aspectRatio},
		  m_Camera{-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel},
		  m_Rotation{rotation} {}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		const float time = timestep.GetSeconds();

		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * time;

		m_Camera.SetPosition(m_CameraPosition);

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * time;
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * time;

			m_Camera.SetRotation(m_CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher{event};

		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	}

	bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		m_CameraTranslationSpeed = m_ZoomLevel;

		return false;
	}
}
