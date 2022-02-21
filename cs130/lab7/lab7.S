/* Nicholas Creech
 * CS130
 * 2/27/19
 * Lab7: Create an assembly program that takes a left operand, right operand, and an operation and prints the result
 */

.section .rodata
format_int: .asciz "%d"
format_char: .asciz "%c"
not_enough: .asciz "Not enough arguments.\n"
too_many: .asciz "Too many arguments.\n"
invalid_op: .asciz "Unknown operator '%c'\n"
print_result: .asciz "Result: %d %c %d = %d\n"

.section .text

.global add
add:
	add		a0, a0, a1 //a0 = left + right
	ret

.global subtract
subtract:
	sub		a0, a0, a1 //a0 = left - right
	ret

.global multiply
multiply:
	mul		a0, a0, a1 //a0 = left x right
	ret

.global divide
divide:
	beq		a1, zero, 1f //Branch if left operand is 0 so -1 is returned
	div		a0, a0, a1 //a0 = left / right
	ret
1:
	li		a0, -1
	ret

.global modulo
modulo:
	beq		a1, zero, 1f //Branch if right operand is 0 so 0 is returned
	rem		a0, a0, a1 //a0 = left % right
	ret
1:
	li		a0, 0
	ret

.global main
main:
	addi	sp, sp, -64
	sd		ra, 0(sp)

	//Check the command line arguments
	li		t0, 4
	blt		a0, t0, not_enough_arguments
	bgt		a0, t0, too_many_arguments

	//Preserve saved registers
	sd		s0, 8(sp)
	sd		s1, 16(sp)
	sd		s2, 24(sp)

	//Correct number of arguments found
	ld		s0, 8(a1) //s0 = argv[1]
	ld		s1, 16(a1) //s1 = argv[2]
	ld		s2, 24(a1) //s2 = argv[3]

	//Use sscanf to convert left arg to int
	mv		a0, s0
	la		a1, format_int
	addi	a2, sp, 32 //&left = sp + 32
	call	sscanf

	//Use sscanf to convert middle arg to char
	mv		a0, s1
	la		a1, format_char
	addi	a2, sp, 40 //&mid = sp + 40
	call	sscanf

	//Use sscanf to convert right arg to int
	mv		a0, s2
	la		a1, format_int
	addi	a2, sp, 48 //&right = sp + 48
	call	sscanf

	//Check if a valid operator was used, then call the operation
	lw		a0, 32(sp) //a0 = left op
	lb		t0, 40(sp) //t0 = mid op
	lw		a1, 48(sp) //a1 = right op
	li		t1, '+'
	li		t2, '-'
	li		t3, 'x'
	li		t4, '/'
	li		t5, '%'
	beq		t0, t1, 1f //if(mid == '+')
	beq		t0, t2, 2f //if(mid == '-')
	beq		t0,	t3, 3f //if(mid == 'x')
	beq		t0,	t4, 4f //if(mid == '/')
	beq		t0,	t5, 5f //if(mid == '%')
	j		unknown_operator //else: Not a valid operator
1:
	call	add
	j		6f
2:
	call	subtract
	j		6f
3:
	call	multiply
	j		6f
4:
	call	divide
	j		6f
5:
	call	modulo
6:
	//Print the result
	mv		a4, a0 //a4 = result
	la		a0, print_result
	lw		a1, 32(sp) //a1 = left op
	lb		a2, 40(sp) //a2 = mid op
	lw		a3,	48(sp) //a3 = right op
	call	printf

	//Reload saved registers
	ld		s0, 8(sp)
	ld		s1, 16(sp)
	ld		s2, 24(sp)
	j		exit

unknown_operator:
	//Reload saved registers
	ld		s0, 8(sp)
	ld		s1, 16(sp)
	ld		s2, 24(sp)

	//Print error
	la		a0, invalid_op
	lb		a1,	40(sp)
	call	printf
	j		exit

not_enough_arguments:
	la		a0, not_enough
	call	printf
	j		exit

too_many_arguments:
	la		a0, too_many
	call	printf
	j		exit

exit:
	ld		ra, 0(sp)
	addi	sp, sp, 64
	ret		
