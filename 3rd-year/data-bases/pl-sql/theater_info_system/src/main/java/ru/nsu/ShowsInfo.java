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

public class ShowsInfo extends DatabaseUtils {
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
    private final Map<Integer, Integer> repertoireInfo = new HashMap<>();
    private final CallableStatement getRepertoireInfo;
    private final CallableStatement getShowActorInfo;
    private final CallableStatement getShowMusicianInfo;

    private JTable showTable;
    private JFormattedTextField dateToTextField;
    private JFormattedTextField dateFromTextField;
    private JComboBox titleComboBox;
    private JCheckBox firstTimeInCheckBox;
    private JFormattedTextField centuryFromTextField;
    private JFormattedTextField centuryToTextField;
    private JComboBox conductorComboBox;
    private JComboBox stateList;
    private JComboBox ageComboBox;
    private JComboBox directorComboBox;
    private JComboBox countryComboBox;
    private JComboBox productionDesignerComboBox;
    private JComboBox authorComboBox;
    private JButton queryButton;
    private JComboBox genreComboBox;
    private JPanel mainPanel;
    private JTable actorTable;
    private JTable musicianTable;
    private JLabel status;

    public ShowsInfo(final Connection connection) throws Exception {
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

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

        getRepertoireInfo = connection.prepareCall("{call repertoire_info(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getRepertoireInfo.registerOutParameter(15, OracleTypes.CURSOR);

        getShowActorInfo = connection.prepareCall("{call actors_roles_in_show_info(?, ?)}");
        getShowActorInfo.registerOutParameter(2, OracleTypes.CURSOR);

        getShowMusicianInfo = connection.prepareCall("{call musicians_in_show_info(?, ?)}");
        getShowMusicianInfo.registerOutParameter(2, OracleTypes.CURSOR);

        showComboBoxListFromSQL(titleComboBox, getShowTitles, shows, "id_show", "name_show");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageComboBox, getAgeCategories, ageCategories, "id_age_category",
                "name_age_category");
        showComboBoxListFromSQL(authorComboBox, getAuthors, authors, "id_author", "name_author");
        showComboBoxListFromSQL(countryComboBox, getCountries, countries, "id_country", "name_country");
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
                showComboBoxListFromSQL(conductorComboBox, getConductors, conductors, "id_employee",
                        "name");
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

        // process query
        queryButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (dateFromTextField.getText().isEmpty()) {
                        getRepertoireInfo.setNull(1, OracleTypes.DATE);
                    } else {
                        getRepertoireInfo.setDate(1,
                                new java.sql.Date(dateFormat.parse(dateFromTextField.getText()).getTime()));
                    }
                    if (dateToTextField.getText().isEmpty()) {
                        getRepertoireInfo.setNull(2, OracleTypes.DATE);
                    } else {
                        getRepertoireInfo.setDate(2,
                                new java.sql.Date(dateFormat.parse(dateToTextField.getText()).getTime()));
                    }
                    getRepertoireInfo.setInt(3, firstTimeInCheckBox.isSelected() ? 1 : 0);
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(4, shows.get(titleComboBox.getSelectedItem()));
                    }
                    getRepertoireInfo.setInt(5, stateList.getSelectedIndex());
                    if (centuryFromTextField.getText().isEmpty()) {
                        getRepertoireInfo.setNull(6, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(6, Integer.parseInt(centuryFromTextField.getText()));
                    }
                    if (centuryToTextField.getText().isEmpty()) {
                        getRepertoireInfo.setNull(7, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(7, Integer.parseInt(centuryToTextField.getText()));
                    }
                    if (Objects.equals(conductorComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(8, conductors.get(conductorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(productionDesignerComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(9, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(9, productionDesigners.get(productionDesignerComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(directorComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(10, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(10, directors.get(directorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(11, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(11, genres.get(genreComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(ageComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(12, ageCategories.get(ageComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(authorComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(13, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(13, authors.get(authorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(countryComboBox.getSelectedItem(), "-")) {
                        getRepertoireInfo.setNull(14, OracleTypes.INTEGER);
                    } else {
                        getRepertoireInfo.setInt(14, countries.get(countryComboBox.getSelectedItem()));
                    }
                    getRepertoireInfo.execute();

                    ResultSet results = (ResultSet) getRepertoireInfo.getObject(15);
                    fillTableFromResultSet(showTable, 2, repertoireInfo, results);
                    results.close();
                    setSuccessMessage(status, showTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        ListSelectionModel selectionModel = showTable.getSelectionModel();
        selectionModel.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                try {
                    if (showTable.getSelectedRow() == -1) {
                        return;
                    }
                    getShowActorInfo.setInt(1, repertoireInfo.get(showTable.getSelectedRow()));
                    getShowActorInfo.execute();

                    ResultSet results = (ResultSet) getShowActorInfo.getObject(2);
                    fillTableFromResultSet(actorTable, 3, null, results);
                    results.close();

                    getShowMusicianInfo.setInt(1, repertoireInfo.get(showTable.getSelectedRow()));
                    getShowMusicianInfo.execute();

                    results = (ResultSet) getShowMusicianInfo.getObject(2);
                    fillTableFromResultSet(musicianTable, 1, null, results);
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