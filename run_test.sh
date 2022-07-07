#! /usr/bin/bash

#dex = gcc ./tests/test_Verifier/test_dataextract.c ./verifier/verifier.c -lcrypto -w
#hfn = gcc ./tests/test_Verifier/test_hashfunc.c ./verifier/verifier.c -lcrypto -w
#ver_mer = gcc ./tests/test_Verifier/test_ver_merkle_proof.c ./verifier/verifier.c -lcrypto -w
#test_pv = python3 ./tests/test_Prover
if [ "$1" = "dex" ]; then
    gcc ./tests/test_Verifier/test_dataextract.c ./verifier/verifier.c -lcrypto -w
    ./a.out
    rm a.out
fi

if [ "$1" = "hfn" ]; then
    gcc ./tests/test_Verifier/test_hashfunc.c ./verifier/verifier.c -lcrypto -w
    ./a.out
    rm a.out
fi

if [ "$1" = "ver_mer" ]; then
    gcc ./tests/test_Verifier/test_ver_merkle_proof.c ./verifier/verifier.c -lcrypto -w
    ./a.out
    rm a.out
fi

if [ "$1" = "test_pv" ]; then
    python3 run_test.py ./tests/test_Prover
fi
