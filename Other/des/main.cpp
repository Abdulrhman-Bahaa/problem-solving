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
    bitset<64> message("0000000100100011010001010110011110001001101010111100110111101111"), 
    key("0001001100110100010101110111100110011011101111001101111111110001");

    bitset<32> l[DES_ROUNDS], r[DES_ROUNDS];

    /* Step 1: Create 16 subkeys, each of which is 48-bits long. */
    bitset<48>* subkeys = sub_key_generator(key);

    bitset<64> r_16_l_16_concatenated;

    /* Step 2: Encode each 64-bit block of data. */
    cout << "Original Message: " << message << endl;

    bitset<64> permuted_message = permute<64, 64>(message, DES_IP);


    cout << "Permuted Message(IP): " << permuted_message << endl;

    for (uint8_t i = 0; i < 32; ++i) {
        l[0][31 - i] = permuted_message[63 - i];
        r[0][31 - i] = permuted_message[31 - i];
    }

    bitset<32> function_result;
    for (uint8_t i = 0; i < DES_ROUNDS; ++i) {
        function_result = function_f(r[i], subkeys[i]);
        l[i + 1] = r[i];
        r[i + 1] = l[i] ^ function_result;
    }

    for (uint8_t i = 0; i < 32; ++i) {
        r_16_l_16_concatenated[63 - i] = r[DES_ROUNDS][31 - i];
        r_16_l_16_concatenated[31 - i] = l[DES_ROUNDS][31 - i];
    }

    cout << r_16_l_16_concatenated << endl;

    /* Step 3: Permute the result using the inverse initial permutation (IP^-1). */
    bitset<64> final_result = permute<64, 64>(r_16_l_16_concatenated, DES_IP_INV);
    cout << "Final Result: " << final_result << endl;
    cout << "Final Result in Hex: " << hex << final_result.to_ullong() << endl;
    cout << "Final Result in Decimal: " << dec << final_result.to_ullong() << endl;
    cout << "Final Result in Binary: ";
    print_binary(final_result.to_ullong(), 64);
    cout << endl;
    return 0;
}