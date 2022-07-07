#include "test_ver_merkle_proof.h"
#include "../../verifier/verifier.h"
/* Compile : gcc ./tests/test_Verifier/test_ver_merkle_proof.c ./verifier/verifier.c -lcrypto -w */


int len = 5;
int ind = 4;

byte root[32] = {
    128, 160, 216, 61, 197, 157, 184, 216, 155,
    85, 240, 159, 61, 249, 93, 248, 182, 220,
    133, 25, 25, 40, 53, 255, 183, 187, 223,
    154, 75, 214, 170, 65
};

byte leaf[32] = {
    178, 117, 192, 38, 56, 195, 246, 217, 147,
    143, 106, 202, 72, 90, 216, 108, 61, 57, 
    246, 59, 192, 248, 117, 157, 97, 82, 179,
    89, 218, 65, 40, 39
};

byte proof[1][32] = {
    {
        154, 248, 212, 107, 171, 112, 76, 47, 177,
        30, 33, 247, 195, 157, 58, 117, 1, 246, 43,
        107, 239, 83, 234, 96, 251, 41, 102, 63, 35,
        62, 189, 217
    }
};

int proof_size = 1;

TEST test_ver_merkle_proof(void) {
    int x = verify_merkle_proof(root, len, ind, leaf, proof, proof_size);

    for(int i=0;i<10;i++){
        /* Compare, with an automatic "1 != x" failure message */
        ASSERT_EQ(1, x);

        /* Compare, with a custom failure message */
        ASSERT_EQm("OOPs", 1, x);

        /* Compare, and if they differ, print both values,
        * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
        ASSERT_EQ_FMT(1, x, "%d");
    }

    PASS();
}

/* Suites can group multiple tests with common setup. 
   Grouping the following tests:
   1) test_ver_merkle_proof

   Only one test function has been defined but it is a better
   pratcise to execute the tests through SUITE since in future
   more new test functions may have to be defined to make the testing
   more accurate.
*/
SUITE(the_suite) {
    RUN_TEST(test_ver_merkle_proof);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    /* Individual tests can be run directly in main, outside of suites. */
    /* Eg - RUN_TEST(x_should_equal_1); */

    /* Tests can also be gathered into test suites. 
       Since we have grouped tests therfore running them using 
       RUN_SUITE
    */
    RUN_SUITE(the_suite);

    GREATEST_MAIN_END();        /* display results */
}