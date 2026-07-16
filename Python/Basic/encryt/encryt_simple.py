# 암호화
def encrypt(plaintext):
    result = ''

    for ch in plaintext:
        result += chr(ord(ch) + 1)

    return result

# 복호화
def decrypt(ciphertext):
    result = ''

    for ch in ciphertext:
        result += chr(ord(ch) - 1)

    return result


text = 'The language of truth is simple.'

encrypted = encrypt(text)
decrypted = decrypt(encrypted)

print('평문   :', text)
print('암호문 :', encrypted)
print('복호문 :', decrypted)
