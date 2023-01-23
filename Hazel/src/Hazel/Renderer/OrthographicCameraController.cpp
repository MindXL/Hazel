#include "pch.h"

#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCode.h"

namespace Hazel
{
	OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation)
		: m_AspectRatio{aspectRatio},
		  m_Camera{-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel},
		  m_Rotation{rotation} {}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		const float time = timestep.GetSeconds();

		// TODO: Maybe convert those if-statements below to function call.
		if (const bool A = Input::IsKeyPressed(HZ_KEY_A), D = Input::IsKeyPressed(HZ_KEY_D);
			(A || D) && !(A && D))
		{
			if (A)
				m_CameraPosition.x -= m_CameraTranslationSpeed * time;
			else // D
				m_CameraPosition.x += m_CameraTranslationSpeed * time;
		}

		if (const bool W = Input::IsKeyPressed(HZ_KEY_W), S = Input::IsKeyPressed(HZ_KEY_S);
			(W || S) && !(W && S))
		{
			if (W)
				m_CameraPosition.y += m_CameraTranslationSpeed * time;
			else // S
				m_CameraPosition.y -= m_CameraTranslationSpeed * time;
		}

		m_Camera.SetPosition(m_CameraPosition);

		if (m_Rotation)
		{
			if (const bool Q = Input::IsKeyPressed(HZ_KEY_Q), E = Input::IsKeyPressed(HZ_KEY_E);
				(Q || E) && !(Q && E))
			{
				if (Q)
					m_CameraRotation += m_CameraRotationSpeed * time;
				else // E
					m_CameraRotation -= m_CameraRotationSpeed * time;
			}

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
