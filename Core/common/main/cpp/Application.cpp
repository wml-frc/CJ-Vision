#include "Application.h"

namespace CJ {
	Application *Application::_instance = nullptr;

	Application::Application(bool &bindClose, const std::string &name) : _close(bindClose) {
		CJ_CORE_ASSERT(!_instance, "An application already exists. Cannot create another");
		_instance = this;
		_name = name;

		// Set the runner
		_running = true;
	}

	Application::~Application() {

	}

	void Application::pushLayer(Layer *layer) {
		_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer *overlay) {
		_layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::popLayer(Layer *layer) {
		_layerStack.popLayer(layer);
		layer->onDetach();
	}

	void Application::popOverlay(Layer *overlay) {
		_layerStack.popOverlay(overlay);
		overlay->onDetach();
	}

	void Application::run() {
		while (_running) {
			if (_close) { _running = false; }
			// Update Layers
			for (Layer *layer : _layerStack) {
				layer->onUpdate();
			}
		}
	}
}