#!/bin/bash

export LANG=en_US.UTF-8

checkprocess()
{
	if [ $# != 2 ]
	then
		echo "parameter is not enough"
		return 2
	fi

	program=`ps -ef | grep $1 | grep -v grep | awk '{print $8,$9}' | sort -u`

	if [ "${program}" = "" ]
	then
		return 0
	else
		i=0
		for list in $program
		do
			processInfo[$i]=$list

			i=`expr $i + 1`

			if [ $i = 2 ]
			then
				if [ "${processInfo[0]}" = "$1" -a "${processInfo[1]}" = "$2" ]
				then
					return 1
				fi

				i=0
			fi
		done
	fi

	return 0
}

cd /home/byco/ZhihuSpider/ZhihuSpider/

if [ $# = 1 ];
then
	OP_TYPE=$1
else
	echo "Parameter error!"
	chmod 777 *
	./pull_zhihu
	exit
fi

checkprocess "./pull_zhihu" $OP_TYPE
check_result=$?
if [ "$check_result" == 0 ]
then 
	chmod 777 *
	./pull_zhihu $OP_TYPE
	
	echo "finish!"
fi
