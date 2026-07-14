# Google Colab에서 자주 사용하는 명령어

## 1. Python 모듈 설치

``` python
!pip install pandas
```

예시

``` python
!pip install opencv-python
!pip install plotly
!pip install streamlit
!pip install openpyxl
```

여러 개 설치

``` python
!pip install pandas matplotlib numpy seaborn
```

업그레이드

``` python
!pip install -U pandas
```

------------------------------------------------------------------------

# 2. Ubuntu 명령 실행

Colab에서는 명령 앞에 `!`를 붙이면 리눅스 명령을 실행할 수 있다.

현재 폴더

``` python
!pwd
```

파일 목록

``` python
!ls
```

상세 목록

``` python
!ls -al
```

폴더 생성

``` python
!mkdir data
```

파일 삭제

``` python
!rm shopping.csv
```

폴더 삭제

``` python
!rm -r data
```

------------------------------------------------------------------------

# 3. 현재 작업 위치 확인

``` python
!pwd
```

일반적으로

    /content

에서 작업한다.

------------------------------------------------------------------------

# 4. 파일 찾기

``` python
!find /content -name "shopping.csv"
```

전체 검색

``` python
!find / -name "shopping.csv"
```

------------------------------------------------------------------------

# 5. 설치된 패키지 확인

``` python
!pip list
```

또는

``` python
!pip freeze
```

------------------------------------------------------------------------

# 6. Python 버전 확인

``` python
!python --version
```

또는

``` python
import sys
print(sys.version)
```

------------------------------------------------------------------------

# 7. Ubuntu 버전 확인

``` python
!cat /etc/os-release
```

------------------------------------------------------------------------

# 8. 디스크 사용량

``` python
!df -h
```

------------------------------------------------------------------------

# 9. 메모리 사용량

``` python
!free -h
```

------------------------------------------------------------------------

# 10. GPU 확인

``` python
!nvidia-smi
```

------------------------------------------------------------------------

# 11. 인터넷에서 파일 다운로드

``` python
!wget https://example.com/data.csv
```

또는

``` python
!curl -O https://example.com/data.csv
```

------------------------------------------------------------------------

# 12. 압축 해제

ZIP

``` python
!unzip dataset.zip
```

TAR

``` python
!tar -xvf dataset.tar
```

GZIP

``` python
!gunzip data.gz
```

------------------------------------------------------------------------

# 13. Google Drive 연결

``` python
from google.colab import drive

drive.mount('/content/drive')
```

CSV 읽기

``` python
import pandas as pd

df = pd.read_csv("/content/drive/MyDrive/data/shopping.csv")
```

------------------------------------------------------------------------

# 14. 작업 폴더 변경

``` python
import os

os.chdir("/content")
print(os.getcwd())
```

------------------------------------------------------------------------

# 자주 사용하는 명령어 요약

  목적                명령
  ------------------- ---------------------------------
  모듈 설치           `!pip install 패키지명`
  현재 위치           `!pwd`
  파일 목록           `!ls`, `!ls -al`
  파일 찾기           `!find /content -name "파일명"`
  Python 버전         `!python --version`
  설치 패키지         `!pip list`
  GPU 확인            `!nvidia-smi`
  메모리 확인         `!free -h`
  디스크 확인         `!df -h`
  파일 다운로드       `!wget URL`
  Google Drive 연결   `drive.mount('/content/drive')`

------------------------------------------------------------------------

# 실습 과제

1.  현재 작업 폴더를 확인한다.
2.  `shopping.csv`를 업로드한다.
3.  파일이 존재하는지 `!ls`로 확인한다.
4.  `!find`를 이용하여 파일 위치를 찾는다.
5.  `pandas`로 CSV를 읽는다.
6.  `matplotlib`를 이용해 그래프를 출력한다.
7.  Google Drive를 연결하여 동일한 CSV를 다시 읽어본다.
