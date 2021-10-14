#ifndef LAYERSTACK_H
#define LAYERSTACL_H

#include "common_headers.h"
#include "Layers/Layer.h"

namespace CJ {
	/**
	 * Layer Stack
	 * Class for controller layers and overlays. Push processing layers and have them be processed one after the other
	 */
	class LayerStack {
	 public:
		LayerStack() = default;
		~LayerStack();

		/**
		 * Push layer onto stack (places it after the last layer pushed)
		 */
		void pushLayer(Layer *layer);

		/**
		 * Push overlay onto stack (places overlay at the end of the stack after all layers)
		 * Overlays are always after layers. And are placed acordingly
		 */
		void pushOverlay(Layer *overlay);

		/**
		 * Delete a layer (shuffles stack afterwards)
		 */
		void popLayer(Layer *layer);

		/**
		 * Delete overlay (shuffles stack afterwards)
		 */
		void popOverlay(Layer *overlay);

		/**
		 * Return the beginning layer of the layer stack
		 */
		std::vector<Layer*>::iterator begin() {
			return _layers.begin();
		}

		/**
		 * Returns the end layer of the layer stack
		 */
		std::vector<Layer *>::iterator end() {
			return _layers.end();
		}

		std::vector<Layer *>::reverse_iterator rbegin() {
			return _layers.rbegin();
		}

		std::vector<Layer *>::reverse_iterator rend() {
			return _layers.rend();
		}

		std::vector<Layer *>::const_iterator begin() const {
			return _layers.begin();
		}

		std::vector<Layer *>::const_iterator end()	const {
			return _layers.end();
		}

		std::vector<Layer *>::const_reverse_iterator rbegin() const {
			return _layers.rbegin();
		}
		
		std::vector<Layer *>::const_reverse_iterator rend() const {
			return _layers.rend();
		}

	 private:
		std::vector<Layer *> _layers;
		unsigned int _layerInsertIndex = 0;
	};
}

#endif