// Normal LKM stuff
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>

// SYS_CALL_TABLE_ADDRESS - this will be filled via the makefile
#define SYS_CALL_TABLE_ADDRESS 0xc0028084

// No longer exported, could actually enable this in the kernel though...
//extern void* sys_call_table[];
void **sys_call_table;

int (*original_unlink)(const char *filename);
