# Chapter 0. Streamlit 개발 환경 설치

---

# 학습 목표

이번 장에서는 Streamlit 개발을 위한 기본 환경을 설치하고, 첫 번째 데모 앱을 실행해 본다.

이번 장을 마치면 다음을 할 수 있다.

- Python 설치 여부를 확인할 수 있다.
- 가상환경을 생성할 수 있다.
- Streamlit을 설치할 수 있다.
- Streamlit 데모 앱을 실행할 수 있다.
- `streamlit run app.py` 명령으로 웹 앱을 실행할 수 있다.

---

# 1. Streamlit 개발에 필요한 것

Streamlit 개발을 위해 기본적으로 필요한 것은 다음과 같다.

|항목|필수 여부|설명|
|----|----|----|
|Python|필수|Streamlit은 Python 기반 프레임워크이다.|
|pip|필수|Python 패키지 설치 도구이다.|
|venv|권장|프로젝트별 독립 환경을 만들기 위해 사용한다.|
|VS Code|권장|Python 코드 작성용 편집기이다.|
|Node.js|불필요|일반적인 Streamlit 개발에는 필요하지 않다.|

---

# 2. Node.js를 설치해야 하는가?

일반적인 Streamlit 개발에서는 **Node.js가 필요하지 않다.**

Streamlit은 Python 코드만으로 웹 화면을 만들 수 있는 프레임워크이다.

따라서 이번 강의에서는 Node.js를 설치하지 않는다.

Node.js가 필요한 경우는 다음과 같은 특수한 경우이다.

- Streamlit Custom Component를 직접 개발하는 경우
- React 기반 프론트엔드 컴포넌트를 직접 만드는 경우
- 별도의 JavaScript 프론트엔드와 연동하는 경우

이번 강의의 목표는 다음과 같다.

```text
Python

↓

Streamlit

↓

웹 화면
```

따라서 Python 환경만 준비하면 된다.

---

# 3. Python 설치 확인

터미널 또는 명령 프롬프트를 열고 다음 명령어를 실행한다.

## Windows

```bash
python --version
```

또는

```bash
py --version
```

## macOS / Ubuntu

```bash
python3 --version
```

정상적으로 설치되어 있다면 다음과 비슷하게 출력된다.

```text
Python 3.12.x
```

---

# 4. pip 설치 확인

pip는 Python 패키지를 설치하는 도구이다.

## Windows

```bash
pip --version
```

또는

```bash
py -m pip --version
```

## macOS / Ubuntu

```bash
python3 -m pip --version
```

정상 출력 예시는 다음과 같다.

```text
pip 24.x from ...
```
## pip가 없다면
```bash
sudo apt install python3-pip
```

---

# 5. 프로젝트 폴더 만들기

이번 강의에서는 다음과 같은 폴더를 사용한다.

```text
streamlit/
```

## Windows

```bash
mkdir streamlit
cd streamlit
```

## macOS / Ubuntu

```bash
mkdir streamlit
cd streamlit
```

---

# 6. 가상환경 만들기

가상환경은 프로젝트별로 Python 패키지를 독립적으로 관리하기 위한 공간이다.

가상환경을 사용하면 다른 프로젝트와 라이브러리 충돌을 줄일 수 있다.

---

## Windows

```bash
python -m venv prj1
```

또는

```bash
py -m venv prj1
```

가상환경 활성화

```bash
./prj1/Scripts/activate
```

정상적으로 활성화되면 터미널 앞에 다음과 같이 표시된다.

```text
(prj1)
```

---

## macOS / Ubuntu
가상환경 venv 모듈이 없다면
```bash
sudo apt install python3-venv
```
가상환경 설치

```bash
python3 -m venv .venv
```

가상환경 활성화

```bash
source .venv/bin/activate
```

정상적으로 활성화되면 터미널 앞에 다음과 같이 표시된다.

```text
(.venv)
```

---

# 7. Streamlit 설치

가상환경이 활성화된 상태에서 다음 명령어를 실행한다.

```bash
pip install streamlit
```

설치가 끝나면 버전을 확인한다.

```bash
streamlit --version
```

정상 출력 예시는 다음과 같다.

```text
Streamlit, version 1.xx.x
```

---

# 8. Streamlit 데모 실행

Streamlit이 정상 설치되었는지 확인하기 위해 데모 앱을 실행한다.

```bash
streamlit hello
```

브라우저가 열리면서 Streamlit 예제 화면이 나타난다.

보통 다음 주소로 실행된다.

```text
http://localhost:8501
```

---

# 9. 첫 번째 app.py 만들기

프로젝트 폴더에 다음 파일을 만든다.

```text
app.py
```

파일 내용은 다음과 같다.

```python
import streamlit as st

st.title("Hello Streamlit")

st.write("Streamlit 설치가 완료되었습니다.")
```

---

# 10. app.py 실행하기

터미널에서 다음 명령어를 실행한다.

```bash
streamlit run app.py
```

브라우저에서 다음과 같은 화면이 나타난다.

```text
Hello Streamlit

Streamlit 설치가 완료되었습니다.
```

---

# 11. 프로젝트 기본 구조

앞으로 강의에서 사용할 기본 구조는 다음과 같다.

```text
streamlit_course/

├── app.py
├── pages/
├── data/
├── db/
├── images/
├── source/
├── .venv/
└── requirements.txt
```

---

## 폴더 설명

|폴더/파일|설명|
|---------|----|
|app.py|Streamlit 메인 실행 파일|
|pages/|멀티 페이지 파일 저장|
|data/|CSV, Excel 등 데이터 파일 저장|
|db/|SQLite 데이터베이스 파일 저장|
|images/|실행 화면 캡처 이미지 저장|
|source/|챕터별 예제 코드 저장|
|.venv/|Python 가상환경|
|requirements.txt|필요한 Python 패키지 목록|

---

# 12. requirements.txt 만들기

현재 프로젝트에서 사용하는 패키지 목록을 저장한다.

```bash
pip freeze > requirements.txt
```

생성된 파일 예시는 다음과 같다.

```text
streamlit==1.xx.x
pandas==x.x.x
```

다른 컴퓨터에서 같은 환경을 만들 때는 다음 명령어를 사용한다.

```bash
pip install -r requirements.txt
```

---

# 13. 자주 발생하는 오류

## 오류 1. streamlit 명령어를 찾을 수 없음

```text
streamlit: command not found
```

또는

```text
'streamlit'은 내부 또는 외부 명령이 아닙니다.
```

해결 방법

```bash
pip install streamlit
```

또는 가상환경이 활성화되어 있는지 확인한다.

```bash
.venv\Scripts\activate
```

또는

```bash
source .venv/bin/activate
```

---

## 오류 2. pip 명령어가 동작하지 않음

해결 방법

Windows

```bash
py -m pip install streamlit
```

macOS / Ubuntu

```bash
python3 -m pip install streamlit
```

---

## 오류 3. 포트가 이미 사용 중인 경우

기본 포트는 8501이다.

다른 포트로 실행하려면 다음과 같이 입력한다.

```bash
streamlit run app.py --server.port 8502
```

---

## 오류 4. 브라우저가 자동으로 열리지 않는 경우

터미널에 출력된 주소를 직접 브라우저에 입력한다.

```text
http://localhost:8501
```

---

# 14. 실습 1. 설치 확인

다음 명령어를 순서대로 실행해 보자.

```bash
python --version
pip --version
streamlit --version
```

macOS / Ubuntu에서는 다음 명령어를 사용할 수 있다.

```bash
python3 --version
python3 -m pip --version
streamlit --version
```

---

# 15. 실습 2. 데모 앱 실행

```bash
streamlit hello
```

브라우저에서 Streamlit 데모 화면이 나타나는지 확인한다.

---

# 16. 실습 3. 첫 번째 웹 앱 실행

`app.py` 파일을 만들고 다음 코드를 작성한다.

```python
import streamlit as st

st.title("My First Streamlit App")

st.write("첫 번째 Streamlit 웹 앱입니다.")
```

실행한다.

```bash
streamlit run app.py
```

---

# 17. 실습 4. 개발 폴더 구성하기

다음 구조로 폴더를 만들어 보자.

```text
streamlit_course/

├── app.py
├── pages/
├── data/
├── db/
├── images/
└── source/
```

---

# 핵심 정리

✔ Streamlit은 Python 기반 웹 앱 프레임워크이다.

✔ 일반적인 Streamlit 개발에는 Node.js가 필요하지 않다.

✔ 프로젝트별 가상환경을 사용하는 것이 좋다.

✔ Streamlit 설치 명령어는 다음과 같다.

```bash
pip install streamlit
```

✔ Streamlit 데모 실행 명령어는 다음과 같다.

```bash
streamlit hello
```

✔ Streamlit 앱 실행 명령어는 다음과 같다.

```bash
streamlit run app.py
```

---

# 연습 문제

## 문제 1

Streamlit 개발에 필수적인 언어는 무엇인가?

---

## 문제 2

Streamlit 설치 명령어를 작성하시오.

---

## 문제 3

Streamlit 데모 앱을 실행하는 명령어를 작성하시오.

---

## 문제 4

`app.py` 파일을 실행하는 명령어를 작성하시오.

---

## 문제 5

일반적인 Streamlit 개발에서 Node.js가 필요하지 않은 이유를 설명하시오.

---

# 다음 장 예고

다음 장에서는 Streamlit이 무엇인지 이해하고, Flask나 Django와 어떤 차이가 있는지 학습한다.

다음 장 학습 내용

- Streamlit 소개
- 왜 Flask, Django 대신 Streamlit인가
- 실행 방법
- 프로젝트 구조
