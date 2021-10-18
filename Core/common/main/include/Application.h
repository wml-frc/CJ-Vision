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
		 * setter for application & layers
		 */
		void setRunning(bool status);
		void setLayersRunning(bool status);

		/**
		 * getter for application and layers
		 */
		bool getRunning();
		bool getLayersRunning();

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

		/**
		 * Get App name
		 */
		std::string getName() {
			return _name;
		}

		/**
		 * Get current time in ms
		 */
		double getCurrentTime() {
			return _currTime;
		}

		double getDt() {
			return _dt;
		}

	 private:
		// Application
		static Application *_instance;
		bool _running = false;
		bool _layers_running = false;

		double _dt = 0, _prevTime = 0, _currTime = 0;

		// application args
		std::string _name;

		// layers
		LayerStack _layerStack;
	};

	Application *createApplication(); // DEFINED BY USER VIA CJ_CREATE_APPLICATION (there can only be one application)
}

/**
 * Main creator for public applications
 */
#define CJ_CREATE_APPLICATION(APP) CJ::Application *CJ::createApplication() { return new APP(); }

#endif