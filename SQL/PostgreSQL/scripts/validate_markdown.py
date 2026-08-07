#!/usr/bin/env python3
"""교재 Markdown의 기본 구조와 로컬 링크를 검사한다."""

from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
CHAPTERS = sorted((ROOT / "chapters").glob("*.md"))
SQL_FILES = sorted((ROOT / "examples").glob("*.sql"))
MARKDOWN_FILES = [ROOT / "README.md", *CHAPTERS, *sorted((ROOT / "references").glob("*.md"))]
REQUIRED_HEADINGS = (
    "이 장에서 배울 내용",
    "선행 지식",
    "원리 이해",
    "주의 및 오류 해결",
    "실습 문제",
    "실습 문제 정답",
    "핵심 정리",
    "확인 문제",
    "다음 장 안내",
)
LINK_PATTERN = re.compile(r"(?<!!)\[[^]]+\]\(([^)]+)\)")
LOCAL_CODE_PATH_PATTERN = re.compile(r"`((?:chapters|examples|references|scripts)/[A-Za-z0-9_./-]+)`")


def validate_file(path: Path) -> list[str]:
    errors: list[str] = []
    text = path.read_text(encoding="utf-8")

    if text.count("```") % 2:
        errors.append(f"{path.relative_to(ROOT)}: 닫히지 않은 코드 블록")

    for raw_target in LINK_PATTERN.findall(text):
        target = raw_target.strip().strip("<>").split("#", 1)[0]
        if not target or "://" in target or target.startswith("mailto:"):
            continue
        linked_path = (path.parent / target).resolve()
        if not linked_path.exists():
            errors.append(
                f"{path.relative_to(ROOT)}: 존재하지 않는 로컬 링크 {raw_target}"
            )

    for raw_target in LOCAL_CODE_PATH_PATTERN.findall(text):
        if not (ROOT / raw_target).exists():
            errors.append(
                f"{path.relative_to(ROOT)}: 존재하지 않는 로컬 파일 표기 {raw_target}"
            )

    return errors


def main() -> int:
    errors: list[str] = []

    for path in MARKDOWN_FILES:
        errors.extend(validate_file(path))

    chapter_numbers = [int(path.name.split("_", 1)[0]) for path in CHAPTERS]
    expected_numbers = list(range(1, 13))
    if chapter_numbers != expected_numbers:
        errors.append(f"장 번호 불일치: 예상 {expected_numbers}, 실제 {chapter_numbers}")

    for chapter in CHAPTERS:
        text = chapter.read_text(encoding="utf-8")
        for heading in REQUIRED_HEADINGS:
            if not re.search(rf"^## (?:\d+\. )?{re.escape(heading)}", text, re.MULTILINE):
                errors.append(f"{chapter.relative_to(ROOT)}: 필수 절 누락: {heading}")

    for sql_file in SQL_FILES:
        sql_text = sql_file.read_text(encoding="utf-8")
        if "\\set ON_ERROR_STOP on" not in sql_text:
            errors.append(f"{sql_file.relative_to(ROOT)}: \\set ON_ERROR_STOP on 누락")
        if re.search(r"\bPASSWORD\s+'", sql_text, re.IGNORECASE):
            errors.append(f"{sql_file.relative_to(ROOT)}: 평문 PASSWORD 리터럴 발견")

    schema_text = (ROOT / "examples" / "08_create_schema.sql").read_text(encoding="utf-8")
    schema_tables = set(re.findall(r"^CREATE TABLE ([a-z_]+) [(]", schema_text, re.MULTILINE))
    expected_tables = {"department", "app_user", "project", "project_member", "task", "task_comment", "task_history"}
    if schema_tables != expected_tables:
        errors.append(f"examples/08_create_schema.sql: 기준 테이블 불일치: {sorted(schema_tables)}")

    if errors:
        print("Markdown 검증 실패:")
        for error in errors:
            print(f"- {error}")
        return 1

    print(f"교재 검증 통과: Markdown {len(MARKDOWN_FILES)}개, 장 {len(CHAPTERS)}개, SQL 예제 {len(SQL_FILES)}개")
    return 0


if __name__ == "__main__":
    sys.exit(main())
