
#include "des.h"

/* Function Definitions -----------------------------------------------------*/

bitset<48>* sub_key_generator(const bitset<64>& key) {
    uint64_t pc1_key = 0;

    bitset<56> permuted = permute<64, 56>(key, PC1), c_d;
    bitset<28> c[DES_ROUNDS + 1], d[DES_ROUNDS + 1];
    static bitset<48> subkeys[16];

    #ifdef DEBUG
        cout << "Key: " << key << endl;
        cout << "Permuted Key(PC-1): " << permuted << endl;
    #endif

    for (uint8_t i = 0; i < 28; ++i) {
        c[0][27 - i] = permuted[55 - i];
        d[0][27 - i] = permuted[27 - i];
    }

    #ifdef DEBUG
        cout << "C" << 0 << ": " << c[0] << endl;
        cout << "D" << 0 << ": " << d[0] << endl;
    #endif

    for (uint8_t i = 0; i < DES_ROUNDS; i++) {
        apply_iterations_left_shift(c[i], d[i], c[i + 1], d[i + 1], ITERATIONS_LEFT_SHIFT[i]);

        #ifdef DEBUG
            cout << "C" << (int)i + 1 << ": " << c[i + 1] << endl;
            cout << "D" << (int)i + 1 << ": " << d[i + 1] << endl;
        #endif
    }

    for (uint8_t i = 0; i < DES_ROUNDS; i++) {

        for (uint8_t j = 0; j < 28; j++) {
            c_d[55 - j] = c[i + 1][27 - j];
        }

        for (uint8_t j = 0; j < 28; j++) {
            c_d[27 - j] = d[i + 1][27 - j];
        }

        subkeys[i] = permute<56, 48>(c_d, PC2);

        #ifdef DEBUG
            cout << "Subkey " << (int)i + 1 << ": " << subkeys[i] << endl;
        #endif
    }
    return subkeys;
}

void apply_iterations_left_shift(bitset<28>& c_in, bitset<28>& d_in, bitset<28>& c_out, bitset<28>& d_out, const uint8_t number_of_shifts) {
    bitset<28> c_lost_bits = 0, d_lost_bits = 0;

    c_lost_bits = c_in >> (28 - number_of_shifts);
    d_lost_bits = d_in >> (28 - number_of_shifts);

    c_out = (c_in << number_of_shifts) | c_lost_bits;
    d_out = (d_in << number_of_shifts) | d_lost_bits;
}

bitset<32> function_f(const bitset<32>& r, const bitset<48>& k) {
    bitset<48> expanded_r, xor_result;
    bitset<6> b[8];
    bitset<4> sbox_result[8];
    bitset<2> row;
    bitset<4> column;
    bitset<32> sbox_result_concatenated;

    expanded_r = permute<32, 48>(r, E);

    xor_result = expanded_r ^ k;

    #ifdef DEBUG
        cout << "Expanded R: " << expanded_r << endl;
        cout << "XOR Result: " << xor_result << endl;
        print_binary(xor_result.to_ullong(), 48);
    #endif

    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 6; j++) {
            b[i][5 - j] = xor_result[47 - (((i * 6) + j))];
        }
        row[0] = b[i][0];
        row[1] = b[i][5];

        column[0] = b[i][1];
        column[1] = b[i][2];
        column[2] = b[i][3];
        column[3] = b[i][4];

        sbox_result[i] = S_BOXES[i][row.to_ullong()][column.to_ullong()];

        #ifdef DEBUG
            cout << "S" << (int)i + 1 << " Result: " << sbox_result[i] << endl;
            cout << "S" << (int)i + 1 << " Row: " << row.to_ullong() << endl;
            cout << "S" << (int)i + 1 << " Column: " << column.to_ullong() << endl;
            cout << "S" << (int)i + 1 << " Value: " << sbox_result[i].to_ullong() << endl;
        #endif
    }

    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            sbox_result_concatenated[31 - ((i * 4) + j)] = sbox_result[i][3 - j];
        }
    }

    sbox_result_concatenated = permute<32, 32>(sbox_result_concatenated, P);
    #ifdef DEBUG
        cout << "F function result: " << sbox_result_concatenated << endl;
    #endif

    return sbox_result_concatenated;
}


void print_binary(const uint64_t number, const uint8_t number_of_bits) {
    uint8_t bit_value = 0;
    for (uint8_t i = 0; i < number_of_bits; i++) {
        bit_value = ((number >> (number_of_bits - i - 1)) & 1);

        if (i % 6 == 0 && i != 0) {
            printf(" ");
        }
        printf("%i", bit_value);
    }
    printf("\n");
}
