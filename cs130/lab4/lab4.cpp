/* Lab4: Adding floats works, adding floats with negatives sometimes works, multiply does not function
 * COSC130
 * Nicholas Creech
 * 1/30/19
 */

#include <iostream>
#include <cstdio>
#include <bitset>

using namespace std;

struct Real
{
	int sign; //1-bit sign (0 or 1)
	long exponent; //8-bit unbiased exponent
	unsigned long fraction; //23-bit fraction
};

Real Decode(int);
int Encode(Real);
Real Normalize(Real);
Real Multiply(Real, Real);
Real Add(Real, Real);
int sign(int);
int twos(int);
int bitAdd(int, int);
int bitSub(int, int);
int bitMul(int, int);

int main(int argc, char * argv[])
{
	int left, right;
	char op;
	int value;
	Real rLeft, rRight, result;

	if(argc < 4)
	{
		printf("Usage: %s <left> <op> <right>\n", argv[0]);
		return -1;
	}

	sscanf(argv[1], "%f", (float *)&left);
	sscanf(argv[2], "%c", &op);
	sscanf(argv[3], "%f", (float *)&right);
	rLeft = Decode(left);
	rRight = Decode(right);

	if(op == 'x')
		result = Multiply(rLeft, rRight);
	else if(op == '+')
		result = Add(rLeft, rRight);
	else
	{
		printf("Unknown operator '%c'\n", op);
		return -2;
	}

	value = Encode(result);
	printf("%.3f %c %.3f = %.3f (0x%08x)\n",
			*((float*)&left),
			op,
			*((float*)&right),
			*((float*)&value),
			value);

	return 0;
}

Real Decode(int float_value)
{
	Real result;

	result.sign = (float_value >> 31) & 1;
	result.exponent = ((float_value >> 23) & 0xff) - 127;
	result.fraction = (float_value & 0x7fffff);
	
	return result;
}

int Encode(Real real_value)
{
	int result = 0;

	result = real_value.sign << 31;
	result |= ((real_value.exponent + 127) << 23);
	result |= real_value.fraction & 0x7fffff;

	return result;
}

Real Normalize(Real val)
{
	if((val.fraction >> 24) == 1)
	{
		while((val.fraction >> 24) == 1)
		{
			val.fraction >>= 1;
			val.exponent++;
		}
	}

	else if((val.fraction >> 23) != 1)
	{
		while((val.fraction >> 23) != 1)
		{
			val.fraction <<= 1;
			val.exponent--;
		}
	}

	return val;
}

Real Multiply(Real left, Real right)
{
	//Not finished
	
	Real result;

	result.fraction = left.fraction;
	result.fraction = right.fraction;
	return result;
}

Real Add(Real left, Real right)
{
	//Putting fractions in normalized form
	right.fraction |= 0b100000000000000000000000;
	left.fraction |= 0b100000000000000000000000;

	//Making exponents equivalent
	while(right.exponent != left.exponent)
	{
		if(right.exponent > left.exponent)
		{
			right.fraction <<= 1;
			right.exponent = bitSub(right.exponent, 1);
		}

		else if(right.exponent < left.exponent)
		{
			right.fraction >>= 1;
			right.exponent = bitAdd(right.exponent, 1);
		}
	}

	//Twos complement
	if(right.sign == 1)
		right.fraction = twos(right.fraction);
	if(left.sign == 1)
		left.fraction = twos(left.fraction);

	Real sum;

	//Now that the left and right exponents are equal, add the fractions
	sum.sign = 0;
	sum.fraction = bitAdd(left.fraction, right.fraction);
	sum.exponent = left.exponent;

	//Normalize the result of the sum
	sum = Normalize(sum);
	
	//Remove the implied one
	sum.fraction &= 0b011111111111111111111111;

	return sum;
}

int sign(int val)
{
	return (val >> 31) & 1;
}

int twos(int val)
{
	return bitAdd(~val, 1);
}

int bitAdd(int left, int right)
{
	int carry = 0;
	int sum = 0;

	for(int x = 0; x < 32; x++)
	{
		int l = (left >> x) & 1;
		int r = (right >> x) & 1;
		int s = l ^ r ^ carry;
		carry = (l & r) | (r & carry) | (l & carry);
		sum = sum | (s << x);
	}

	return sum;
}

int bitSub(int left, int right)
{
	return bitAdd(left, twos(right));
}

int bitMul(int left, int right)
{
	bool neg = true;
	int sum = 0;
	int i = 0;

	if((sign(left) == 0 && sign(right) == 0) || (sign(left) == 1 && sign(right) == 1))
		neg = false;
	if(sign(left) == 1)
		left = twos(left);
	if(sign(right) == 1)
		right = twos(right);

	while(i < right)
	{
		sum = bitAdd(sum, left);
		i = bitAdd(i, 1);
	}

	if(neg)
		sum = twos(sum);

	return sum;
}
