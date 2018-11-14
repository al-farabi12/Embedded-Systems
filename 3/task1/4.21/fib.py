fib_cache = {}


def fibonacc(n):
    if n in fib_cache:
        return fib_cache[n]
    if n == 1:
        value = 1
    elif n == 2:
        value = 1
    elif n > 2:
        value = fibonacc(n-1)+fibonacc(n-2)
    fib_cache[n] = value
    return value


for n in range(1, 1001):
    print(n, ":", fibonacc(n))
