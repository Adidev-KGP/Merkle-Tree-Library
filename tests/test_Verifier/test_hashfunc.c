#include "test_hashfunc.h"
#include "../../verifier/verifier.h"

/* Compile : gcc ./tests/test_Verifier/test_hashfunc.c ./verifier/verifier.c -lcrypto -w */

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST test_SHA256(void) {
    byte data[2][4] = {
        {108, 101, 102, 116},
        {98, 97, 99, 107}
    };

    byte data_hash[2][32];

    byte data_hash_check[2][32] = {
        {
         54, 15, 132, 3, 89, 66, 36, 60, 106, 54,
         83, 122, 226, 248, 103, 52, 133, 230, 192,
         68, 85, 160, 168, 90, 13, 177, 150, 144,
         242, 84, 20, 128
        },
        {
         60, 72, 35, 70, 243, 117, 2, 118, 119,
         250, 138, 13, 104, 48, 163, 39, 20,
         212, 241, 63, 158, 148, 194, 217,
         226, 21, 224, 172, 32, 90, 212, 229
        }
    };

    for(int i = 0; i < 2; i++){
        sha256(data[i], data_hash[i], 4);
        for(int j = 0; j < 32; j++){
            /* Compare, with an automatic "data_hash_check[i][j] != data_hash[i][j]" failure message */
            ASSERT_EQ(data_hash_check[i][j], data_hash[i][j]);

            /* Compare, with a custom failure message */
            ASSERT_EQm("OOPs", data_hash_check[i][j], data_hash[i][j]);

            /* Compare, and if they differ, print both values,
            * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
            ASSERT_EQ_FMT(data_hash_check[i][j], data_hash[i][j], "%d");
        }
    }

    PASS();
}

TEST test_element_hash(void) {
    
    byte ele[2][4] = {
        {108, 101, 102, 116},
        {98, 97, 99, 107}
    };

    byte ele_hash[2][32];

    byte ele_hash_check[2][32] = {
        {
         1, 150, 43, 20, 135, 222, 69, 169, 213,
         118, 34, 23, 48, 209, 107, 113, 169, 
         202, 79, 65, 93, 13, 192, 159, 23, 200, 
         41, 36, 22, 157, 86, 153
        },
        {
         54, 92, 138, 224, 228, 239, 179, 114, 200,
         158, 229, 245, 20, 8, 143, 215, 110, 153,
         150, 173, 203, 6, 170, 25, 252, 207, 57,
         86, 206, 60, 174, 124
        }
    };
    /* Compare, with an automatic "1 != x" failure message */

    for(int i = 0; i < 2; i++){
        element_hash(ele[i], ele_hash[i], 4);
        for(int j = 0; j < 32; j++){
            /* Compare, with an automatic "ele_hash_check[i][j] != ele_hash[i][j]" failure message */
            ASSERT_EQ(ele_hash_check[i][j], ele_hash[i][j]);

            /* Compare, with a custom failure message */
            ASSERT_EQm("OOPs", ele_hash_check[i][j], ele_hash[i][j]);

            /* Compare, and if they differ, print both values,
            * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
            ASSERT_EQ_FMT(ele_hash_check[i][j], ele_hash[i][j], "%d");
        }
    }

    PASS();
}

TEST test_combine_hashes(void) {
   byte data1[32] = {
         1, 150, 43, 20, 135, 222, 69, 169, 213,
         118, 34, 23, 48, 209, 107, 113, 169, 
         202, 79, 65, 93, 13, 192, 159, 23, 200, 
         41, 36, 22, 157, 86, 153
   };

   byte data2[32] = {
         54, 92, 138, 224, 228, 239, 179, 114, 200,
         158, 229, 245, 20, 8, 143, 215, 110, 153,
         150, 173, 203, 6, 170, 25, 252, 207, 57,
         86, 206, 60, 174, 124
   };

   byte com_hash[32];

   byte com_hash_check[32] = {
         6, 222, 29, 249, 79, 212, 241, 249, 123, 128,
         246, 213, 124, 241, 241, 63, 45, 74, 240, 213,
         89, 184, 156, 60, 93, 43, 240, 219, 199, 171,
         55, 17
   };

    combine_hashes(data1, data2, com_hash);
    
    for(int i = 0; i < 32; i++){
        /* Compare, with an automatic "com_hash_check[i] != com_hash[i]" failure message */
        ASSERT_EQ(com_hash_check[i], com_hash[i]);

        /* Compare, with a custom failure message */
        ASSERT_EQm("OOPs", com_hash_check[i], com_hash[i]);

        /* Compare, and if they differ, print both values,
        * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
        ASSERT_EQ_FMT(com_hash_check[i], com_hash[i], "%d");
    }

    PASS();
}

/* Suites can group multiple tests with common setup. 
   Grouping the following tests:
   1) test_SHA256
   2) test_element_hash
   3) test_combine_hashes
*/
SUITE(the_suite) {
    RUN_TEST(test_SHA256);
    RUN_TEST(test_element_hash);
    RUN_TEST(test_combine_hashes);
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