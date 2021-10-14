#include "Application.h"

namespace CJ {
	Application *Application::_instance = nullptr;

	Application::Application(const std::string &name) {
		CJ_CORE_ASSERT(!_instance, "An application already exists. Cannot create another");
		_instance = this;
		_name = name;

		// Set the runner
		_running = true;
		_layers_running = true;
	}

	Application::~Application() {}

	void Application::setRunning(bool status) {
		_running = status;
	}
	
	void Application::setLayersRunning(bool status) {
		_layers_running = status;
	}

	bool Application::getRunning() {
		return _running;
	}

	bool Application::getLayersRunning() {
		return _layers_running;
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
			// Update Layers
			if (_layers_running) {
				for (Layer *layer : _layerStack) {
					layer->onUpdate();
				}
			}
		}
	}
}