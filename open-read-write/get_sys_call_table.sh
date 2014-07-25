#!/bin/sh
SYS_CALL_TABLE=`grep 'sys_call_table' $1/System.map | sed 's/^\(.*\) \(. sys_call_table\)/0x\1/'`
sed -i '' "s/\#define SYS_CALL_TABLE_ADDRESS.*/\#define SYS_CALL_TABLE_ADDRESS $SYS_CALL_TABLE/g" $2