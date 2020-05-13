package ru.nsu;

import javax.swing.*;
import java.sql.Connection;
import java.sql.DriverManager;

public class Authorization {
    private static final String passwordDB = "accesssosat";
    private static final String loginDB = "c##gafiyatullin";
    private static final String addressDB = "192.168.56.2:1521";
    private static final String SID = "orcl";
    private JPanel mainPanel;
    private JTextField loginTextField;
    private JPasswordField passwordField1;
    private JButton enterButton;
    private JLabel passwordTextField;

    public static void main(String[] args) throws Exception {
        Class.forName("oracle.jdbc.driver.OracleDriver");
        Connection connection = DriverManager.getConnection("jdbc:oracle:thin:@" + addressDB
                + ":" + SID, loginDB, passwordDB);

        // TODO: don't forget change that test code
        Show show = new Show(connection);
        Authors authors = new Authors(connection);
        Employee employee = new Employee(connection);
        ActorsInfo actors_info = new ActorsInfo(connection);
        ActorsEditing actorsEditing = new ActorsEditing(connection);
        Directors directors = new Directors(connection);
    }
}
