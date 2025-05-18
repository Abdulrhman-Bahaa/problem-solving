# DES Encryption Project

## Overview
This project implements the Data Encryption Standard (DES) algorithm, a symmetric-key block cipher that operates on 64-bit blocks of data using a 56-bit key. The implementation includes key generation, encryption, and decryption functions based on the Feistel network structure.

![Video](https://github.com/user-attachments/assets/00b35228-2432-4a42-9e11-f4689509ee99)

## Files

- **main.cpp**: Contains the main function that implements the DES algorithm. It handles user input, validates message and key formats, generates subkeys, and performs the encryption process.

- **des.cpp**: Implements the core functions of the DES algorithm, including subkey generation, the function f (which applies S-boxes and permutations), and left shift operations for key scheduling.

- **des.h**: Header file that contains declarations for the DES tables and functions. It includes necessary includes, macro definitions, and function prototypes used in the DES implementation.

- **des_tables.cpp**: Defines the various tables used in the DES algorithm, including the PC1, PC2, IP, IP_INV, E, S-boxes, and P tables.

## Usage
To compile and run the project, use the following commands:

```bash
g++ main.cpp des.cpp des_tables.cpp -o des_encryption
./des_encryption <message> <key> [--show-steps]
```

### Arguments
- `<message>`: The 64-bit message to be encrypted, provided in hexadecimal (0x), decimal (0d), or binary (0b) format.
- `<key>`: The 64-bit key used for encryption, provided in the same format as the message.
- `--show-steps` (optional): Enables debug mode to display intermediate steps of the encryption process.

## Note
This implementation is for educational purposes only and should not be used in production systems. DES is considered weak by modern standards and is not recommended for secure applications.