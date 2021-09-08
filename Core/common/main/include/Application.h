#ifndef APPLICATION_H
#define APPLICATION_H

#include "common_headers.h"
#include "Layers/LayerStack.h"

namespace CJ {
	/**
	 * Main application class that runs using CJ-Vision platform
	 */
	class Application {
	 public:
		Application(const std::string &name = "CJ-Vision Application");
		virtual ~Application();
		void run();

		/**
		 * Statically get application
		 */
		static Application &get() {
			return *_instance;
		}

		/**
		 * Push layer onto stack
		 */
		void pushLayer(Layer *layer);
		/**
		 * Push ovelay onto stack
		 */
		void pushOverlay(Layer *overlay);
		/**
		 * Pop layer from stack
		 */
		void popLayer(Layer *layer);
		/**
		 * Pop overlay from stack
		 */
		void popOverlay(Layer *overlay);

	 private:
		// Application
		static Application *_instance;
		bool _running = false;

		// application args
		std::string _name;

		// layers
		LayerStack _layerStack;
	};

	Application *createApplication(); // DEFINED BY USER VIA CJ_CREATE_APPLICATION (there can only be one application)
}

#define CJ_CREATE_APPLICATION(APP) CJ::Application *CJ::createApplication() { return new APP(); }

#endif