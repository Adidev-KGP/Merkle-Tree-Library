#include "test_dataextract.h"
#include "../../verifier/verifier.h"

/* Compile : gcc ./tests/test_Verifier/test_dataextract.c ./verifier/verifier.c -lcrypto -w */

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST test_hexstr_to_num(void) {

    char ele[10][3] = {
        {"ae"}, {"be"}, {"ce"}, {"de"}, {"fe"},
        {"1b"}, {"2b"}, {"3b"}, {"4b"}, {"5b"}
        };

    int byte_ele[10] = {
        174, 190, 206, 222, 254, 
        27, 43, 59, 75, 91
    };

    for(int i = 0; i < 10; i++){
        int x = hexstr_to_num(ele[i]);
        /* Compare, with an automatic "byte_ele[i] != x" failure message */
        ASSERT_EQ(byte_ele[i], x);

        /* Compare, with a custom failure message */
        ASSERT_EQm("OOPs", byte_ele[i], x);

        /* Compare, and if they differ, print both values,
        * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
        ASSERT_EQ_FMT(byte_ele[i], x, "%d");
    }

    PASS();
}

TEST test_hexstr_to_data(void) {
    char hash[65] = "360f84035942243c6a36537ae2f8673485e6c04455a0a85a0db19690f2541480";
    byte hash_byte[32];

    byte hash_byte_check[32] = {
        54, 15, 132, 3, 89, 66, 36, 60, 106, 54,
        83, 122, 226, 248, 103, 52, 133, 230, 192,
        68, 85, 160, 168, 90, 13, 177, 150, 144,
        242, 84, 20, 128
        };

    hexstr_to_data(hash, hash_byte);
    for(int i = 0; i < 32; i++){

        /* Compare, with an automatic "hash_byte_check[i] != hash_byte[i]" failure message */
        ASSERT_EQ(hash_byte_check[i], hash_byte[i]);

        /* Compare, with a custom failure message */
        ASSERT_EQm("OOPs", hash_byte_check[i], hash_byte[i]);

        /* Compare, and if they differ, print both values,
        * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
        ASSERT_EQ_FMT(hash_byte_check[i], hash_byte[i], "%d");
    }

    PASS();
}

TEST test_final_processed_data(void) {
    char hash[3][65] = {
        {"360f84035942243c6a36537ae2f8673485e6c04455a0a85a0db19690f2541480"},
        {"d2b60397f20f86fbbd870aa11d506af1826238b2607d240e55cd25d79164a127"},
        {"365c8ae0e4efb372c89ee5f514088fd76e9996adcb06aa19fccf3956ce3cae7c"}
    };
    byte hash_byte[3][32];

    byte hash_byte_check[3][32] = {
        {54, 15, 132, 3, 89, 66, 36, 60, 106, 54,
         83, 122, 226, 248, 103, 52, 133, 230, 192,
         68, 85, 160, 168, 90, 13, 177, 150, 144,
         242, 84, 20, 128
        },
        {210, 182, 3, 151, 242, 15, 134, 251, 189,
         135, 10, 161, 29, 80, 106, 241, 130, 98,
         56, 178, 96, 125, 36, 14, 85, 205, 37,
         215, 145, 100, 161, 39
        },
        {54, 92, 138, 224, 228, 239, 179, 114, 200,
         158, 229, 245, 20, 8, 143, 215, 110, 153,
         150, 173, 203, 6, 170, 25, 252, 207, 57,
         86, 206, 60, 174, 124
        }
    };

    final_processed_data(hash, 3, hash_byte);
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 32; j++){
            
            /* Compare, with an automatic "hash_byte_check[i][j] != hash_byte[i][j]" failure message */
            ASSERT_EQ(hash_byte_check[i][j], hash_byte[i][j]);

            /* Compare, with a custom failure message */
            ASSERT_EQm("OOPs", hash_byte_check[i][j], hash_byte[i][j]);

            /* Compare, and if they differ, print both values,
            * formatted like `printf("Expected: %d\nGot: %d\n", 1, x);` */
            ASSERT_EQ_FMT(hash_byte_check[i][j], hash_byte[i][j], "%d");
        }
    }

    PASS();
}

/* Suites can group multiple tests with common setup. 
   Grouping the following tests:
   1) test_hexstr_to_num
   2) test_hexstr_to_data)
   3) test_final_processed_data
*/
SUITE(the_suite) {
    RUN_TEST(test_hexstr_to_num);
    RUN_TEST(test_hexstr_to_data);
    RUN_TEST(test_final_processed_data);
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