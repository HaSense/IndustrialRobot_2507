# Chapter 02. Maven 프로젝트와 데이터베이스 연결

## 1. 개념 설명

Maven은 Java 프로젝트의 외부 라이브러리를 관리합니다. SQLite JDBC 드라이버를 `pom.xml`에 등록하면 Java가 SQLite 파일에 연결할 수 있습니다.

## 2. 설정과 Java 코드

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0">
    <modelVersion>4.0.0</modelVersion>
    <groupId>edu.example</groupId>
    <artifactId>sqlite-jdbc-study</artifactId>
    <version>1.0-SNAPSHOT</version>
    <properties>
        <maven.compiler.release>17</maven.compiler.release>
    </properties>
    <dependencies>
        <dependency>
            <groupId>org.xerial</groupId>
            <artifactId>sqlite-jdbc</artifactId>
            <version>3.50.3.0</version>
        </dependency>
    </dependencies>
</project>
```

```java
package app;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class Main {
    public static void main(String[] args) {
        String url = "jdbc:sqlite:data/study.db";

        try (Connection connection = DriverManager.getConnection(url)) {
            System.out.println("데이터베이스 연결 성공");
        } catch (SQLException e) {
            System.out.println("연결 실패: " + e.getMessage());
        }
    }
}
```

## 3. 실행 결과

```text
데이터베이스 연결 성공
```

## 4. 실습

데이터베이스 파일명을 `library.db`로 변경하고 연결해 보세요.

## 5. 정답

```java
String url = "jdbc:sqlite:data/library.db";
try (Connection connection = DriverManager.getConnection(url)) {
    System.out.println("library.db 연결 성공");
} catch (SQLException e) {
    System.out.println(e.getMessage());
}
```

