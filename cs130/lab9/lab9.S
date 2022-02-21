/* Nicholas Creech
 * CS130
 * 3/27/19
 * Lab9: VPN decoder
 */

.section .rodata
format_error: .asciz "Usage: ./lab9 0x<address>\n"
format_scan: .asciz "%lx"
format_header: .asciz "  Level   Index  Byte Offset\n"
format_VPN: .asciz "VPN[00%d] = %-5d(%4d)\n"
format_PO: .asciz "%-8s = %lx\n"
PO_string: .asciz "PO"

.section .text

.global main
main:
	addi	sp, sp, -60
	sd		ra, 0(sp)
	sd		s0, 8(sp) 
	sd		s1, 16(sp) 
	sd		s2, 24(sp) 
	sd		s3, 32(sp) 

	addi	s3, sp, 48 //s3 = array of VPN indexes
	
	li		t0, 2
	bne		a0, t0, 4f //if(argc != 2) {error message}

	//Scan out the VPN hex address
	ld		a0, 8(a1) //a0 = argv[1] (VPN Hex Address)
	la		a1, format_scan
	addi	a2, sp, 40	
	call	sscanf
	ld		s0, 40(sp) //s0 = VPN hex address

	//AND out the Page Offset
	li		t5, 4095
	and		s2, s0, t5 //s2 = address & 0xFFF (Page Offset)

	srai	s0, s0, 12 //address >>= 12 (drop the page offset bits)
	
	li		s1, 0 //x = 0
	li		t0, 3 //x < 3

1:
	bge		s1, t0, 1f //for(int x = 0; x < 3; x++)
	li		t5, 9
	mul		t5, t5, s1 //Calculate shift amount via s1 iterator
	sra		t4, s0, t5 //Shift address by calculated amount
	li		t5, 511
	and		t1, t4, t5 //t1 = address & 0b111111111
	sw		t1, 0(s3) //VPN[x] = address & 0b111111111
	addi	s3, s3, 4
	addi	s1, s1, 1 //x++
	j		1b
1:

	addi	s3, s3, -4 //Account for extra offset of 4 from previous for loop

	//Print the header
	la		a0, format_header
	call	printf

	li		s1, 2 //x = 2

2:
	blt		s1, zero, 2f //for(int x = 2; x >= 0; x--)
	la		a0, format_VPN
	mv		a1, s1
	lw		a2, 0(s3) //Get VPN index from array
	addi	s3, s3, -4 
	li		t1, 8
	mul		a3, a2, t1 //byte offset = index * 8
	call printf
	addi	s1, s1, -1 //x--
	j		2b
2:
	
	la		a0, format_PO
	la		a1, PO_string
	mv		a2, s2 //a1 = page offset
	call printf

	j		5f

//error
4:
	la		a0, format_error 
	call	printf

//exit
5:
	ld		ra, 0(sp)
	ld		s0, 8(sp)
	ld		s1, 16(sp)
	ld		s2, 24(sp)
	ld		s3, 32(sp)
	addi	sp, sp, 60
	ret

