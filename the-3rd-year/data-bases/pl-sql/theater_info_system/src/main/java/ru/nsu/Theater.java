package ru.nsu;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;

public class Theater extends JFrame {
    private JPanel mainPanel;
    private JButton actorInfo;
    private JButton authors;
    private JButton directors;
    private JButton showsInfo;
    private JButton tours;
    private JButton employees;
    private JButton musicians;
    private JButton actorsEditinig;
    private JButton showsEditing;
    private JButton ticketsAdding;
    private JButton ticketsSelling;
    private JButton statistics;

    public Theater(final Connection connection, final String role) {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        switch (role) {
            case "user":
                ticketsSelling.setVisible(false);
                statistics.setVisible(false);
                employees.setVisible(false);
                actorsEditinig.setVisible(false);
                showsEditing.setVisible(false);
                ticketsAdding.setVisible(false);
                break;
            case "admin":
                ticketsSelling.setVisible(false);
                employees.setVisible(false);
                actorsEditinig.setVisible(false);
                showsEditing.setVisible(false);
                ticketsAdding.setVisible(false);
                break;
            case "headmaster":
                ticketsSelling.setVisible(false);
                break;
            case "cashier":
                statistics.setVisible(false);
                employees.setVisible(false);
                actorsEditinig.setVisible(false);
                showsEditing.setVisible(false);
                ticketsAdding.setVisible(false);
                break;
        }

        actorInfo.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new ActorsInfo(connection);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        authors.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new Authors(connection, role);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        directors.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new Directors(connection, role);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        musicians.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new Musicians(connection, role);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        showsInfo.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new ShowsInfo(connection);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        tours.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new Tours(connection, role);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        ticketsSelling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

            }
        });

        statistics.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

            }
        });

        employees.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new Employees(connection);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        actorsEditinig.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    new ActorsEditing(connection);
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        showsEditing.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

            }
        });

        ticketsAdding.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

            }
        });

        setContentPane(mainPanel);
        pack();
        setVisible(true);
    }

}
