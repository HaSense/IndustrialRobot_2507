#키워드 매개변수

def print_n_times(*values, n=2):
    for i in range(n):
        for word in values:
            print(word)
        print()

print_n_times("안녕하세요", "즐거운", "파이썬 프로그램", n=3)
