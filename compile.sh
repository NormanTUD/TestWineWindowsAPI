#!/bin/bash

set -x

if [[ ! -z $1 ]]; then
	if [[ -e $1 ]]; then
		if [[ -e a.exe ]]; then
			rm a.exe
		fi

		i686-w64-mingw32-g++ --debug -static-libgcc -lopengl32 -lgdi32 -lkernel32 -luser32 -lcomdlg32 $1

		if [[ -e a.exe ]]; then
			wine a.exe
		else
			echo "a.exe not found"
			exit 2
		fi
	else
		echo "$1 not found"
	fi
else
	echo "No parameter given"
	exit 1
fi
set +x
