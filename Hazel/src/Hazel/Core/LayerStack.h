#pragma once

#include "Core.h"
#include "Layer.h"

namespace Hazel
{
	class HAZEL_API LayerStack
	{
	public:
		~LayerStack();

		/*
		Layers get pushed into the first half of the list,
		while Overlays to the second half.
		Overlays are rendered last.
		*/
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
