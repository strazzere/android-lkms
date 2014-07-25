/*
 * Simple LKM used for testings and watching
 * some applications. I don't recommend doing the
 * things I've done - it's very hacky and likely to
 * cause pile ups.
 *
 * Tim 'diff' Strazzere <diff@lookout.com>
 *
 */

#include "android_module.h"

#define SYSTEM_UID 1000
#define TARGET_UID 10049

int hooked_open(const char* path, int flags, mode_t mode) {
  uid_t uid = original_getuid();

  int fd = -99;

  // 1000 is the system and this is very loud w/o filtering
  if(uid != SYSTEM_UID)
    printk(KERN_INFO "Hooked open function inside [ %d ] [ %d ]! path: %s flags: %d other: %d\n", (int) uid, (int) original_getpid(), path, flags, mode);

  // Group specifically being targeted
  if(uid == TARGET_UID) {

    // This was to try out some hacks, I don't recommend this
    if(strcmp(path, "/system/lib/libdvm.so") == 0) {
      printk(KERN_INFO "Redirecting file to local version\n");
      path = "/data/local/tmp/libdvm.so";
    }

    fd = original_open(path, flags, mode);
    printk(KERN_INFO "FD: %d\n", fd);
  }

  if(fd == -99)
    fd = original_open(path, flags, mode);

  return fd;
}

ssize_t hooked_read(int fd, char *buf, size_t count) {
  uid_t uid = original_getuid();

  // Only care about our target
  if(uid == TARGET_UID) {
    printk(KERN_INFO "Hooked read function inside [ %d ] [ %d ]! fd: %d count: %d\n", (int) uid, (int) original_getpid(), fd, count);
  }

  return original_read(fd, buf, count);
}

size_t hooked_write(int fildes, const char *buf, size_t nbytes) {
  uid_t uid = original_getuid();

  if(uid == TARGET_UID) {
    printk(KERN_INFO "Hooked write function inside [ %d ] [ %d ]! fildes: %d size_t: %d\n", (int) uid, (int) original_getpid(), fildes, nbytes);

    // Sniffing for "dex" in the write bytes
    if((int) nbytes > 3) {
      printk(KERN_INFO "%c%c%c\n", buf[0], buf[1], buf[2]);
      if(buf[0] == 'd' &&
         buf[1] == 'e' &&
         buf[2] == 'x') {
        printk(KERN_INFO "Found dex returning bad number!\n\n");

	// Kill the write
        return -1;
      }
    }
  }

  return original_write(fildes, buf, nbytes);
}

int init_module(void)
{
  printk(KERN_INFO "open module loaded.\n");

  sys_call_table = (void*)SYS_CALL_TABLE_ADDRESS; 
  original_open = sys_call_table[__NR_open];
  original_getuid = sys_call_table[__NR_getuid];
  original_getpid = sys_call_table[__NR_getpid];
  original_write = sys_call_table[__NR_write];
  original_read = sys_call_table[__NR_read];
  sys_call_table[__NR_open] = hooked_open;
  sys_call_table[__NR_write] = hooked_write;
  sys_call_table[__NR_read] = hooked_read;
  return 0;
}

void cleanup_module(void)
{
  printk(KERN_INFO "open module unloaded.\n");
  sys_call_table[__NR_open] = original_open;
  sys_call_table[__NR_write] = original_write;
  sys_call_table[__NR_read] = original_read;
}

MODULE_AUTHOR("Tim 'diff' Strazzere");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Hook opening/writing/reading in an attempt to sniff for things");
