#include "acutest.h"
#include "lsh.h"
#include "help_func.h"
#include "Fred/frechet.hpp"
#include "hyber_cube.h"


void test_euc_dist(void){
    TEST_CHECK(euc_dist({1,1,1,1},{2,2,2,2})== 4 );
}

void test_modulo(void){
    TEST_CHECK(modulo(14, 4) == 2);
}

TEST_LIST = {
    { "test_euc_dist", test_euc_dist },
    { "test_modulo", test_modulo },
    { NULL, NULL }
};