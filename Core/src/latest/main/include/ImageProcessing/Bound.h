#ifndef BOUND_H
#define BOUND_H

#include "common_headers.h"
#include "ImageContainer.h"

namespace CJ {
	struct BoundingPoints {
		double center_x = 0, center_y = 0;
	};

	class Bound {
	 public:
		/**
		 * bind hull around contours (convex hull)
		 * Use disable draw to stop drawing
		 */
		static void drawConvexHull(Image &input, Image &output, bool disableDraw = false);

		/**
		 * Draw bounding box
		 * Define size threshold to draw box around blob/hull
		 * Use disable draw to stop drawing (only outputs bounding points)
		 */
		static BoundingPoints drawBoundingBox(Image &input, Image &output, bool disableDraw = false);
	};
}

#endif