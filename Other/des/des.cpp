
#include "des.h"

/* Function Definitions -----------------------------------------------------*/
bitset<48>* sub_key_generator(const bitset<64>& key, bool show_steps) {

    /* The PC-1 table is used to permute the key bits before splitting it into two halves. */
    bitset<56> pc1_permuted, c_d;
    bitset<28> c[DES_ROUNDS + 1], d[DES_ROUNDS + 1];
    static bitset<48> subkeys[16];

    /* The PC-1 table is used to permute the key bits before splitting it into two halves. */
    pc1_permuted = permute<64, 56>(key, PC1);

    /* Split the permuted key into two halves: C and D. */
    for (uint8_t i = 0; i < 28; ++i) {
        c[0][27 - i] = pc1_permuted[55 - i];
        d[0][27 - i] = pc1_permuted[27 - i];
    }

    if (show_steps) {
        cout << "\n------------------ Sub Keys Generation -----------------" << endl
             << "\nKey: " << key << endl
             << "Permuted Key(PC-1): " << pc1_permuted << endl
             << "\nSplit into C and D, and apply left shifts:"
             << "\nC0: " << c[0] << "  D0: " << d[0] << endl;
    }

    /* Apply the left shifts and generate the subkeys. */
    for (uint8_t i = 0; i < DES_ROUNDS; i++) {
        apply_iterations_left_shift(c[i], d[i], c[i + 1], d[i + 1], ITERATIONS_LEFT_SHIFT[i]);

        if (show_steps) {
            cout << "C" << (int)i + 1 << ": " << c[i + 1] << "  D" << (int)i + 1 << ": " << d[i + 1] << endl;
        }
    }

    if (show_steps) {
        cout << "\nConcatenate C and D and apply PC-2: " << endl;
    }

    /* The PC-2 table is used to permute the combined C and D halves into the final subkey. */
    for (uint8_t i = 0; i < DES_ROUNDS; i++) {

        for (uint8_t j = 0; j < 28; j++) {
            c_d[55 - j] = c[i + 1][27 - j];
        }

        for (uint8_t j = 0; j < 28; j++) {
            c_d[27 - j] = d[i + 1][27 - j];
        }

        /* Apply the PC-2 permutation to the combined C and D halves. */
        subkeys[i] = permute<56, 48>(c_d, PC2);

        if (show_steps) {
            cout << "Subkey " << (int)i + 1 << ": " << subkeys[i] << endl;
        }
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

bitset<32> function_f(const bitset<32>& r, const bitset<48>& k, bool show_steps) {
    bitset<48> expanded_r, xor_result;
    bitset<6> b[8];
    bitset<4> sbox_result[8];
    bitset<2> row;
    bitset<4> column;
    bitset<32> sbox_result_concatenated;

    expanded_r = permute<32, 48>(r, E);

    xor_result = expanded_r ^ k;

    if (show_steps) {
        cout << rang::fg::magenta << "\n---------------- Function f --------------" << rang::style::reset << endl
             << "Expanded R: " << expanded_r << endl
             << "XOR with K: " << xor_result << endl;
    }

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

        if (show_steps) {
            cout << "S" << (int)i + 1 << " Row: " << row.to_ullong() << " Column: " << column.to_ullong() << 
            " Value: " << sbox_result[i] << endl;
        }
    }

    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            sbox_result_concatenated[31 - ((i * 4) + j)] = sbox_result[i][3 - j];
        }
    }

    if (show_steps) {
        cout << "\nApply the P permutation to the concatenated S-Box result: " << endl;
    }

    /* Apply the P permutation to the concatenated S-Box result. */

    sbox_result_concatenated = permute<32, 32>(sbox_result_concatenated, P);

    return sbox_result_concatenated;
}

void print_binary(const uint64_t number, const uint8_t number_of_bits) {
    for (int i = number_of_bits - 1; i >= 0; --i) {
        cout << ((number >> i) & 1);
        if (i % 4 == 0) {
            cout << " ";
        }
    }
    cout << endl;
}


void print_block(const bitset<64>& plaintext, const bitset<64>& key, const bitset<64>& ciphertext) {
    cout << rang::fg::yellow << "\n================ DES Encryption =================" << rang::style::reset << endl;

    cout << "\n" << rang::fg::magenta << ">>> Input:" << rang::style::reset << endl;

    cout << rang::fg::cyan << "Plaintext : " << rang::style::reset;
    print_binary(plaintext.to_ullong(), 64);
    cout << rang::fg::green <<"\t    0x"
         << uppercase << hex << plaintext.to_ullong()
         << rang::style::reset << dec << endl << endl;

    cout << rang::fg::cyan << "Key       : " << rang::style::reset;
    print_binary(key.to_ullong(), 64);
    cout << rang::fg::green <<"\t    0x"
        << uppercase << hex << key.to_ullong()
        << rang::style::reset << dec << endl << endl;

    cout << rang::fg::magenta << "\n>>> Output:" << rang::style::reset << endl;

    cout << rang::fg::cyan << "Ciphertext: " << rang::style::reset;
    print_binary(ciphertext.to_ullong(), 64);
    cout << rang::fg::green <<"\t    0x"
        << uppercase << hex << ciphertext.to_ullong()
        << rang::style::reset << dec << endl << endl;

    cout << rang::fg::yellow << "\n=================================================\n" << rang::style::reset << endl;
}
