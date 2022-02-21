/* Nicholas Creech
 * 1/30/19
 * Lab3: Create functions that add, subtract, and multiply integers without arithmetic operators
 */

#include <cstdio>

using namespace std;

int sign(int);
int twos(int);
int add(int, int);
int sub(int, int);
int mul(int, int);

int main(int argc, char * argv[])
{
	if(argc < 4)
	{
		printf("Usage: %s <num>	<op> <num>\n", argv[0]);
		return -1;
	}

	int left, right, result;
	char op;

	sscanf(argv[1], "%d", &left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%d", &right);

	switch(op)
	{
		case '+':
			result = add(left, right);
			break;
		case '-':
			result = sub(left, right);
			break;
		case 'x':
			result = mul(left, right);
			break;
		default:
			printf("Unknown operation '%c'\n", op);
			return -2;

	}

	printf("%d %c %d = %d\n", left, op, right, result);

	return 0;
}

int sign(int val)
{
	return (val >> 31) & 1;
}

int twos(int val)
{
	return add(~val, 1);
}

int add(int left, int right)
{
	int carry = 0;
	int sum = 0;

	for(int x = 0; x < 32; x++)
	{
		int l = (left >> x) & 1;
		int r = (right >> x) & 1;
		int s = l ^ r ^ carry;
		carry =	(l & r) | (r & carry) | (l & carry);
		sum = sum | (s << x);
	}

	return sum;
}

int sub(int left, int right)
{
	//Adding a negative is the same as subtracting
	return add(left, twos(right));
}

int mul(int left, int right)
{
	bool neg = true;
	int sum = 0;
	int i = 0;

	//If both values are positive or both negative, the result should be positive
	if((sign(left) == 0 && sign(right) == 0) || (sign(left) == 1 && sign(right) == 1))
		neg = false;

	if(sign(left) == 1)
		left = twos(left);

	if(sign(right) == 1)
		right = twos(right);

	//Add (left value) to itself (right value) times 
	while(i < right)
	{
		sum = add(sum, left);
		i = add(i, 1); //i keeps track of how many times the left value has been added to itself
	}

	//Make result negative if needed
	if(neg)
		sum = twos(sum);

	return sum;
}
