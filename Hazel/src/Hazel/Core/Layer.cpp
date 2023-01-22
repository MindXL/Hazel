#include "pch.h"

#include "Layer.h"

namespace Hazel
{
	Layer::Layer(std::string debugName)
		: m_DebugName{std::move(debugName)} {}
}
