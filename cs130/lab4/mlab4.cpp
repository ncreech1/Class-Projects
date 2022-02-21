/* Nicholas Creech
 * 1/30/19
 * Pre-lab4
 */

#include <cstdio>

using namespace std;

struct Real
{
	int sign; //1-bit sign (0 or 1)
	long exponent; //8-bit unbiased exponent
	unsigned long fraction; //23-bit fraction
};

Real Decode(int);
int Encode(Real);

int main()
{
	/*int float_value;

	scanf("%f", &float_value);

	Real real_value;
	real_value = Decode(float_value);

	printf("%d\n %d\n %d\n", real_value.sign, real_value.exponent, real_value.fraction);*/

	return 0;
}

Real Decode(int float_value)
{
	Real result;

	int mask_8 = 0b11111111;
	int mask_23 = 0b011111111111111111111111;

	result.sign = (float_value >> 31) & 1;
	result.exponent = ((float_value >> 23) & mask_8) - 127;
	result.fraction = (float_value & mask_23);

	return result;
}

int Encode(Real real_value)
{
	int result = 0;
	
	result |= (real_value.sign << 31);
	result |= ((real_value.exponent + 127) << 23);
	result |= (real_value.fraction);

	return result;
}

