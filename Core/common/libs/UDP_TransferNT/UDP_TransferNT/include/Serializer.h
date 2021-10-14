#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "nt_headers.h"
#include "Datapacket.h"

namespace UDP_TransferNT {
	class Serializer {
	 public:

		/**
		 * Serial cycler. Serialize data type arrays [T] into byte stream [V]
		 */
		template <typename T, typename V>
		static T *serialCycler(T *singleDT, V *data, bool reverse = false) {
			T *dataType = (T *)data;

			for (size_t i = 0; i < (DATAPACKET_TYPESIZE/sizeof(T)); i++) {
				if (reverse) {
					singleDT[i] = *dataType;
				} else {
					*dataType = singleDT[i];
				}

				dataType++;
			}

			return dataType;
		} 

		/**
		 * Serialize datapacket into large byte array, returns compiled outdata
		 */
		static auto *serialize(DataPacket &dp, char *outData) {
			auto *t1 = serialCycler(dp.dp.characters, outData, false);
			auto *t2 = serialCycler(dp.dp.integers, t1, false);
			auto *t3 = serialCycler(dp.dp.booleans, t2, false);
			auto *t4 = serialCycler(dp.dp.decimals, t3, false);
			return t4;
		}

		/**
		 * Deserialize data from large byte array into datapacket, returns completed outdata
		 */
		static auto *deserialize(DataPacket &dp, char *inData) {
			auto *t1 = serialCycler(dp.dp.characters, inData, true);
			auto *t2 = serialCycler(dp.dp.integers, t1, true);
			auto *t3 = serialCycler(dp.dp.booleans, t2, true);
			auto *t4 = serialCycler(dp.dp.decimals, t3, true);
			return t4;
		}
	};
}

#endif