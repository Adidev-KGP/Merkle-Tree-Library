#ifndef FINAL_VERIFIER_H
#define FINAL_VERIFIER_H

#define byte unsigned char

int hexstr_to_num(char str[]);

void hexstr_to_data(char *hex_str, byte byte_data[]);

void final_processed_data(char hex_data[][65], int size, byte final_data[][32]);

void sha256(byte data[], byte sha256_hash[], int n);

void element_hash(byte data[], byte ele_hash[], int n);

int verify_merkle_proof_leaves_power_of_2(byte root[], int len, int index, byte leaf[], byte proof[][32], int proof_size);

int verify_merkle_right_branch(char dir, byte root[], int index, byte leaf[], byte proof[][32], int proof_size);

int verify_merkle_proof(byte root[], int len, int index, byte leaf[], byte proof[][32], int proof_size);

#endif

