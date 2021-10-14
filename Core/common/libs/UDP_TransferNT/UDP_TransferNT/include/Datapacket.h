#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "nt_headers.h"

#define DP_CHECK_SCOPE(x, v, code) if (x > (DATAPACKET_TYPESIZE)/sizeof(v)) { DEFAULT_NT_LOGGER("INDEX OUT OF SCOPE, index: " + std::to_string(x) + ", max index: " + std::to_string((DATAPACKET_TYPESIZE)/sizeof(v))); } else { code } 

namespace UDP_TransferNT {
	/**
	 * Datapacket for user data
	 */
	struct DataPacket {

		/**
		 * Raw datapacket arrays
		 */
		struct DP {
			char characters[(DATAPACKET_TYPESIZE)/sizeof(char)]{0};
			int integers[(DATAPACKET_TYPESIZE)/sizeof(int)]{0};
			bool booleans[(DATAPACKET_TYPESIZE)/sizeof(bool)]{0};
			float decimals[(DATAPACKET_TYPESIZE)/sizeof(float)]{0};
		};

		DP dp;

		/**
		 * Set character in datapacket using index and value
		 */
		void setCharacters(unsigned int index, char value) { DP_CHECK_SCOPE(index, value, dp.characters[index] = (char)value;) }

		/**
		 * Set integer in datapacket using index and value
		 */
		void setIntegers(unsigned int index, int value) { DP_CHECK_SCOPE(index, value, dp.integers[index] = (int)value;) }

		/**
		 * Set boolean in datapacket using index and value
		 */
		void setBooleans(unsigned int index, bool value) { DP_CHECK_SCOPE(index, value, dp.booleans[index] = (int)value;) }

		/**
		 * Set deceimal value in datapacket using index and value
		 */
		void setDecimals(unsigned int index, float value) { DP_CHECK_SCOPE(index, value, dp.decimals[index] = (float)value;) }




		/**
		 * Get character from datapacket using index
		 */
		char getCharacters(int index) { return (char)dp.characters[index]; }

		/**
		 * Get integer from datapacket using index
		 */
		int getIntegers(int index) { return (int)dp.integers[index]; }

		/**
		 * Get boolean from datapacket using index
		 */
		bool getBooleans(int index) { return (bool)dp.booleans[index]; }

		/**
		 * Get decimal value from datapacket using index
		 */
		float getDecimals(int index) { return (float)dp.decimals[index]; }
	};
	
	/**
	 * Complete size of datapacket (should match set buffersize)
	 */
	#define PACKETSIZE sizeof(DataPacket::DP::characters) + sizeof(DataPacket::DP::integers) + sizeof(DataPacket::DP::booleans) + sizeof(DataPacket::DP::booleans)  
}

#endif // DATAPACKET_H