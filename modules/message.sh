#!/bin/zsh
# Description: A module to handle retrieve, process, format and transfer msgs
# author: cheto
# copyleft - 2021 CDM Labs 

#=============================================================================#
#                         Initilize "libraries"                               #
#=============================================================================#

source libs/pretty_cli_lib.sh

#=============================================================================#
#                         Global variables		                              #
#=============================================================================#

declare message
declare timestamp
TEMPLATE="assets/templates/message1"

#=============================================================================#
#                              Functions 		                              #
#=============================================================================#

function get_message()
{
	indent_text "Message: "
	read message
	timestamp=$(date +"%H:%M:%S")
	sleep 3
}

function format_message()
{
	sed -e "s=\$username=$username=" -e "s=\$timestamp=$timestamp=" -e "s=\$message=$message=" $TEMPLATE
}

function send_message()
{
	get_message
	format_message | fold -w 50 | ncat $server $channel
	#TODO Handle error exit code, and inform user e.g: server inaccesible
}
