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
import java.util.Objects;

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

    private JComboBox titleComboBox;
    private JCheckBox firstTimeInCheckBox;
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
    private JButton queryButton;
    private JComboBox countryComboBox;
    private JLabel sellAmountLabel;
    private JLabel moneyAmountLabel;
    private JPanel mainPanel;
    private JLabel status;

    public Statistics(final Connection connection) throws Exception {
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

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

        getTicketsStatistics
                = connection.prepareCall("{call tickets_statistic(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getTicketsStatistics.registerOutParameter(14, OracleTypes.INTEGER);
        getTicketsStatistics.registerOutParameter(15, OracleTypes.DOUBLE);

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
                showComboBoxListFromSQL(countryComboBox, getCountries, countries, "id_country",
                        "name_country");
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
                    if (dateFromTextField.getText().isEmpty()) {
                        getTicketsStatistics.setNull(1, OracleTypes.DATE);
                    } else {
                        getTicketsStatistics.setDate(1,
                                new java.sql.Date(dateFormat.parse(dateFromTextField.getText()).getTime()));
                    }
                    if (dateToTextField.getText().isEmpty()) {
                        getTicketsStatistics.setNull(2, OracleTypes.DATE);
                    } else {
                        getTicketsStatistics.setDate(2,
                                new java.sql.Date(dateFormat.parse(dateToTextField.getText()).getTime()));
                    }
                    getTicketsStatistics.setInt(3, firstTimeInCheckBox.isSelected() ? 1 : 0);
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(4, shows.get(titleComboBox.getSelectedItem()));
                    }
                    if (centuryFromTextField.getText().isEmpty()) {
                        getTicketsStatistics.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(5, Integer.parseInt(centuryFromTextField.getText()));
                    }
                    if (centuryToTextField.getText().isEmpty()) {
                        getTicketsStatistics.setNull(6, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(6, Integer.parseInt(centuryToTextField.getText()));
                    }
                    if (Objects.equals(conductorComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(7, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(7, conductors.get(conductorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(productionDesignerComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(8,
                                productionDesigners.get(productionDesignerComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(directorComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(9, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(9, directors.get(directorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(10, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(10, genres.get(genreComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(ageComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(11, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(11, ageCategories.get(ageComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(authorComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(12, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(12, authors.get(authorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(countryComboBox.getSelectedItem(), "-")) {
                        getTicketsStatistics.setNull(13, OracleTypes.INTEGER);
                    } else {
                        getTicketsStatistics.setInt(13, countries.get(countryComboBox.getSelectedItem()));
                    }
                    getTicketsStatistics.execute();
                    sellAmountLabel.setText(Integer.toString(getTicketsStatistics.getInt(14)));
                    moneyAmountLabel.setText(Double.toString(getTicketsStatistics.getDouble(15)));

                    status.setText("Статус: Успех.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
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
