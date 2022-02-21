/* Nicholas Creech
 * 1/24/19
 * Pre-lab3
 */

using namespace std;

int add(int, int);

int main()
{
	return 0;
}

int add(int left, int right)
{
	while(right != 0)
	{
		int carry = left & right;
		left ^= right;
		right = carry << 1;
	}

	return left;
}
