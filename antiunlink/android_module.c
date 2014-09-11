/*
 * Simple LKM for preventing unlinking (deleting) of files
 * from a specified directory.
 *
 * Used as a very simple unpacker aspect for dexprotector, will
 * land you the unencrypted new.apk jar which contains the obfuscated
 * code - and access to the dexprotector.so library if needed
 *
 * Tim 'diff' Strazzere <diff@lookout.com>
 *
 */

#include "android_module.h"
#define TARGET "com.example.protected.package"


int hooked_unlink(const char *filename) {
  int ret = 0;

  // Look for target package in file path, but avoid odex files being removed (otherwise it will fail)
  if(strstr(filename, TARGET) != NULL && strstr(filename, "odex") == NULL) {
    // Skip remove the file so we can play with it
    printk(KERN_INFO "Found someone trying to unlink [ %s ] !\n", filename);
  } else {
    ret = original_unlink(filename);
  }

  return ret;
}

int init_module(void) {
  printk(KERN_INFO "Anti-unlinker module loaded.\n");

  // grep "sys_call_table" System.map | sed "s/^\(.*\) \(. sys_call_table\)/0x\1/"
  sys_call_table = (void*)SYS_CALL_TABLE_ADDRESS; 
  // Got from "cat /proc/kallsyms | grep sys_call_table" on emulator
  original_unlink = sys_call_table[__NR_unlink];
  sys_call_table[__NR_unlink] = hooked_unlink;

  return 0;
}

void cleanup_module(void) {
  printk(KERN_INFO "Anti-unlinker module unloaded.\n");
  sys_call_table[__NR_unlink] = original_unlink;
}

MODULE_AUTHOR("Tim 'diff' Strazzere");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Anti-unlinker for a specified package; for use against dexprotector");
