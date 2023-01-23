#include "pch.h"

#include "LayerStack.h"

namespace Hazel
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
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
		const auto end = std::next(m_Layers.begin(), m_LayerInsertIndex);
		if (const auto it = std::find(m_Layers.begin(), end, layer);
			it != end)
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			layer->OnDetach();
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto begin = std::next(m_Layers.begin(), m_LayerInsertIndex);
		if (const auto it = std::find(begin, m_Layers.end(), overlay);
			it != m_Layers.end())
		{
			m_Layers.erase(it);
			overlay->OnDetach();
		}
	}
}
