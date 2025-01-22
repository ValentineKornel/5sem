#!/bin/bash

if [ -z "$1" ]; then
	echo "Error: wasn't given a name."
	exit 1
fi

current_date=$(date +"%d.%m.%y")

echo "Hello, $1! Today is $current_date."
