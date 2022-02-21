/* Nicholas Creech
 * CS130
 * 2/27/19
 * Pre-Lab8
 */

.section .text

/*

int get_sum(const short array[], int size)
{
	int total = 0;
	for(int x = 0; x < size; x++)
		total += array[x];
	return total;
}

*/

.global get_sum
get_sum:
	li	t0, 0 //x = 0
	li	t1, 0 //total = 0
1:
	bge		t0, a1, 1f //for(int x = 0; x < size; x++) 
	ld		t2, 0(a0) //t2 = array[x]
	add		t1, t1, t2 //total += array[x]
	addi	a0, a0, 2 //Increase array pointer
	addi	t0, t0, 1 //x++
1:
	mv		a0, t1 //Return the sum
	ret

	
