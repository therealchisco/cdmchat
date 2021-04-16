#!/bin/bash
# Script to automate signature creation, key creation, and file verification 

# Comentarios: el file deberia tener un setuid de root para cambiarle el chmod a la private key, que solo root lo pueda ver y escribir


# Configuration variables
USER="hap"
PUBLIC_KEY_FILE="pubkey_${USER}.pem"
PRIVATE_KEY_FILE="privkey_${USER}.pem"
RSA_LENGTH=4096 # either 2048 or 4096
# 4096 is probably even post-quantum safe

keyCreation() {
# Generate key pair, in $PRIVATE_KEY_FILE are both private and public key
openssl genpkey -out $PRIVATE_KEY_FILE -algorithm rsa $RSA_LENGTH

# Only root can read and write on the $PRIVATE_KEY_FILE
chmod 500 $PRIVATE_KEY_FILE

# Extract the public key from the key pair file
openssl rsa -in $PRIVATE_KEY_FILE -outform PEM -pubout -out 
chmod 511 $PUBLIC_KEY_FILE

}
