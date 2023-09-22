#########################################################################
###
 # @Author: 快出来了哦
 # @Date: 2023-09-19 22:41:17
 # @LastEditors: 快出来了哦
 # @LastEditTime: 2023-09-19 22:41:18
 # @FilePath: /chatserver/autobuild.sh
 # @Description: 
### 
# File Name: autobuild.sh
# Author: shi lei
# mail: shitou3000@qq.com
# Created Time: 2020年02月23日 星期日 18时40分28秒
#########################################################################
#!/bin/bash

set -x

rm -rf `pwd`/build/*
cd `pwd`/build &&
	cmake .. &&
	make
