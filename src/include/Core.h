// CJ Vision
#ifndef CORE_H
#define CORE_H
#include "ImageContainer.h"
#include "ColourFilter.h"
#include "Contours.h"
#include "Camera.h"
#include "Output.h"

namespace CJ {
	class Core {
	public:

		/**
		 * Init program.
		 */
		static void init();

		/**
		* Sets up the vision library with cam and image settings
		*/
		static void setupVision(Image *image, Camera::Cam *cam);

	private:
	};
}
#endif // CORE_H