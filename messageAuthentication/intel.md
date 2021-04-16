# Intel about message authentication

## OpenSSL

* ```openssl genrsa``` : generate RSA private key, and output key to stdout
* ```openssl dgst -sha256 file``` : generate the SHA256 hash of the contents of file

## Steps

1. **Create the key pair** : ```openssl genpkey -out privkey.pem -algorithm rsa 2048``` : create rsa key pair. File name "privkey.pem" is arbitrary, Privacy Enhanced Mail (PEM) is a customary extension (size can be 2048 or 4096)

2. **Extract the public key from the key pair** : ```openssl rsa -in privkey.pem -outform PEM -pubout -out pubkey.pem``` : extract the public key from the privkey.pem key pair file and store it in pubkey.pem

3. **Sign the file with the private key** : ```openssl dgst -sha256 -sign privkey.pem -out sign.sha256 file``` : use the private key to sign the SHA256 output of a file (the output is a binary) - client side
	- In order to translate the output to base64 use the following command ```openssl enc -base64 -in sign.sha256 -out sign.sha256.base64```

4. **Use the public key to verify the authenticity of the signed file** : ```openssl dgst -sha256 -verify pubkey.pem -signature sign.sha256 file``` verify authenticity of file using public key (server side)
	- If the signature is in base64, first translate it to a binary with the command ```openssl enc -base64 -d -in sign.sha256.base64 -out sign.sha256```

### Links
* Overview about [Digital signatures](https://opensource.com/article/19/6/cryptography-basics-openssl-part-2)
