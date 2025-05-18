/**
 ******************************************************************************
 * \file       des.h
 * \author     Abdulrhman Bahaa
 * \brief      This source file contains declarations for des tables and functions
 * \date       2025-04-29
 ******************************************************************************
*/
#ifndef DES_H
#define	DES_H

/* Includes -----------------------------------------------------------------*/
#include <iostream>
#include <bitset>
#include <rang.hpp>

using namespace std;

/* Macro Declarations -------------------------------------------------------*/
#define DES_ROUNDS      16

/* Variables Declarations ---------------------------------------------------*/
extern const uint8_t PC1[64];
extern const uint8_t ITERATIONS_LEFT_SHIFT[16];
extern const uint8_t PC2[48];
extern const uint8_t IP[64];
extern const uint8_t IP_INV[64];
extern const uint8_t E[48];
extern const uint8_t S_BOXES[8][4][16];
extern const uint8_t P[32];

/* Functions Declarations ---------------------------------------------------*/
template<uint8_t input_size, uint8_t output_size>
bitset<output_size> permute(const bitset<input_size> input, const uint8_t table[output_size]) {
    bitset<output_size> output;
    for (uint8_t i = 0; i < output_size; i++) {
        output[output_size - i - 1] = input[input_size - table[i]];
    }
    return output;
}

bitset<48>* sub_key_generator(const bitset<64>& key, bool show_steps);
void apply_iterations_left_shift(bitset<28>& c_in, bitset<28>& d_in, bitset<28>& c_out, bitset<28>& d_out, const uint8_t number_of_shifts);

void print_binary(const uint64_t number, const uint8_t number_of_bits);

bitset<32> function_f(const bitset<32>& r, const bitset<48>& k, bool show_steps);

void print_block(const bitset<64>& plaintext, const bitset<64>& key, const bitset<64>& ciphertext) ;

#endif	/* DES_H */