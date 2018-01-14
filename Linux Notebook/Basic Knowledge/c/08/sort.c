#include <stdio.h>
#include <string.h>

typedef int (*cmp_t)(const void *, const void *);

void sort_bubble(void *base, int size, int nmemb, cmp_t cmp);
static int int_cmp(const void *data1, const void *data2)
{
	const int *d1 = data1;
	const int *d2 = data2;

	return *d1 - *d2;
}

static int str_cmp(const void *data1, const void *data2)
{
	char **d1 = (void *)data1;
	char **d2 = (void *)data2;

	return strcmp(*d2, *d1);
}

int main(int argc, char **argv)
{
	char *str[5] = {};
	int i;

	if (argc != 5) {
		return 1;
	}
	
	for (i = 0; i < argc; i++) {
		str[i] = argv[i];
	}
	printf("before sort\n");
	for (i = 0; i < argc; i++) {
		printf("%s\n", str[i]);
	}

	sort_bubble(str, sizeof(char *), 5, str_cmp);

	printf("after sort\n");
	for (i = 0; i < argc; i++) {
		printf("%s\n", str[i]);
	}

#if 0
	int arr[] = {3,1,5,7,9,10,2,8,6};
	int i;

	sort_bubble(arr, sizeof(int), sizeof(arr)/sizeof(*arr), int_cmp);

	for (i = 0; i < sizeof(arr)/sizeof(*arr); i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
#endif

	return 0;
}
/*base:数组首地址（容纳任意类型的数组）
 *size:数组成员所占的字节个数
 *nmemb:数组的成员个数
 *cmp:数组中成员的比较函数的地址
 * */
void sort_bubble(void *base, int size, int nmemb, cmp_t cmp)
{
	int i, j;				
	char buf[100] = {};
	
	for (i = 0; i < nmemb-1; i++ ) {
		for (j = 0; j < nmemb-i-1; j++) {	
			if (cmp((char *)base+j*size, (char *)base+(j+1)*size) > 0) {
				memcpy(buf, (char *)base+j*size, size);
				memcpy((char *)base+j*size,\
					   	(char *)base+(j+1)*size, size);
				memcpy((char *)base+(j+1)*size, buf, size);
			}
		}
	}

}

