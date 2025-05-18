/** 
 * \file    main.cpp
 * \brief   Implementation of the Data Encryption Standard (DES) algorithm.
 * \details This file contains the implementation of the DES algorithm, including key generation,
 *          encryption, and decryption functions. The DES algorithm is a symmetric-key block cipher
 *          that operates on 64-bit blocks of data using a 56-bit key. It is based on a Feistel network
 *          structure and consists of 16 rounds of permutation and substitution operations.
 * \author  Abdulrhman Bahaa
 * \date    2025-04-29
 * \warning This implementation is for educational purposes only and should not be used in production systems.
 *          DES is considered weak by modern standards and should not be used for secure applications.
 * \see     https://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm
*/
#include "des.h"

/* Main Function ------------------------------------------------------------*/
int main(int argc, char* argv[]) {

    /* Variable Declarations */
    bool show_steps = false;
    bitset<64> message, key, ciphertext, r_16_l_16_concatenated, ip_permuted;
    bitset<48>* subkeys = nullptr;
    bitset<32> l[DES_ROUNDS], r[DES_ROUNDS], function_f_result;

    /* Check the number of arguments and their validity. */
    if (argc != 3 && argc != 4) {
        cout << rang::fg::red << "Error: Invalid number of arguments." << rang::style::reset << endl
             << "Usage: " << argv[0] << " <message>" << " <key>" << " --show-steps(optional, default: false)" << endl;
        return 1;
    }
    else {
        if (argc == 4) {
            if (string(argv[3]) == "--show-steps") {
                show_steps = true;
                cout << "Debug mode enabled." << endl;
            }
            else {
                cout << rang::fg::red << "Error: Invalid flag." << rang::style::reset << endl
                     << "Usage: " << argv[0] << " <message>" << " <key>" << " --show-steps(optional, default: false)" << endl;
                return 1;
            }
        }

        switch(argv[1][1]) {
            case 'x':
                message = bitset<64>(stoull(argv[1] + 2, nullptr, 16));
                key = bitset<64>(stoull(argv[2] + 2, nullptr, 16));
                break;
            case 'd':
                message = bitset<64>(stoull(argv[1] + 2, nullptr, 10));
                key = bitset<64>(stoull(argv[2] + 2, nullptr, 10));
                break;
            case 'b':
                message = bitset<64>(stoull(argv[1] + 2, nullptr, 2));
                key = bitset<64>(stoull(argv[2] + 2, nullptr, 2));
                break;
            default:
                cout << rang::fg::red << "Error: Invalid message/key format. Use 0x, 0d or 0b." << rang::style::reset << endl;
                return 1;
        }

        if (message.size() != 64 || key.size() != 64) {
            cout << rang::fg::red << "Error: Message and key must be 64 bits long." << rang::style::reset << endl;
            return 1;
        }
    }

    /* Step 1: Create 16 subkeys, each of which is 48-bits long. */
    subkeys = sub_key_generator(key, show_steps);

    /* Step 2: Encode each 64-bit block of data. */
    /* Apply the initial permutation (IP) to the message.*/ 
    ip_permuted = permute<64, 64>(message, IP);

    /* Split the permuted message into two halves: left (l) and right (r). */
    for (uint8_t i = 0; i < 32; ++i) {
        l[0][31 - i] = ip_permuted[63 - i];
        r[0][31 - i] = ip_permuted[31 - i];
    }

    /* Apply the 16 rounds of the DES algorithm. */
    for (uint8_t i = 0; i < DES_ROUNDS; i++) {

        /* Print Debug Information if show_steps is enabled. */
        if (show_steps) {
            cout << rang::fg::cyan << "\n------------------- Round " << (int)i + 1 << " -----------------" << rang::style::reset << endl
                 << "Subkey " << (int)i + 1 << ": " << subkeys[i] << endl;
        }

        function_f_result = function_f(r[i], subkeys[i], show_steps);
        l[i + 1] = r[i];
        r[i + 1] = l[i] ^ function_f_result;    /* R1 = L0 + f(R0,K1)  */

        if (show_steps) {
            cout << "f(R" << (int)i + 1 << ", K" << (int)i + 1 << ") = " << function_f_result << endl
                 << "R" << (int)i + 1 << " = " << "L" << (int)i << " + f(R" << (int)i << ", K" << (int)i + 1 << ") = " << r[i + 1] << endl
                 << "L" << (int)i + 1 << " = " <<  "R" << (int)i << " = " << l[i + 1] << endl;

        }
    }

    /* Concatenate the last round's right and left halves. */
    /* The left half (l) is placed in the lower 32 bits, and the right half (r) is placed in the upper 32 bits. */
    for (uint8_t i = 0; i < 32; i++) {
        r_16_l_16_concatenated[63 - i] = r[DES_ROUNDS][31 - i];
        r_16_l_16_concatenated[31 - i] = l[DES_ROUNDS][31 - i];
    }

    /* Apply the final permutation (IP-1) to the concatenated result. */
    ciphertext = permute<64, 64>(r_16_l_16_concatenated, IP_INV);

    /* Print the block. */
    print_block(message, key, ciphertext);

    return 0;
}

