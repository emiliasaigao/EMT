#include "emtpch.h"
#include "LayerStack.h"

namespace EMT {
	LayerStack::LayerStack()
	{
		m_LayerInsert = 0;
	}

	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayerInsert, layer);
		m_LayerInsert++;
	}

	void LayerStack::PushOverLayer(Layer* overLayer) {
		m_Layers.emplace_back(overLayer);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverLayer(Layer* overLayer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overLayer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}
