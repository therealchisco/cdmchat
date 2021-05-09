# Message authentication with OpenSSL
A quick summary of general OpenSSL commands we have found useful, a step by step guide to create a digital signature using RSA and useful links about using OpenSSL.

## General OpenSSL commands

* ```openssl genrsa``` : generate RSA private key, and output key to stdout
* ```openssl dgst -sha256 file``` : generate the SHA256 hash of the contents of `file`

## Digital signature with RSA and SHA256

### Client side
1. **Create a key pair** : ```openssl genpkey -out privkey.pem -algorithm rsa 2048``` 
Create an RSA key pair stored in file `privkey.pem` (file name is arbitrary). Privacy Enhanced Mail (PEM) is a customary extension. Size of RSA key can be 2048 or 4096 bits. **IMPORTANT: NEVER EVER share the file `privkey.pem`, since it contains your private key. It should be stored locally in a safe manner!!**

2. **Extract the public key from the key pair** : ```openssl rsa -in privkey.pem -outform PEM -pubout -out pubkey.pem``` 
Extract the public key from the `privkey.pem` key pair file and store it in `pubkey.pem`. The public key file `pubkey.pem` can be shared with other systems.

3. **Sign a file with the private key** : ```openssl dgst -sha256 -sign privkey.pem -out sign.sha256 file```
Use the private key to sign the SHA256 output of a file (the output is a **binary**)
	- In order to translate the output to **base64** use the following command ```openssl enc -base64 -in sign.sha256 -out sign.sha256.base64```

### Server side
4. **Use the public key to verify the authenticity of the signed file** : ```openssl dgst -sha256 -verify pubkey.pem -signature sign.sha256 file``` 
Verify the authenticity of a file using a public key
	- If the signature is in base64, first translate it to a binary with the command ```openssl enc -base64 -d -in sign.sha256.base64 -out sign.sha256```

## Helpful links
* Overview about [Digital signatures](https://opensource.com/article/19/6/cryptography-basics-openssl-part-2)
