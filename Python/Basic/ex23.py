import sys

def print_n_times(value, n=2, end='\n', sep=' ', 
                  flush=False, file=sys.stdout):
    #n번 반복
    for i in range(n):
        print(value)

print_n_times('안녕하세요')
