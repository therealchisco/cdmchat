# CDM Labs Chat Service

## Description

> A simple and mostly useless _in any real world scenario_ tool to chat with your *bois* (bois who happen to have access and be logged into the same ssh server as you)

Reqs:

* figlet
* netcat
* tput
* tmux (not necesarry but recommended) 

Steps:

### Server Sidea (proper script coming soon)

* Create a directory that all your boys should have access to
* cd into it and ```touch chat_log```
* Select a port (channel) e.g: 5213
* run ```ncat -l -k 5213 >> chat_log &``` 

### Client Side
* clone repo 
* run cdmchat
* enter, username, ip, and port
* enjoy the bantz
