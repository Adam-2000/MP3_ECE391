#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	printf("test fail\n");
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;
	int i;
	int result = PASS;
	for (i = 0; i < 9; ++i){
		printf("idt entry: %d\n content: 0x%x \n", i, idt[i]);
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			printf("oops: idt_test: i = %d\n", i);
			assertion_failure();
			result = FAIL;
		}
		
	}
	return result;
}

// add more tests here
/* KEY Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int key_test(){
	TEST_HEADER;
	while(1);
	return PASS;
}

/* RTC Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int rtc_test(){
	TEST_HEADER;
	uint32_t old = 0;
	while(1){
		if (old != rtc_counter){
			old = rtc_counter;
			printf("rtc counter= %d\n", old);
		}
	}
	return PASS;
}

/* syscall Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int syscall_test(){
	TEST_HEADER;
	asm volatile("int $0x80");
	printf("endend where were we???\n");
	return PASS;
}

/* paging_test_exist
* Checks that the kernel and video memory addresses can be dereferenced.
* Inputs: None
* Outputs: None
* Side Effects: Halts the system and displays fault message
* Coverage: 4MB Kernel page and 4KB vedio memory
* Files: paging.c
*/
int paging_test_exist() {
    TEST_HEADER;
    //Used to test dereference locations.
    char result;

    char* pointer = (char*)0x400000;    	   //start of Kernel memory
    result = *pointer;

    pointer = (char*)0x7FFFFF;                 //Bottom of kernel memory
    result = *pointer;

    pointer = (char*)0xB8000;                  //start of Video memory address
    result = *pointer;

    pointer = (char*)0xB8FFF;                 //Bottom of video memory
    result = *pointer;
    return PASS;                              // if no exception not occure, pass
}


/* paging_test_null
* dereference the null mem address, it will occur a exception.
* Inputs: None
* Outputs: None
* Side Effects: Halts the system and displays fault message
* Coverage: memory that would not be used
* Files: paging.c
*/
int paging_test_null(){
	TEST_HEADER;
	char result;
	// choose one condition to test
    char* pointer = (char*)0xB7FFF;    //point to a null mem at upper boundary of vedio mem	
	result = *pointer;

	pointer = (char*)0xB9000;    //point to a null mem at lower boundary of vedio mem	
	result = *pointer;

	pointer = (char*)0x3FFFFF;  //point to a null mem at upper boundary of kernal mem	
	result = *pointer;

	pointer = (char*)0x800000;  //point to a null mem at lower boundary of kernal mem	
	result = *pointer;

	return FAIL;                     // if no exception not occure, pass
}




/* devide_zero
* description: give a system call, check whether it is ok
* Inputs: None
* Outputs: None
* Side Effects: Halts the system and displays fault message
* Coverage: devide zero
* Files: exception.c/h
*/
int devide_zero(){
	TEST_HEADER;
	const int a = 10;
	const int b = 0;
	int c;	// let denominator be zero
	
	c = a/b;
	return FAIL;
}


// add more tests here
/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */
/* Test suite entry point */
void launch_tests(){
	printf("*********************\n");
	TEST_OUTPUT("syscall_test", syscall_test());
	printf("*********************\n");
	TEST_OUTPUT("idt_test", idt_test());

	printf("*********************\n");
	TEST_OUTPUT("paging_test_exist", paging_test_exist());	

	printf("*********************\n");
	TEST_OUTPUT("paging_test_null", paging_test_null());

	// printf("*********************\n");
	// TEST_OUTPUT("syscall_test", syscall_test());

	// printf("*********************\n");
	// TEST_OUTPUT("devide_zero", devide_zero());
	printf("*********************\n");
	TEST_OUTPUT("key_test", key_test());
}
