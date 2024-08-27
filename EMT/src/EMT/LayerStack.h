#pragma once

#include "Layer.h"
namespace EMT {
	class EMT_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overLayer);

		esgstl::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		esgstl::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		esgstl::vector<Layer*> m_Layers;
		unsigned int m_LayerInsert;
	};
}