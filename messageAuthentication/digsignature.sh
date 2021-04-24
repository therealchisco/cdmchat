#!/bin/bash
# Script to automate signature creation, key creation, and file verification 

# Comentarios: el file deberia tener un setuid de root para cambiarle el chmod a la private key, que solo root lo pueda ver y escribir


# Configuration variables
USER="hap"
PUBLIC_KEY_FILE="pubkey_${USER}.pem"
PRIVATE_KEY_FILE="privkey_${USER}.pem"
RSA_LENGTH=2048 # either 2048 or 4096
# 4096 is probably even post-quantum safe

keyCreation() {
# Generate key pair, in $PRIVATE_KEY_FILE are both private and public key
# Mac OS command: 
#openssl genpkey -out $PRIVATE_KEY_FILE -algorithm rsa $RSA_LENGTH
# Linux Command:
openssl genpkey -out $PRIVATE_KEY_FILE -outform PEM -algorithm rsa
# Only root can read and write on the $PRIVATE_KEY_FILE
chmod 600 $PRIVATE_KEY_FILE

# Extract the public key from the key pair file
openssl rsa -in $PRIVATE_KEY_FILE -outform PEM -pubout -out $PUBLIC_KEY_FILE
chmod 644 $PUBLIC_KEY_FILE

# TODO
# Send public key to server
# scp to payaserver public key
}

signFile(){
# Sign file
case $1 in
"sig")
local FILE=$2
# Store signature of file in ${SIG_FILE}.sign
openssl dgst -sha256 -sign $PRIVATE_KEY_FILE -out ${FILE}.sign $FILE
# Create token in base64 for each file
openssl enc -base64 -in ${FILE}.sign -out ${FILE}.token

;;
*) # all other cases
echo "usage: digsignature sig file"
;;
esac
}

# use this function to decode token from base 64 to binary
tokenbase64tobinary(){
openssl enc -base64 -d -in token -out sign.bin
}

authenticateFile(){
local SIG_FILE=$1
local FILE=$2
openssl dgst -sha256 -verify $PUBLIC_KEY_FILE -signature $SIG_FILE $FILE

#Further implementation:
# $?=1 if authentication fails
# $?=0 if verification OK 
# prompt of openssl dgst in stdout 
}

#keyCreation
# $@ to pass all arguments into the scope of function signFile
signFile $@
#authenticateFile $@

