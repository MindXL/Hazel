#pragma once

#include "OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		[[nodiscard]] OrthographicCamera& GetCamera() { return m_Camera; }
		[[nodiscard]] const OrthographicCamera& GetCamera() const { return m_Camera; }

		[[nodiscard]] float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(const float zoomLevel) { m_ZoomLevel = zoomLevel; }

	private:
		bool OnWindowResized(const WindowResizeEvent& event);
		bool OnMouseScrolled(const MouseScrolledEvent& event);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition{0.0f, 0.0f, 0.0f};
		float m_CameraTranslationSpeed = 5.0f;

		float m_CameraRotation = 0.0f; // Degree
		float m_CameraRotationSpeed = 180.0f;
	};
}
