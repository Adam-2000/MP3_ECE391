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

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	TEST_OUTPUT("syscall_test", syscall_test());
	printf("where were we???\n");
	TEST_OUTPUT("key_test", key_test());
	// launch your tests here
}
