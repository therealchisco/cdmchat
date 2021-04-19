#!/bin/zsh
# Description: A module to create all the cli "screens" and "windows" 
# author: cheto
# copyleft - 2021 CDM Labs 

#=============================================================================+
#                      Global variable declaration
#=============================================================================+
declare username
declare TITLE
declare current_date
declare channel
declare info_string
declare server
declare welcome_msg
declare ascii_monk
declare ascii_title

LOGO="assets/txt/ascii_monk_small"

welcome_msg=$(figlet 'welcome to cdmchat')

TITLE="Cac de Moun Chat"

TITLECARD="assets/txt/name_ascii"

#=============================================================================+
#                                Source libraries
#=============================================================================+

source libs/pretty_cli_lib.sh

#=============================================================================#
#                              Functions 		                              #
#=============================================================================#

function get_user()
{
	indent_text $@
	read username
}

function get_server()
{
	indent_text $@
	read server
}

function get_channel()
{
	indent_text $@
	read channel
}

function validate_username()
{
	get_user "Enter Username: "
	username_len=$( echo $username | wc -c)
	while [ $username_len -gt 8 ];
	do
		get_user "Enter Username (max length 8 characters): "
		username_len=$( echo $username | wc -c)
	done
}


#TODO: Add Regex to only accept valid IP addresses (and localhost)
function validate_server()
{
	get_server "Enter server (IP): "
	server_len=$( echo $server | wc -c)
	while [ $server_len -eq 8 ];
	do
		get_server "Enter Username (max length 8 characters): "
		server_len=$( echo $server | wc -c)
	done
}


#TODO: Add Regex to only acccept valid port format
function validate_channel()
{
	get_channel "Enter channel (Port): "
	channel_len=$( echo $channel | wc -c)
	while [ $channel_len -gt 5 ];
	do
		get_channel "Enter channel (max length 4 characters): "
		channel_len=$( echo $channel | wc -c)
	done
}


function detail_menu()
{
	indent_text "username: "
	printf "$username"
	print_newline
	indent_text "server: "
	printf "$server"
	print_newline
	indent_text "channel: "
	printf "$channel"
	print_newline
	print_separation
}


function main_menu()
{
	current_date=$(date +"%d. %B %Y %H:%M")
	reload_cli
	indent_text $current_date
	print_newline
	print_header $TITLE
	detail_menu
	print_newline
}

function startup_menu()
{
	main_menu
	validate_username
	main_menu
	validate_server
	main_menu
	validate_channel
}

function welcome_screen()
{	
	reload_cli
	print_header_upper "Cac de Moun Labs"
	center_file $LOGO
	print_header_upper " "
	sleep 1
	
	reload_cli
	print_header_upper "Cac de Moun Labs"
	center_file $LOGO
	print_header_upper "Presents:"
	sleep 1

	reload_cli
	print_newline
	print_newline
	center_figlet "CDMChat"
	sleep 1

	reload_cli
	print_newline
	print_newline
	center_figlet "Welcome!"
	sleep 1

}


function first_run()
{
	welcome_screen
	reload_cli
	startup_menu
	main_menu
}
