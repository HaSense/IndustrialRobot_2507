# Chapter 13. 이미지 처리(Image Processing)

---

# 학습 목표

이번 장에서는 Streamlit에서 이미지를 업로드하고, OpenCV를 이용하여 간단한 이미지 처리를 수행하는 방법을 학습한다.

스마트팩토리에서는 카메라를 이용하여 제품 상태를 검사하는 경우가 많다.

예를 들어

- 제품 외관 검사
- 불량 검출
- 색상 검사
- 치수 검사
- 바코드/QR 코드 인식
- 표면 스크래치 검사

등이 이미지 처리와 관련된다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 이미지 업로드 화면
- 업로드 이미지 미리보기
- OpenCV 이미지 변환
- 흑백 이미지 변환
- 엣지 검출
- 간단한 제품 검사 화면

이번 장에서 학습할 내용

- 이미지 업로드
- `st.file_uploader()`
- OpenCV
- PIL
- NumPy
- Grayscale
- Edge Detection
- 제품 검사 실습

---

# 1. 이미지 처리란?

이미지 처리는 사진이나 영상에서 필요한 정보를 추출하거나 변환하는 기술이다.

공장에서는 카메라로 촬영한 이미지를 분석하여 제품의 상태를 판단할 수 있다.

예를 들어 다음과 같은 작업이 가능하다.

```text
제품 이미지 촬영

↓

이미지 업로드

↓

흑백 변환

↓

윤곽선 추출

↓

불량 여부 판단
```

---

# 2. 필요한 라이브러리

이번 장에서는 다음 라이브러리를 사용한다.

|라이브러리|설명|
|---------|----|
|streamlit|웹 화면 구성|
|opencv-python|이미지 처리|
|Pillow|이미지 파일 처리|
|numpy|배열 변환|

---

## 설치

```bash
pip install streamlit opencv-python pillow numpy
```

---

# 3. 이미지 업로드

Streamlit에서는 `st.file_uploader()`를 이용하여 이미지를 업로드할 수 있다.

```python
import streamlit as st

file = st.file_uploader(
    "이미지 선택",
    type=["jpg", "jpeg", "png"]
)

if file is not None:
    st.image(file)
```

---

# 4. 업로드한 이미지를 OpenCV로 변환하기

`st.file_uploader()`로 업로드한 파일은 바로 OpenCV에서 사용할 수 없다.

따라서 다음 순서로 변환한다.

```text
UploadedFile

↓

PIL Image

↓

NumPy Array

↓

OpenCV Image
```

---

## 변환 코드

```python
import streamlit as st
from PIL import Image
import numpy as np
import cv2

file = st.file_uploader(
    "이미지 선택",
    type=["jpg", "jpeg", "png"]
)

if file is not None:

    image = Image.open(file)

    img_array = np.array(image)

    img_cv = cv2.cvtColor(
        img_array,
        cv2.COLOR_RGB2BGR
    )

    st.image(image)
```

---

# 5. OpenCV 색상 구조

OpenCV는 이미지를 기본적으로 BGR 순서로 처리한다.

일반적인 이미지 라이브러리는 RGB 순서를 사용한다.

|구분|색상 순서|
|----|---------|
|PIL|RGB|
|Streamlit|RGB|
|OpenCV|BGR|

따라서 OpenCV와 Streamlit을 함께 사용할 때는 색상 변환이 필요하다.

---

## BGR을 RGB로 변환

```python
img_rgb = cv2.cvtColor(
    img_cv,
    cv2.COLOR_BGR2RGB
)
```

---

# 6. 흑백 이미지 변환

흑백 이미지는 밝기 정보만 사용한다.

색상 정보가 필요 없는 검사에서는 흑백 변환이 자주 사용된다.

```python
gray = cv2.cvtColor(
    img_cv,
    cv2.COLOR_BGR2GRAY
)

st.image(gray)
```

---

## 흑백 변환 전체 코드

```python
import streamlit as st
from PIL import Image
import numpy as np
import cv2

st.title("흑백 이미지 변환")

file = st.file_uploader(
    "이미지 선택",
    type=["jpg", "jpeg", "png"]
)

if file is not None:

    image = Image.open(file)

    img_array = np.array(image)

    img_cv = cv2.cvtColor(
        img_array,
        cv2.COLOR_RGB2BGR
    )

    gray = cv2.cvtColor(
        img_cv,
        cv2.COLOR_BGR2GRAY
    )

    st.subheader("원본 이미지")
    st.image(image)

    st.subheader("흑백 이미지")
    st.image(gray)
```

---

# 7. 엣지 검출

엣지(Edge)는 이미지에서 밝기가 급격하게 변하는 부분이다.

제품의 외곽선, 균열, 스크래치 등을 찾을 때 사용한다.

OpenCV에서는 `cv2.Canny()` 함수를 사용할 수 있다.

```python
edges = cv2.Canny(
    gray,
    100,
    200
)
```

---

## 엣지 검출 코드

```python
edges = cv2.Canny(
    gray,
    100,
    200
)

st.image(edges)
```

---

# 8. 임계값 처리

임계값 처리는 밝기를 기준으로 이미지를 흑백으로 나누는 방법이다.

예를 들어 밝기가 127보다 크면 흰색,
작으면 검은색으로 처리한다.

```python
ret, binary = cv2.threshold(
    gray,
    127,
    255,
    cv2.THRESH_BINARY
)
```

---

# 9. 제품 검사 개념

제품 검사에서는 보통 다음 흐름으로 이미지 처리를 수행한다.

```text
제품 이미지 입력

↓

전처리

↓

특징 추출

↓

기준값 비교

↓

정상 / 불량 판정
```

---

## 예시 기준

|검사 항목|판정 기준|
|---------|---------|
|밝기|너무 어두우면 불량|
|엣지 수|외곽선이 너무 많으면 스크래치 가능성|
|면적|제품 크기가 기준보다 작으면 불량|
|색상|지정 색상과 다르면 불량|

---

# 10. 간단한 제품 검사 로직

이번 실습에서는 엣지 픽셀 수를 기준으로 단순 판정을 수행한다.

```text
엣지 픽셀 수가 많다

↓

스크래치나 결함 가능성

↓

불량 의심
```

---

## 엣지 픽셀 수 계산

```python
edge_count = np.count_nonzero(edges)
```

---

## 판정

```python
if edge_count > 5000:
    st.error("불량 의심")
else:
    st.success("정상")
```

---

# 실습 1. 이미지 업로드

```python
import streamlit as st

st.title("이미지 업로드")

file = st.file_uploader(
    "제품 이미지",
    type=["jpg", "jpeg", "png"]
)

if file is not None:
    st.image(file)
```

---

# 실습 2. 흑백 변환

```python
import streamlit as st
from PIL import Image
import numpy as np
import cv2

st.title("흑백 변환")

file = st.file_uploader(
    "제품 이미지",
    type=["jpg", "jpeg", "png"]
)

if file is not None:

    image = Image.open(file)

    img = np.array(image)

    gray = cv2.cvtColor(
        img,
        cv2.COLOR_RGB2GRAY
    )

    st.image(gray)
```

---

# 실습 3. 엣지 검출

```python
import streamlit as st
from PIL import Image
import numpy as np
import cv2

st.title("엣지 검출")

file = st.file_uploader(
    "제품 이미지",
    type=["jpg", "jpeg", "png"]
)

if file is not None:

    image = Image.open(file)

    img = np.array(image)

    gray = cv2.cvtColor(
        img,
        cv2.COLOR_RGB2GRAY
    )

    edges = cv2.Canny(
        gray,
        100,
        200
    )

    st.subheader("원본 이미지")
    st.image(image)

    st.subheader("엣지 이미지")
    st.image(edges)
```

---

# 종합 실습 : 제품 검사 화면

이번 실습에서는 업로드한 제품 이미지를 분석하여 간단한 정상/불량 판정을 수행한다.

파일명

```text
app.py
```

```python
import streamlit as st
from PIL import Image
import numpy as np
import cv2

st.title("제품 검사 시스템")

st.write("제품 이미지를 업로드하면 간단한 이미지 처리를 수행합니다.")

file = st.file_uploader(
    "제품 이미지 업로드",
    type=["jpg", "jpeg", "png"]
)

threshold = st.slider(
    "불량 판정 기준",
    min_value=1000,
    max_value=20000,
    value=5000,
    step=500
)

if file is not None:

    image = Image.open(file)

    img = np.array(image)

    gray = cv2.cvtColor(
        img,
        cv2.COLOR_RGB2GRAY
    )

    edges = cv2.Canny(
        gray,
        100,
        200
    )

    edge_count = np.count_nonzero(edges)

    col1, col2 = st.columns(2)

    with col1:
        st.subheader("원본 이미지")
        st.image(image)

    with col2:
        st.subheader("엣지 검출")
        st.image(edges)

    st.markdown("---")

    st.metric(
        "엣지 픽셀 수",
        edge_count
    )

    if edge_count > threshold:
        st.error("불량 의심")
    else:
        st.success("정상")
```

---

# 코드 해설

## 1. 이미지 업로드

```python
file = st.file_uploader(
    "제품 이미지 업로드",
    type=["jpg", "jpeg", "png"]
)
```

사용자가 제품 이미지를 업로드한다.

---

## 2. 이미지 읽기

```python
image = Image.open(file)
```

업로드한 이미지를 PIL Image로 읽는다.

---

## 3. NumPy 배열 변환

```python
img = np.array(image)
```

OpenCV에서 처리할 수 있도록 배열로 변환한다.

---

## 4. 흑백 변환

```python
gray = cv2.cvtColor(
    img,
    cv2.COLOR_RGB2GRAY
)
```

색상 이미지를 흑백 이미지로 변환한다.

---

## 5. 엣지 검출

```python
edges = cv2.Canny(
    gray,
    100,
    200
)
```

제품의 경계선이나 스크래치 가능성이 있는 부분을 추출한다.

---

## 6. 엣지 픽셀 수 계산

```python
edge_count = np.count_nonzero(edges)
```

엣지로 검출된 픽셀 수를 계산한다.

---

## 7. 정상/불량 판정

```python
if edge_count > threshold:
    st.error("불량 의심")
else:
    st.success("정상")
```

기준값보다 엣지 픽셀이 많으면 불량 의심으로 판정한다.

---

# 도전 실습 1

이미지를 흑백으로 변환한 뒤 밝기 평균값을 계산해 보자.

힌트

```python
gray.mean()
```

---

# 도전 실습 2

밝기 평균값이 너무 낮으면 다음 메시지를 출력해 보자.

```text
이미지가 너무 어둡습니다.
```

---

# 도전 실습 3

엣지 검출 기준값을 사용자가 직접 조절할 수 있도록 만들어 보자.

힌트

```python
st.slider()
```

---

# 도전 실습 4

원본 이미지, 흑백 이미지, 엣지 이미지를 3개의 컬럼으로 나누어 출력해 보자.

힌트

```python
st.columns(3)
```

---

# 도전 실습 5

검사 결과를 다음과 같은 형태로 출력해 보자.

```text
제품명

검사일

엣지 픽셀 수

판정 결과
```

---

# 실무 활용 사례

OpenCV와 Streamlit을 함께 사용하면 다음과 같은 시스템을 만들 수 있다.

- 제품 외관 검사
- 불량 이미지 판정
- 설비 계기판 인식
- 바코드 이미지 처리
- 작업자 안전모 착용 여부 확인
- 금속 표면 스크래치 검출
- 포장 상태 검사
- 색상 검사

---

# 주의 사항

이번 장의 제품 검사는 학습용 예제이다.

실제 산업 현장에서는 다음 요소가 함께 고려되어야 한다.

- 조명 조건
- 카메라 해상도
- 제품 위치
- 배경 색상
- 노이즈 제거
- 기준 이미지 비교
- 딥러닝 모델 적용
- 검사 정확도 검증

따라서 단순 엣지 개수만으로 실제 불량을 확정해서는 안 된다.

---

# 핵심 정리

✔ Streamlit에서는 `st.file_uploader()`로 이미지를 업로드할 수 있다.

✔ 업로드한 이미지는 PIL과 NumPy를 거쳐 OpenCV에서 처리할 수 있다.

✔ OpenCV는 기본적으로 BGR 색상 순서를 사용한다.

✔ 흑백 변환은 이미지 처리의 기본 전처리 과정이다.

✔ Canny Edge Detection은 경계선이나 스크래치 검출에 사용할 수 있다.

✔ Streamlit과 OpenCV를 결합하면 간단한 제품 검사 화면을 만들 수 있다.

---

# 연습 문제

## 문제 1

이미지를 업로드하여 화면에 출력하는 프로그램을 작성하시오.

---

## 문제 2

업로드한 이미지를 흑백으로 변환하여 출력하시오.

---

## 문제 3

업로드한 이미지에서 엣지를 검출하여 출력하시오.

---

## 문제 4

엣지 픽셀 수를 계산하여 화면에 출력하시오.

---

## 문제 5

엣지 픽셀 수를 기준으로 정상/불량을 판정하는 제품 검사 프로그램을 작성하시오.

---

# 다음 장 예고

다음 장에서는 지금까지 학습한 내용을 통합하여 Mini MES 프로젝트를 구현한다.

학습 내용

- 로그인
- 고객 관리
- 제품 관리
- 생산 실적 관리
- 이미지 기반 제품 검사
- Dashboard
- SQLite 저장
- Streamlit 화면 통합
