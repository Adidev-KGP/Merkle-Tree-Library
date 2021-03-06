# pylint: disable=line-too-long
import pytest
import colors
from prover.merkle import sha256, element_hash, combine_hashes

#variable ELEMENTS stores the elements that are leaves in the Merkle Tree
ELEMENTS = [b'merkle', b'tree', b'library',
            b'hardware wallet', b'python',
            b'cryptography', b'hashing funcitons',
            b'sha256']

@pytest.mark.sha256
def test_sha256():

    """
    test_sha256() tests the sha256() function
    of merkle.py which returns the sha256 digest
    of a string byte passed to it.

    """

    #list variable lis stores the sha256 digest of the leaves of the Merkle Tree
    _LIS = [b'yu\xed\xd9\xe79<"\x9etI\x13\xfe\r\x0b\xb8o\xb4\xcfF\x90n.Q\x15!7\xe2\n\xd1U\x90',
            b'\xdc\x9c^\xdb\x8b-G\x9ei{K\x0b\x8a\xb8t\xf3+2Q8Y\x8c\xe9\xe7\xb7Y\xeb\x82\x92\x11\x06"',
            b'\xb7\x18\xf15OrG1.\xca\x08m\x9a\x02J\xfe_\xa7\x17\xdd\xeaZ\xde\xdd\xd6\xf1+\xcf\x94[.\x8c',
            b'\xb7r\x0bn\xec\xf4\xdf\n5\xb5\xf90\xa3\xff\xd1\x8b\xe4BA\xd8\xea\\r\x9e\xf3"K7\xb2N\xc5\xcf',
            b'\x11\xa4\xa6\x0bQ\x8b\xf2I\x89\xd4\x81F\x80v\xe5\xd5\x98(\x84bj\xed\x9f\xae\xb3[\x85v\xfc\xd2#\xe1',
            b'\xe0eT\x81\x8e\x90+K\xa39\xf0f\x96|\x00\x00\xda?\xcd\xa4\xfd~\xb4\xef\x89\xc1$\xfax\xbd\xa4\x19',
            b'\xb3\xe3\xe1\x9e\xb4\xb8+\xb9\xbf\x9f\x00d0\xb4\x1c\xe8\x04wA\x08\x1e:Y\x13.\x83\xcfR\xa5\xf9\x1a\xb3',
            b'][\t\xf6\xdc\xb2\xd5:_\xff\xc6\x0cJ\xc0\xd5_\xab\xdfU`i\xd6c\x15E\xf4*\xa6\xe3P\x0f.']
    _c = 0
    res = True

    for i in ELEMENTS:
        if sha256(i) == _LIS[_c]:
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
    assert res is True

@pytest.mark.element_hash
def test_element_hash():

    """
    test_element_hash() tests the element_hash() function
    of merkle.py which computes the hash of an element
    to be stored in the Merkle tree.

    """

    #list variable lis stores the hash of ELEMENTS to be stored in the Merkle Tree
    _LIS = [b'\x8f&\xd9HqnYd\xfc\xdf}B\\\xdd\x95\xd6\xa2u\xa572\x19~\xfe\x1d\xd9v\xa1\x1a\x0b\xe6x',
            b'f\x01\xef\x14\x03\x82\xce\xb3\xeaVE\xb3\xbc\x8d{:\xf8\xcd\x1f\x1f\x98\xb7-\x85\x88\x90\xb5 ;\xbf\x9e.',
            b'\x01f\xa0\xddW\xac\xdf\xe4\x9c\xa3\xf6\xbc\xf2\xe3\xf9\xc1\xe0L@\xa1\xdaP\xddZ\xa99\xf2*#\x8a\xe8(',
            b'\x18k\xf1G\xcd\x8e\x81\xda\xa4\xea\x1e\xe3s\xcf+\xcd|\x15\x10\x7f\xb2\xd8|!Ya!\xc0\xf5lz\xbf',
            b'Lm\xe7)v\xf5,\xd9\x8c\xa61B\x9a0\xdbrR\xfa\xeb\x97\xc3\xc3\xeb2\xb4d\xaa\x12\x9e#\xf1\xda',
            b'\x9a^\x8c\x93L\xb9\x86\xb2w\xde\xe2`\xbb\xac\xd2\xd6\x88S\x0bT\x93\xce\xb8j4W\xce\xa7=\x02:\xd8',
            b'Cm\xa2B\xf5\xb9\x95z\x0c\xfc\xa8%\xb0d\xe0\xf8\xb7\x90n\xafX\x8d5)\x13\xb0\x9b\xfd\x85\x1cf\xc7',
            b'\xe0\x12\xe0\x154%f\x0b\xe3\xfd|O\xffI\xb2\tJ\x88:\xbd\x8b\x05\xea\x8d\x18\xa1E\xcb\xd7\xed\xccg']

    _c = 0
    res = True
    for i in ELEMENTS:
        if element_hash(i) == _LIS[_c]:
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
    assert res is  True

@pytest.mark.combine_hash
def test_combine_hashes():

    """
    test_combine_hashes() tests the combine_hashes() function
    of merkle.py which combines the hashes of two ELEMENTS
    and returns a byte string

    Here ELEMENTS of lis1 and lis2 are combined to obtain
    ELEMENTS of lis3
    """

    #list variable lis1 stores some hashes
    _LIS1 = [b'\x8f&\xd9HqnYd\xfc\xdf}B\\\xdd\x95\xd6\xa2u\xa572\x19~\xfe\x1d\xd9v\xa1\x1a\x0b\xe6x',
             b'f\x01\xef\x14\x03\x82\xce\xb3\xeaVE\xb3\xbc\x8d{:\xf8\xcd\x1f\x1f\x98\xb7-\x85\x88\x90\xb5 ;\xbf\x9e.',
             b'\x01f\xa0\xddW\xac\xdf\xe4\x9c\xa3\xf6\xbc\xf2\xe3\xf9\xc1\xe0L@\xa1\xdaP\xddZ\xa99\xf2*#\x8a\xe8(',
             b'\x18k\xf1G\xcd\x8e\x81\xda\xa4\xea\x1e\xe3s\xcf+\xcd|\x15\x10\x7f\xb2\xd8|!Ya!\xc0\xf5lz\xbf',
             b'Lm\xe7)v\xf5,\xd9\x8c\xa61B\x9a0\xdbrR\xfa\xeb\x97\xc3\xc3\xeb2\xb4d\xaa\x12\x9e#\xf1\xda',
             b'\x9a^\x8c\x93L\xb9\x86\xb2w\xde\xe2`\xbb\xac\xd2\xd6\x88S\x0bT\x93\xce\xb8j4W\xce\xa7=\x02:\xd8',
             b'Cm\xa2B\xf5\xb9\x95z\x0c\xfc\xa8%\xb0d\xe0\xf8\xb7\x90n\xafX\x8d5)\x13\xb0\x9b\xfd\x85\x1cf\xc7',
             b'\xe0\x12\xe0\x154%f\x0b\xe3\xfd|O\xffI\xb2\tJ\x88:\xbd\x8b\x05\xea\x8d\x18\xa1E\xcb\xd7\xed\xccg']

    #list variable lis2 stores some hashes
    _LIS2 = [b':\xa4x\xb3y\xfez\r\xd3\x06\xbc9\xf0oq\xed\xa6@\x82+\xbe\x04\x01\x1f\xa7\xccuR\xe2(\xf8\xc5',
             b'_\xc2 \x03o\xaez\xb1\xba\xe10&\xc0\x0eNS\xa7\xc4\xc2R\xe2\x7f\x1d\xc4\x88\xb0\xcc\xf8.S"\xb4',      
             b"\xd1'\x81\xb7\xc4\xd7\x88\xa5\xa5\xfd\x02\x00N\xf0\x88\x04lt\xe0h(\x02t%m5\xe7\xd9\xb1\xb8\x07\xa8",
             b'f\xab7_\xde\x807\xf6\x14\x88\xdd\xf2\nx\x0c\x9498}%\x7f\xce5\xe5\xe3\xf5\xaalB\xf4\xaaU',
             b'\xfd\xbc\x14#\xfc]W\xa7o\xa6\x8f\x87N\xda\xf8\xcb\x06\xbe\xd3\xe5d\x95c\xd9<\xc4+\xe7\xaaj\xf2y',   
             b'K~?\xb8\x82\xe3Me\x08\x00V\x11\xe3\x7f\xdc\xe4\xb2U\xbai\x86\xbd\x80\xcd\xe1=\x87\xf9\x19>\xcd\xcf',
             b'\xa2\x93\x85k\xb9l\xe0\x1e\xf0(\x86\xae\xa3\x8a\xd4\xa1\xce;[doO7G\xdb\xb9F\xb8#K\x81\x8d',
             b'\xa3\x8b\xbfO{\xcb"\xe8\xdcR5\x12S\x17p?\x0e\x165J A\xc2\xb3\xa2\xb5\xf0\xf7\n\x1e\xc7\x01']

    #list variable lis3 stores the combined hashes of the ELEMENTS of lis1 and lis2
    _LIS3 = [b'T\xf3&\x99\xbe\x8aL\xfbE\xf6\xa5\xe3\x11\xa4q\x15\xf4`\xca\r\xd62\xda\xfc\x95c\xfa\xcf\xc54k\x93',
             b'\x16\xe0F)\x95Kq\xbe\\\xea\xd3\x02\x13$;\xf2\xce\xaf?z\xa7\xb9\xd5c\x0b\xcbA\xb7m\xe9\xe3\xf4',
             b'\xe1\xdbm\xc3jR\x13\xaf\x95\x12\x9fv%\xa5j\xef\xac]\xd0Q\x02\x04}/\xcf\xd7\x02Gr\xf21e',
             b'\xd3\x7f,\xfc\xae\xc3\xe7h\x1cg\xd6\xa5\x8a\x8e\x89\x1c\xc8\xa6;\x8fm{\xa6\xdd\x1d\x17\xe5\xb2\x93aC-',
             b'\xd8\xda_\xd50I`#H\x8d\x00oV\x1f\x9fA\xa7\x8b\xdaZo5"!\x89T\xc2\xc9\xd3)m\xef',
             b'\x8e(\xa7\xba-\xec\x1d\xd8\x8b[\x99\xa05\x19_(k_\x04\xee\xcc\r\x9d\xf9\ng\xd0^\xcc\xb1\xa2\x92',
             b"p9w`'\xe9\x83\x99\x17\xc5EE\x89%\x1cD=\xf7&\x04\x1c:W\x0b\r\x11a@\xbekh~",
             b'\x86~\x161\x9e\xfd\x08X\xe3\x96\x88\x1f2P\x913\xa2\xdb\xe5\xbc\x06:MG\xcad\xce4\xc3\xe2\x0e\x06']

    _c = 0
    res = True
    for i in _LIS1:
        if combine_hashes(i,_LIS2[_c]) == _LIS3[_c]:
            print(f"test no. {_c+1} passed")
        else:
            print(f"test no. {_c+1} {colors.RED}failed{colors.RESET}")
            res = False
        _c += 1
    assert res is True
