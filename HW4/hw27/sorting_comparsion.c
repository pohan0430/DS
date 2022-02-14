#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define swap(x, y) {int temp = x; x = y; y = temp;}
#define true 1
#define false 0
#define Test 100

typedef int bool;

void permute (int *, int);

void insertionSort(int *, int);

void generate_quickSort_worst_case(int *, int);
void quickSort (int *, int, int);
int partition (int *, int, int);

void merge (int *, int, int, int);
void mergeSort_recursive (int *, int, int);
void mergePass (int *, int, int);
void mergeSort_iterative (int *, int);
void generate_worst_case_mergeSort(int *, int);

void generate_heapSort_worst_case(int *, int);
void create (int *, int);
void adjust (int *, int, int);
void heapSort (int *, int);

int main () {
	clock_t start, end;
	
	int n[] = {17, 18, 19, 20, 21, 22, 50, 100, 300, 500, 700, 1000, 1300, 1500, 1700, 2000, 2500, 3000, 3500, 4000};
	int Num = sizeof(n) / sizeof(int);
	double insertTime[Num], quickTime[Num], heapTime[Num], mergeTime[Num];
	double duration;
	long repetitions;

	for (int i = 0; i < Num; i++) {
		insertTime[i] = quickTime[i] = heapTime[i] = mergeTime[i] = 0;
	}
	
	for (int i = 0; i < Num; i++) {
		int element[n[i]+1], worst_case[n[i]+1];
		
		// generation insertion sort worst case
		for (int j = 1, r = n[i]; j <= n[i]; j++, r--) {
			element[j] = r; 
		}
		
		for (int j = 0; j < Test; j++) {
			
			repetitions = 0;
			start = clock();
			do {
				repetitions++;
				// initialization
				for (int k = 1; k <= n[i]; k++) {
					worst_case[k] = element[k];
				}

				insertionSort(worst_case, n[i]);
			}while (clock() - start < 10000);
			end = clock();

			duration = ((double)(end - start) / CLOCKS_PER_SEC);
			duration /= repetitions;

			insertTime[i] += duration;
		}
		insertTime[i] /= (double) Test;

		// generation quickSort worst case 
		generate_quickSort_worst_case(element, n[i]);

		for (int j = 0; j < Test; j++) {
			
			repetitions = 0;
			start = clock();
			do {
				repetitions++;
				// initialization
				for (int k = 1; k <= n[i]; k++) {
					worst_case[k] = element[k];
				}

				quickSort(worst_case, 1, n[i]);
			}while (clock() - start < 10000);
			end = clock();

			duration = ((double)(end - start) / CLOCKS_PER_SEC);
			duration /= repetitions;

			quickTime[i] += duration;
		}
		quickTime[i] /= (double) Test;

		// generating heapSort worst case
		generate_heapSort_worst_case(element, n[i]);

		for (int j = 0; j < Test; j++) {
			
			repetitions = 0;
			start = clock();
			do {
				repetitions++;
				// initialization
				for (int k = 1; k <= n[i]; k++) {
					worst_case[k] = element[k];
				}

				heapSort(worst_case, n[i]);
			}while (clock() - start < 10000);
			end = clock();

			duration = ((double)(end - start) / CLOCKS_PER_SEC);
			duration /= repetitions;

			heapTime[i] += duration;
		}
		heapTime[i] /= (double) Test;

		// generating mergeSort worst case
		generate_worst_case_mergeSort(element, n[i]);

		for (int j = 0; j < Test; j++) {
			
			repetitions = 0;
			start = clock();
			do {
				repetitions++;
				// initialization
				for (int k = 1; k <= n[i]; k++) {
					worst_case[k] = element[k];
				}

				mergeSort_iterative(worst_case, n[i]);
			}while (clock() - start < 10000);
			end = clock();

			duration = ((double)(end - start) / CLOCKS_PER_SEC);
			duration /= repetitions;

			mergeTime[i] += duration;
		}
		mergeTime[i] /= (double) Test;
	}
	
	for (int i = 0; i < Num; i++) {
		printf("n: %d\n", n[i]);
		printf("insertion sort worst case running time: %lf\n", insertTime[i]);
		printf("quickSort worst case running time: %lf\n", quickTime[i]);
		printf("heapSort worst case running time: %lf\n", heapTime[i]);
		printf("mergeSort worst case running time: %lf\n", mergeTime[i]);
		printf("----------------------------------------------------------------------\n");
	}

	return 0;
}

void permute (int *a, int n) {
	/* random permutation generator  */
	int i, j;
	for (i = n; i >= 2; i--) {
		j = rand() % i + 1;
		swap(a[j], a[i]);
	}
}

void insertionSort(int *a, int n) {
	for (int i = 2; i <= n; i++) {
		int j = i;
		while (j >= 2 && a[j] < a[j-1]) {
			swap(a[j], a[j-1]);
			j--;
		}
	}
}

void generate_quickSort_worst_case(int *element, int n) {
	clock_t start, end;
	int arr[n+1], worst_case[n+1];
	double max = 0, duration;
	long repetitions;
	for (int i = 0; i < Test; i++) {
		for (int j = 1; j < n; j++) {
			arr[j] = rand();
		}

		permute(arr, n);

		repetitions = 0;
		start = clock();
		do {
			repetitions++;
			// initialization
			for (int k = 1; k <= n; k++) {
				worst_case[k] = arr[k];
			}

			quickSort(worst_case, 1, n);
		}while (clock() - start < 10000);
		end = clock();

		duration = ((double)(end - start) / CLOCKS_PER_SEC);
		duration /= repetitions;

		if (duration > max) {
			max = duration;
			for (int k = 1; k <= n; k++) {
				element[k] = arr[k];
			}
		}
	}
 }

int partition (int *a, int low, int high) {
	int i = low-1;

	for (int j = low; j < high; j++) {
		if (a[j] < a[high]) {
			i++;
			swap(a[i], a[j]);
		}
	}
	i++;
	swap(a[i], a[high]);
	return i;
}

void quickSort (int *a, int low, int high) {
	if (high > low) {
		int mid = (high + low) / 2;
		if (a[high] > a[low]) {
			if (a[mid] > a[low]) {
				if (a[high] > a[mid]) {
					swap(a[high], a[mid]);
				}
			}
			else {
				swap(a[high], a[low]);
			}
		}
		else {
			if (a[mid] > a[low]) {
				swap(a[high], a[low]);
			}
			else {
				if (a[mid] > a[high]) {
					swap(a[mid], a[high]);
				}
			}
		}
		int pivot = partition(a, low, high);
		quickSort(a, low, pivot-1);
		quickSort(a, pivot+1, high);
	}
}

void generate_worst_case_mergeSort(int *element, int n) {
	for (int i = 1; i <= n; i++) {
		element[i] = -1;
	}

	int num = n;
	int pos = pow(2, floor(log10(n) / log10(2)));
	while (pos >= 1) {
		for (int i = pos; i <= n; i += pos) {
			if (element[i] == -1) {
				element[i] = num;
				num--;
			}
		}
		if (element[n] == -1) {
			element[n] = num;
			num--;
		}
		pos /= 2;
	}
}

void merge (int *a, int low, int high, int mid) {
	int L[mid-low+1], R[high-mid];
	for (int i = low; i <= mid; i++) {
		L[i-low] = a[i];
	}

	for (int i = mid+1; i <= high; i++) {
		R[i-mid-1] = a[i];
	}

	int i = 0, j = 0, k = low;
	while (i < mid-low+1 && j < high-mid) {
		if (L[i] < R[j]) {
			a[k] = L[i];
			i++;
		}
		else {
			a[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < mid-low+1) {
		a[k] = L[i];
		i++;
		k++;
	}

	while (j < high-mid) {
		a[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort_recursive (int *a, int low, int high) {
	if (high > low) {
		int mid = (high + low) / 2;
		mergeSort_recursive(a, low, mid);
		mergeSort_recursive(a, mid+1, high);
		merge(a, low, high, mid);
	}
}

void mergePass (int *a, int n, int s) {
	int mid, high;
	for (mid = s; mid < n; mid += 2 * s) {
		high = mid + s > n ? n : mid + s;
		merge(a, mid-s+1, high, mid);
	}
}

void mergeSort_iterative (int *a, int n) {
	int s = 1;
	
	while (s < n) {
		mergePass(a, n, s);
		s *= 2;
	}
}

void generate_heapSort_worst_case(int *element, int n) {
	int arr[n+1], worst_case[n+1];
	double max = 0, duration;
	long repetitions;
	clock_t start, end;
	for (int i = 0; i < Test; i++) {
		for (int j = 1; j < n; j++) {
			arr[j] = rand();
		}

		permute(arr, n);

		repetitions = 0;
		start = clock();
		do {
			repetitions++;
			// initialization
			for (int k = 1; k <= n; k++) {
				worst_case[k] = arr[k];
			}

			heapSort(worst_case, n);
		}while (clock() - start < 10000);
		end = clock();

		duration = ((double)(end - start) / CLOCKS_PER_SEC);
		duration /= repetitions;

		if (duration > max) {
			max = duration;
			for (int k = 1; k <= n; k++) {
				element[k] = arr[k];
			}
		}
	}
}

void create (int *a, int n) {
	for (int i = n / 2; i > 0; i--) {
		adjust(a, i, n);
	}
}

void adjust (int *a, int i, int n) {
	int j = 2 * i, r = a[i];
	while (j <= n) {
		if (j < n && a[j] < a[j+1]) {
			j++;
		}
		if (r >= a[j]) {
			break;
		}
		else {
			a[j/2] = a[j];
			j *= 2;
		}
	}
	a[j/2] = r;
}

void heapSort (int *a, int n) {
	create(a, n);
	for (int i = n; i > 1; i--) {
		swap(a[i], a[1]);
		n--;
		adjust(a, 1, n);
	}
}
