# 평문을 받아서 각 알파벳을 한 칸씩 이동하여 암호화한다.
def encrypt(plaintext):
    result = ''

    for ch in plaintext.lower():
        if 'a' <= ch <= 'z':
            # a~z를 0~25로 바꾼 후 한 칸 이동
            encrypted_ch = chr(
                (ord(ch) - ord('a') + 1) % 26 + ord('a')
            )
            result += encrypted_ch
        else:
            # 공백, 마침표 등의 문자는 그대로 유지
            result += ch

    return result


# 암호문의 각 알파벳을 한 칸씩 되돌린다.
def decrypt(ciphertext):
    result = ''

    for ch in ciphertext:
        if 'a' <= ch <= 'z':
            decrypted_ch = chr(
                (ord(ch) - ord('a') - 1) % 26 + ord('a')
            )
            result += decrypted_ch
        else:
            result += ch

    return result


text = 'The language of truth is simple.'

encrypted = encrypt(text)
decrypted = decrypt(encrypted)

print('평문   :', text)
print('암호문 :', encrypted)
print('복호문 :', decrypted)
