#include "string.h"
#include "stdio.h"
#include "stdint.h"

#define POLYNOMIAL 0x2F

uint8_t crcTable[256];

/*(DIVIDEND) - (POLYNOMIAL) =  REMAINDER*/


/*
This function generates a lookup table for 8 bit CRC. Remainder of division of a Byte by polynomial is saved in Table.
Outermost 'for' loop iterates over all possible bytes, i.e 0-255.
The Second 'for' loop iterates over all the bit.
If the MSB of the Data is 1 then only XOR is done else the remainder is just shifted.
NOTE: MSB of polynomial is neglected because of code in 'if' condition.
*/
void CrcTableGenerate(void)
{
	uint8_t remainder = 0;
	for(int i=0;i<256;i++)
	{
		remainder = i;
		for(int j=8;j>0;--j)
		{
			if(remainder & 0x80) //The MSB in polynomial is neglected due to this line.
				remainder = (remainder<<1) ^ POLYNOMIAL;
			else
				remainder = remainder<<1;
		}	
		crcTable[i] = remainder;
		//printf("%d: %x\n", i, remainder);
	}
}

/*
At first to crc is filled with remainder of byte/polynomial.
On further iterations same operation is done on successive byte and the end result is CRC for the frame

NOTE: XORING is for subtraction
*/
uint8_t calculateCrc(uint8_t* array, int len)
{
	uint8_t crc = 0;
	for(int i = 0; i< len; i++)
	{
		uint8_t data = array[i] ^ (crc);
		crc = crcTable[data];
		printf("I/P data: %x, Index: %x, CRC: %x\n",array[i] , data, crc);
	}
	return crc;
}

void main()
{
	CrcTableGenerate();
	/*0xFF added as the seed*/
	/*No need to add 8 bits of zero padding in the end*/
	uint8_t buffer[] = {0xFF, 0x01, 0xC4, 0x0F, 0x22};
	calculateCrc(buffer, sizeof(buffer));
	//printf("CRC: %x\n", calculateCrc(buffer, sizeof(buffer)));
}