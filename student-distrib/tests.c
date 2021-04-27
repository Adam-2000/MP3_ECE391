#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "keyboard.h"
#include "file_system.h"
#include "system_calls.h"
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

/* RTC Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
// int rtc_test(){
// 	TEST_HEADER;
// 	uint32_t old = 0;
// 	while(1){
// 		if (old != rtc_counter){
// 			old = rtc_counter;
// 			printf("rtc counter= %d\n", old);
// 		}
// 	}
// 	return PASS;
// }

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

/* file_test()
* description: test the file system
* Inputs: None
* Outputs: None
* Side Effects: none
*/
int file_test(){
    uint8_t fname1[NAME_LEN] = "frame0.txt";
	uint8_t fname2[NAME_LEN+2] = "verylargetextwithverylongname.tx";
	//uint8_t fname3[NAME_LEN] = "fish";
	
    int32_t bytes_to_read  = BLOCK_SIZE * 10;
    uint8_t block_buf[BLOCK_SIZE * 10];
    int32_t bytes_read;
    int i;
    clear();		
	set_cursor(0, 0);	
	int32_t fd;
	int32_t fd2;
	fd = open(fname1);
	//printf("%s's node index is %d\n",fname1,fd);
	bytes_read = read(fd , block_buf, bytes_to_read);
	for(i =0; i <bytes_read ;i++){
	    putc(block_buf[i]);
	}
	printf("\n");
	printf("read %s success, total read %d bytes\n",fname1,bytes_read);	       
	close(fd);

	char readbuf[10];
	printf("input anything and enter to continue\n");
	read(0, readbuf, 10);

	fd2 = open(fname2);
	//printf("%s's node index is %d\n",fname2,fd2);
	bytes_read = read(fd2, block_buf, bytes_to_read);
	for(i =0; i <bytes_read ;i++){
		putc(block_buf[i]);
	}
	printf("\n");
	printf("bytes_read: %d\n",bytes_read);


	if(bytes_read == 0){
		printf("reached the end of the file\n");
	}
	printf("\n");
	printf("contine read verylargetextwithverylongname.txt success\n");     
	printf("total read bytes %d B\n",bytes_read);

	close(fd);
    return PASS;
}

/* directory_test()
* description: test the file system
* Inputs: None
* Outputs: None
* Side Effects: none
*/
int directory_test(){
    uint8_t fname1[NAME_LEN] = ".";
	//uint8_t fname3[NAME_LEN] = "fish";
	
    int32_t bytes_to_read  = NAME_LEN;
    uint8_t block_buf[NAME_LEN];
    int32_t bytes_read;
    int32_t total_read_time = 0;
	int32_t total_read_bytes = 0;
    int i;
    clear();		
	set_cursor(0, 0);	
	int32_t fd;
	fd = open(fname1);
	//printf("%s's node index is %d\n",fname1,fd);

	printf("ls: ");
    while(1){
        bytes_read = read(fd , block_buf, bytes_to_read);
        for(i =0; i <bytes_read ;i++){
            if(block_buf[i] != NULL){
                putc(block_buf[i]);
            }
        }
        printf(" ");

        total_read_time ++;
		total_read_bytes += bytes_read;

        if(bytes_read == 0){
            printf("\nreached the end of the dir\n");
            break;
		}
    }	
	close(fd);
	printf("\n");
	printf("read directory success\n");     
	printf("total read times is %d\ntotal read bytes %d B\n",total_read_time,total_read_bytes);

	char readbuf[10];
	printf("input anything and enter to continue\n");
	read(0, readbuf, 10);

    return PASS;
}
/* rtc Test
 * test for the frequency 
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int rtc_test(){
	TEST_HEADER;
	int fd;
	fd = open((uint8_t*)"rtc");
	clear();
	set_cursor(0, 0);
	int i;
	printf("feel the frequecy of 2HZ\n");
	char buf[10];
	for (i = 0; i < 10;){
		set_cursor(0, 1);
		printf("%u", rtc_counter);
		read(fd, buf, 0);
		i++;
	}

	clear();
	set_cursor(0, 0);
	uint32_t new_freq = 8;
	write(fd, &new_freq, 4);
	printf("feel the frequecy of 8HZ\n");
	for (i = 0; i < 40;){
		set_cursor(0, 1);
		printf("%u", rtc_counter);
		read(fd, buf, 0);
		i++;
	}
	close(fd);
	return PASS;
}

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
		printf("\nstarting reading:\n");
		ret = read(0, buffer, 128);
		printf("# bytes:%u\n", ret);
		printf("what have you typed:\n");
		write(1, buffer, ret);
	}
	return PASS;
}

/* Checkpoint 3 tests */
/* execute Test: Execute a executable file
 */
int execute_test(){
	TEST_HEADER;
	//int ret;
	// char cmd[33] = "hello";
	// ret = execute((uint8_t*)cmd);
	// if (ret == -1){
	// 	return FAIL;
	// }
	// char cmd1[33] = "shell";
	// ret = execute((uint8_t*)cmd1);
	// if (ret == -1){
	// 	return FAIL;
	// }
	while (1);
	return PASS;
}
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

	// printf("*********************\n");
	// TEST_OUTPUT("rtc_test", rtc_test());

	// printf("*********************\n");
	// TEST_OUTPUT("directory_test", directory_test());

	// printf("*********************\n");
	// TEST_OUTPUT("file_test", file_test());

	// printf("*********************\n");
	// TEST_OUTPUT("key_test", key_test());

	printf("*********************\n");
	TEST_OUTPUT("execute_test", execute_test());

	while(1);

	

}
