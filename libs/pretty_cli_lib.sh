#!/bin/zsh
# Filename: pretty_cli_lib.sh
# Description: Functions to pretify CLIs
# authors: chesto


# Declaring global variables

declare terminal_width
declare terminal_height

# $(tput cols) => Print the number of columns for the current terminal
terminal_width=$(tput cols)

#=============================================================================#
#                              Functions 		                              #
#=============================================================================#

# prints f without linebreak n times (n=$1)
function printf_n()
{
	for i in {1..$1};
	do
		# Understanding ${@:2} see:
		# https://unix.stackexchange.com/questions/92978/what-does-this-2-mean-in-shell-scripting
		# Print all inputs to function starting from the second input (since the first input equals
		# n the amount of repetitions of the print funcion)
		printf ${@:2}
	done
}

function reload_cli()
{
	clear
	# We get the "window" size
	terminal_width=$(tput cols)
	terminal_height=$(tput lines)
	# https://linux.die.net/man/1/tput
}


function print_newline()
{
	printf '\n'
}

function print_separation()
{
	# print "="s for all the terminal_width
	# use the previously described function printf_n (see above)
	printf_n $terminal_width '='
	print_newline
}

function upper_case()
{
   if [ $# -ge 1 ]
   then
     echo $@ | tr '[a-z]' '[A-Z]'
   fi
}

#centers text in current_width of terminal window cols
function center_text()
{
	text=$@
	char_count=$( echo $text | wc -c)
	cols_rest=$(( $terminal_width - $char_count ))
	n=$(( $cols_rest / 2 ))
	printf_n $n " "
	echo $text
}

function center_upper()
{
	text=$@
	char_count=$( echo $text | wc -c)
	cols_rest=$(( $terminal_width - $char_count ))
	n=$(( $cols_rest / 2 ))
	printf_n $n " "
	upper_case $text
}


# Goes line by line centering to current term_width
# this is useful for ascii art assets
function center_file()
{
	File=$1
	cat $File | while read y
	do
		center_text $y
	done	
}

function center_figlet()
{
	figlet -w $terminal_width -c $@
}


function indent_text()
{
	printf '\t'
	printf $@
}

function print_header()
{
	print_separation
	center_text $@
	print_separation
}


function print_header_upper()
{
	print_separation
	center_upper $@
	print_separation
}


function print_subtitle()
{
	print_separation
	indent_text $@
	print_separation
}
