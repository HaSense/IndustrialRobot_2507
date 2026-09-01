# Windows PowerShell에서 `uv` 설치하고 사용하기

> 대상: Windows 10/11에서 PowerShell로 Python 프로젝트를 시작하려는 사용자  
> 작성 기준: 2026-09-01, Astral 공식 `uv` 문서

## 1. `uv`란?

`uv`는 Python 설치, 가상환경 생성, 패키지 설치, 의존성 잠금과 프로그램 실행을 하나의 명령으로 처리하는 도구입니다.

기존 방식의 여러 도구를 다음과 같이 통합할 수 있습니다.

| 기존 작업 | `uv` 명령 |
|---|---|
| Python 설치·버전 관리 | `uv python install` |
| 가상환경 생성 | `uv venv` |
| 패키지 설치 | `uv add` 또는 `uv pip install` |
| 패키지 목록 고정 | `uv.lock` 자동 생성 |
| 프로젝트 환경에서 실행 | `uv run` |
| 일회성 CLI 도구 실행 | `uvx` |

## 2. 설치 전 확인

시작 메뉴에서 **PowerShell** 또는 **Windows Terminal**을 실행합니다. 관리자 권한은 일반적으로 필요하지 않습니다.

현재 PowerShell 버전을 확인합니다.

```powershell
$PSVersionTable.PSVersion
```

Windows에 Python이 없어도 `uv`를 먼저 설치할 수 있습니다. 이후 `uv`가 Python까지 설치·관리할 수 있습니다.

## 3. `uv` 설치

PowerShell에서 다음 명령을 실행합니다.

```powershell
powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"
```

설치가 끝나면 **PowerShell 또는 Windows Terminal을 완전히 닫고 새로 실행**합니다. 그런 다음 버전을 확인합니다.

```powershell
uv --version
```

정상 예시는 다음과 같습니다.

```text
uv 0.x.x
```

업데이트는 다음 명령으로 할 수 있습니다.

```powershell
uv self update
```

## 4. `uv` 명령을 찾지 못할 때

다음 오류가 나오면 새 터미널을 먼저 열어 다시 확인합니다.

```text
uv : 'uv' 용어가 cmdlet, 함수, 스크립트 파일 또는 실행할 수 있는 프로그램의 이름으로 인식되지 않습니다.
```

현재 PATH를 확인합니다.

```powershell
$env:Path -split ';'
```

설치 파일을 검색합니다.

```powershell
Get-ChildItem "$env:USERPROFILE\.local\bin\uv.exe" -ErrorAction SilentlyContinue
Get-Command uv -ErrorAction SilentlyContinue
```

`uv.exe`가 `$env:USERPROFILE\.local\bin`에 있지만 PATH에 없다면 사용자 PATH에 추가합니다.

```powershell
$uvBin = "$env:USERPROFILE\.local\bin"
$userPath = [Environment]::GetEnvironmentVariable('Path', 'User')

if (($userPath -split ';') -notcontains $uvBin) {
    [Environment]::SetEnvironmentVariable(
        'Path',
        ($userPath.TrimEnd(';') + ';' + $uvBin),
        'User'
    )
}
```

터미널을 완전히 닫고 다시 연 후 확인합니다.

```powershell
uv --version
```

> 설치 위치가 다르다면 설치 완료 메시지에 표시된 실제 디렉터리를 PATH에 추가해야 합니다.

## 5. Python 설치

Python 3.12를 설치하는 예입니다.

```powershell
uv python install 3.12
```

`uv`가 인식하는 Python을 확인합니다.

```powershell
uv python list
```

설치된 Python으로 버전을 출력합니다.

```powershell
uv run --python 3.12 python --version
```

Windows의 `python` 명령이 Microsoft Store를 열더라도 `uv run python`은 프로젝트용 Python을 정상적으로 사용할 수 있습니다.

## 6. 새 프로젝트 만들기

문서 폴더에 `temperature-app` 프로젝트를 만드는 예입니다.

```powershell
cd $HOME\Documents
uv init temperature-app
cd temperature-app
```

생성된 파일을 확인합니다.

```powershell
Get-ChildItem -Force
```

대표 파일은 다음과 같습니다.

| 파일 | 역할 |
|---|---|
| `pyproject.toml` | 프로젝트 정보와 의존성 정의 |
| `.python-version` | 프로젝트에서 사용할 Python 버전 |
| `main.py` | 기본 Python 실행 파일 |
| `.venv` | 프로젝트 전용 가상환경; 처음 실행·동기화할 때 생성될 수 있음 |
| `uv.lock` | 패키지의 정확한 버전을 기록하는 잠금 파일 |

기본 프로그램을 실행합니다.

```powershell
uv run main.py
```

`uv run`은 필요한 가상환경을 준비하고 의존성을 동기화한 뒤 프로그램을 실행합니다.

## 7. 패키지 추가·삭제

예를 들어 `requests`, `pandas`, `streamlit`을 설치합니다.

```powershell
uv add requests pandas streamlit
```

이 명령은 다음 작업을 함께 처리합니다.

- `pyproject.toml`에 의존성 등록
- `.venv` 생성 또는 갱신
- `uv.lock` 생성 또는 갱신

개발용 패키지는 `--dev` 옵션을 사용합니다.

```powershell
uv add --dev pytest ruff
```

패키지를 삭제합니다.

```powershell
uv remove requests
```

현재 의존성 구조를 확인합니다.

```powershell
uv tree
```

## 8. Python 프로그램 실행

`main.py`를 실행합니다.

```powershell
uv run python main.py
```

Python 파일은 다음처럼 줄여서 실행할 수도 있습니다.

```powershell
uv run main.py
```

모듈 방식 실행 예입니다.

```powershell
uv run python -m pytest
```

Streamlit 앱 실행 예입니다.

```powershell
uv run streamlit run app.py
```

FastAPI 개발 서버 실행 예입니다.

```powershell
uv add fastapi "uvicorn[standard]"
uv run uvicorn main:app --reload
```

## 9. 가상환경을 직접 활성화하는 방법

`uv`에서는 가상환경을 활성화하지 않고 `uv run`을 사용하는 방법이 가장 간단합니다.

```powershell
uv run python main.py
```

기존 방식처럼 가상환경을 활성화하려면 먼저 동기화합니다.

```powershell
uv sync
.\.venv\Scripts\Activate.ps1
```

활성화되면 프롬프트 앞에 보통 `(.venv)`가 표시됩니다.

```powershell
python main.py
```

가상환경을 종료합니다.

```powershell
deactivate
```

### 활성화 스크립트 실행이 차단될 때

다음과 같은 오류가 날 수 있습니다.

```text
이 시스템에서 스크립트를 실행할 수 없으므로 Activate.ps1 파일을 로드할 수 없습니다.
```

현재 PowerShell 창에서만 실행 정책을 완화하려면 다음을 실행합니다.

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\.venv\Scripts\Activate.ps1
```

이 설정은 해당 PowerShell 프로세스를 닫으면 사라집니다. 실행 정책을 변경하지 않으려면 가상환경을 활성화하지 말고 `uv run`을 사용하면 됩니다.

## 10. 기존 프로젝트 내려받아 실행하기

Git 저장소를 내려받은 뒤 프로젝트 폴더로 이동합니다.

```powershell
git clone https://github.com/example/sample-project.git
cd sample-project
```

`pyproject.toml`과 `uv.lock`을 기준으로 환경을 복원합니다.

```powershell
uv sync
uv run python main.py
```

팀원이 동일한 버전의 패키지를 사용하도록 다음 파일을 Git에 포함하는 것이 일반적입니다.

```text
pyproject.toml
uv.lock
.python-version
```

`.venv`는 Git에 올리지 않습니다.

```gitignore
.venv/
__pycache__/
*.pyc
```

## 11. `requirements.txt`를 사용하는 기존 프로젝트

### 기존 목록을 가상환경에 설치

```powershell
uv venv
uv pip install -r requirements.txt
```

실행합니다.

```powershell
uv run python main.py
```

### 프로젝트 관리 방식으로 전환

새 `uv` 프로젝트를 만든 뒤 요구 패키지를 프로젝트 의존성으로 가져옵니다.

```powershell
uv init
uv add -r requirements.txt
```

이후에는 `uv add`, `uv remove`, `uv sync`, `uv run` 중심으로 관리합니다.

## 12. 일회성 명령과 전역 CLI 도구

프로젝트 의존성에 넣지 않고 Python CLI 도구를 한 번 실행하려면 `uvx`를 사용합니다.

```powershell
uvx ruff check .
```

도구를 지속적으로 설치하려면 다음과 같이 사용할 수 있습니다.

```powershell
uv tool install ruff
uv tool list
uv tool uninstall ruff
```

## 13. 자주 쓰는 명령 요약

| 목적 | 명령 |
|---|---|
| 설치 확인 | `uv --version` |
| uv 업데이트 | `uv self update` |
| Python 3.12 설치 | `uv python install 3.12` |
| Python 목록 확인 | `uv python list` |
| 새 프로젝트 생성 | `uv init 프로젝트명` |
| 패키지 추가 | `uv add 패키지명` |
| 개발 패키지 추가 | `uv add --dev 패키지명` |
| 패키지 삭제 | `uv remove 패키지명` |
| 환경 동기화 | `uv sync` |
| Python 파일 실행 | `uv run python main.py` |
| Streamlit 실행 | `uv run streamlit run app.py` |
| 의존성 확인 | `uv tree` |
| 가상환경 생성 | `uv venv` |
| pip 호환 설치 | `uv pip install 패키지명` |
| 일회성 도구 실행 | `uvx 도구명` |

## 14. 실습: Streamlit 프로젝트 만들기

### 1단계: 프로젝트 생성

```powershell
cd $HOME\Documents
uv init streamlit-demo
cd streamlit-demo
```

### 2단계: 패키지 추가

```powershell
uv add streamlit pandas
```

### 3단계: `app.py` 작성

```python
import pandas as pd
import streamlit as st

st.title("uv로 실행한 Streamlit")

df = pd.DataFrame(
    {
        "시간": ["09:00", "10:00", "11:00"],
        "온도": [23.5, 24.1, 25.0],
    }
)

st.dataframe(df, use_container_width=True)
st.line_chart(df.set_index("시간"))
```

### 4단계: 실행

```powershell
uv run streamlit run app.py
```

브라우저가 자동으로 열리지 않으면 PowerShell에 출력된 주소를 브라우저에서 엽니다. 일반적으로 다음 주소입니다.

```text
http://localhost:8501
```

종료할 때는 PowerShell에서 `Ctrl+C`를 누릅니다.

## 15. 흔한 문제 해결

### `uv` 명령을 찾지 못함

1. 터미널을 완전히 닫고 다시 실행합니다.
2. `Get-Command uv`로 인식 여부를 확인합니다.
3. `$env:Path -split ';'`로 사용자 실행 디렉터리가 포함되었는지 확인합니다.

### 원하는 Python 버전이 사용되지 않음

프로젝트에서 사용할 버전을 지정합니다.

```powershell
uv python pin 3.12
uv sync
uv run python --version
```

### 패키지를 설치했지만 import가 되지 않음

프로젝트 폴더에서 실행했는지 확인하고 다음 명령을 사용합니다.

```powershell
uv add 패키지명
uv run python main.py
```

다른 Python이나 다른 터미널 환경에 설치되는 혼선을 줄이려면 프로젝트에서는 `pip install`보다 `uv add`를 사용하는 편이 안전합니다.

### 프로젝트 환경을 처음부터 다시 만들고 싶음

PowerShell을 종료하고 `.venv` 폴더만 삭제한 뒤 다시 동기화합니다.

```powershell
Remove-Item -Recurse -Force .venv
uv sync
```

> 위 명령은 현재 폴더의 `.venv`를 삭제합니다. 반드시 프로젝트 루트가 맞는지 `Get-Location`으로 먼저 확인하십시오. 소스 코드와 `uv.lock`은 삭제하지 않습니다.

## 16. 권장 작업 흐름

새 프로젝트에서는 다음 네 가지 명령을 중심으로 사용하면 됩니다.

```powershell
uv init my-project
cd my-project
uv add 필요한패키지
uv run python main.py
```

다른 사람이 만든 프로젝트를 받은 경우에는 다음 흐름이 기본입니다.

```powershell
cd 프로젝트폴더
uv sync
uv run python main.py
```

## 참고 자료

- [uv 공식 설치 문서](https://docs.astral.sh/uv/getting-started/installation/)
- [uv 프로젝트 사용 가이드](https://docs.astral.sh/uv/guides/projects/)
- [uv Python 설치 가이드](https://docs.astral.sh/uv/guides/install-python/)
- [uv 명령어 전체 문서](https://docs.astral.sh/uv/reference/cli/)

