#ifdef DEBUG
	// #define CJ_BUFFSIZE 1
	#include "Core.h"

	int main(int argc, char** argv) {
		
		printf("\nProgram Ended\n");
		return 0;
	}

#else
	#include "Vision.h"
	#ifdef VISION_H
		#include "Core.h"
		int main(int argc, char** argv) {
			Vision v;
			printf("Vision Program Starting");
			CJ::Core::init();
			v.Init();
			while (PROG::PROG_RUNNING()) {
				v.Periodic();
			}
			printf("Vision Program Exited");
			return 0;
		}
	#else
		#error "#define VISION_H not found"
	#endif
#endif