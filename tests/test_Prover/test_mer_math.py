# pylint: disable=missing-module-docstring
import colors, pytest

from prover.merkle import floor_lg, ceil_lg, is_power_of_2, largest_power_of_2_less_than

#list variable NO stores some numbers on which the functions will be tested
NO = [23, 32, 44, 63, 72, 76, 80]

@pytest.mark.floor_lg
def test_floor_lg():

    """
    test_floor_lg() tests the floor_lg() function
    of merkle.py which returns the floor value of
    log2(n).
    """

    _LIS = [4, 5, 5, 5, 6, 6, 6]
    _c = 0
    res = True

    for i in NO:
        if floor_lg(i) == _LIS[_c]:
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
    assert res is True

@pytest.mark.ceil_lg
def test_ceil_lg():

    """
    test_ceil_lg() tests the ceil_lg() function
    of merkle.py which returns the ceil value of
    log2(n).

    """

    _LIS = [5, 5, 6, 6, 7, 7, 7]
    _c = 0
    res = True

    for i in NO:
        if ceil_lg(i) == _LIS[_c]:    
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
    assert res is True

@pytest.mark.power_of_2
def test_is_power_of_2():

    """
    test_is_power_of_2() tests the is_power_of_2() function
    of merkle.py which returns true/false depending on
    whether a certain number is power of 2 or not.

    """

    _c = 0
    res = True

    _NOS = [2, 4, 32, 16, 128, 1024, 512]
    for i in _NOS:
        if is_power_of_2(i):
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
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

    _LIS = [16, 16, 32, 32, 64, 64, 64]
    _c = 0
    res = True

    for i in NO:    
        if largest_power_of_2_less_than(i) == _LIS[_c]:
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
    assert res is True
