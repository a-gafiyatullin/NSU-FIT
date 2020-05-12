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
import java.text.NumberFormat;
import java.util.HashMap;
import java.util.Map;

public class Show extends Theater {
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
    private final Map<Integer, Integer> showsInfo = new HashMap<>();
    private final CallableStatement getShowInfo;
    private final CallableStatement getShowActorInfo;
    private final CallableStatement getShowMusicianInfo;
    private JTable showTable;
    private JFormattedTextField dateTo;
    private JFormattedTextField dateFrom;
    private JComboBox titles;
    private JCheckBox firstTimeInCheckBox;
    private JFormattedTextField centuryFrom;
    private JFormattedTextField centuryTo;
    private JComboBox conductorList;
    private JComboBox stateList;
    private JComboBox ageList;
    private JComboBox directorList;
    private JComboBox countryList;
    private JComboBox productionDesignerList;
    private JComboBox authorList;
    private JButton queryButton;
    private JComboBox genreList;
    private JPanel mainPanel;
    private JTable actorTable;
    private JTable musicianTable;
    private JLabel status;

    public Show(final Connection connection) throws Exception {
        musicianTable.getTableHeader().setReorderingAllowed(false);
        musicianTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        actorTable.getTableHeader().setReorderingAllowed(false);
        actorTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        showTable.getTableHeader().setReorderingAllowed(false);
        showTable.setModel(new DefaultTableModel() {

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

        getShowInfo = connection.prepareCall("{call show_info(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getShowInfo.registerOutParameter(15, OracleTypes.CURSOR);

        getShowActorInfo = connection.prepareCall("{call actor_role_show_info(?, ?)}");
        getShowActorInfo.registerOutParameter(2, OracleTypes.CURSOR);

        getShowMusicianInfo = connection.prepareCall("{call musician_show_info(?, ?)}");
        getShowMusicianInfo.registerOutParameter(2, OracleTypes.CURSOR);

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

        // process query
        queryButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (dateFrom.getText().isEmpty()) {
                        getShowInfo.setNull(1, OracleTypes.DATE);
                    } else {
                        getShowInfo.setDate(1, new java.sql.Date(dateFormat.parse(dateFrom.getText()).getTime()));
                    }
                    if (dateTo.getText().isEmpty()) {
                        getShowInfo.setNull(2, OracleTypes.DATE);
                    } else {
                        getShowInfo.setDate(2, new java.sql.Date(dateFormat.parse(dateTo.getText()).getTime()));
                    }
                    getShowInfo.setInt(3, firstTimeInCheckBox.isSelected() ? 1 : 0);
                    if (titles.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(4, shows.get(titles.getSelectedItem()));
                    }
                    getShowInfo.setInt(5, stateList.getSelectedIndex());
                    if (centuryFrom.getText().isEmpty()) {
                        getShowInfo.setNull(6, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(6, Integer.parseInt(centuryFrom.getText()));
                    }
                    if (centuryTo.getText().isEmpty()) {
                        getShowInfo.setNull(7, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(7, Integer.parseInt(centuryTo.getText()));
                    }
                    if (conductorList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(8, conductors.get(conductorList.getSelectedItem()));
                    }
                    if (productionDesignerList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(9, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(9, productionDesigners.get(productionDesignerList.getSelectedItem()));
                    }
                    if (directorList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(10, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(10, directors.get(directorList.getSelectedItem()));
                    }
                    if (genreList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(11, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(11, genres.get(genreList.getSelectedItem()));
                    }
                    if (ageList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(12, ageCategories.get(ageList.getSelectedItem()));
                    }
                    if (authorList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(13, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(13, authors.get(authorList.getSelectedItem()));
                    }
                    if (countryList.getSelectedItem().equals("-")) {
                        getShowInfo.setNull(14, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(14, countries.get(countryList.getSelectedItem()));
                    }
                    getShowInfo.execute();

                    ResultSet results = (ResultSet) getShowInfo.getObject(15);
                    fillTableFromResultSet(showTable, 2, showsInfo, results);
                    status.setText("Статус: Успех. Возвращено " + showTable.getRowCount() + " записей.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
                }
            }
        });

        ListSelectionModel selectionModel = showTable.getSelectionModel();
        selectionModel.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                try {
                    if (showTable.getSelectedRows().length == 0) {
                        return;
                    }
                    getShowActorInfo.setInt(1, showsInfo.get(showTable.getSelectedRows()[0]));
                    getShowActorInfo.execute();

                    ResultSet results = (ResultSet) getShowActorInfo.getObject(2);
                    fillTableFromResultSet(actorTable, 1, null, results);

                    getShowMusicianInfo.setInt(1, showsInfo.get(showTable.getSelectedRows()[0]));
                    getShowMusicianInfo.execute();

                    results = (ResultSet) getShowMusicianInfo.getObject(2);
                    fillTableFromResultSet(musicianTable, 1, null, results);
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
        centuryFrom = new JFormattedTextField(NumberFormat.getNumberInstance());
        centuryTo = new JFormattedTextField(NumberFormat.getNumberInstance());
    }
}