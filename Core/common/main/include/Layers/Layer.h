#ifndef LAYER_H
#define LAYER_H

#include "common_headers.h"

namespace CJ {

	/**
	 * Layer class, a layer of processing for program which is controlled by a layer stack
	 */
	class Layer {
	 public:
		Layer(const std::string &name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}

		inline const std::string &getName() {
			return _name;
		}

	 protected:
		std::string _name;
	};
}

#endif