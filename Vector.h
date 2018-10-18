/*
 * Vector.h
 *
 * Created: 6-10-2018 11:47:38
 *  Author: Wiebe
 */ 


#ifndef VECTOR_H_
#define VECTOR_H_

#include <sam.h>
#include <cstddef>
#include <math.h>
#include <cstring>
#include <climits>
#include "BuildSettings.h"
#include "MemoryPool.hpp"

template <class T, uint32_t VEC_SIZE = MAX_VECTOR_SIZE, class POOL_BASE = uint8_t>
class Vector {
protected:
	MemoryPool<POOL_BASE, VEC_SIZE> &mPool;

	T* vecStart = nullptr;
	T* vecEnd = nullptr;
	unsigned int actVectorSize = 0;

public:

	Vector(MemoryPool<POOL_BASE, VEC_SIZE> &mPool) : mPool(mPool) {
		allocatePool();
	};

	Vector(Vector &anotherVector) {
		if (anotherVector.mPool.getPoolSize() > VEC_SIZE)
			return;

		// Copy the whole memory map.
		for (unsigned int i = 0; i < anotherVector.mPool.getMapBlockAmount(); ++i) {
			mPool.sram[i] = anotherVector.mPool.sram[i];
		}
	}

	void push_back(T elem) {
		// We are full.
		if (actVectorSize + 1 > VEC_SIZE)
			return;

		vecEnd++;

		// Put at the back of the array.
		*vecEnd = elem;

		actVectorSize++;
	}

	T pop_back() {
		if (actVectorSize > 0) {
			T data = *vecEnd;

			vecEnd--;
			actVectorSize--;
		}
	};

	T push_front();

	T pop_front();

	T operator[] (unsigned int index) {
		if (index > actVectorSize + 1) {
			return INT_MAX;
		}

		return vecStart[index];
	}

	unsigned int count() {
		return actVectorSize;
	}

	void erase() {
		if (vecStart) {
			vecStart = mPool.deallocate(vecStart);
		}
	}

private:
	void allocatePool() {
		if (vecStart) {
			return;
		}

		vecStart = mPool.allocate(vecStart, VEC_SIZE);

		if (VEC_SIZE > 1) {
			vecEnd = vecStart;
		} else {
			vecEnd = vecStart;
		}
	}
};


#endif /* VECTOR_H_ */