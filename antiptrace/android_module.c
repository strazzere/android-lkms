/*
 * Simple LKM for having fun with ptrace'ing
 * applications.
 *
 * Tim 'diff' Strazzere <diff@lookout.com>
 *
 */

#include "android_module.h"

// Stupid enum so we aren't staring
const char* stringFromPtrace(int request) {
  static const char *strings[] = {
    "PTRACE_TRACEME",
    "PTRACE_PEEKTEXT",
    "PTRACE_PEEKDATA",
    "PTRACE_PEEKUSR",
    "PTRACE_POKETEXT",
    "PTRACE_POKEDATA",
    "PTRACE_POKEUSR",
    "PTRACE_CONT",
    "PTRACE_KILL",
    "PTRACE_SINGLESTEP",
    // unknown
    "UNK", "UNK","UNK","UNK","UNK","UNK",
    "PTRACE_ATTACH",
    "PTRACE_DETACH",
    // unknown
    "UNK","UNK","UNK","UNK","UNK","UNK",
    "PTRACE_SYSCALL"
  };

  return strings[request];
}

// Hooked ptrace function
int hooked_ptrace(int request, int pid, int address, int data) {
  int ret = 0;

  printk(KERN_INFO "Ptrace was called; request[%s] pid[%d] addr[%x] data[%x]\n", stringFromPtrace(request), pid, address, data);

  // For various reasons this can be useful, just send a ptrace function with this value to
  // ignore the rest of the ptraces
  if(data == 0xFEEDD1FF) {
    ignore_ptrace_requests = 1;
  }

  if(current->ptrace & PT_PTRACED || ignore_ptrace_requests) {
    // If someone is being ptraced and asks to be ptraced,
    // just tell them they are instead of returning < 0
    printk("Force feeding 0 back to pid...\n");
    ret = 0;
  } else {
    // pass to real ptrace
    ret = original_ptrace(request, pid, address, data);
  }

  return ret;
}

int init_module(void)
{
  printk(KERN_INFO "Anti-ptracer module loaded.\n");

  // grep "sys_call_table" System.map | sed "s/^\(.*\) \(. sys_call_table\)/0x\1/"
  sys_call_table = (void*)SYS_CALL_TABLE_ADDRESS; 
  // Got from "cat /proc/kallsyms | grep sys_call_table" on emulator
  original_ptrace = sys_call_table[__NR_ptrace];
  sys_call_table[__NR_ptrace] = hooked_ptrace;

  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "Anti-ptracer module unloaded.\n");
  sys_call_table[__NR_ptrace] = original_ptrace;
}

MODULE_AUTHOR("Tim 'diff' Strazzere");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Ptracers gunna ptrace; if any application being ptraced requests to ptrace itself, tell them it's ok!");
