#ifndef FINAL_VERIFIER_H
#define FINAL_VERIFIER_H

#define byte unsigned char

void hexstr_to_data(char *hex_str , byte byte_data[]);

void final_processed_data(char hex_data[][65] , int size , byte final_data[][32]);

int verify_merkle_proof(byte root[] , int len , int index , byte leaf[] , byte proof[][32] ,int proof_size);

#endif

