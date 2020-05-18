package ru.nsu;

import oracle.jdbc.OracleTypes;

import javax.swing.*;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.util.HashMap;
import java.util.Map;

public class Statistics extends DatabaseUtils {
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
    private final CallableStatement getTicketsStatistics;
    private JComboBox titles;
    private JCheckBox firstTimeInCheckBox;
    private JComboBox conductorList;
    private JComboBox stateList;
    private JFormattedTextField centuryFrom;
    private JFormattedTextField dateFrom;
    private JFormattedTextField dateTo;
    private JFormattedTextField centuryTo;
    private JComboBox genreList;
    private JComboBox ageList;
    private JComboBox directorList;
    private JComboBox productionDesignerList;
    private JComboBox authorList;
    private JButton queryButton;
    private JComboBox countryList;
    private JLabel sellAmount;
    private JLabel moneyAmount;
    private JPanel mainPanel;
    private JLabel status;

    public Statistics(final Connection connection) throws Exception {
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

        getTicketsStatistics = connection.prepareCall("{call tickets_statistic(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getTicketsStatistics.registerOutParameter(14, OracleTypes.INTEGER);
        getTicketsStatistics.registerOutParameter(15, OracleTypes.DOUBLE);

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

        queryButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (dateFrom.getText().isEmpty()) {
                        getTicketsStatistics.setNull(1, OracleTypes.DATE);
                    } else {
                        getTicketsStatistics.setDate(1, new java.sql.Date(dateFormat.parse(dateFrom.getText()).getTime()));
                    }
                    if (dateTo.getText().isEmpty()) {
                        getTicketsStatistics.setNull(2, OracleTypes.DATE);
                    } else {
                        getTicketsStatistics.setDate(2, new java.sql.Date(dateFormat.parse(dateTo.getText()).getTime()));
                    }
                    getTicketsStatistics.setInt(3, firstTimeInCheckBox.isSelected() ? 1 : 0);
                    if (titles.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(4, shows.get(titles.getSelectedItem()));
                    }
                    if (centuryFrom.getText().isEmpty()) {
                        getTicketsStatistics.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(5, Integer.parseInt(centuryFrom.getText()));
                    }
                    if (centuryTo.getText().isEmpty()) {
                        getTicketsStatistics.setNull(6, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(6, Integer.parseInt(centuryTo.getText()));
                    }
                    if (conductorList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(7, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(7, conductors.get(conductorList.getSelectedItem()));
                    }
                    if (productionDesignerList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(8, productionDesigners.get(productionDesignerList.getSelectedItem()));
                    }
                    if (directorList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(9, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(9, directors.get(directorList.getSelectedItem()));
                    }
                    if (genreList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(10, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(10, genres.get(genreList.getSelectedItem()));
                    }
                    if (ageList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(11, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(11, ageCategories.get(ageList.getSelectedItem()));
                    }
                    if (authorList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(12, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(12, authors.get(authorList.getSelectedItem()));
                    }
                    if (countryList.getSelectedItem().equals("-")) {
                        getTicketsStatistics.setNull(13, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(13, countries.get(countryList.getSelectedItem()));
                    }
                    getTicketsStatistics.execute();
                    sellAmount.setText(Integer.toString(getTicketsStatistics.getInt(14)));
                    moneyAmount.setText(Double.toString(getTicketsStatistics.getDouble(15)));

                    status.setText("Статус: Успех.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void createUIComponents() {
        dateTo = new JFormattedTextField(dateFormat);
        dateFrom = new JFormattedTextField(dateFormat);
        centuryFrom = new JFormattedTextField(numberFormat);
        centuryTo = new JFormattedTextField(numberFormat);
    }
}
