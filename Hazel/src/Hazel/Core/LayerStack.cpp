#include "pch.h"

#include "LayerStack.h"

namespace Hazel
{
	LayerStack::~LayerStack()
	{
		for (const Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;

		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);

		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		if (const auto it = std::ranges::find(m_Layers, layer); it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			layer->OnDetach();
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		if (const auto it = std::ranges::find(m_Layers, overlay); it != m_Layers.end())
		{
			m_Layers.erase(it);
			overlay->OnDetach();
		}
	}
}
