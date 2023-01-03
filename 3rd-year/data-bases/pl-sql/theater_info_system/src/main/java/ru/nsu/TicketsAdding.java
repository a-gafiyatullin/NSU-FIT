package ru.nsu;

import oracle.jdbc.OracleTypes;

import javax.swing.*;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class TicketsAdding extends DatabaseUtils {
    private final Map<String, Integer> shows = new HashMap<>();
    private final CallableStatement getShowTitles;
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final Map<String, Integer> authors = new HashMap<>();
    private final CallableStatement getAuthors;
    private final Map<String, Integer> performances = new HashMap<>();
    private final CallableStatement getPerformancesDate;
    private final CallableStatement ticketAdd;
    private final CallableStatement ticketDelete;
    private final CallableStatement ticketInsertIntoSubscription;
    private final Map<Integer, Integer> subscriptions = new HashMap<>();
    private final CallableStatement subscriptionInfo;
    private final CallableStatement subscriptionInsert;
    private final CallableStatement subscriptionDelete;
    private final Map<Integer, Integer> tickets = new HashMap<>();
    private final CallableStatement ticketInfo;

    private JPanel mainPanel;
    private JTable ticketsTable;
    private JTable subscriptionTable;
    private JComboBox titleComboBox;
    private JComboBox performanceDateComboBox;
    private JFormattedTextField placeTextField;
    private JFormattedTextField costTextField;
    private JButton deleteTicketButton;
    private JButton addTicketButton;
    private JButton addTicketToSubscriptionButton;
    private JComboBox genreComboBox;
    private JComboBox authorComboBox;
    private JButton addSubscriptionButton;
    private JButton deleteSubscriptionButton;
    private JLabel status;

    public TicketsAdding(final Connection connection) throws Exception {
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        ticketsTable.getTableHeader().setReorderingAllowed(false);
        ticketsTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        subscriptionTable.getTableHeader().setReorderingAllowed(false);
        subscriptionTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });

        getShowTitles = connection.prepareCall("{call get_shows_list(?)}");
        getShowTitles.registerOutParameter("list", OracleTypes.CURSOR);

        getGenres = connection.prepareCall("{call get_genres_list(?)}");
        getGenres.registerOutParameter("list", OracleTypes.CURSOR);

        getAuthors = connection.prepareCall("{call get_authors_list(?)}");
        getAuthors.registerOutParameter("list", OracleTypes.CURSOR);

        getPerformancesDate = connection.prepareCall("{call get_performances_date_by_show(?, ?)}");
        getPerformancesDate.registerOutParameter("list", OracleTypes.CURSOR);

        ticketAdd = connection.prepareCall("{call ticket_insert(?, ?, ?)}");
        ticketDelete = connection.prepareCall("{call ticket_delete(?)}");
        ticketInsertIntoSubscription = connection.prepareCall("{call ticket_to_subscription_insert(?, ?)}");

        ticketInfo = connection.prepareCall("{call get_ticket(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        ticketInfo.registerOutParameter(13, OracleTypes.CURSOR);

        subscriptionInfo = connection.prepareCall("{call get_subscription(?, ?, ?, ?, ?, ?)}");
        subscriptionInfo.registerOutParameter(6, OracleTypes.CURSOR);
        subscriptionInsert = connection.prepareCall("{call subscription_insert(?, ?)}");
        subscriptionDelete = connection.prepareCall("{call subscription_delete(?)}");

        showComboBoxListFromSQL(titleComboBox, getShowTitles, shows, "id_show", "name_show");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(authorComboBox, getAuthors, authors, "id_author", "name_author");

        updateTables();
        titleComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                performanceDateComboBox.setSelectedItem("-");
                showComboBoxListFromSQL(titleComboBox, getShowTitles, shows, "id_show", "name_show");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        genreComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        authorComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(authorComboBox, getAuthors, authors, "id_author", "name_author");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        performanceDateComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                    JOptionPane.showMessageDialog(mainPanel, "Укажите спектакль!",
                            "Ошибка выбора даты", JOptionPane.ERROR_MESSAGE);
                } else {
                    try {
                        getPerformancesDate.setInt("id_show", shows.get(titleComboBox.getSelectedItem()));
                        showComboBoxListFromSQL(performanceDateComboBox, getPerformancesDate, performances,
                                "id_performance", "Дата");
                    } catch (SQLException throwables) {
                        throwables.printStackTrace();
                    }
                }
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        addTicketButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(performanceDateComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите дату спектакля!",
                                "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    ticketAdd.setInt(1, performances.get(performanceDateComboBox.getSelectedItem()));
                    if (placeTextField.getText().isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите место посадки!",
                                "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    ticketAdd.setInt(2, Integer.parseInt(placeTextField.getText()));
                    if (costTextField.getText().isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Введите стоимость!",
                                "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    ticketAdd.setInt(3, Integer.parseInt(costTextField.getText()));
                    ticketAdd.execute();

                    updateTables();
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        deleteTicketButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (ticketsTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите билет для удаления!",
                                "Ошибка удаления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    ticketDelete.setInt(1, tickets.get((ticketsTable.getSelectedRows()[0])));
                    ticketDelete.execute();

                    updateTables();
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        addTicketToSubscriptionButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (ticketsTable.getSelectedRows().length == 0 || subscriptionTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите и билет, и абонемент!",
                                "Ошибка связывания!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }

                    ticketInsertIntoSubscription.setInt(1, tickets.get(ticketsTable.getSelectedRows()[0]));
                    ticketInsertIntoSubscription.setInt(2,
                            subscriptions.get(subscriptionTable.getSelectedRows()[0]));
                    ticketInsertIntoSubscription.execute();

                    status.setText("Статус: билет и абонемент связаны!");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка связывания!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        addSubscriptionButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(authorComboBox.getSelectedItem(), "-")
                            == Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите жанр или автора!",
                                "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }

                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        subscriptionInsert.setNull(1, OracleTypes.INTEGER);
                        subscriptionInsert.setInt(2, authors.get(authorComboBox.getSelectedItem()));
                    } else {
                        subscriptionInsert.setInt(1, genres.get(genreComboBox.getSelectedItem()));
                        subscriptionInsert.setNull(2, OracleTypes.INTEGER);
                    }
                    subscriptionInsert.execute();

                    updateTables();
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        deleteSubscriptionButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (subscriptionTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите абонемент!",
                                "Ошибка расформировывания!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    subscriptionDelete.setInt(1, subscriptions.get(subscriptionTable.getSelectedRows()[0]));
                    subscriptionDelete.execute();

                    updateTables();
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка расформировывания!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void createUIComponents() {
        placeTextField = new JFormattedTextField(numberFormat);
        costTextField = new JFormattedTextField(numberFormat);
    }

    private void updateTables() throws Exception {
        ticketInfo.setNull(1, OracleTypes.DATE);
        ticketInfo.setNull(2, OracleTypes.DATE);
        ticketInfo.setNull(3, OracleTypes.INTEGER);
        ticketInfo.setNull(4, OracleTypes.INTEGER);
        ticketInfo.setNull(5, OracleTypes.INTEGER);
        ticketInfo.setNull(6, OracleTypes.INTEGER);
        ticketInfo.setNull(7, OracleTypes.INTEGER);
        ticketInfo.setNull(8, OracleTypes.INTEGER);
        ticketInfo.setNull(9, OracleTypes.INTEGER);
        ticketInfo.setNull(10, OracleTypes.INTEGER);
        ticketInfo.setNull(11, OracleTypes.INTEGER);
        ticketInfo.setNull(12, OracleTypes.INTEGER);
        ticketInfo.execute();

        ResultSet results = (ResultSet) ticketInfo.getObject(13);
        fillTableFromResultSet(ticketsTable, 2, tickets, results);
        results.close();

        subscriptionInfo.setNull(1, OracleTypes.INTEGER);
        subscriptionInfo.setNull(2, OracleTypes.INTEGER);
        subscriptionInfo.setNull(3, OracleTypes.INTEGER);
        subscriptionInfo.setNull(4, OracleTypes.INTEGER);
        subscriptionInfo.setNull(5, OracleTypes.INTEGER);
        subscriptionInfo.execute();

        results = (ResultSet) subscriptionInfo.getObject(6);
        fillTableFromResultSet(subscriptionTable, 2, subscriptions, results);
        results.close();
    }
}
