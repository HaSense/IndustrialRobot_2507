# Java Strategy 패턴

Strategy 패턴은 **변경될 수 있는 알고리즘이나 동작을 별도의 객체로 분리하고, 실행 중에 교체할 수 있도록 만드는 설계 패턴**입니다.

예를 들어 주문 프로그램에 다음과 같은 결제 방식이 있다고 가정합니다.

- 현금 결제
- 카드 결제
- 간편 결제

결제 방식마다 처리 방법은 다르지만 `Order` 입장에서는 모두 다음과 같이 사용할 수 있습니다.

```java
paymentStrategy.pay(amount);
```

`Order`는 현재 전략이 현금인지, 카드인지, 간편 결제인지 알 필요가 없습니다.

---

## 1. Strategy 패턴을 사용하지 않은 코드

```java
public class Order {

    public void pay(String paymentType, int amount) {
        if (paymentType.equals("cash")) {
            System.out.println(
                "현금으로 " + amount + "원 결제합니다."
            );

        } else if (paymentType.equals("card")) {
            System.out.println(
                "카드로 " + amount + "원 결제합니다."
            );

        } else if (paymentType.equals("simple")) {
            System.out.println(
                "간편 결제로 " + amount + "원 결제합니다."
            );

        } else {
            throw new IllegalArgumentException(
                "지원하지 않는 결제 방식입니다."
            );
        }
    }
}
```

처음에는 간단해 보이지만 결제 방식이 늘어나면 조건문도 계속 증가합니다.

```java
if (paymentType.equals("cash")) {
    ...
} else if (paymentType.equals("card")) {
    ...
} else if (paymentType.equals("simple")) {
    ...
} else if (paymentType.equals("point")) {
    ...
} else if (paymentType.equals("bank")) {
    ...
}
```

이 코드에는 다음 문제가 있습니다.

- 새로운 결제 방식이 추가될 때 `Order` 클래스를 수정해야 합니다.
- 주문 로직과 결제 로직이 한 클래스에 섞입니다.
- 조건문이 점점 길어집니다.
- 결제 방식별 단위 테스트가 어려워집니다.
- 결제 방식 하나를 변경해도 `Order` 전체를 수정해야 합니다.

Strategy 패턴은 서로 다른 결제 알고리즘을 별도의 클래스로 분리하여 이러한 문제를 해결합니다.

---

## 2. Strategy 패턴의 구성 요소

| 구성 요소 | 역할 | 예제 클래스 |
|---|---|---|
| Strategy | 교체 가능한 동작을 선언하는 인터페이스 | `PaymentStrategy` |
| Concrete Strategy | 실제 알고리즘을 구현하는 클래스 | `CashPayment`, `CardPayment`, `SimplePayment` |
| Context | Strategy를 사용하여 작업을 수행하는 클래스 | `Order` |
| Client | 사용할 Strategy를 선택하는 코드 | `Main` |

---

## 3. UML 클래스 다이어그램

![Strategy 패턴 UML 클래스 다이어그램](images/strategy_class_diagram.png)

핵심 관계는 다음과 같습니다.

```text
                      ┌─ CashPayment
Order ──사용──> PaymentStrategy
                      ├─ CardPayment
                      └─ SimplePayment
```

`Order`는 구체적인 결제 클래스가 아니라 `PaymentStrategy` 인터페이스에 의존합니다.

---

## 4. 핵심 구조

```java
public interface PaymentStrategy {
    void pay(int amount);
}
```

```java
public class CardPayment implements PaymentStrategy {

    @Override
    public void pay(int amount) {
        System.out.println(
            "카드 결제: " + amount + "원"
        );
    }
}
```

```java
public class Order {

    private PaymentStrategy paymentStrategy;

    public void setPaymentStrategy(
            PaymentStrategy paymentStrategy
    ) {
        this.paymentStrategy = paymentStrategy;
    }
}
```

`Order`는 결제 알고리즘을 직접 구현하지 않습니다.

실제 결제는 Strategy 객체에 위임합니다.

```java
paymentStrategy.pay(totalAmount);
```

---

## 5. 프로젝트 구조

```text
strategy-example/
├── pom.xml
└── src/
    └── main/
        └── java/
            └── com/
                └── example/
                    ├── Main.java
                    ├── order/
                    │   └── Order.java
                    └── payment/
                        ├── PaymentStrategy.java
                        ├── CashPayment.java
                        ├── CardPayment.java
                        └── SimplePayment.java
```

---

## 6. Strategy 인터페이스

### `PaymentStrategy.java`

```java
package com.example.payment;

public interface PaymentStrategy {

    void pay(int amount);

    String getName();
}
```

이 인터페이스는 모든 결제 전략이 제공해야 하는 공통 기능을 선언합니다.

```java
void pay(int amount);
```

결제 방법이 현금인지 카드인지 간편 결제인지는 인터페이스가 결정하지 않습니다.

구체적인 결제 알고리즘은 각각의 구현 클래스가 담당합니다.

---

## 7. 현금 결제 전략

### `CashPayment.java`

```java
package com.example.payment;

public class CashPayment implements PaymentStrategy {

    @Override
    public void pay(int amount) {
        validateAmount(amount);

        System.out.println(
            "현금으로 " + amount + "원을 결제합니다."
        );
    }

    @Override
    public String getName() {
        return "현금 결제";
    }

    private void validateAmount(int amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException(
                "결제 금액은 0원보다 커야 합니다."
            );
        }
    }
}
```

`CashPayment`는 현금 결제 알고리즘만 담당합니다.

---

## 8. 카드 결제 전략

### `CardPayment.java`

```java
package com.example.payment;

public class CardPayment implements PaymentStrategy {

    private final String cardCompany;

    public CardPayment(String cardCompany) {
        if (cardCompany == null || cardCompany.isBlank()) {
            throw new IllegalArgumentException(
                "카드사 이름을 입력해야 합니다."
            );
        }

        this.cardCompany = cardCompany;
    }

    @Override
    public void pay(int amount) {
        validateAmount(amount);

        System.out.println(
            cardCompany + " 카드로 "
            + amount + "원을 결제합니다."
        );
    }

    @Override
    public String getName() {
        return cardCompany + " 카드 결제";
    }

    private void validateAmount(int amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException(
                "결제 금액은 0원보다 커야 합니다."
            );
        }
    }
}
```

카드 결제에 필요한 카드사 정보는 생성자를 통해 전달합니다.

```java
PaymentStrategy cardPayment =
        new CardPayment("국민");
```

---

## 9. 간편 결제 전략

### `SimplePayment.java`

```java
package com.example.payment;

public class SimplePayment implements PaymentStrategy {

    private final String provider;

    public SimplePayment(String provider) {
        if (provider == null || provider.isBlank()) {
            throw new IllegalArgumentException(
                "간편 결제 서비스 이름을 입력해야 합니다."
            );
        }

        this.provider = provider;
    }

    @Override
    public void pay(int amount) {
        validateAmount(amount);

        System.out.println(
            provider + "로 "
            + amount + "원을 결제합니다."
        );
    }

    @Override
    public String getName() {
        return provider + " 간편 결제";
    }

    private void validateAmount(int amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException(
                "결제 금액은 0원보다 커야 합니다."
            );
        }
    }
}
```

---

## 10. Context 클래스

### `Order.java`

```java
package com.example.order;

import com.example.payment.PaymentStrategy;

public class Order {

    private final int orderId;
    private final int totalAmount;

    private PaymentStrategy paymentStrategy;

    public Order(int orderId, int totalAmount) {
        if (totalAmount <= 0) {
            throw new IllegalArgumentException(
                "주문 금액은 0원보다 커야 합니다."
            );
        }

        this.orderId = orderId;
        this.totalAmount = totalAmount;
    }

    public void setPaymentStrategy(
            PaymentStrategy paymentStrategy
    ) {
        if (paymentStrategy == null) {
            throw new IllegalArgumentException(
                "결제 전략이 필요합니다."
            );
        }

        this.paymentStrategy = paymentStrategy;
    }

    public void checkout() {
        if (paymentStrategy == null) {
            throw new IllegalStateException(
                "결제 방식을 먼저 선택해야 합니다."
            );
        }

        System.out.println("주문 번호: " + orderId);
        System.out.println(
            "결제 방식: " + paymentStrategy.getName()
        );

        paymentStrategy.pay(totalAmount);
    }
}
```

`Order` 클래스는 실제 결제 방법을 구현하지 않습니다.

다음 코드로 결제 작업을 Strategy 객체에 위임합니다.

```java
paymentStrategy.pay(totalAmount);
```

이러한 방식을 **위임(Delegation)**이라고 합니다.

---

## 11. Main 클래스

### `Main.java`

```java
package com.example;

import com.example.order.Order;
import com.example.payment.CardPayment;
import com.example.payment.CashPayment;
import com.example.payment.PaymentStrategy;
import com.example.payment.SimplePayment;

public class Main {

    public static void main(String[] args) {
        Order order = new Order(1001, 35_000);

        PaymentStrategy cashPayment =
                new CashPayment();

        order.setPaymentStrategy(cashPayment);
        order.checkout();

        System.out.println();

        PaymentStrategy cardPayment =
                new CardPayment("국민");

        order.setPaymentStrategy(cardPayment);
        order.checkout();

        System.out.println();

        PaymentStrategy simplePayment =
                new SimplePayment("카카오페이");

        order.setPaymentStrategy(simplePayment);
        order.checkout();
    }
}
```

같은 `Order` 객체를 사용하면서 결제 전략만 교체할 수 있습니다.

```java
order.setPaymentStrategy(new CashPayment());
order.checkout();

order.setPaymentStrategy(new CardPayment("국민"));
order.checkout();

order.setPaymentStrategy(new SimplePayment("카카오페이"));
order.checkout();
```

---

## 12. 실행 결과

```text
주문 번호: 1001
결제 방식: 현금 결제
현금으로 35000원을 결제합니다.

주문 번호: 1001
결제 방식: 국민 카드 결제
국민 카드로 35000원을 결제합니다.

주문 번호: 1001
결제 방식: 카카오페이 간편 결제
카카오페이로 35000원을 결제합니다.
```

동일한 `checkout()` 메서드를 실행했지만 내부에서 사용하는 결제 알고리즘이 달라집니다.

---

## 13. 실행 흐름

![Strategy 패턴 실행 흐름](images/strategy_execution_flow.png)

실행 순서는 다음과 같습니다.

1. `Main`이 사용할 결제 전략을 생성합니다.
2. `Main`이 `Order`에 결제 전략을 전달합니다.
3. `Order`가 `checkout()`을 실행합니다.
4. `Order`가 `PaymentStrategy.pay()`를 호출합니다.
5. 실제 객체의 결제 알고리즘이 실행됩니다.

`PaymentStrategy` 타입으로 호출했지만 실제로는 설정된 객체의 메서드가 실행됩니다.

이를 **다형성에 의한 동적 바인딩**이라고 합니다.

---

## 14. Strategy 객체를 생성자로 전달하기

Strategy는 setter가 아니라 생성자를 통해 전달할 수도 있습니다.

### 생성자 주입 방식

```java
package com.example.order;

import com.example.payment.PaymentStrategy;

public class Order {

    private final int orderId;
    private final int totalAmount;
    private final PaymentStrategy paymentStrategy;

    public Order(
            int orderId,
            int totalAmount,
            PaymentStrategy paymentStrategy
    ) {
        if (paymentStrategy == null) {
            throw new IllegalArgumentException(
                "결제 전략이 필요합니다."
            );
        }

        this.orderId = orderId;
        this.totalAmount = totalAmount;
        this.paymentStrategy = paymentStrategy;
    }

    public void checkout() {
        paymentStrategy.pay(totalAmount);
    }
}
```

사용 방법은 다음과 같습니다.

```java
Order order = new Order(
    1001,
    35_000,
    new CardPayment("국민")
);

order.checkout();
```

### setter 주입과 생성자 주입 비교

| 방식 | 특징 |
|---|---|
| setter 주입 | 실행 중 Strategy를 자유롭게 교체할 수 있음 |
| 생성자 주입 | 객체 생성 시 Strategy가 반드시 결정됨 |
| setter 주입 | Strategy 미설정 상태가 발생할 수 있음 |
| 생성자 주입 | 필드를 `final`로 만들 수 있어 안정적임 |

실행 중 알고리즘을 변경해야 한다면 setter 방식이 적합합니다.

객체가 생성될 때 알고리즘이 반드시 결정되어야 한다면 생성자 방식이 적합합니다.

---

## 15. 새로운 전략 추가

포인트 결제 기능을 추가한다고 가정합니다.

### `PointPayment.java`

```java
package com.example.payment;

public class PointPayment implements PaymentStrategy {

    private final String memberId;

    public PointPayment(String memberId) {
        this.memberId = memberId;
    }

    @Override
    public void pay(int amount) {
        System.out.println(
            memberId + " 회원의 포인트로 "
            + amount + "원을 결제합니다."
        );
    }

    @Override
    public String getName() {
        return "포인트 결제";
    }
}
```

사용 코드는 다음과 같습니다.

```java
order.setPaymentStrategy(
    new PointPayment("MEMBER-1001")
);

order.checkout();
```

새로운 결제 방식이 추가되었지만 `Order` 클래스는 수정하지 않았습니다.

이 구조는 객체지향 설계의 **개방-폐쇄 원칙(Open-Closed Principle)**과 관련이 있습니다.

> 기능 확장에는 열려 있고 기존 코드 수정에는 닫혀 있어야 한다.

---

## 16. 할인 정책 예제로 확장하기

Strategy 패턴은 결제 방식뿐 아니라 할인 정책에도 사용할 수 있습니다.

### 할인 전략 인터페이스

```java
public interface DiscountStrategy {

    int calculateDiscount(int price);
}
```

### 할인 없음

```java
public class NoDiscount
        implements DiscountStrategy {

    @Override
    public int calculateDiscount(int price) {
        return 0;
    }
}
```

### 정률 할인

```java
public class RateDiscount
        implements DiscountStrategy {

    private final int rate;

    public RateDiscount(int rate) {
        if (rate < 0 || rate > 100) {
            throw new IllegalArgumentException(
                "할인율은 0부터 100 사이여야 합니다."
            );
        }

        this.rate = rate;
    }

    @Override
    public int calculateDiscount(int price) {
        return price * rate / 100;
    }
}
```

### 정액 할인

```java
public class FixedDiscount
        implements DiscountStrategy {

    private final int discountAmount;

    public FixedDiscount(int discountAmount) {
        if (discountAmount < 0) {
            throw new IllegalArgumentException(
                "할인 금액은 0원 이상이어야 합니다."
            );
        }

        this.discountAmount = discountAmount;
    }

    @Override
    public int calculateDiscount(int price) {
        return Math.min(price, discountAmount);
    }
}
```

---

## 17. Strategy 패턴을 적용하기 좋은 경우

Strategy 패턴은 다음 조건에서 효과적입니다.

- 같은 목적을 가진 알고리즘이 여러 개 존재합니다.
- 실행 중에 알고리즘을 교체해야 합니다.
- 긴 `if-else` 또는 `switch` 문이 알고리즘 선택에 사용됩니다.
- 알고리즘별로 독립적인 테스트가 필요합니다.
- 새로운 알고리즘이 자주 추가될 가능성이 있습니다.

대표적인 적용 대상은 다음과 같습니다.

- 결제 방식
- 할인 정책
- 배송비 계산
- 정렬 방법
- 파일 저장 방식
- 압축 방식
- 인증 방식
- 메시지 전송 방식

---

## 18. Strategy 패턴의 장점

### 18.1 조건문 감소

알고리즘 선택을 위한 긴 조건문을 줄일 수 있습니다.

### 18.2 책임 분리

각 전략 클래스는 하나의 알고리즘만 담당합니다.

### 18.3 실행 중 전략 교체

같은 Context 객체에서도 Strategy 객체를 바꿀 수 있습니다.

```java
order.setPaymentStrategy(new CashPayment());
order.setPaymentStrategy(new CardPayment("국민"));
```

### 18.4 확장성 향상

새로운 전략 클래스를 추가해도 기존 Context 코드를 수정하지 않을 수 있습니다.

### 18.5 테스트 용이성

각 결제 전략을 독립적으로 테스트할 수 있습니다.

---

## 19. Strategy 패턴의 단점

Strategy 패턴이 항상 좋은 것은 아닙니다.

### 19.1 클래스 수 증가

알고리즘마다 별도의 클래스가 필요하므로 파일 수가 증가합니다.

### 19.2 Client가 전략을 알아야 함

`Main` 또는 Controller는 어떤 전략을 사용할지 선택해야 합니다.

### 19.3 단순한 조건문에는 과도할 수 있음

알고리즘이 두 개뿐이고 거의 변경되지 않는다면 Strategy 패턴이 오히려 복잡도를 높일 수 있습니다.

### 19.4 공통 코드 중복 가능성

여러 Strategy 구현체에 검증 코드나 공통 처리 코드가 반복될 수 있습니다.

---

## 20. Repository 패턴과 Strategy 패턴 비교

두 패턴 모두 인터페이스와 구현 클래스를 사용하지만 목적이 다릅니다.

| 구분 | Repository 패턴 | Strategy 패턴 |
|---|---|---|
| 주요 목적 | 데이터 접근 로직 분리 | 알고리즘 교체 |
| 인터페이스 예 | `BookRepository` | `PaymentStrategy` |
| 구현체 예 | JDBC, Memory 저장소 | 현금, 카드, 간편 결제 |
| 사용하는 클래스 | Service | Context |
| 교체 대상 | 저장 방식 | 처리 알고리즘 |
| 공통 원리 | 구체 클래스보다 인터페이스에 의존 | 구체 클래스보다 인터페이스에 의존 |

Repository도 넓게 보면 저장 방식을 교체하는 전략과 비슷한 구조를 가질 수 있습니다.

하지만 Repository 패턴은 **도메인 객체의 저장과 조회를 추상화하는 목적**이 더 분명합니다.

Strategy 패턴은 **행동이나 알고리즘 자체를 교체하는 목적**에 집중합니다.

---

## 21. 수업에서 강조할 코드

Strategy 패턴 수업에서는 다음 세 부분을 집중적으로 설명하면 됩니다.

### Strategy 인터페이스

```java
public interface PaymentStrategy {
    void pay(int amount);
}
```

### Concrete Strategy

```java
public class CardPayment
        implements PaymentStrategy {

    @Override
    public void pay(int amount) {
        // 카드 결제 알고리즘
    }
}
```

### Context

```java
public class Order {

    private PaymentStrategy paymentStrategy;

    public void setPaymentStrategy(
            PaymentStrategy paymentStrategy
    ) {
        this.paymentStrategy = paymentStrategy;
    }

    public void checkout() {
        paymentStrategy.pay(totalAmount);
    }
}
```

학생들에게는 다음 세 문장으로 정리할 수 있습니다.

1. **Strategy 인터페이스는 교체 가능한 행동을 정의한다.**
2. **각 구현 클래스는 서로 다른 행동을 구현한다.**
3. **Context는 직접 처리하지 않고 Strategy에 작업을 위임한다.**

---

## 22. 수업 진행 권장 순서

### 1단계: 조건문 코드 제시

```java
if (paymentType.equals("cash")) {
    ...
} else if (paymentType.equals("card")) {
    ...
}
```

먼저 결제 방식이 늘어날 때 조건문이 길어지는 문제를 보여줍니다.

### 2단계: 인터페이스 분리

```java
public interface PaymentStrategy {
    void pay(int amount);
}
```

### 3단계: 구현 클래스 작성

```text
CashPayment
CardPayment
SimplePayment
```

### 4단계: Order에 Strategy 필드 추가

```java
private PaymentStrategy paymentStrategy;
```

### 5단계: 실행 중 전략 교체

```java
order.setPaymentStrategy(new CashPayment());
order.checkout();

order.setPaymentStrategy(new CardPayment("국민"));
order.checkout();
```

### 6단계: 새로운 전략 추가 실습

학생들이 직접 `PointPayment`를 구현하게 합니다.

---

## 23. 실습 문제

### 실습 1: 포인트 결제 전략

다음 조건을 만족하는 `PointPayment` 클래스를 작성합니다.

- `PaymentStrategy`를 구현합니다.
- 회원 번호를 필드로 가집니다.
- 결제 시 회원 번호와 결제 금액을 출력합니다.

### 실습 2: 계좌이체 전략

다음 조건을 만족하는 `BankTransferPayment` 클래스를 작성합니다.

- 은행 이름을 필드로 가집니다.
- 계좌이체 결제 메시지를 출력합니다.
- 결제 금액이 0원 이하이면 예외를 발생시킵니다.

### 실습 3: 할인 전략

다음 할인 전략을 구현합니다.

- 할인 없음
- 10% 할인
- 5,000원 정액 할인

---

## 24. 최종 정리

Strategy 패턴의 전체 구조는 다음과 같습니다.

```text
Client
  ↓ 전략 선택
Context
  ↓ 인터페이스 호출
Strategy
  ↓
Concrete Strategy
```

결제 예제에서는 다음과 같습니다.

```text
Main
  ↓ 결제 방식 선택
Order
  ↓ pay(amount)
PaymentStrategy
  ↓
CashPayment / CardPayment / SimplePayment
```

Strategy 패턴의 핵심은 다음 한 줄입니다.

```java
private PaymentStrategy paymentStrategy;
```

이 코드는 `Order`가 특정 결제 구현이 아니라 **교체 가능한 결제 행동의 추상화**에 의존한다는 의미입니다.

> Strategy 패턴은 변경되는 행동을 분리하고, 그 행동을 객체로 교체할 수 있게 만드는 패턴이다.
