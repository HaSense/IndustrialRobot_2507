# WSL에서 VS Code로 Java 개발환경 구성하기

Windows에서 **WSL 2(Ubuntu)**를 사용하고, **Visual Studio Code**에서 Java 프로젝트를 작성·실행·디버깅하기 위한 설정 가이드입니다.

> 권장 구성  
> Windows 11 또는 최신 Windows 10 + WSL 2 + Ubuntu + JDK 21 + VS Code + Extension Pack for Java

---

## 목차

1. [개발환경 구성 개요](#1-개발환경-구성-개요)
2. [WSL 2 설치](#2-wsl-2-설치)
3. [Ubuntu 초기 설정](#3-ubuntu-초기-설정)
4. [WSL에 JDK 설치](#4-wsl에-jdk-설치)
5. [Windows에 VS Code 설치](#5-windows에-vs-code-설치)
6. [VS Code 확장 프로그램 설치](#6-vs-code-확장-프로그램-설치)
7. [WSL에서 프로젝트 폴더 열기](#7-wsl에서-프로젝트-폴더-열기)
8. [Java 프로그램 작성 및 실행](#8-java-프로그램-작성-및-실행)
9. [Java 디버깅](#9-java-디버깅)
10. [Maven 프로젝트 만들기](#10-maven-프로젝트-만들기)
11. [Gradle 프로젝트 만들기](#11-gradle-프로젝트-만들기)
12. [Git과 GitHub 연결](#12-git과-github-연결)
13. [권장 프로젝트 구조](#13-권장-프로젝트-구조)
14. [자주 발생하는 문제 해결](#14-자주-발생하는-문제-해결)
15. [최종 점검 체크리스트](#15-최종-점검-체크리스트)

---

## 1. 개발환경 구성 개요

WSL 기반 Java 개발에서는 프로그램별 설치 위치를 구분해야 합니다.

| 구성 요소 | 설치 위치 | 설명 |
|---|---|---|
| WSL 2 | Windows | Linux 실행 환경 |
| Ubuntu | WSL | Java와 Git이 실행되는 Linux 배포판 |
| JDK | WSL Ubuntu | Java 컴파일 및 실행 환경 |
| VS Code | Windows | 코드 편집기 |
| WSL 확장 | VS Code | Windows의 VS Code를 WSL에 연결 |
| Java 확장 | WSL 환경 | Java 실행, 디버깅, 프로젝트 관리 |
| Git | WSL Ubuntu | 소스 버전 관리 |

핵심 원칙은 다음과 같습니다.

- VS Code 프로그램은 Windows에 설치합니다.
- JDK와 Git은 WSL Ubuntu에 설치합니다.
- 프로젝트는 WSL의 Linux 파일 시스템에 저장합니다.
- VS Code는 WSL 모드로 프로젝트를 엽니다.

---

## 2. WSL 2 설치

### 2.1 PowerShell을 관리자 권한으로 실행

Windows 시작 메뉴에서 `PowerShell`을 검색한 후 **관리자 권한으로 실행**합니다.

### 2.2 WSL 설치

```powershell
wsl --install
```

기본적으로 WSL과 Ubuntu가 함께 설치됩니다.

특정 배포판을 확인하려면 다음 명령어를 사용합니다.

```powershell
wsl --list --online
```

Ubuntu를 지정하여 설치하려면 다음과 같이 실행합니다.

```powershell
wsl --install -d Ubuntu
```

설치가 끝나면 Windows를 다시 시작합니다.

### 2.3 WSL 버전 확인

```powershell
wsl --list --verbose
```

예상 출력:

```text
  NAME      STATE           VERSION
* Ubuntu    Running         2
```

Ubuntu의 버전이 `1`로 표시되면 WSL 2로 변경합니다.

```powershell
wsl --set-version Ubuntu 2
```

새로 설치되는 배포판의 기본 버전을 WSL 2로 지정할 수도 있습니다.

```powershell
wsl --set-default-version 2
```

---

## 3. Ubuntu 초기 설정

처음 Ubuntu를 실행하면 Linux 사용자 이름과 비밀번호를 설정합니다.

설정이 완료되면 패키지 목록과 설치된 패키지를 업데이트합니다.

```bash
sudo apt update
sudo apt upgrade -y
```

필수 도구를 설치합니다.

```bash
sudo apt install -y curl wget unzip zip build-essential
```

Ubuntu 버전을 확인합니다.

```bash
cat /etc/os-release
```

현재 작업 위치를 확인합니다.

```bash
pwd
```

---

## 4. WSL에 JDK 설치

Java 소스 코드를 컴파일하려면 JRE가 아니라 **JDK**가 필요합니다.

이 문서에서는 호환성과 안정성을 고려하여 JDK 21을 사용합니다.

### 4.1 OpenJDK 21 설치

```bash
sudo apt update
sudo apt install -y openjdk-21-jdk
```

### 4.2 설치 확인

```bash
java -version
javac -version
```

예상 출력의 주요 부분:

```text
openjdk version "21..."
javac 21...
```

`java`는 Java 프로그램을 실행하고, `javac`는 Java 소스 코드를 컴파일합니다.

### 4.3 Java 설치 경로 확인

```bash
readlink -f "$(which java)"
```

JDK 목록을 확인하려면 다음 명령어를 사용합니다.

```bash
update-java-alternatives --list
```

### 4.4 여러 JDK 버전이 설치된 경우

기본 Java 실행 버전을 변경합니다.

```bash
sudo update-alternatives --config java
```

기본 Java 컴파일러를 변경합니다.

```bash
sudo update-alternatives --config javac
```

### 4.5 JAVA_HOME 설정

JDK 경로를 확인합니다.

```bash
dirname "$(dirname "$(readlink -f "$(which javac)")")"
```

일반적인 OpenJDK 21 경로는 다음과 같습니다.

```text
/usr/lib/jvm/java-21-openjdk-amd64
```

`~/.bashrc` 파일을 엽니다.

```bash
nano ~/.bashrc
```

파일 마지막에 다음 내용을 추가합니다.

```bash
export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64
export PATH="$JAVA_HOME/bin:$PATH"
```

설정을 적용합니다.

```bash
source ~/.bashrc
```

설정 결과를 확인합니다.

```bash
echo "$JAVA_HOME"
java -version
javac -version
```

> ARM64 환경에서는 JDK 경로가 `/usr/lib/jvm/java-21-openjdk-arm64`처럼 다를 수 있습니다.  
> 반드시 `readlink` 명령으로 실제 경로를 확인한 후 설정합니다.

---

## 5. Windows에 VS Code 설치

VS Code는 WSL 내부가 아니라 **Windows에 설치**합니다.

공식 사이트에서 Visual Studio Code를 설치합니다.

- <https://code.visualstudio.com/>

설치 과정에서는 다음 항목을 선택하는 것이 편리합니다.

- PATH에 추가
- 지원되는 파일 형식의 편집기로 등록
- 탐색기 파일 컨텍스트 메뉴에 `Code로 열기` 추가
- 탐색기 디렉터리 컨텍스트 메뉴에 `Code로 열기` 추가

설치 후 Windows에서 VS Code를 한 번 실행합니다.

---

## 6. VS Code 확장 프로그램 설치

VS Code의 Extensions 화면에서 다음 확장 프로그램을 설치합니다.

### 6.1 필수 확장

#### WSL

- 확장 이름: `WSL`
- 게시자: Microsoft
- 확장 ID: `ms-vscode-remote.remote-wsl`

#### Extension Pack for Java

- 확장 이름: `Extension Pack for Java`
- 게시자: Microsoft
- 확장 ID: `vscjava.vscode-java-pack`

Extension Pack for Java에는 일반적으로 다음 기능이 포함됩니다.

- Language Support for Java
- Debugger for Java
- Test Runner for Java
- Maven for Java
- Project Manager for Java
- IntelliCode 관련 기능

### 6.2 명령어로 설치

Windows 터미널 또는 PowerShell에서 다음 명령어를 사용할 수 있습니다.

```powershell
code --install-extension ms-vscode-remote.remote-wsl
code --install-extension vscjava.vscode-java-pack
```

### 6.3 WSL 환경에 확장 설치 확인

WSL로 연결된 VS Code에서 Extensions 화면을 열면 확장 프로그램에 다음과 같은 표시가 나타납니다.

```text
Installed in WSL: Ubuntu
```

Java 확장이 Windows 로컬 영역에만 설치되어 있다면 **Install in WSL: Ubuntu**를 선택합니다.

---

## 7. WSL에서 프로젝트 폴더 열기

### 7.1 프로젝트 저장 위치

WSL 프로젝트는 Linux 홈 디렉터리 아래에 저장하는 것을 권장합니다.

권장 경로:

```text
/home/<사용자명>/projects
```

예:

```bash
mkdir -p ~/projects/java-wsl-demo
cd ~/projects/java-wsl-demo
```

### 7.2 VS Code 실행

프로젝트 디렉터리에서 다음 명령어를 실행합니다.

```bash
code .
```

처음 실행할 때 VS Code Server가 WSL에 자동으로 설치될 수 있습니다.

VS Code 왼쪽 아래 상태 표시줄에 다음과 비슷한 문구가 표시되어야 합니다.

```text
WSL: Ubuntu
```

### 7.3 Windows에서 WSL 창 열기

VS Code에서 다음 순서로 실행할 수도 있습니다.

1. `Ctrl + Shift + P`
2. `WSL: Connect to WSL` 입력
3. Ubuntu 선택
4. `File > Open Folder`
5. `/home/<사용자명>/projects/...` 경로 선택

### 7.4 피해야 할 프로젝트 위치

다음과 같은 Windows 마운트 경로에서도 작업은 가능하지만, Linux 도구 사용 시 파일 성능과 권한 문제가 발생할 수 있습니다.

```text
/mnt/c/Users/<Windows사용자명>/...
```

Java, Maven, Gradle, Git을 WSL에서 실행한다면 프로젝트도 다음과 같이 WSL 파일 시스템에 두는 것이 좋습니다.

```text
~/projects/project-name
```

---

## 8. Java 프로그램 작성 및 실행

### 8.1 프로젝트 폴더 만들기

```bash
mkdir -p ~/projects/java-wsl-demo/src
cd ~/projects/java-wsl-demo
code .
```

### 8.2 Java 파일 작성

`src/Main.java` 파일을 생성합니다.

```java
public class Main {

    public static void main(String[] args) {
        System.out.println("Hello, Java on WSL!");
        System.out.println("Java version: " + System.getProperty("java.version"));
        System.out.println("OS: " + System.getProperty("os.name"));
    }
}
```

### 8.3 VS Code에서 실행

`main` 메서드 위에 표시되는 `Run`을 선택합니다.

또는 편집기 오른쪽 위의 실행 버튼을 선택합니다.

실행 결과는 VS Code의 Terminal 또는 Debug Console에서 확인할 수 있습니다.

예:

```text
Hello, Java on WSL!
Java version: 21...
OS: Linux
```

`OS: Linux`가 출력되면 Java 프로그램이 Windows가 아니라 WSL에서 실행되고 있는 것입니다.

### 8.4 터미널에서 직접 컴파일

프로젝트 루트에서 다음 명령어를 실행합니다.

```bash
javac -d out src/Main.java
```

컴파일된 클래스 파일을 실행합니다.

```bash
java -cp out Main
```

폴더 구조:

```text
java-wsl-demo/
├── out/
│   └── Main.class
└── src/
    └── Main.java
```

---

## 9. Java 디버깅

### 9.1 중단점 설정

Java 파일에서 실행을 멈추고 싶은 줄 번호 왼쪽을 클릭합니다.

빨간 점이 표시되면 중단점이 설정된 것입니다.

### 9.2 디버그 실행

다음 방법 중 하나를 사용합니다.

- `main` 메서드 위의 `Debug` 선택
- `F5` 실행
- Run and Debug 화면에서 실행

### 9.3 launch.json 예시

단순한 Java 프로젝트는 Java 확장이 자동으로 실행 구성을 처리하므로 `launch.json`이 없어도 됩니다.

직접 설정하려면 `.vscode/launch.json`을 생성합니다.

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "type": "java",
      "name": "Debug Main",
      "request": "launch",
      "mainClass": "Main",
      "projectName": "java-wsl-demo"
    }
  ]
}
```

패키지를 사용한다면 `mainClass`에 전체 클래스 이름을 입력합니다.

```json
"mainClass": "com.example.Main"
```

---

## 10. Maven 프로젝트 만들기

### 10.1 Maven 설치

```bash
sudo apt update
sudo apt install -y maven
```

설치를 확인합니다.

```bash
mvn -version
```

출력 내용에서 다음 항목을 확인합니다.

- Maven 버전
- Java 버전
- Java home
- OS name: linux

### 10.2 Maven 프로젝트 생성

```bash
cd ~/projects

mvn archetype:generate \
  -DgroupId=com.example \
  -DartifactId=java-maven-demo \
  -DarchetypeArtifactId=maven-archetype-quickstart \
  -DarchetypeVersion=1.4 \
  -DinteractiveMode=false
```

프로젝트를 엽니다.

```bash
cd java-maven-demo
code .
```

### 10.3 Maven 프로젝트 구조

```text
java-maven-demo/
├── pom.xml
└── src/
    ├── main/
    │   └── java/
    │       └── com/
    │           └── example/
    │               └── App.java
    └── test/
        └── java/
            └── com/
                └── example/
                    └── AppTest.java
```

### 10.4 컴파일 및 테스트

```bash
mvn clean compile
mvn test
```

패키징:

```bash
mvn clean package
```

생성 결과는 `target` 폴더에서 확인합니다.

### 10.5 Java 21 설정 예시

`pom.xml`의 `properties`에 Java 버전을 지정할 수 있습니다.

```xml
<properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.release>21</maven.compiler.release>
</properties>
```

---

## 11. Gradle 프로젝트 만들기

### 11.1 SDKMAN으로 Gradle 설치

Ubuntu 기본 저장소의 Gradle은 버전이 오래될 수 있으므로 SDKMAN 사용이 편리합니다.

SDKMAN 설치:

```bash
curl -s "https://get.sdkman.io" | bash
```

현재 셸에 적용:

```bash
source "$HOME/.sdkman/bin/sdkman-init.sh"
```

설치 확인:

```bash
sdk version
```

Gradle 설치:

```bash
sdk install gradle
```

설치 확인:

```bash
gradle -version
```

### 11.2 Gradle 프로젝트 생성

```bash
cd ~/projects
mkdir java-gradle-demo
cd java-gradle-demo
gradle init
```

대화형 선택 예시:

```text
Type of project: application
Implementation language: Java
Build script DSL: Kotlin 또는 Groovy
Test framework: JUnit Jupiter
Java version: 21
Project name: java-gradle-demo
Application structure: Single application project
```

프로젝트 열기:

```bash
code .
```

빌드:

```bash
./gradlew build
```

실행:

```bash
./gradlew run
```

> 프로젝트에 포함된 Gradle Wrapper인 `./gradlew`를 사용하는 것이 권장됩니다.  
> 개발자와 CI 환경에서 동일한 Gradle 버전을 사용할 수 있습니다.

---

## 12. Git과 GitHub 연결

### 12.1 Git 설치

```bash
sudo apt update
sudo apt install -y git
```

설치 확인:

```bash
git --version
```

### 12.2 사용자 정보 설정

```bash
git config --global user.name "YOUR_NAME"
git config --global user.email "YOUR_EMAIL@example.com"
```

설정 확인:

```bash
git config --global --list
```

### 12.3 줄바꿈 설정

WSL에서는 다음 설정을 권장합니다.

```bash
git config --global core.autocrlf input
```

### 12.4 SSH 키 생성

```bash
ssh-keygen -t ed25519 -C "YOUR_EMAIL@example.com"
```

기본 경로를 사용하려면 Enter를 누릅니다.

SSH 에이전트를 실행합니다.

```bash
eval "$(ssh-agent -s)"
```

키를 등록합니다.

```bash
ssh-add ~/.ssh/id_ed25519
```

공개키를 출력합니다.

```bash
cat ~/.ssh/id_ed25519.pub
```

출력된 공개키를 GitHub의 다음 위치에 등록합니다.

```text
GitHub > Settings > SSH and GPG keys > New SSH key
```

연결을 확인합니다.

```bash
ssh -T git@github.com
```

### 12.5 저장소 초기화 및 업로드

```bash
cd ~/projects/java-wsl-demo

git init
git add .
git commit -m "Initial Java project setup"
git branch -M main
git remote add origin git@github.com:YOUR_GITHUB_ID/YOUR_REPOSITORY.git
git push -u origin main
```

---

## 13. 권장 프로젝트 구조

패키지를 사용하는 기본 Java 프로젝트 예시입니다.

```text
java-wsl-demo/
├── .gitignore
├── README.md
├── pom.xml
├── .vscode/
│   └── settings.json
└── src/
    ├── main/
    │   └── java/
    │       └── com/
    │           └── example/
    │               └── Main.java
    └── test/
        └── java/
            └── com/
                └── example/
                    └── MainTest.java
```

### 13.1 Java 패키지 예시

`src/main/java/com/example/Main.java`

```java
package com.example;

public class Main {

    public static void main(String[] args) {
        System.out.println("Java development with WSL and VS Code");
    }
}
```

### 13.2 .gitignore 예시

프로젝트 루트에 `.gitignore` 파일을 생성합니다.

```gitignore
# Java
*.class
*.jar
*.war
*.ear
hs_err_pid*
replay_pid*

# Maven
target/
pom.xml.tag
pom.xml.releaseBackup
pom.xml.versionsBackup
pom.xml.next
release.properties
dependency-reduced-pom.xml
.mvn/timing.properties

# Gradle
.gradle/
build/
!gradle/wrapper/gradle-wrapper.jar

# VS Code
.vscode/*
!.vscode/extensions.json
!.vscode/settings.json
!.vscode/launch.json

# IntelliJ IDEA
.idea/
*.iml
out/

# Operating system
.DS_Store
Thumbs.db

# Environment files
.env
.env.*
```

### 13.3 VS Code 설정 예시

`.vscode/settings.json`

```json
{
  "files.eol": "\n",
  "editor.formatOnSave": true,
  "java.configuration.updateBuildConfiguration": "automatic",
  "java.compile.nullAnalysis.mode": "automatic",
  "java.format.settings.url": "",
  "terminal.integrated.defaultProfile.linux": "bash"
}
```

### 13.4 권장 확장 공유

`.vscode/extensions.json`

```json
{
  "recommendations": [
    "vscjava.vscode-java-pack",
    "redhat.java",
    "vscjava.vscode-java-debug",
    "vscjava.vscode-java-test",
    "vscjava.vscode-maven"
  ]
}
```

---

## 14. 자주 발생하는 문제 해결

### 문제 1. `code: command not found`

VS Code가 Windows에 설치되어 있는지 확인합니다.

VS Code에서 명령 팔레트를 열고 다음 명령을 확인합니다.

```text
WSL: Connect to WSL
```

WSL 터미널을 종료한 후 다시 실행합니다.

```bash
exit
```

Windows PowerShell에서 WSL을 재시작할 수도 있습니다.

```powershell
wsl --shutdown
```

그다음 Ubuntu를 다시 실행하고 확인합니다.

```bash
code --version
```

---

### 문제 2. `java: command not found`

JDK를 설치합니다.

```bash
sudo apt update
sudo apt install -y openjdk-21-jdk
```

확인:

```bash
which java
which javac
java -version
javac -version
```

---

### 문제 3. VS Code가 Windows JDK를 사용함

VS Code 왼쪽 아래에 `WSL: Ubuntu`가 표시되는지 확인합니다.

VS Code 통합 터미널에서 다음 명령어를 실행합니다.

```bash
which java
java -version
echo "$JAVA_HOME"
uname -a
```

정상적인 `which java` 출력 예:

```text
/usr/bin/java
```

다음처럼 Windows 경로가 나오면 WSL이 아닌 Windows 환경에서 프로젝트를 연 것입니다.

```text
C:\Program Files\Java\...
```

WSL 터미널에서 프로젝트 폴더로 이동한 후 다시 실행합니다.

```bash
code .
```

---

### 문제 4. VS Code가 JDK를 찾지 못함

명령 팔레트에서 다음 명령을 실행합니다.

```text
Java: Configure Java Runtime
```

또는 WSL용 VS Code 사용자 설정에 JDK 경로를 지정합니다.

```json
{
  "java.jdt.ls.java.home": "/usr/lib/jvm/java-21-openjdk-amd64"
}
```

실제 JDK 경로는 다음 명령으로 확인합니다.

```bash
dirname "$(dirname "$(readlink -f "$(which javac)")")"
```

설정 변경 후 다음 명령을 실행할 수 있습니다.

```text
Java: Clean Java Language Server Workspace
```

VS Code 창을 다시 불러옵니다.

```text
Developer: Reload Window
```

---

### 문제 5. Java 확장이 활성화되지 않음

Extensions 화면에서 Java 확장이 다음 영역에 설치되어 있는지 확인합니다.

```text
WSL: Ubuntu - Installed
```

Windows Local에만 설치되어 있다면 다음 버튼을 선택합니다.

```text
Install in WSL: Ubuntu
```

---

### 문제 6. Maven이 다른 Java 버전을 사용함

```bash
mvn -version
```

출력된 Java version과 Java home을 확인합니다.

필요하면 `JAVA_HOME`을 다시 설정합니다.

```bash
export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64
export PATH="$JAVA_HOME/bin:$PATH"
```

영구 적용을 위해 `~/.bashrc`에 추가합니다.

```bash
source ~/.bashrc
mvn -version
```

---

### 문제 7. `/mnt/c` 프로젝트의 빌드가 느림

Windows 파일 시스템과 WSL 파일 시스템 사이의 파일 접근 때문에 성능 차이가 발생할 수 있습니다.

프로젝트를 WSL 홈 디렉터리로 이동합니다.

```bash
mkdir -p ~/projects
cp -r /mnt/c/Users/YOUR_NAME/path/project ~/projects/
cd ~/projects/project
code .
```

Git 저장소라면 복사보다 WSL 내부에 다시 clone하는 방법이 안전합니다.

```bash
cd ~/projects
git clone git@github.com:YOUR_ID/YOUR_REPOSITORY.git
```

---

### 문제 8. 파일 권한 오류

현재 소유권을 확인합니다.

```bash
ls -al
```

프로젝트 폴더가 root 소유라면 사용자 소유로 변경합니다.

```bash
sudo chown -R "$USER":"$USER" ~/projects/project-name
```

프로젝트 생성이나 빌드 명령에 불필요하게 `sudo`를 사용하지 않습니다.

잘못된 예:

```bash
sudo mvn package
sudo gradle build
sudo code .
```

권장 예:

```bash
mvn package
./gradlew build
code .
```

---

### 문제 9. WSL 네트워크 또는 상태 이상

WSL을 종료합니다.

```powershell
wsl --shutdown
```

WSL을 업데이트합니다.

```powershell
wsl --update
```

설치 상태를 확인합니다.

```powershell
wsl --status
wsl --list --verbose
```

Ubuntu를 다시 실행합니다.

---

### 문제 10. Java Language Server가 계속 로딩됨

명령 팔레트에서 다음 명령을 순서대로 실행합니다.

```text
Java: Clean Java Language Server Workspace
Developer: Reload Window
```

프로젝트 루트가 올바르게 열렸는지 확인합니다.

Maven 프로젝트:

```text
pom.xml이 있는 폴더
```

Gradle 프로젝트:

```text
build.gradle 또는 build.gradle.kts가 있는 폴더
```

---

## 15. 최종 점검 체크리스트

### WSL

- [ ] `wsl --list --verbose`에서 Ubuntu가 WSL 2로 표시된다.
- [ ] Ubuntu 패키지 업데이트를 완료했다.
- [ ] 프로젝트가 `/home/<사용자명>/projects` 아래에 있다.

### Java

- [ ] `java -version`이 정상 출력된다.
- [ ] `javac -version`이 정상 출력된다.
- [ ] `echo $JAVA_HOME`이 WSL의 JDK 경로를 출력한다.
- [ ] `which java`가 `/usr/bin/java` 또는 Linux 경로를 출력한다.

### VS Code

- [ ] Windows에 VS Code가 설치되어 있다.
- [ ] WSL 확장이 설치되어 있다.
- [ ] Extension Pack for Java가 설치되어 있다.
- [ ] Java 확장이 `WSL: Ubuntu` 환경에 설치되어 있다.
- [ ] VS Code 왼쪽 아래에 `WSL: Ubuntu`가 표시된다.

### 실행 및 디버깅

- [ ] Java 파일의 `main` 메서드를 실행할 수 있다.
- [ ] 터미널에서 `javac`와 `java` 명령을 실행할 수 있다.
- [ ] 중단점을 설정하고 `F5`로 디버깅할 수 있다.
- [ ] Maven 또는 Gradle 빌드가 성공한다.

### GitHub

- [ ] WSL에 Git이 설치되어 있다.
- [ ] Git 사용자 이름과 이메일을 설정했다.
- [ ] SSH 키를 GitHub에 등록했다.
- [ ] `git push`가 정상적으로 실행된다.
- [ ] `.gitignore`에 빌드 결과물이 제외되어 있다.

---

## 빠른 설치 명령 모음

### Windows PowerShell 관리자 권한

```powershell
wsl --install
wsl --update
wsl --list --verbose
```

### WSL Ubuntu

```bash
sudo apt update
sudo apt upgrade -y

sudo apt install -y \
  openjdk-21-jdk \
  git \
  maven \
  curl \
  wget \
  unzip \
  zip \
  build-essential

java -version
javac -version
git --version
mvn -version
```

### 프로젝트 생성

```bash
mkdir -p ~/projects/java-wsl-demo/src
cd ~/projects/java-wsl-demo
code .
```

---

## 참고 문서

- Microsoft WSL 설치 가이드  
  <https://learn.microsoft.com/windows/wsl/install>

- Visual Studio Code WSL 개발 가이드  
  <https://code.visualstudio.com/docs/remote/wsl>

- Visual Studio Code Java 시작 가이드  
  <https://code.visualstudio.com/docs/java/java-tutorial>

- Visual Studio Code Java 문서  
  <https://code.visualstudio.com/docs/languages/java>

- Eclipse Adoptium Temurin 설치 안내  
  <https://adoptium.net/installation/>

---
