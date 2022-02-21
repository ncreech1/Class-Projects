/* Nicholas Creech
 * CS130
 * 3/6/19
 * Lab8: Create the Search and Median functions in RISCV assembly
 */

.section .text

.global Search
Search:
	//arg0 = people[], arg1 = num_people, arg2 = name, arg3 = age, arg4 = gender
	
	//OFFSET % SIZE == 0  <-- must be true
	/* NAME	    OFFSET		SIZE
	   name		0			51 (0 % 51 == 0)		 
	   age		51 + 1		4  (52 % 4 == 0)		
	   gender	56			1  (56 % 1 == 0)
				57 % 4 != 0 --> 60 is the final size
	*/

	li		t0, 0 //int x = 0
1:	
	bge		t0, a1, 1f //for(int x = 0; x < num_people; x++)
		addi	a0, a0, 52 //Increment people pointer to age
		addi	t0, t0, 1 //x++
		lw		t1, 0(a0) //t1 = people[x].age
		bne		t1, a3, 4f //if(people[x].age == age)
			addi	a0, a0, 4 //Increment people pointer to gender
			lb		t1, 0(a0) //t1 = people[x].gender
			bne		t1, a4, 3f //if(people[x].gender == gender)
				li		t2, 0 //int i = 0
				li		t3, 51 //Upper limit of for loop
				bge		t2, t3, 2f //for(int i = 0; i < 51; i++)
					addi	a0, a0, -56 //Decrement people pointer to name
5:
					lb		t4, 0(a0) //t4 = people[x].name[i]
					lb		t5, 0(a2) //t5 = name[i]
					addi	a0, a0, 1 //Increment people[x].name pointer to next char
					addi	a2, a2, 1 //Increment name pointer to next char
					addi	t2, t2, 1 //i++
					bne		t4, t5, 2f //if(people[x].name[i] == name[i])
						li		t6, 50
						beq		t2, t6, 6f //if(i == 50)
							j		5b		
							
					
2:
	sub		a0, a0, t2 //Set people pointer back to starting address for current person
	sub		a2, a2, t2 //Set name pointer back to starting address
	addi	a0, a0, 60 //Set people pointer to next person
	j		1b
3:
	addi	a0, a0, -4 //Set people pointer back to address of age for current person
4:
	addi	a0, a0, -52 //Set people pointer back to starting address for current person
	addi	a0, a0, 60 //Set people pointer to next person
	j		1b
1:
	mv		a0, zero //Return 0L if no match found
	ret
6:
	sub		a0, a0, t2 //Set people pointer back to starting address for current person (match found)
	ret
	
.global Median
Median:
	//arg0 = array[], arg1 = array_size, arg2 = int * buffer

	lw		t0, 0(a0) //int smallest = array[0];
	li		t3, 0 //int x = 0;

1:
	//For loop to find the absolute smallest int in the array
	bge		t3, a1, 1f //for(int x = 0; x < array_size; x++)
		lw		t4, 0(a0) //t4 = array[x]
		addi	a0, a0, 4 //Increment array pointer
		addi	t3, t3, 1 //x++
		bge		t4, t0, 1b //if(array[x] < smallest)
			mv		t0, t4 //smallest = array[x]
1:
	//Reset array index to 0
	li		t4, 4
	mul		t3, t3, t4
	sub		a0, a0, t3

	mv		t1, t0 //int currentVal = smallest

	li		t3, 0 //int bufferIndex = 0
	li		t4, 0 //int x = 0

2:
	//For loop to create the sorted buffer
	bge		t4, a1, 2f //for(int x = 0; x < array_size; x++)
		lw		t5, 0(a0)
		bne		t5, t1, 3f //if(array[x] == currentVal)
			sw		t1, 0(a2) //buffer[bufferIndex] = currentVal
			addi	a2, a2, 4 //Increment buffer pointer
			addi	t3, t3, 1 //bufferIndex++
3:
		ble		t5, t1, 4f //if(array[x] > currentVal && smallest == currentVal) 
		bne		t0, t1, 4f
			mv		t0, t5 //smallest = array[x]
			j		5f
4:
		ble		t5, t1, 5f //else if(array[x] > currentVal && array[x] < smallest)
		bge		t5, t0, 5f
			mv		t0, t5 //smallest = array[x]
5:
		mv		t6, a1
		addi	t6, t6, -1
		bne		t4, t6, 6f //if(x == array_size - 1 && bufferIndex < array_size)
		bge		t3, a1, 6f
			mv		t1, t0 //currentVal = smallest
			li		t6, 4
			mul		t6, t6, t4
			sub		a0, a0, t6 //Reset array pointer
			li		t4, 0 //x = -1
			j		2b //Skip incrementing array pointer and x
6:
		addi	a0, a0, 4 //Increment array pointer		
		addi	t4, t4, 1 //x++
		j		2b
2:
	//Reset buffer pointer
	li		t6, 4
	mul		t6, t6, t3
	sub		a2, a2, t6

	//Reset array pointer
	li		t6, 4
	mul		t6, t6, t4
	sub		a0, a0, t6

	li		t3, 2
	rem		t4, a1, t3 //t4 = array_size % 2
	
	beq		t4, zero, 3f //if(array_size % 2 != 0)
		div		t5, a1, t3 //t5 = array_size / 2
		li		t3, 4
		mul		t5, t5, t3 //Calc offset
		add		a2, a2, t5 //Increment buffer pointer to (array_size / 2)
		lw		a0, 0(a2) //a0 = buffer[array_size / 2]
		j		4f
3:
	div		t5, a1, t3 //t5 = array_size / 2
	li		t6, 4
	mul		t5, t5, t6
	add		a2, a2, t5 //Increment buffer pointer to (array_size / 2)
	lw		a4, 0(a2) //a4 = buffer[array_size / 2]
	addi	a2, a2, -4 //Decrement buffer pointer to ((array_size / 2) - 1)
	lw		a5, 0(a2) //a5 = buffer[(array_size / 2) - 1]
	add		a0, a4, a5 //a0 = (buffer[array_size / 2] + buffer[(array_size / 2) - 1])
	div		a0, a0, t3 //a0 /= 2
4:
	ret
