#ifndef SERVER_H
#define SERVER_H

#define byte unsigned char

void parse_get(char buffer[], int *len, int *ind, int *proof_size, byte root[], byte leaf[], byte proof[][32]);

void parse_set(char buffer[], int *len, int *ind, int *proof_size, byte old_root[], byte old_leaf[], byte new_root[], byte new_leaf[], byte proof[][32]);

void server();

#endif