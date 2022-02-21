#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		printf("Usage: ./lab9 0x<address>\n");
		return 1;
	}

	long hexVal;
	sscanf(argv[1], "%lx", &hexVal);

	int pageOffset = hexVal & 0xFFF;
	int VPN[3];
	hexVal >>= 12;

	for(int x = 0; x < 3; x++)
	{
		VPN[x] = hexVal & 0b111111111;
		hexVal >>= 9;
	}

	printf("  Level   Index  Byte Offset\n");
	for(int x = 2; x >= 0; x--)
		printf("VPN[00%d] = %-5d(%4d)\n", x, VPN[x], VPN[x] * 8);
	printf("%-8s = %lx\n", "PO", pageOffset);
	

	return 0;
}
