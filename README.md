android-lkms
============

Android Loadable Kernel Modules - mostly used for reversing and debugging on controlled systems/emulators.

 * antiptrace - simple ptrace hooking module for use to aid in reversing native applications on Android
 * open-read-write - hooks lots of functions in an attempt to find out what files a piece of malware was touching

Warning
-------

Beware using any of these in a production like environment, they have been tested only in my personal
qemu environments. Some are (bad) attempts at trying different methods of hooking things to see the
outcome. The results may be unexpected and cause qemu environments to slow down due to performing slower
operations in the kernel than you should be. Someone help your poor soul if you try to run all of these
on a real device.

Compiling
---------

Edit the Makefile prior and ensure that the paths are correct for your system. There are
assumptions made how your environments are set up. You will also need the 'sys_call_table'
from the compiled kernel - which is grabbed automatically by a script. You can manually insert
this value into android_module.h if you would prefer.

Using the LKMS
--------------

I've written a blog about setting up an OSX environment for compiling the a Kernel capable of running
LKMs, you should reference this for a quick and easy how-to;
[Compiling an Android Emulator Kernel for Loadable Kernel Modules](http://www.strazzere.com/blog/2014/07/compiling-an-emulator-kernel-for-loadable-modules/)


diff@lookout.com