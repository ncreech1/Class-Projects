#include <ctime>
#include <cstdlib>
#include <cstdio>
extern "C" {
	int get_sum(const short array[], int size_of_array);
}

int get_rand(int min, int max)
{
	return min + rand() % (max - min + 1);
}

int main()
{
	short array[1024];
	int size;
	srand(time(NULL));
	size = get_rand(200, 1024);
	
	for(int i = 0; i < size; i++)
	{
		array[i] = get_rand(-30000, 30000);
	}

	int result = get_sum(array, size);
	printf("Result = %d\n", result);
	return 0;
}

