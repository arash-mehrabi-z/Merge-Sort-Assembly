/* C program for Merge Sort */
/* Some parts of the code are written in inline assembly*/
#include<stdlib.h> 
#include<stdio.h> 
#include <time.h>    // for clock_t, clock()
//#include <unistd.h>    // for sleep()

#define BILLION  1000000000.0;
/*
Defined arrays in global section so we can use OFFSET operator in assembly language.
*/
int arr[1000];
int L[1000];
int R[1000];
// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	/*
	int *L, *R;

	//allocating memory
	L = (int*)malloc(n1 * sizeof(int));
	R = (int*)malloc(n2 * sizeof(int));
	*/

	/* Copy data to temp arrays L[] and R[] */

	for (i = 0; i < n1; i++)
		L[i] = arr[l + i]; //L = a[l...m]
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j]; //R = a[m+1...r]

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 

	__asm 
	{

		; in while we are implementing this while:

		; while (i < n1 && j < n2)
		; {
		;	if (L[i] <= R[j])
		;   {
		;		arr[k] = L[i];
		;		i++;
		;	}
		;	else
		;   {
		;		arr[k] = R[j];
		;		j++;
		;	}
		;   k++;
		; }
		
		jmp WHILE
			
		WHILE:
			mov eax, i
			mov ebx, n1
			mov ecx, j
			mov edx, n2
			; checking two conditional if statement
			cmp eax, ebx
			jge ENDOFWHILE
			cmp ecx, n2
			jge ENDOFWHILE
			; checking the two conditions of if statement is done
			; now it is time for checking if L[i] <= R[j]

			; calculating &L[i]
			mov eax, i
			mov ebx, TYPE L
			mul ebx
			add eax, OFFSET L; now eax = &L[i]
			mov ecx, eax

			; calculating &R[j]
			mov eax, j
			mov ebx, TYPE R
			mul ebx
			add eax, OFFSET R

			mov ebx, [ecx]; ebx = L[i]
			; eax = &R[j]
			cmp	ebx, [eax]; compare L[i] with R[j]
			jg INNERELSE

			; do arr[k] = L[i]

			; first calculate &a[k]
			mov eax, k
			mov ebx, TYPE arr
			mul ebx
			add eax, OFFSET arr; eax = &arr[k]
			mov ecx, eax; ecx = &arr[k]

			; now calculate &L[i]
			mov eax, i
			mov ebx, TYPE L
			mul ebx
			add eax, OFFSET L; now eax = &L[i]

			mov eax, [eax]; eax = L[i]
			mov[ecx], eax; a[k] = L[i]

			inc i
			jmp ENDOFITERATION

			INNERELSE:
				; do a[k] = R[j]
				; first calculate &a[k]
				mov eax, k
				mov ebx, TYPE arr
				mul ebx
				add eax, OFFSET arr; eax = &arr[k]
				mov ecx, eax; ecx = &arr[k]
			
				; now calculate &R[j]
				mov eax, j
				mov ebx, TYPE R
				mul ebx
				add eax, OFFSET R; now eax = &R[i]

				mov eax, [eax]; eax = R[i]
				mov[ecx], eax; a[k] = R[i]

				inc j
				jmp ENDOFITERATION

			ENDOFITERATION:
				inc k
				jmp WHILE

		ENDOFWHILE:
		

		;/* Copy the remaining elements of L[], if there
		;are any */
		;/* by implementing this while:
		;while (i < n1)
		;{
		;	arr[k] = L[i];
		;	i++;
		;	k++;
		;}
		;
		;*/

			jmp WHILEI

			WHILEI :
				mov eax, i
				mov ebx, n1
				cmp eax, ebx
				jge ENDOFWHILEI
				; do arr[k] = L[i]

				; first calculate &a[k]
				mov eax, k
				mov ebx, TYPE arr
				mul ebx
				add eax, OFFSET arr; eax = &arr[k]
				mov ecx, eax; ecx = &arr[k]

				; now calculate &L[i]
				mov eax, i
				mov ebx, TYPE L
				mul ebx
				add eax, OFFSET L; now eax = &L[i]

				mov eax, [eax]; eax = L[i]
				mov[ecx], eax; a[k] = L[i]

				jmp ENDOFITERATIONI
				ENDOFITERATIONI :
					inc i
					inc k
					jmp WHILEI

			ENDOFWHILEI :


			;/* Copy the remaining elements of R[], if there
			;are any */
			;/* by by implementing this while:
			;while (j < n2)
			;{
			;	arr[k] = R[j];
			;	j++;
			;	k++;
			;}
			;
			;*/

				jmp WHILEJ

				WHILEJ :
					mov eax, j
					mov ebx, n2
					cmp eax, ebx
					jge ENDOFWHILEJ
					; do arr[k] = R[j]

					; first calculate &a[k]
					mov eax, k
					mov ebx, TYPE arr
					mul ebx
					add eax, OFFSET arr; eax = &arr[k]
					mov ecx, eax; ecx = &arr[k]

					; now calculate &R[j]
					mov eax, j
					mov ebx, TYPE R
					mul ebx
					add eax, OFFSET R; now eax = &R[i]

					mov eax, [eax]; eax = R[j]
					mov[ecx], eax; a[k] = R[j]

					jmp ENDOFITERATIONJ
					ENDOFITERATIONJ :
						inc j
						inc k
						jmp WHILEJ

				ENDOFWHILEJ :

	}

	// deallocating the memory
//	free(L);
//	free(R);
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		int m = l + (r - l) / 2;

		// Sort first and second halves 
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

		merge(l, m, r);
	}
}

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int *A, int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", *(A + i));
	printf("\n");
}

/* Driver program to test above functions */
int main()
{

	int n;
	printf("Enter the number of elements you want in the array: ");
	scanf_s("%d", &n);

	//int *arr;
	//arr = (int*)malloc(n * sizeof(int));


	for (int i = 0; i < n; i++)
	{
		scanf_s("%d", arr + i);
	}

	int arr_size = n;
	printf("Given array is \n");
	printArray(arr, arr_size);

	clock_t start = clock();

	mergeSort(arr, 0, arr_size - 1);

	clock_t stop = clock();
	double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;

	printf("\nSorted array is \n");
	printArray(arr, arr_size);
	printf("time taken : %f", elapsed);


	int z;
	scanf_s("%d", &z);	// so the command prompt will wait on the screen.

	// deallocating the memory
	//free(arr);
	return 0;
}
