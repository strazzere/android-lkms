// Normal LKM stuff
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

// Ptrace specific stuff
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/unistd.h>

// SYS_CALL_TABLE_ADDRESS - this will be filled via the makefile
#define SYS_CALL_TABLE_ADDRESS 0xc0028084

// No longer exported, could actually enable this in the kernel though...
//extern void* sys_call_table[];
void **sys_call_table;

// Is this required? Only causes warning and doesn't seem to matter
//extern struct task_struct *current(void);
int ignore_ptrace_requests = 0;

int (*original_ptrace)(int request, int pid, int address, int data);
