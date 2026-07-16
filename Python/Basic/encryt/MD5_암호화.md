# MD5(Message Digest Algorithm 5)

## 1. MD5란?

MD5(Message Digest Algorithm 5)는 입력 데이터의 길이와 상관없이 **128비트(16Byte)**의 고정된 해시(Hash) 값을 생성하는 알고리즘이다.

1992년 MIT의 **Ronald Rivest**가 개발하였으며, 한때 가장 널리 사용되던 해시 알고리즘 중 하나였다.

> MD5는 **암호화(Encryption)** 알고리즘이 아니라 **해시(Hash)** 알고리즘이다.

---

# 2. 해시(Hash)란?

해시는 임의의 데이터를 일정한 길이의 값으로 변환하는 함수이다.

예를 들어

```
안녕하세요
```

↓

```
654f54d8b2d0f0d6c5f3f2bdf2f0f8f9
```

처럼 변환된다.

입력 길이가 아무리 길어도 출력은 항상 동일한 길이이다.

---

# 3. MD5의 특징

- 출력 길이 : 128bit (16Byte)
- 16진수 문자열로 표현하면 32글자
- 입력 데이터의 크기와 관계없이 결과 길이는 동일
- 같은 입력은 항상 같은 결과를 생성
- 입력이 조금만 바뀌어도 결과가 완전히 달라짐

예)

| 입력 | MD5 |
|------|----------------------------------|
| hello | 5d41402abc4b2a76b9719d911017c592 |
| Hello | 8b1a9953c4611296a827abf8c47804d7 |

대문자 하나만 달라도 전혀 다른 결과가 나온다.

---

# 4. MD5는 암호화가 아니다.

많은 사람들이 MD5를 암호화라고 생각하지만 사실은 아니다.

## 암호화(Encryption)

```
평문
↓

암호문

↓

복호화 가능
```

예)

AES

RSA

DES

---

## 해시(Hash)

```
평문

↓

해시값

↓

복원 불가능
```

대표적인 해시 알고리즘

- MD5
- SHA-1
- SHA-256
- SHA-512

---

# 5. MD5 사용 목적

과거에는 다음과 같은 용도로 많이 사용되었다.

- 비밀번호 저장
- 파일 무결성 검사
- 다운로드 파일 검증
- 중복 데이터 검사

예)

```
ubuntu.iso

↓

MD5

↓

3ab28f7d6d98...
```

다운로드 후 다시 MD5를 계산하여

같은 값이면 파일이 손상되지 않았다고 판단한다.

---

# 6. Avalanche Effect

MD5는 입력이 조금만 바뀌어도 결과가 완전히 달라진다.

예)

```
apple
```

↓

```
1f3870be274f6c49b3e31a0c6728957f
```

하지만

```
Apple
```

↓

```
9f6290f4436e5a2351f12e03b6433c3c
```

첫 글자만 바뀌었는데도 전혀 다른 값이 생성된다.

이를 **Avalanche Effect(눈사태 효과)** 라고 한다.

---

# 7. Python에서 MD5 사용하기

```python
import hashlib

text = "hello"

result = hashlib.md5(text.encode())

print(result.hexdigest())
```

실행 결과

```
5d41402abc4b2a76b9719d911017c592
```

---

## 문자열을 함수로 작성

```python
import hashlib


def md5_hash(text):
    return hashlib.md5(text.encode()).hexdigest()


print(md5_hash("hello"))
print(md5_hash("python"))
```

---

# 8. 파일의 MD5 계산

```python
import hashlib

with open("sample.txt", "rb") as f:
    data = f.read()

md5 = hashlib.md5(data).hexdigest()

print(md5)
```

---

## 큰 파일 계산

메모리를 아끼기 위해 조금씩 읽는다.

```python
import hashlib

md5 = hashlib.md5()

with open("movie.mp4", "rb") as f:

    while True:
        data = f.read(4096)

        if not data:
            break

        md5.update(data)

print(md5.hexdigest())
```

---

# 9. 비밀번호 저장 예제

```python
import hashlib

password = input("비밀번호 : ")

hashed = hashlib.md5(password.encode()).hexdigest()

print(hashed)
```

예)

입력

```
1234
```

출력

```
81dc9bdb52d04dc20036dbd8313ed055
```

---

# 10. 왜 MD5는 더 이상 사용하지 않을까?

현재는 보안 목적으로 MD5 사용을 권장하지 않는다.

이유는 **충돌(Collision)** 이 발견되었기 때문이다.

충돌이란

```
A

↓

MD5

↓

123456...
```

그리고

```
B

↓

MD5

↓

123456...
```

처럼 서로 다른 데이터가 동일한 MD5 값을 가질 수 있는 현상이다.

즉,

```
A ≠ B

하지만

MD5(A) = MD5(B)
```

가 가능하다.

이 때문에

- 전자서명
- 인증서
- 보안 프로그램

등에서는 더 이상 사용하지 않는다.

---

# 11. 현재 사용하는 해시 알고리즘

| 알고리즘 | 상태 |
|-----------|----------------|
| MD5 | 사용 비권장 |
| SHA-1 | 사용 비권장 |
| SHA-256 | 현재 가장 많이 사용 |
| SHA-384 | 안전 |
| SHA-512 | 매우 안전 |

---

# 12. SHA-256 예제

```python
import hashlib

text = "hello"

result = hashlib.sha256(text.encode())

print(result.hexdigest())
```

출력

```
2cf24dba5fb0...
```

SHA-256은

256bit

64자리의 16진수 문자열을 생성한다.

---

# 13. MD5와 SHA-256 비교

| 항목 | MD5 | SHA-256 |
|------|------|----------|
| 개발연도 | 1992 | 2001 |
| 출력길이 | 128bit | 256bit |
| 문자열 길이 | 32자 | 64자 |
| 속도 | 매우 빠름 | 빠름 |
| 충돌 발생 | 가능 | 현실적으로 매우 어려움 |
| 보안 | 취약 | 안전 |
| 현재 사용 | 무결성 확인 등 제한적 | 보안 시스템 전반 |

---

# 14. 정리

- MD5는 **해시 알고리즘**이다.
- **암호화 알고리즘이 아니다.**
- 항상 **128bit(32자리)**의 해시값을 생성한다.
- 입력이 조금만 달라도 결과가 완전히 달라진다.
- 파일 무결성 검사에는 여전히 사용되지만, **보안 목적(비밀번호 저장, 전자서명 등)** 으로는 더 이상 권장되지 않는다.
- 현대의 보안 시스템에서는 **SHA-256**, **SHA-512**, 또는 비밀번호 저장에는 **bcrypt**, **scrypt**, **Argon2**와 같은 전용 비밀번호 해시 알고리즘을 사용하는 것이 일반적이다.
