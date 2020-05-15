package ru.nsu;

import oracle.jdbc.OracleTypes;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;

public class Authorization extends JFrame {
    private static final String passwordDB = "123456789";
    private static final String loginDB = "17208-gafiyatullin";
    private static final String addressDB = "84.237.50.81:1521";
    private static final String SID = "XE";

    private JPanel mainPanel;
    private JTextField loginTextField;
    private final CallableStatement loginQuery;
    private JPasswordField passwordTextField;
    private JButton login;
    private JButton loginAsGuest;

    public Authorization(final Connection connection) throws Exception {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        loginQuery = connection.prepareCall("{call login(?, ?, ?)}");
        loginQuery.registerOutParameter(3, OracleTypes.VARCHAR);

        login.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (loginTextField.getText().isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Введите имя пользователя!",
                                "Ошибка входа", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    if (passwordTextField.getPassword().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Введите пароль!",
                                "Ошибка входа", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    loginQuery.setString(1, loginTextField.getText());
                    loginQuery.setString(2, new String(passwordTextField.getPassword()));
                    loginQuery.execute();

                    String role = loginQuery.getString(3);
                    new Theater(connection, role);

                    setVisible(false);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка входа", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        loginAsGuest.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String role = "user";
                    new Theater(connection, role);

                    setVisible(false);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка входа", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setVisible(true);
    }

    public static void main(String[] args) throws Exception {
        Class.forName("oracle.jdbc.driver.OracleDriver");
        Connection connection = DriverManager.getConnection("jdbc:oracle:thin:@" + addressDB
                + ":" + SID, loginDB, passwordDB);

        Authorization authorization = new Authorization(connection);
    }
}
