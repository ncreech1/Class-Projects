#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>
#include "fraction.hpp"

/* Nicholas Creech
 * 10/19/19
 * Lab 6: Create the Keno program that uses the Fraction class to analyze probability for different Keno games 
 */ 

using namespace std;

int main()
{
	double bet;
	int balls;
	vector <int> winCatch; //Stores all the catch options
	vector <double> winPay; //Stores all the payouts for each catch option, at the same index
	Fraction prob; //Fraction for probability

	int index = 1;
	double value = 0;

	//Read in the input, with bet and balls as the first two inputs and catch/payout pairs after 
	while(cin >> value)
	{
		if(index == 1)
			bet = value;
		else if(index == 2)
			balls = value;
		else
		{
			//Catch
			if(index % 2 != 0)
				winCatch.push_back(value);
			//Payout
			else
				winPay.push_back(value);
		}

		index++;
	}

	printf("Bet: %03.2lf\n", bet);
	printf("Balls Picked: %d\n", balls);

	double sumReturns = 0;
	for(size_t x = 0; x < winCatch.size(); x++)
	{
		//Create new fraction using the provided algorithm for probability
		prob.Clear();
		prob.Multiply_Binom(80 - balls, 20 - winCatch[x]);
		prob.Multiply_Binom(balls, winCatch[x]);
		prob.Divide_Binom(80, 20);

		cout << "  Probability of catching " << winCatch[x] << " of " << balls << ": ";
		cout << prob.Calculate_Product() << " -- Expected return: " << prob.Calculate_Product() * winPay[x] << endl;
		
		//Sum each return for the return per bet and normalized return per bet
		sumReturns += prob.Calculate_Product() * winPay[x];
	}

	printf("Your return per bet: %03.2lf\n", sumReturns - bet);
	printf("Normalized: %03.2lf\n", (sumReturns - bet) / bet);

	return 0;
}
