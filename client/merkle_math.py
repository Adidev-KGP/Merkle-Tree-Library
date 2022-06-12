import hashlib

def floor_log2(n : int)->int:

    assert n>0
    c=0
    t=1

    while 2*t<=n:
        t*=2
        c+=1
    return c

#print(floor_log(28)) --> should give 4

def ceil_log2(n : int)->int:
     
     assert n>0
     c=0
     t=1

     while 2*t<n:
         t*=2
         c+=1
     return c+1

#print(ceil_log2(32)) --> should give 5

def is_pow_2(n : int)->bool:

    assert n>=1

    return n & (n-1) == 0 

#print(is_pow_2(31)) --> should print True

def largest_pow_2_less_than_n(n :int)->int:

    assert n>1

    if(is_pow_2(n)):
        return n//2
    else:
        return 1<<floor_log2(n)

#print(largest_pow_2_less_than_n(32)) --> should print 16

def sha256(s: bytes) -> bytes:
    return hashlib.new('sha256', s).digest()

#print(type(sha256(bytes(32))))

def get_leaf_hash(preimage : bytes)->bytes:
    return sha256(b'\x00' + preimage)

#print(get_leaf_hash(bytes(32)))

def combine_hash(left : bytes, right : bytes)->bytes:
    if len(left)!=32 or len(right) !=32:
        raise ValueError("The elements must be 32-bytes for sha-256 outputs")
    
    return sha256(b'\x01'+left+right)

#print((combine_hash("34".encode('utf-64'),"35".encode('utf-64'))))
