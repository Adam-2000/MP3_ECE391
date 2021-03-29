#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "keyboard.h"
#include "file_system.h"
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
	char buffer[128];
	uint32_t ret;
	while(1){
		printf("starting reading:\n");
		ret = terminal_read(0, buffer, 128);
		printf("# bytes:%u\n", ret);
		printf("what have you typed:\n");
		terminal_write(0, buffer, ret);
	}
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

int file_test(){
    uint8_t fname1[NAME_LEN] = "frame0.txt";
	uint8_t fname2[NAME_LEN+2] = "verylargetextwithverylongname.txt";
	//uint8_t fname3[NAME_LEN] = "fish";
	
    int32_t bytes_to_read  = BLOCK_SIZE;
    uint8_t block_buf[BLOCK_SIZE] = {};
    int32_t bytes_read;
    int32_t total_read_time = 0;
	int32_t total_read_bytes = 0;
    int i;
    clear();			
	int32_t fd;
	int32_t fd2;
	fd = file_open(fname1);
	//printf("%s's node index is %d\n",fname1,fd);
	bytes_read = file_read(fd , block_buf, bytes_to_read);
	for(i =0; i <BLOCK_SIZE ;i++){
	    if(block_buf[i] != NULL){
	        putc(block_buf[i]);
	    }
	}
	printf("\n");
	printf("read %s success, total read %d bytes\n",fname1,bytes_read);	       
	file_close(fd);

	char readbuf[10];
	printf("input anything and enter to continue\n");
	terminal_read(0, readbuf, 10);

	fd2 = file_open(fname2);
	//printf("%s's node index is %d\n",fname2,fd2);
    while(1){
        bytes_read = file_read(fd2, block_buf, bytes_to_read);
        for(i =0; i <bytes_read ;i++){
            if(block_buf[i] != NULL){
                putc(block_buf[i]);
            }
        }
        printf("\n");
        printf("bytes_read: %d\n",bytes_read);

        total_read_time ++;
		total_read_bytes += bytes_read;

        if(bytes_read == 0){
            printf("reached the end of the file\n");
            break;
		}
    }	
	printf("\n");
	printf("contine read verylargetextwithverylongname.txt success\n");     
	printf("total read times is %d\ntotal read bytes %d B\n",total_read_time,total_read_bytes);


    return PASS;
}

// add more tests here
/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */
/* Test suite entry point */
void launch_tests(){
	// printf("*********************\n");
	// TEST_OUTPUT("syscall_test", syscall_test());
	// printf("*********************\n");
	// TEST_OUTPUT("idt_test", idt_test());

	// printf("*********************\n");
	// TEST_OUTPUT("paging_test_exist", paging_test_exist());	

	// printf("*********************\n");
	// TEST_OUTPUT("paging_test_null", paging_test_null());

	// printf("*********************\n");
	// TEST_OUTPUT("syscall_test", syscall_test());

	// printf("*********************\n");
	// TEST_OUTPUT("devide_zero", devide_zero());
	//printf("*********************\n");
	//TEST_OUTPUT("key_test", key_test());

	printf("*********************\n");
	TEST_OUTPUT("file_test", file_test());
}
