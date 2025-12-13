#include <assert.h>

static int
get_num(const char *stream)
{
    int num = 0;

    while(*stream && *stream != ' ')  {
        num *= 10;
        num += *stream - 48;
        ++stream;
    }

    return (num);
}

int
main(void)
{
    char *nums = "32 1293 92 52 1 091 0 91 51 2 1 2 3 4 5 6 7 8 9 10";
    assert(get_num(nums) == 32);
    nums += 3;
    assert(get_num(nums) == 1293);
    nums += 5;
    assert(get_num(nums) == 92);
    nums += 3;
    assert(get_num(nums) == 52);
    nums += 3;
    assert(get_num(nums) == 1);
    nums += 2;
    assert(get_num(nums) == 91);
    nums += 4;
    assert(!get_num(nums));
    nums += 2;
    assert(get_num(nums) == 91);
    nums += 3;
    assert(get_num(nums) == 51);
    nums += 3;
    assert(get_num(nums) == 2);
    nums += 2;
    assert(get_num(nums) == 1);
    nums += 2;
    assert(get_num(nums) == 2);
    nums += 2;
    assert(get_num(nums) == 3);
    nums += 2;
    assert(get_num(nums) == 4);    
    nums += 2;
    assert(get_num(nums) == 5);
    nums += 2;
    assert(get_num(nums) == 6);
    nums += 2;
    assert(get_num(nums) == 7);
    nums += 2;
    assert(get_num(nums) == 8);
    nums += 2;
    assert(get_num(nums) == 9);
    nums += 2;
    assert(get_num(nums) == 10);
}
