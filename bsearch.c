int bsearch(const int* begin, const int* end, int x) {
	const int* left = begin - 1;
	const int* right = end;
	while (left + 1 < right) {
		const int* middle = left + (right - left) / 2;
		if (*middle <= x) {
			left = middle;
		} else {
			right = middle;
		}
	}
	return right - begin;
}

#include <stdio.h>
#include <assert.h>

int main() {
	int array[] = {1, 2, 4, 4, 6, 8, 8, 8, 8};
	int length = sizeof(array) / sizeof(int);
	assert(bsearch(array, array + length, 0) == 0);
	assert(bsearch(array, array + length, 1) == 1);
	assert(bsearch(array, array + length, 2) == 2);
	assert(bsearch(array, array + length, 3) == 2);
	assert(bsearch(array, array + length, 4) == 4);
	assert(bsearch(array, array + length, 5) == 4);
	assert(bsearch(array, array + length, 6) == 5);
	assert(bsearch(array, array + length, 7) == 5);
	assert(bsearch(array, array + length, 8) == 9);
	assert(bsearch(array, array + length, 9) == 9);
	return 0;
}
