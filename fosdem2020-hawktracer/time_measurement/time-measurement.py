#!/usr/bin/python3
import time
from hawktracer.core import trace, register_file_listener

register_file_listener("base-python.htdump", 1024)

@trace
def fibonacci(n):
    time.sleep(0.00001)
    if n == 1:
        return 0
    elif n == 2:
        return 1
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)


for i in range(1, 10):
    print(fibonacci(i))