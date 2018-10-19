/*
 * MemoryPool.h
 *
 * Created: 18-7-2018 14:39:47
 *  Author: Wiebe
 */ 


#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include <sam.h>
#include <cstddef>
#include <math.h>
#include <cstring>
#include "UARTConnection.hpp"
#include "Time.hpp"

namespace hwstl {
	template <class BASE_TYPE, uint32_t MEM_SIZE, uint32_t MAP_MEM_SIZE = 200>
	class MemoryPool {
	private:
		
		//uint8_t data[MEM_SIZE];

		
		unsigned int mapSize = 0;

		uint16_t findEmptyMapBlock(uint32_t blockSize) {
			uint8_t *nextAddr = nullptr;
			uint8_t *curAddr = memoryMap[0];

			if (mapSize == 1) {
				return 0;
			}

			for (unsigned int i = 1; i < mapSize + 1; i++) {
						
				nextAddr = memoryMap[i];

				if (static_cast<uint32_t>(abs((nextAddr - curAddr))) > blockSize) {
					//return curAddr;
					return i - 1;
				}

				curAddr = nextAddr;
			}

			return INT16_MAX;
		}

		BASE_TYPE* mapIndexToPointer(uint16_t mapIndex) {
			return memoryMap[mapIndex];
		}

		bool reserveMemory(BASE_TYPE *startLocation, const uint16_t mapIndex, const unsigned int size) {
			if ((startLocation + size) >= (sram + (MEM_SIZE - 1))) { // poolEnd
				// Prevent allocating out of memory.
				return false;
			}

			// Create a new map entry.
			memoryMap[mapIndex] = (startLocation + size);

			// Increase the size of the map.
			mapSize++;

			return true;
		}

		bool freeMemory(const BASE_TYPE *startLocation) {
			// Search for the memory location.
			for (unsigned int i = 0; i < mapSize + 1; i++) { // i = 1 instead of i == 0
				if (memoryMap[i] == startLocation) {
					if (i == 0) {
						// Reset map top pointer to the begin of the pool.
						memoryMap[0] = poolStart;
					} else {
						// Free the memory by setting it to zero.
						memoryMap[i] = 0;
					}
					
					mapSize--;
					return true;
				}
			}

			return false;
		}

		void setupMemoryMap() {
			poolStart = sram;
			poolEnd = sram + (MEM_SIZE - 1);

			memoryMap[0] = sram;
			mapSize = 1;
		}

		BASE_TYPE* allocateMemory(const unsigned int size) {
			uint16_t mapIndex = findEmptyMapBlock(size);

			if (mapIndex == INT16_MAX) {
				return nullptr;
			}

			BASE_TYPE *storeLocation = mapIndexToPointer(mapIndex);

			if (!reserveMemory(storeLocation, mapIndex + 1, size)) {
				return nullptr;
			}

			return storeLocation;
		}
	protected:
		BASE_TYPE sram[MEM_SIZE];

		BASE_TYPE *memoryMap[MAP_MEM_SIZE] = {};
		BASE_TYPE *poolStart = nullptr, *poolEnd = nullptr;

	public:
		
		MemoryPool() {
			setupMemoryMap();
		}

		~MemoryPool() {
			flush();
		}

		void dumpMemoryMap(UARTLib::UARTConnection &uart) {
			for (unsigned int i = 0; i < getMapBlockAmount() + 1; i++) {
				//uart << i;
				//uart << " -> ";	
				//uart << (unsigned int)(memoryMap[i]);
				//uart << '\n';
			}
		}

		BASE_TYPE* allocate(const unsigned int size) {
			return allocateMemory(size);
		}

		template <typename T>
		T* allocate(T* object) {
			BASE_TYPE* objectPtr = reinterpret_cast<BASE_TYPE*>(&object);
			BASE_TYPE* storeLocation = allocateMemory(sizeof(object));

			// Move into SRAM
			for (uint32_t i = 0; i < sizeof(object); i++) {
				storeLocation[i] = objectPtr[i];
			}

			return reinterpret_cast<T*>(storeLocation);
		}

		template <typename T, typename... T2>
		T* allocate(T2... args) {
			T object(args...);
			BASE_TYPE* objectPtr = reinterpret_cast<BASE_TYPE*>(&object);
			//BASE_TYPE* storeLocation = findEmptyBlock(sizeof(object));
			BASE_TYPE* storeLocation = allocateMemory(sizeof(object));

			// Move into SRAM
			for (uint32_t i = 0; i < sizeof(object); i++) {
				storeLocation[i] = objectPtr[i];
			}

			return reinterpret_cast<T*>(storeLocation);
		}

		template <typename T>
		T* allocate() {
			T object;
			BASE_TYPE* objectPtr = reinterpret_cast<BASE_TYPE*>(&object);
			//BASE_TYPE* storeLocation = findEmptyBlock(sizeof(object));
			BASE_TYPE* storeLocation = allocateMemory(sizeof(object));
				
			// Move into SRAM
			for (uint32_t i = 0; i < sizeof(object); i++) {
				storeLocation[i] = objectPtr[i];
			}

			return reinterpret_cast<T*>(storeLocation);
		}

		template <typename T>
		T* allocate(T* ptr, const unsigned int arraySize) {
			//BASE_TYPE* objectPtr = reinterpret_cast<BASE_TYPE*>(&ptr);
			//BASE_TYPE* storeLocation = findEmptyBlock(sizeof(object));
			BASE_TYPE* storeLocation = allocateMemory(arraySize);
				
			// Move into SRAM
			//for (uint32_t i = 0; i < sizeof(object) * arraySize; i++) {
			//	storeLocation[i] = objectPtr[i];
			//}

			return reinterpret_cast<T*>(storeLocation);
		}

		BASE_TYPE* deallocate(BASE_TYPE* ptr) {
			// Remove memory address from map.
			freeMemory(ptr);

			ptr = nullptr;

			return ptr;
		}

		template<typename T>
		T* deallocate(T* ptr) {
			freeMemory(reinterpret_cast<BASE_TYPE*>(ptr));

			ptr = nullptr;
			
			return ptr;
		}

		void flush() {
			for (unsigned int i = 0; i < mapSize + 1; i++) {
				memoryMap[i] = 0;
			}

			mapSize = 0;

			// We will just leave the sram alone.
		}

		uint32_t getMemorySize() {
			return MAP_MEM_SIZE;
		}

		uint32_t getPoolSize() const {
			//return abs(*poolEnd - *poolStart);
			return maxMemSize;
		}

		unsigned int getMapBlockAmount() {
			return mapSize;
		}

		static const uint32_t maxMemSize = MEM_SIZE;
	};
} // namespace hwstl


#endif /* MEMORYPOOL_H_ */