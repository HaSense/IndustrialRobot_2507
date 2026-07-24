package com.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Scanner;

public class InsertTest {
    public static void main(String[] args) {
        String url = "jdbc:sqlite:data/study.db";
        try (Connection connection = DriverManager.getConnection(url)) {
            System.out.println("데이터베이스 연결 성공");
        } catch (SQLException e) {
            System.out.println("연결 실패: " + e.getMessage());
        }

        String sql = "INSERT INTO book(title, author, price) VALUES (?, ?, ?)";

        try (Connection con = DriverManager.getConnection(url);
            PreparedStatement ps = con.prepareStatement(sql);
            Scanner sc = new Scanner(System.in)
            ) 
        {       
                System.out.print("책 제목을 입력하세요 : ");
                String title = sc.nextLine();
                ps.setString(1, title);
                System.out.print("작가 이름을 입력하세요 : ");
                String author = sc.nextLine();
                ps.setString(2, author);
                System.out.print("책 가격을 입력하세요 : ");
                int price = sc.nextInt();
                ps.setInt(3, price);
                int count = ps.executeUpdate();
                System.out.println(count + "권 등록");
        }catch (SQLException e) {
            System.out.println("연결 실패: " + e.getMessage());
        }
    }
}
