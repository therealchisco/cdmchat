# CDM Labs Chat Service

## Description

> A simple and mostly useless _in any real world scenario_ tool to chat with your *bois* (bois who happen to have access and be logged into the same ssh server as you)

Reqs:

* figlet
* ncat (preferrably with ssl, so install nmap)
* tput
* tmux (not necesarry but recommended) 

Steps:

### Server Side (proper script coming soon)

* Create a directory that all your boys should have access to
* cd into it and ```touch chat_log```
* Select a port (channel) e.g: 5213
* ~~run ```ncat -l -k 5213 >> chat_log &```~~

### Client Side
* clone repo 
* run cdmchat
* enter, username, ip, and port
* enjoy the bantz

# Message authentication
* Branch _crypto_ is used as a playground to create an asymmetric cryptography message authentication system for the cdmchat system. 
	* The crypto playground can be found inside the *messageAuthentication* folder

# TODO

* [ ] create config file that is created automatically, e.g: uname to check OS
* [ ] How to properly use flags (etlsp) digsignature correct file

