/** 
 * \file    main.cpp
 * \brief   Implementation of the Data Encryption Standard (DES) algorithm.
 * \details This file contains the implementation of the DES algorithm, including key generation,
 *          encryption, and decryption functions. The DES algorithm is a symmetric-key block cipher
 *          that operates on 64-bit blocks of data using a 56-bit key. It is based on a Feistel network
 *          structure and consists of 16 rounds of permutation and substitution operations.
 * \author  Abdulrhman Bahaa
 * \date    2025-04-29
 * \warning    This implementation is for educational purposes only and should not be used in production systems.
 *          DES is considered weak by modern standards and should not be used for secure applications.
 * \see     https://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm
*/
#include "des.h"

/* Main Function ------------------------------------------------------------*/
int main() {
    bitset<64> plaintext("0000000100100011010001010110011110001001101010111100110111101111"), 
    key("0001001100110100010101110111100110011011101111001101111111110001"),
    ciphertext ,r_16_l_16_concatenated;

    bitset<32> l[DES_ROUNDS], r[DES_ROUNDS], function_f_result;

    /* Step 1: Create 16 subkeys, each of which is 48-bits long. */
    bitset<48>* subkeys = sub_key_generator(key);

    /* Step 2: Encode each 64-bit block of data. */
    /* Apply the initial permutation (IP) to the message.*/ 
    bitset<64> permuted_plaintext = permute<64, 64>(plaintext, DES_IP);

    /* Split the permuted message into two halves: left (l) and right (r). */
    for (uint8_t i = 0; i < 32; ++i) {
        l[0][31 - i] = permuted_plaintext[63 - i];
        r[0][31 - i] = permuted_plaintext[31 - i];
    }

    /* Apply the 16 rounds of the DES algorithm. */
    for (uint8_t i = 0; i < DES_ROUNDS; i++) {

        #ifdef DEBUG
            cout << "\n------------------- Round " << (int)i + 1 << " -----------------" << endl;
            cout << "key " << (int)i + 1 << ": " << subkeys[i];
        #endif

        function_f_result = function_f(r[i], subkeys[i]);
        l[i + 1] = r[i];
        r[i + 1] = l[i] ^ function_f_result;    /* R1 = L0 + f(R0,K1)  */

        #ifdef DEBUG
            cout << "L" << (int)i + 1 << ": " << l[i + 1] << " | R" << (int)i + 1 << ": " << r[i + 1] << endl;
        #endif
    }

    /* Concatenate the last round's right and left halves. */
    /* The left half (l) is placed in the lower 32 bits, and the right half (r) is placed in the upper 32 bits. */
    for (uint8_t i = 0; i < 32; i++) {
        r_16_l_16_concatenated[63 - i] = r[DES_ROUNDS][31 - i];
        r_16_l_16_concatenated[31 - i] = l[DES_ROUNDS][31 - i];
    }

    /* Apply the final permutation (IP-1) to the concatenated result. */
    ciphertext = permute<64, 64>(r_16_l_16_concatenated, DES_IP_INV);

    /* Print the block. */
    print_block(plaintext, key, ciphertext);

    return 0;
}

