package ru.nsu;

import oracle.jdbc.OracleTypes;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class TicketsSelling extends DatabaseUtils {
    private final Map<String, Integer> shows = new HashMap<>();
    private final CallableStatement getShowTitles;
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final Map<String, Integer> ageCategories = new HashMap<>();
    private final CallableStatement getAgeCategories;
    private final Map<String, Integer> authors = new HashMap<>();
    private final CallableStatement getAuthors;
    private final Map<String, Integer> countries = new HashMap<>();
    private final CallableStatement getCountries;
    private final Map<String, Integer> directors = new HashMap<>();
    private final CallableStatement getDirectors;
    private final Map<String, Integer> conductors = new HashMap<>();
    private final CallableStatement getConductors;
    private final Map<String, Integer> productionDesigners = new HashMap<>();
    private final CallableStatement getProductionDesigners;
    private final Map<Integer, Integer> tickets = new HashMap<>();
    private final CallableStatement ticketInfo;
    private final CallableStatement getSubscriptionByTicketInfo;
    private final CallableStatement sellTicket;
    private final Map<Integer, Integer> subscriptions = new HashMap<>();
    private final CallableStatement subscriptionInfo;
    private final CallableStatement getTicketsInSubscription;
    private final CallableStatement sellSubscription;

    private JPanel mainPanel;
    private JTable subscriptionTable;
    private JComboBox titleComboBox;
    private JComboBox conductorComboBox;
    private JFormattedTextField centuryFromTextField;
    private JFormattedTextField dateFromTextField;
    private JFormattedTextField dateToTextField;
    private JFormattedTextField centuryToTextField;
    private JComboBox genreComboBox;
    private JComboBox ageComboBox;
    private JComboBox directorComboBox;
    private JComboBox productionDesignerComboBox;
    private JComboBox authorComboBox;
    private JButton ticketQuery;
    private JComboBox countryComboBox;
    private JTable ticketsTable;
    private JButton ticketSelling;
    private JButton subscriptionSelling;
    private JButton authorSubscriptionQuery;
    private JButton genreSubscriptionQuery;
    private JLabel status;

    public TicketsSelling(final Connection connection) throws Exception {
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

        getAgeCategories = connection.prepareCall("{call get_age_categories_list(?)}");
        getAgeCategories.registerOutParameter("list", OracleTypes.CURSOR);

        getAuthors = connection.prepareCall("{call get_authors_list(?)}");
        getAuthors.registerOutParameter("list", OracleTypes.CURSOR);

        getCountries = connection.prepareCall("{call get_countries_list(?)}");
        getCountries.registerOutParameter("list", OracleTypes.CURSOR);

        getDirectors = connection.prepareCall("{call get_directors_list(?)}");
        getDirectors.registerOutParameter("list", OracleTypes.CURSOR);

        getConductors = connection.prepareCall("{call get_conductors_list(?)}");
        getConductors.registerOutParameter("list", OracleTypes.CURSOR);

        getProductionDesigners = connection.prepareCall("{call get_designers_list(?)}");
        getProductionDesigners.registerOutParameter("list", OracleTypes.CURSOR);

        ticketInfo = connection.prepareCall("{call get_ticket(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        ticketInfo.registerOutParameter(13, OracleTypes.CURSOR);

        subscriptionInfo = connection.prepareCall("{call get_subscription(?, ?, ?, ?, ?, ?)}");
        subscriptionInfo.registerOutParameter(6, OracleTypes.CURSOR);

        getTicketsInSubscription = connection.prepareCall("{call get_tickets_in_subscription(?, ?)}");
        getTicketsInSubscription.registerOutParameter(2, OracleTypes.CURSOR);

        getSubscriptionByTicketInfo = connection.prepareCall("{call get_subscription_by_ticket(?, ?)}");
        getSubscriptionByTicketInfo.registerOutParameter(2, OracleTypes.CURSOR);

        sellTicket = connection.prepareCall("{call sell_ticket(?)}");
        sellSubscription = connection.prepareCall("{call sell_subscription(?)}");

        showComboBoxListFromSQL(titleComboBox, getShowTitles, shows, "id_show", "name_show");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageComboBox, getAgeCategories, ageCategories, "id_age_category",
                "name_age_category");
        showComboBoxListFromSQL(authorComboBox, getAuthors, authors, "id_author", "name_author");
        showComboBoxListFromSQL(countryComboBox, getCountries, countries, "id_country",
                "name_country");
        showComboBoxListFromSQL(directorComboBox, getDirectors, directors, "id_employee", "name");
        showComboBoxListFromSQL(conductorComboBox, getConductors, conductors, "id_employee", "name");
        showComboBoxListFromSQL(productionDesignerComboBox, getProductionDesigners, productionDesigners,
                "id_employee", "name");

        titleComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(titleComboBox, getShowTitles, shows, "id_show", "name_show");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        conductorComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(conductorComboBox, getConductors, conductors, "id_employee", "name");
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

        ageComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(ageComboBox, getAgeCategories, ageCategories, "id_age_category",
                        "name_age_category");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        directorComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(directorComboBox, getDirectors, directors, "id_employee", "name");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        productionDesignerComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(productionDesignerComboBox, getProductionDesigners, productionDesigners,
                        "id_employee", "name");
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

        countryComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(countryComboBox, getCountries, countries, "id_country", "name_country");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        ticketQuery.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (dateFromTextField.getText().isEmpty()) {
                        ticketInfo.setNull(1, OracleTypes.DATE);
                    } else {
                        ticketInfo.setDate(1,
                                new java.sql.Date(dateFormat.parse(dateFromTextField.getText()).getTime()));
                    }
                    if (dateToTextField.getText().isEmpty()) {
                        ticketInfo.setNull(2, OracleTypes.DATE);
                    } else {
                        ticketInfo.setDate(2,
                                new java.sql.Date(dateFormat.parse(dateToTextField.getText()).getTime()));
                    }
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(3, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(3, shows.get(titleComboBox.getSelectedItem()));
                    }
                    if (centuryFromTextField.getText().isEmpty()) {
                        ticketInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(4, Integer.parseInt(centuryFromTextField.getText()));
                    }
                    if (centuryToTextField.getText().isEmpty()) {
                        ticketInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(5, Integer.parseInt(centuryToTextField.getText()));
                    }
                    if (Objects.equals(conductorComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(6, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(6, conductors.get(conductorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(productionDesignerComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(7, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(7,
                                productionDesigners.get(productionDesignerComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(directorComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(8, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(8, directors.get(directorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(9, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(9, genres.get(genreComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(ageComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(10, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(10, ageCategories.get(ageComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(authorComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(11, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(11, authors.get(authorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(countryComboBox.getSelectedItem(), "-")) {
                        ticketInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(12, countries.get(countryComboBox.getSelectedItem()));
                    }
                    ticketInfo.execute();

                    ResultSet results = (ResultSet) ticketInfo.getObject(13);
                    fillTableFromResultSet(ticketsTable, 2, tickets, results);
                    results.close();
                    setSuccessMessage(status, ticketsTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        ticketSelling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (ticketsTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите билет!",
                                "Ошибка продажи!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    sellTicket.setInt(1, tickets.get(ticketsTable.getSelectedRows()[0]));
                    sellTicket.execute();

                    DefaultTableModel model = (DefaultTableModel) ticketsTable.getModel();
                    model.removeRow(ticketsTable.getSelectedRow());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка продажи!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        subscriptionSelling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (subscriptionTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите абонемент!",
                                "Ошибка продажи!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    sellSubscription.setInt(1, subscriptions.get(subscriptionTable.getSelectedRows()[0]));
                    sellSubscription.execute();

                    DefaultTableModel model = (DefaultTableModel) subscriptionTable.getModel();
                    model.removeRow(subscriptionTable.getSelectedRows()[0]);
                    model = (DefaultTableModel) ticketsTable.getModel();
                    model.setRowCount(0);
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка продажи!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        genreSubscriptionQuery.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    subscriptionInfo.setNull(1, OracleTypes.INTEGER);
                    subscriptionInfo.setNull(2, OracleTypes.INTEGER);
                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите жанр!",
                                "Ошибка запроса!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        subscriptionInfo.setInt(3, genres.get(genreComboBox.getSelectedItem()));
                    }
                    subscriptionInfo.setNull(4, OracleTypes.INTEGER);
                    subscriptionInfo.setNull(5, OracleTypes.INTEGER);
                    subscriptionInfo.execute();

                    ResultSet results = (ResultSet) subscriptionInfo.getObject(6);
                    fillTableFromResultSet(subscriptionTable, 2, subscriptions, results);
                    results.close();
                    setSuccessMessage(status, subscriptionTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        authorSubscriptionQuery.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (centuryFromTextField.getText().isEmpty()) {
                        subscriptionInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        subscriptionInfo.setInt(4, Integer.parseInt(centuryFromTextField.getText()));
                    }
                    if (centuryToTextField.getText().isEmpty()) {
                        subscriptionInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        subscriptionInfo.setInt(5, Integer.parseInt(centuryToTextField.getText()));
                    }
                    ticketInfo.setNull(9, OracleTypes.INTEGER);
                    if (Objects.equals(authorComboBox.getSelectedItem(), "-") &&
                            countryComboBox.getSelectedItem().equals("-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите автора или страну!",
                                "Ошибка запроса!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        subscriptionInfo.setInt(11, authors.get(authorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(countryComboBox.getSelectedItem(), "-")) {
                        subscriptionInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        subscriptionInfo.setInt(12, countries.get(countryComboBox.getSelectedItem()));
                    }
                    subscriptionInfo.execute();

                    ResultSet results = (ResultSet) subscriptionInfo.getObject(13);
                    fillTableFromResultSet(subscriptionTable, 2, subscriptions, results);
                    results.close();
                    setSuccessMessage(status, subscriptionTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        ListSelectionModel selectionModel = subscriptionTable.getSelectionModel();
        selectionModel.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                try {
                    if (subscriptionTable.getSelectedRows().length == 0) {
                        return;
                    }
                    getTicketsInSubscription.setInt(1, subscriptions.get(subscriptionTable.getSelectedRows()[0]));
                    getTicketsInSubscription.execute();

                    ResultSet results = (ResultSet) getTicketsInSubscription.getObject(2);
                    fillTableFromResultSet(ticketsTable, 2, tickets, results);
                    results.close();
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        selectionModel = ticketsTable.getSelectionModel();
        selectionModel.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                try {
                    if (ticketsTable.getSelectedRows().length == 0) {
                        return;
                    }
                    getSubscriptionByTicketInfo.setInt(1, tickets.get(ticketsTable.getSelectedRows()[0]));
                    getSubscriptionByTicketInfo.execute();

                    ResultSet results = (ResultSet) getSubscriptionByTicketInfo.getObject(2);
                    fillTableFromResultSet(subscriptionTable, 2, subscriptions, results);
                    results.close();
                } catch (Exception exception) {
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
        dateToTextField = new JFormattedTextField(dateFormat);
        dateFromTextField = new JFormattedTextField(dateFormat);
        centuryFromTextField = new JFormattedTextField(numberFormat);
        centuryToTextField = new JFormattedTextField(numberFormat);
    }
}
