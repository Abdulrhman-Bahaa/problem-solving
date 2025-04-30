/** 
 * \file    des.cpp
 * \brief   Implementation of the Data Encryption Standard (DES) algorithm.
 * \details This file contains the implementation of the DES algorithm, including key generation,
 *          encryption, and decryption functions. The DES algorithm is a symmetric-key block cipher
 *          that operates on 64-bit blocks of data using a 56-bit key. It is based on a Feistel network
 *          structure and consists of 16 rounds of permutation and substitution operations.
 * \author  Abdulrhman Bahaa
 * \date    2025-04-29
 * \version 1.0
 * \note    This implementation is for educational purposes only and should not be used in production systems.
 * \warning DES is considered weak by modern standards and should not be used for secure applications.
 * \see     https://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm
*/

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

/* Macros ------------------------------------------------------------------*/
#define DES_BLOCK_SIZE        64
#define DES_INITIAL_KEY_SIZE  64
#define DES_PC1_KEY_SIZE      56



#define DES_KEY_SIZE    8
#define DES_ROUNDS      16
#define DES_KEY_BITS    56
#define DES_BLOCK_BITS  64
#define DES_SUBKEY_BITS 48


/* Data Types --------------------------------------------------------------*/
union pc1_key {
    uint64_t pc1_key;
    struct des_key {
        uint32_t d0 : 28;
        uint32_t c0 : 28;
    } key; 
};

/* Functions Declarations ---------------------------------------------------*/
void print_binary(const uint64_t number, const uint8_t number_of_bits);
void sub_key_generator(const uint64_t initial_key);
void apply_iterations_left_shift(const uint64_t pc1_key);
uint64_t apply_pc1(const uint64_t initial_key);
uint64_t circular_shift_left(uint64_t number, const uint8_t number_of_shifts ,const uint8_t number_of_bits);

/* Variables Definitions ----------------------------------------------------*/ 
const uint8_t PC1[DES_PC1_KEY_SIZE] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

uint8_t iterations_left_shift[DES_ROUNDS] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

/* Main Function ------------------------------------------------------------*/
int main() {
    uint64_t message_block = 0x123456ABCD132536;
    uint64_t initial_key = 0x133457799BBCDFF1;

    /* Sub-keys generation */
    sub_key_generator(initial_key);

    return 0;
}

/* Function Definitions -----------------------------------------------------*/
void sub_key_generator(const uint64_t initial_key) {
    uint64_t pc1_key = 0;

    pc1_key = apply_pc1(initial_key);
    print_binary(pc1_key, 64);
    apply_iterations_left_shift(pc1_key);
}

uint64_t apply_pc1(const uint64_t initial_key) {
    uint64_t pc1_key = 0, initial_key_bit_value = 0;
    uint8_t initial_key_bit_pos = 0;
    for (uint8_t pc1_key_bit_pos = 1; pc1_key_bit_pos < DES_PC1_KEY_SIZE + 1; pc1_key_bit_pos++) {
        initial_key_bit_pos = PC1[pc1_key_bit_pos - 1];
        initial_key_bit_value = ((initial_key >> (DES_INITIAL_KEY_SIZE - initial_key_bit_pos)) & 1);
        pc1_key = (pc1_key | (initial_key_bit_value << (DES_INITIAL_KEY_SIZE - pc1_key_bit_pos)));
    }
    return pc1_key;
}

void apply_iterations_left_shift(const uint64_t pc1_key) {
    uint64_t c0 = 0, d0 = 0;
    d0 = (pc1_key << 28); // 28 bits
    c0 = pc1_key & 0xFFFFFFF000000000; // 28 bits

    uint64_t c1 = circular_shift_left(c0, 1, 28);
    print_binary(c0, 64);
    print_binary(d0, 64);
    print_binary(c1, 64);
}


void print_binary(const uint64_t number, const uint8_t number_of_bits) {
    uint8_t bit_value = 0;
    for (uint8_t i = 0; i < number_of_bits; i++) {
        bit_value = ((number >> (number_of_bits - i - 1)) & 1);

        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
        printf("%i", bit_value);
    }
    printf("\n");
}

uint64_t circular_shift_left(uint64_t number, const uint8_t number_of_shifts ,const uint8_t number_of_bits) {
    uint64_t shifted_value = 0, last_bits = 0;
    last_bits = number >> ((number_of_bits / number_of_shifts) - 1);
    shifted_value = (number << number_of_shifts) | last_bits;
    shifted_value &= 0xfffffff000000000;
    return shifted_value;
}
