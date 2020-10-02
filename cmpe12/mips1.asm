##########################################################################
# Created by:  Sutro, Dylan
#              dsutro
#              20 February 2019
#
# Assignment:  Lab 4: ASCII Converter
#              CMPE 012, Computer Systems and Assembly Language
#              UC Santa Cruz, Winter 2019
#
# Description: This program Reads and Converts ASCII inputs.
#
# Notes:       This program is intended to be run from the MARS IDE.
#
#
# Pseudo Code:
# 	
# 	
# 	Prompt User Inputs and Store as ASCII Strings
# 	
# 	Print ASCII Strings to Console
# 	
# 	Read Index[1] of Inputed Strings to Check for X of b to Determine if imputs are Hex or Binary 
# 	
# 	If Input is Hex
# 	
# 		Determine Most Significant Bit
# 		
# 		If Most Significant Bit is 0
# 	
# 			Sign Extend Value with F to 8 bits
# 	
# 
# 		If Most Significant Bit is 1 or 0
# 	
# 			Sign Extend Value with 0 to 8 bits	
# 		
# 		Convert to Two's Complement 	
# 	
# 	If Input is Binary
# 	
# 		Determine Most Significant Bit
# 		
# 		If Most Significant Bit is 0
# 	
# 			Sign Extend Value with 0 to 8 bits
# 	
# 
# 		If Most Significant Bit is 1
# 	
# 			Sign Extend Value with 1 to 8 bits
# 	
# 	Add Arguments and Store Sum as Signed binary 
# 	
# 	Convert signed binary to signed base 4
# 	
# 	Convert Signed Base 4 into Sign and Magnitude with "-"
# 	
# 	Print Sign and Magnitude
# 	
# 	
##########################################################################
.data	
	buffer: .space 8
	nl: .asciiz "\n"
	
.text


.globl main
main:
    #Register Usage:
    #$s0: Sum 
    #$s1: 2CS Value 1
    #$s2: 2CS Value 2
    #$t1: Input 1
    #$t2: Input 2
    #$t8: Holds "x'
    #$t9: Holds 'b'
    
    #Loads ASCII Values for x and b
    li $t8, 98
    li $t9, 120
    
    #Reads Program Arguments
    lw $t1, ($a1)
    lw $t2, 4($a1)
    
    #Prints Arguments
    li $v0, 4
    la $a0, ($t1)
    syscall
 
    li $v0, 4
    la $a0, nl
    syscall
     
    li $v0, 4
    la $a0, ($t2)
    syscall
    
    li $v0, 4
    la $a0, nl
    syscall
        
    #Reads Charecter
    #$t3: Bit [1]
    lb $t4, 1($t2)
    lb $t3, 1($t1)
    

    #Determins if Value is Binary or Hex
    beq, $t3, $t8, Binary
    beq, $t3, $t9, Hex
  	
    Hex:	
  	
  	#Converts first bit
  	lb $t3, 2($t1)
	beq $t3, 48, zero
	beq $t3, 49, one
	beq $t3, 50, two
	beq $t3, 51, three
	beq $t3, 52, four
	beq $t3, 53, five
	beq $t3, 54, six
	beq $t3, 55, seven
	beq $t3, 56, eight
	beq $t3, 57, nine
	beq $t3, 65, A
	beq $t3, 66, B
	beq $t3, 67, C
	beq $t3, 68, D
	beq $t3, 69, E
	beq $t3, 70, F
	zero:
		addu $s1, $s1, 0
		j bit2
	one:
		addu $s1, $s1, 16
		j bit2		
	two:
		addu $s1, $s1, 32
		j bit2		
	three:
		addu $s1, $s1, 48
		j bit2		
	four:
		addu $s1, $s1, 64
		j bit2		
	five:
		addu $s1, $s1, 80
		j bit2		
	six:
		addu $s1, $s1, 96
		j bit2		
	seven:
		addu $s1, $s1, 112
		j bit2		
	eight:
		addu $s1, $s1, 128
		j bit2		
	nine:
		addu $s1, $s1, 144
		j bit2		
	A:
		addu $s1, $s1, 160
		j bit2		
	B:
		addu $s1, $s1, 176
		j bit2		
	C:
		addu $s1, $s1, 192
		j bit2		
	D:
		addu $s1, $s1, 208
		j bit2		
	E:
		addu $s1, $s1, 224
		j bit2		
	F:
		addi $s1, $s1, 240
		j bit2		
		
	bit2:
		
	#Converts second bit
  	lb $t3, 3($t1)
	beq $t3, 48, zero2
	beq $t3, 49, one2
	beq $t3, 50, two2
	beq $t3, 51, three2
	beq $t3, 52, four2
	beq $t3, 53, five2
	beq $t3, 54, six2
	beq $t3, 55, seven2
	beq $t3, 56, eight2
	beq $t3, 57, nine2
	beq $t3, 65, A2
	beq $t3, 66, B2
	beq $t3, 67, C2
	beq $t3, 68, D2
	beq $t3, 69, E2
	beq $t3, 70, F2
	zero2:
		addu $s1, $s1, 0
		j bit2done		
	one2:
		addu $s1, $s1, 1
		j bit2done		
	two2:
		addu $s1, $s1, 2
		j bit2done		
	three2:
		addu $s1, $s1, 3
		j bit2done		
	four2:
		addu $s1, $s1, 4
		j bit2done		
	five2:
		addu $s1, $s1, 5
		j bit2done		
	six2:
		addu $s1, $s1, 6
		j bit2done		
	seven2:
		addu $s1, $s1, 7
		j bit2done		
	eight2:
		addu $s1, $s1, 8
		j bit2done		
	nine2:
		addu $s1, $s1, 9
		j bit2done		
	A2:
		addu $s1, $s1, 10
		j bit2done		
	B2:
		addu $s1, $s1, 11
		j bit2done		
	C2:
		addu $s1, $s1, 12
		j bit2done		
	D2:
		addu $s1, $s1, 13
		j bit2done		
	E2:
		addu $s1, $s1, 14
		j bit2done		
	F2:
		addi $s1, $s1, 15
		j bit2done
		
	bit2done:	
	
   	j converterEnd	


    Binary:
    	
    	li $t3, 0	
    	
    	lb $t3, 2($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext
    		li $t0, 128
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext:
        
    	lb $t3, 3($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext1
    		li $t0, 64
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext1:
    	       
    	lb $t3, 4($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext3
    		li $t0, 32
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext3:
    	
    	lb $t3, 5($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext4
    		li $t0, 16
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext4:
    	
    	lb $t3, 6($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext5
    		li $t0, 8
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext5:
    	
    	lb $t3, 7($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext6
    		li $t0, 4
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext6:
    	
    	lb $t3, 8($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext7
    		li $t0, 2
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext7:    
    	
    	lb $t3, 9($t1)
    	subiu $t3, $t3, 0x30
    	beqz $t3, tonext8
    		li $t0, 1
    		multu $t3, $t0
    		mflo $t5
    		addu $s1, $s1, $t5
    	tonext8:   
    	  		    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    		    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    		
    
   converterEnd:

    beq, $t4, $t8, Binary2
    beq, $t4, $t9, Hex2
  	
    Hex2:	
    	
    	#Converts first bit
  	lb $t3, 2($t2)
	beq $t3, 48, zero3
	beq $t3, 49, one3
	beq $t3, 50, two3
	beq $t3, 51, three3
	beq $t3, 52, four3
	beq $t3, 53, five3
	beq $t3, 54, six3
	beq $t3, 55, seven3
	beq $t3, 56, eight3
	beq $t3, 57, nine3
	beq $t3, 65, A3
	beq $t3, 66, B3
	beq $t3, 67, C3
	beq $t3, 68, D3
	beq $t3, 69, E3
	beq $t3, 70, F3
	zero3:
		addu $s2, $s2, 0
		j bit3
	one3:
		addu $s2, $s2, 16
		j bit3		
	two3:
		addu $s2, $s2, 32
		j bit3		
	three3:
		addu $s2, $s2, 48
		j bit3		
	four3:
		addu $s2, $s2, 64
		j bit3		
	five3:
		addu $s2, $s2, 80
		j bit3		
	six3:
		addu $s2, $s2, 96
		j bit3		
	seven3:
		addu $s2, $s2, 112
		j bit3		
	eight3:
		addu $s2, $s2, 128
		j bit3		
	nine3:
		addu $s2, $s2, 144
		j bit3		
	A3:
		addu $s2, $s2, 160
		j bit3		
	B3:
		addu $s2, $s2, 176
		j bit3		
	C3:
		addu $s2, $s2, 192
		j bit3		
	D3:
		addu $s2, $s2, 208
		j bit3		
	E3:
		addu $s2, $s2, 224
		j bit3		
	F3:
		addu $s2, $s2, 240
		j bit3		
		
	bit3:
		
	#Converts second bit
  	lb $t3, 3($t2)
	beq $t3, 48, zero4
	beq $t3, 49, one4
	beq $t3, 50, two4
	beq $t3, 51, three4
	beq $t3, 52, four4
	beq $t3, 53, five4
	beq $t3, 54, six4
	beq $t3, 55, seven4
	beq $t3, 56, eight4
	beq $t3, 57, nine4
	beq $t3, 65, A4
	beq $t3, 66, B4
	beq $t3, 67, C4
	beq $t3, 68, D4
	beq $t3, 69, E4
	beq $t3, 70, F4
	zero4:
		addu $s2, $s2, 0
		j bit3done		
	one4:
		addu $s2,$s2, 1
		j bit3done		
	two4:
		addu $s2, $s2, 2
		j bit3done		
	three4:
		addu $s2, $s2, 3
		j bit3done		
	four4:
		addu $s2, $s2, 4
		j bit3done		
	five4:
		addu $s2, $s2, 5
		j bit3done		
	six4:
		addu $s2, $s2, 6
		j bit3done		
	seven4:
		addu $s2, $s2, 7
		j bit3done		
	eight4:
		addu $s2, $s2, 8
		j bit3done		
	nine4:
		addu $s2, $s2, 9
		j bit3done		
	A4:
		addu $s2, $s2, 10
		j bit3done		
	B4:
		addu $s2, $s2, 11
		j bit3done		
	C4:
		addu $s2, $s2, 12
		j bit3done		
	D4:
		addu $s2, $s2, 13
		j bit3done		
	E4:
		addu $s2, $s2, 14
		j bit3done		
	F4:
		addu $s2, $s2, 15
		j bit3done
		
	bit3done:	

    	 
   	 j converterEnd2	


    Binary2:
    li $t3, 0	
    	
    	lb $t3, 2($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next
    		li $t0, 128
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next:
        
    	lb $t3, 3($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next2
    		li $t0, 64
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next2:
    	       
    	lb $t3, 4($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next3
    		li $t0, 32
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next3:
    	
    	lb $t3, 5($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next4
    		li $t0, 16
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next4:
    	
    	lb $t3, 6($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next5
    		li $t0, 8
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next5:
    	
    	lb $t3, 7($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next6
    		li $t0, 4
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next6:
    	
    	lb $t3, 8($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next7
    		li $t0, 2
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next7:    
    	
    	lb $t3, 9($t2)
    	subiu $t3, $t3, 0x30
    	beqz $t3, next8
    		li $t0, 1
    		multu $t3, $t0
    		mflo $t5
    		addu $s2, $s2, $t5
    	next8:     		    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    		    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	

   converterEnd2:
   
   #Converts to 2SC
   subi $s1, $s1, 128
   subi $s2, $s2, 128
   
   
#Adder
    add $s0, $s1, $s2
    
    bltz $s0, negative
    j base4
    negative:
    	li $v0, 11
    	li $a0, 45
    	syscall
    	
    	li $t0, -1
    	mult $s0, $t0
    	mflo $s0
    
    
    #base 4 converter
    base4:
    li $t4, 0
    move $t5, $s0
    li $t0, 256
    div $t5, $t0   
    mfhi $t5
    mflo $t4 
    
    li $v0, 11
    addi $t4, $t4, 48
    la $a0, ($t4)
    syscall
    li $t4, 0
    
    li $t0, 64
    div $t5, $t0   
    mfhi $t5
    mflo $t4 
    
    li $v0, 11
    addi $t4, $t4, 48
    la $a0, ($t4)
    syscall
    li $t4, 0

    li $t0, 16
    div $t5, $t0   
    mfhi $t5
    mflo $t4
    
    li $v0, 11
    addi $t4, $t4, 48
    la $a0, ($t4)
    syscall
    li $t4, 0
    
    li $t0, 4
    div $t5, $t0   
    mfhi $t5
    mflo $t4

    li $v0, 11
    addi $t4, $t4, 48
    la  $a0, ($t4)
    syscall
    
    li $v0, 11
    addi $t5, $t5, 48
    la $a0, ($t5)  
    syscall         

# exit program
exit:
    li      $v0, 10
    syscall
