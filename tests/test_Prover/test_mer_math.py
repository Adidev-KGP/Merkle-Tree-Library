import colors, pytest

from prover.merkle import floor_lg, ceil_lg, is_power_of_2, largest_power_of_2_less_than

#list variable no stores some numbers on which the functions will be tested
no = [23, 32, 44, 63, 72, 76, 80]

@pytest.mark.floor_lg
def test_floor_lg():

    """
    test_floor_lg() tests the floor_lg() function
    of merkle.py which returns the floor value of
    log2(n).
    """

    lis = [4, 5, 5, 5, 6, 6, 6]
    c = 0
    res = True

    for i in no:
        if floor_lg(i) == lis[c]:
            print(f"test no. {c+1} passed")
        else:
            print(f"test no. {c+1} {colors.RED}failed{colors.RESET}")
            res = False
        c+=1
    assert res is True

@pytest.mark.ceil_lg
def test_ceil_lg():

    """
    test_ceil_lg() tests the ceil_lg() function
    of merkle.py which returns the ceil value of
    log2(n).

    """

    lis = [5, 5, 6, 6, 7, 7, 7]
    c = 0
    res = True

    for i in no:
        if ceil_lg(i) == lis[c]:    
            print(f"test no. {c+1} passed")
        else:
            print(f"test no. {c+1} {colors.RED}failed{colors.RESET}")
            res = False
        c+=1
    assert res is True

@pytest.mark.power_of_2
def test_is_power_of_2():

    """
    test_is_power_of_2() tests the is_power_of_2() function
    of merkle.py which returns true/false depending on
    whether a certain number is power of 2 or not.

    """

    c = 0
    res = True

    nos = [2, 4, 32, 16, 128, 1024, 512]        
    for i in nos:
        if is_power_of_2(i):
            print(f"test no. {c+1} passed")
        else:
            print(f"test no. {c+1} {colors.RED}failed{colors.RESET}")
            res = False
        c+=1
    assert res is True

@pytest.mark.largest_power_of_2_less_than
def test_largest_power_of_2_less_than():

    """
    test_largest_power_of_2_less_than() tests the
    largest_power_of_2_less_than() function
    of merkle.py which returns the largest possible
    number which is a power of 2 and is strictly
    less than the parameter of largest_power_of_2_less_than().

    """

    lis = [16, 16, 32, 32, 64, 64, 64]
    c = 0
    res = True

    for i in no:    
        if largest_power_of_2_less_than(i) == lis[c]:
            print(f"test no. {c+1} passed")
        else:
            print(f"test no. {c+1} {colors.RED}failed{colors.RESET}")
            res = False
        c+=1
    assert res is True
