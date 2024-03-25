#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int i, j, x, y, max, left, right;
	for(i = n / 2 - 1; i >= 0; i--){
		j = i;
		for(;;){
			max = j;
			right = 2 * j + 2;
			left = 2 * j + 1;
			if(left < n && arr[left] > arr[max]){
				max = left;
			}
			if(right < n && arr[right] > arr[max]){
				max = right;
			}
			if(max != j){
				y = arr[j];
				arr[j] = arr[max];
				arr[max] = y;
				j = max;
			}
			else{
				break;
			}
		}
	}
	for(i = n - 1; i > 0; i--){
		x = arr[0];
		arr[0] = arr[i];
		arr[i] = x;
		j = 0;
		for(;;){
			max = j;
			right = 2 * j + 2;
			left = 2 * j + 1;
			if(left < i && arr[left] > arr[max]){
				max = left;
			}
			if(right < i && arr[right] > arr[max]){
				max = right;
			}
			if(max != j){
				y = arr[j];
				arr[j] = arr[max];
				arr[max] = y;
				j = max;
			}
			else{
				break;
			}
		}
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	int mid, i, j, k, n1, n2;
	int* left;
	int* right;
	if(l < r){
		mid = (l + r) / 2;
		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);
		n1 = mid - l + 1;
		n2 = r - mid;
		left = (int*)Alloc(n1 * sizeof(int));
		right = (int*)Alloc(n2 * sizeof(int));
		for(i = 0; i < n1; i++){
			left[i] = pData[l + i];
		}
		for(j = 0; j < n2; j++){
			right[j] = pData[mid + 1 + j];
		}
		i = 0;
		j = 0;
		k = l;
		while(i < n1 && j < n2){
			if(left[i] < right[j]){
				pData[k] = left[i];
				i++;
			}
			else{
				pData[k] = right[j];
				j++;
			}
			k++;
		}
		while(i < n1){
			pData[k] = left[i];
			i++;
			k++;
		}
		while(j < n2){
			pData[k] = right[j];
			j++;
			k++;
		}
		DeAlloc(left);
		DeAlloc(right);
	}
	
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, j, x;
	for(i = 1; i < n; i++){
		x = pData[i];
		for(j = i - 1; j >= 0; j--){
			if(pData[j] > x){
				pData[j + 1] = pData[j];
			}
			else{
				break;
			}
		}
		pData[j + 1] = x;
	}

}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j, x;
	for(i = 0; i < n - 1; i++){
		for(j = 0; j < n - i - 1; j++){
			if(pData[j] > pData[j + 1]){
				x = pData[j];
				pData[j] = pData[j + 1];
				pData[j + 1] = x;
			}
		}
	}
	
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, x, min;
	for(i = 0; i < n - 1; i++){
		min = i;
		for(j = i + 1; j < n; j++){
			if(pData[j] < pData[min]){
				min = j;
			}
		}
		x = pData[i];
		pData[i] = pData[min];
		pData[min] = x;
	}
	
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
        if (*ppData == NULL) {
            fclose(inFile);
            return 0;
        }
        for (int i = 0; i < dataSz; i++) {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
        fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);//function only has 2 parameters, 2nd value (0) not needed
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
