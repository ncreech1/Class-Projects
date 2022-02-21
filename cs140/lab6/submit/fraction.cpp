#include "fraction.hpp"
#include <cstdio>

using namespace std;

/* Nicholas Creech
 * 10/19/19
 * Lab 6: Create the Fraction program that constructs and manipulates fractions using a numerator and denominator multiset
 */ 

void Cancel_Values(multiset <int> &num, multiset <int> &denom);

//Clears both sets
void Fraction::Clear()
{
	numerator.clear();
	denominator.clear();
}

//Adds n to the numerator multiset
bool Fraction::Multiply_Number(int n)
{
	if(n <= 0)
		return false;
	if(n == 1)
		return true;

	numerator.insert(n);
	Cancel_Values(numerator, denominator);

	return true;
}


//Adds n to the denominator multiset
bool Fraction::Divide_Number(int n)
{
	if(n <= 0)
		return false;
	if(n == 1)
		return true;

	denominator.insert(n);
	Cancel_Values(numerator, denominator);

	return true;
}

//Adds [2, n] to the numerator multiset
bool Fraction::Multiply_Factorial(int n)
{
	if(n <= 0)
		return false;
	if(n == 1)
		return true;

	for(int x = 2; x <= n; x++)
		numerator.insert(x);

	Cancel_Values(numerator, denominator);

	return true;
}

//Adds [2, n] to the denominator multiset
bool Fraction::Divide_Factorial(int n)
{
	if(n <= 0)
		return false;
	if(n == 1)
		return true;

	for(int x = 2; x <= n; x++)
		denominator.insert(x);
	
	Cancel_Values(numerator, denominator);

	return true;
}

//Adds the value of binom(n, k) to the numerator multiset 
bool Fraction::Multiply_Binom(int n, int k)
{
	if(n <= 0 || k < 0)
		return false;

	Multiply_Factorial(n);
	Divide_Factorial(k);
	Divide_Factorial(n - k);

	Cancel_Values(numerator, denominator);

	return true;
}

//Adds the value of binom(n, k) to the denominator multiset
bool Fraction::Divide_Binom(int n, int k)
{
	if(n <= 0 || k < 0)
		return false;

	Multiply_Factorial(k);
	Multiply_Factorial(n - k);
	Divide_Factorial(n);

	Cancel_Values(numerator, denominator);

	return true;
}

//Swaps the numerator and denominator
void Fraction::Invert()
{
	numerator.swap(denominator);
}

//Prints the current fraction
void Fraction::Print() const
{
	multiset <int>::const_iterator nit;

	//Print 1 if numerator is empty, otherwise print each numerator value
	if(numerator.size() == 0)
		printf("1");
	else
	{
		unsigned int index = 0;
		for(nit = numerator.begin(); nit != numerator.end(); nit++)
		{
			if(index != numerator.size() - 1)
				printf("%d * ", *nit);
			else
				printf("%d", *nit);
			
			index++;
		}
	}

	//No denominator, print newline and return
	if(denominator.size() == 0)
	{
		printf("\n");
		return;
	}

	printf(" / ");

	//Print each denominator value
	unsigned int index = 0;
	for(nit = denominator.begin(); nit != denominator.end(); nit++)
	{
		if(index != denominator.size() - 1)
			printf("%d / ", *nit);
		else
			printf("%d\n", *nit);
		
		index++;
	}
}

//Calculates the decimal value of the current fraction
double Fraction::Calculate_Product() const
{
	double ntotal = 1, dtotal = 1;
	multiset <int>::const_iterator nit;

	//Add up numerator and denominator separately and then divide 
	for(nit = numerator.begin(); nit != numerator.end(); nit++)
		ntotal *= *nit;
	for(nit = denominator.begin(); nit != denominator.end(); nit++)
		dtotal *= *nit;

	return ntotal / dtotal;
}

//Cancels values that match in the numerator and denominator
void Cancel_Values(multiset <int> &num, multiset <int> &denom)
{
	multiset <int>::iterator nit;
	multiset <int>::iterator dit;

	//Check each numerator value
	bool reset = false;
	for(nit = num.begin(); nit != num.end(); nit++)
	{
		if(reset)
		{
			nit = num.begin();
			reset = false;
		}

		dit = denom.find(*nit);

		//Attempt to find numerator value in denominator and remove it from both sets
		if(dit != denom.end())	
		{
			denom.erase(dit);
			num.erase(nit);
			reset = true; //Reset for loop to avoid iterator invalidity
		}
	}
}
