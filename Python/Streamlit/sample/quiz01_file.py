try : 
    infilename = input('입력 파일 이름:')
    outfilename = input('출력 파일 이름:')
    #코딩
    with (open(infilename, 'r') as infile, 
         open(outfilename, 'w') as outfile):
         sum = 0
         count = 0
         line = infile.readline()
         while line != '':
             s = int(line)
             sum += s
             count += 1
             line = infile.readline()
         outfile.write(f'총매출={str(sum)}\n')
         outfile.write(f'평균 일매출={str(sum/count)}')
        #with구문을 사용하면 close를 신경쓰지 않아도된다
except BaseException as e:
    print(e)
