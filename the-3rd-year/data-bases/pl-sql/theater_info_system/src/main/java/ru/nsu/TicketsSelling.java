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
    private JComboBox titles;
    private JComboBox conductorList;
    private JFormattedTextField centuryFrom;
    private JFormattedTextField dateFrom;
    private JFormattedTextField dateTo;
    private JFormattedTextField centuryTo;
    private JComboBox genreList;
    private JComboBox ageList;
    private JComboBox directorList;
    private JComboBox productionDesignerList;
    private JComboBox authorList;
    private JButton ticketQuery;
    private JComboBox countryList;
    private JTable ticketsTable;
    private JButton ticketSelling;
    private JButton subscriptionSelling;
    private JButton authorSubscriptionQuery;
    private JButton genreSubscriptionQuery;
    private JLabel status;

    public TicketsSelling(final Connection connection) throws Exception {
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

        ticketInfo = connection.prepareCall("{call get_tickets(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        ticketInfo.registerOutParameter(13, OracleTypes.CURSOR);

        subscriptionInfo = connection.prepareCall("{call get_subscription(?, ?, ?, ?, ?, ?)}");
        subscriptionInfo.registerOutParameter(6, OracleTypes.CURSOR);

        getTicketsInSubscription = connection.prepareCall("{call get_tickets_in_subscription(?, ?)}");
        getTicketsInSubscription.registerOutParameter(2, OracleTypes.CURSOR);

        getSubscriptionByTicketInfo = connection.prepareCall("{call get_subscription_by_ticket(?, ?)}");
        getSubscriptionByTicketInfo.registerOutParameter(2, OracleTypes.CURSOR);

        sellTicket = connection.prepareCall("{call sell_ticket(?)}");
        sellSubscription = connection.prepareCall("{call sell_subscription(?)}");

        showComboBoxListFromSQL(titles, getShowTitles, shows, "id_show", "name_show");
        showComboBoxListFromSQL(genreList, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageList, getAgeCategories, ageCategories, "id_age_category", "name_age_category");
        showComboBoxListFromSQL(authorList, getAuthors, authors, "id_author", "name_author");
        showComboBoxListFromSQL(countryList, getCountries, countries, "id_country",
                "name_country");
        showComboBoxListFromSQL(directorList, getDirectors, directors, "id_employee", "name");
        showComboBoxListFromSQL(conductorList, getConductors, conductors, "id_employee", "name");
        showComboBoxListFromSQL(productionDesignerList, getProductionDesigners, productionDesigners, "id_employee", "name");

        titles.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(titles, getShowTitles, shows, "id_show", "name_show");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        conductorList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(conductorList, getConductors, conductors, "id_employee", "name");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        genreList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(genreList, getGenres, genres, "id_genre", "name_genre");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        ageList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(ageList, getAgeCategories, ageCategories, "id_age_category",
                        "name_age_category");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        directorList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(directorList, getDirectors, directors, "id_employee", "name");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        productionDesignerList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(productionDesignerList, getProductionDesigners, productionDesigners,
                        "id_employee", "name");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        authorList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(authorList, getAuthors, authors, "id_author",
                        "name_author");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        countryList.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(countryList, getCountries, countries, "id_country", "name_country");
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
                    if (dateFrom.getText().isEmpty()) {
                        ticketInfo.setNull(1, OracleTypes.DATE);
                    } else {
                        ticketInfo.setDate(1, new java.sql.Date(dateFormat.parse(dateFrom.getText()).getTime()));
                    }
                    if (dateTo.getText().isEmpty()) {
                        ticketInfo.setNull(2, OracleTypes.DATE);
                    } else {
                        ticketInfo.setDate(2, new java.sql.Date(dateFormat.parse(dateTo.getText()).getTime()));
                    }
                    if (titles.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(3, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(3, shows.get(titles.getSelectedItem()));
                    }
                    if (centuryFrom.getText().isEmpty()) {
                        ticketInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(4, Integer.parseInt(centuryFrom.getText()));
                    }
                    if (centuryTo.getText().isEmpty()) {
                        ticketInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(5, Integer.parseInt(centuryTo.getText()));
                    }
                    if (conductorList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(6, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(6, conductors.get(conductorList.getSelectedItem()));
                    }
                    if (productionDesignerList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(7, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(7, productionDesigners.get(productionDesignerList.getSelectedItem()));
                    }
                    if (directorList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(8, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(8, directors.get(directorList.getSelectedItem()));
                    }
                    if (genreList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(9, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(9, genres.get(genreList.getSelectedItem()));
                    }
                    if (ageList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(10, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(10, ageCategories.get(ageList.getSelectedItem()));
                    }
                    if (authorList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(11, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(11, authors.get(authorList.getSelectedItem()));
                    }
                    if (countryList.getSelectedItem().equals("-")) {
                        ticketInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        ticketInfo.setInt(12, countries.get(countryList.getSelectedItem()));
                    }
                    ticketInfo.execute();

                    ResultSet results = (ResultSet) ticketInfo.getObject(13);
                    fillTableFromResultSet(ticketsTable, 2, tickets, results);
                    status.setText("Статус: Успех. Возвращено " + ticketsTable.getRowCount() + " записей.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
                }
            }
        });

        ticketSelling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (ticketsTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите билет!",
                                "Ошибка продажи", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    sellTicket.setInt(1, tickets.get(ticketsTable.getSelectedRows()[0]));
                    sellTicket.execute();

                    DefaultTableModel model = (DefaultTableModel) ticketsTable.getModel();
                    model.removeRow(ticketsTable.getSelectedRows()[0]);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка продажи", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        subscriptionSelling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (subscriptionTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите абонемент!",
                                "Ошибка продажи", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    sellSubscription.setInt(1, subscriptions.get(subscriptionTable.getSelectedRows()[0]));
                    sellSubscription.execute();

                    DefaultTableModel model = (DefaultTableModel) subscriptionTable.getModel();
                    model.removeRow(subscriptionTable.getSelectedRows()[0]);
                    model = (DefaultTableModel) ticketsTable.getModel();
                    model.setRowCount(0);
                } catch (Exception exception) {
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
                    if (genreList.getSelectedItem().equals("-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите жанр!",
                                "Ошибка запроса", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        subscriptionInfo.setInt(3, genres.get(genreList.getSelectedItem()));
                    }
                    subscriptionInfo.setNull(4, OracleTypes.INTEGER);
                    subscriptionInfo.setNull(5, OracleTypes.INTEGER);
                    subscriptionInfo.execute();

                    ResultSet results = (ResultSet) subscriptionInfo.getObject(6);
                    fillTableFromResultSet(subscriptionTable, 2, subscriptions, results);
                    status.setText("Статус: Успех. Возвращено " + subscriptionTable.getRowCount() + " записей.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
                }
            }
        });

        authorSubscriptionQuery.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (centuryFrom.getText().isEmpty()) {
                        subscriptionInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        subscriptionInfo.setInt(4, Integer.parseInt(centuryFrom.getText()));
                    }
                    if (centuryTo.getText().isEmpty()) {
                        subscriptionInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        subscriptionInfo.setInt(5, Integer.parseInt(centuryTo.getText()));
                    }
                    ticketInfo.setNull(9, OracleTypes.INTEGER);
                    if (authorList.getSelectedItem().equals("-") && countryList.getSelectedItem().equals("-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Укажите автора или страну!",
                                "Ошибка запроса", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        subscriptionInfo.setInt(11, authors.get(authorList.getSelectedItem()));
                    }
                    if (countryList.getSelectedItem().equals("-")) {
                        subscriptionInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        subscriptionInfo.setInt(12, countries.get(countryList.getSelectedItem()));
                    }
                    subscriptionInfo.execute();

                    ResultSet results = (ResultSet) subscriptionInfo.getObject(13);
                    fillTableFromResultSet(subscriptionTable, 2, subscriptions, results);
                    status.setText("Статус: Успех. Возвращено " + subscriptionTable.getRowCount() + " записей.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
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
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setVisible(true);
    }

    private void createUIComponents() {
        dateTo = new JFormattedTextField(dateFormat);
        dateFrom = new JFormattedTextField(dateFormat);
        centuryFrom = new JFormattedTextField(numberFormat);
        centuryTo = new JFormattedTextField(numberFormat);
    }
}
