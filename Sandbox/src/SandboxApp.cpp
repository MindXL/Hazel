#include <Hazel.h>

class ExampleLayer :public Hazel::Layer
{
public:
	ExampleLayer() :Layer{ "Example" } {}

	void OnUpdate() override
	{
		HZ_CLIENT_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_CLIENT_TRACE("{0}", event);
	}
};

class Sandbox :public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
