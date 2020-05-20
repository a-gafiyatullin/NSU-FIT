package ru.nsu;

import oracle.jdbc.OracleTypes;

import javax.swing.*;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class ActorsInfo extends DatabaseUtils {
    private final Map<String, Integer> actors = new HashMap<>();
    private final CallableStatement getActors;
    private final Map<String, Integer> genders = new HashMap<>();
    private final CallableStatement getGenders;
    private final Map<String, Integer> ranks = new HashMap<>();
    private final CallableStatement getRanks;
    private final Map<String, Integer> competitions = new HashMap<>();
    private final CallableStatement getCompetitions;
    private final Map<String, Integer> directors = new HashMap<>();
    private final CallableStatement getDirectors;
    private final Map<String, Integer> ageCategories = new HashMap<>();
    private final CallableStatement getAgeCategories;
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final CallableStatement getActorRank;
    private final CallableStatement getActorRole;

    private JTable resultTable;
    private JComboBox actorComboBox;
    private JComboBox genderComboBox;
    private JFormattedTextField ageFromTextField;
    private JFormattedTextField ageToTextField;
    private JComboBox rankComboBox;
    private JFormattedTextField periodFromTextField;
    private JFormattedTextField periodToTextField;
    private JComboBox competitionComboBox;
    private JButton queryButton;
    private JComboBox genreComboBox;
    private JComboBox ageCategoryComboBox;
    private JComboBox directorComboBox;
    private JButton queryButton2;
    private JPanel mainPanel;
    private JLabel status;

    public ActorsInfo(final Connection connection) throws Exception {
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        resultTable.getTableHeader().setReorderingAllowed(false);
        resultTable.setEnabled(false);

        getGenders = connection.prepareCall("{call get_genders_list(?)}");
        getGenders.registerOutParameter("list", OracleTypes.CURSOR);

        getActors = connection.prepareCall("{call get_actors_list(?)}");
        getActors.registerOutParameter("list", OracleTypes.CURSOR);

        getCompetitions = connection.prepareCall("{call get_competitions_list(?)}");
        getCompetitions.registerOutParameter("list", OracleTypes.CURSOR);

        getDirectors = connection.prepareCall("{call get_directors_list(?)}");
        getDirectors.registerOutParameter("list", OracleTypes.CURSOR);

        getRanks = connection.prepareCall("{call get_rank_list(?)}");
        getRanks.registerOutParameter("list", OracleTypes.CURSOR);

        getAgeCategories = connection.prepareCall("{call get_age_categories_list(?)}");
        getAgeCategories.registerOutParameter("list", OracleTypes.CURSOR);

        getGenres = connection.prepareCall("{call get_genres_list(?)}");
        getGenres.registerOutParameter("list", OracleTypes.CURSOR);

        getActorRank = connection.prepareCall("{call actor_rank_info(?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getActorRank.registerOutParameter(9, OracleTypes.CURSOR);

        getActorRole = connection.prepareCall("{call actor_roles_info(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getActorRole.registerOutParameter(10, OracleTypes.CURSOR);

        showComboBoxListFromSQL(actorComboBox, getActors, actors, "id_employee", "name");
        showComboBoxListFromSQL(genderComboBox, getGenders, genders, "id_gender", "name_gender");
        showComboBoxListFromSQL(rankComboBox, getRanks, ranks, "id_rank", "name_rank");
        showComboBoxListFromSQL(competitionComboBox, getCompetitions, competitions, "id_competition",
                "name_competition");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageCategoryComboBox, getAgeCategories, ageCategories, "id_age_category",
                "name_age_category");
        showComboBoxListFromSQL(directorComboBox, getDirectors, directors, "id_employee", "name");

        actorComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(actorComboBox, getActors, actors, "id_employee", "name");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        genderComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(genderComboBox, getGenders, genders, "id_gender",
                        "name_gender");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {

            }
        });

        rankComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(rankComboBox, getRanks, ranks, "id_rank",
                        "name_rank");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        competitionComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(competitionComboBox, getCompetitions, competitions, "id_competition",
                        "name_competition");
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

        ageCategoryComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(ageCategoryComboBox, getAgeCategories, ageCategories, "id_age_category",
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
                showComboBoxListFromSQL(directorComboBox, getDirectors, directors, "id_employee",
                        "name");
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
                    // process query
                    if (Objects.equals(actorComboBox.getSelectedItem(), "-")) {
                        getActorRank.setNull(1, OracleTypes.INTEGER);
                    } else {
                        getActorRank.setInt(1, actors.get(actorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(genderComboBox.getSelectedItem(), "-")) {
                        getActorRank.setNull(2, OracleTypes.INTEGER);
                    } else {
                        getActorRank.setInt(2, genders.get(genderComboBox.getSelectedItem()));
                    }
                    if (ageFromTextField.getText().isEmpty()) {
                        getActorRank.setNull(3, OracleTypes.INTEGER);
                    } else {
                        getActorRank.setInt(3, Integer.parseInt(ageFromTextField.getText()));
                    }
                    if (ageToTextField.getText().isEmpty()) {
                        getActorRank.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getActorRank.setInt(4, Integer.parseInt(ageToTextField.getText()));
                    }
                    if (periodFromTextField.getText().isEmpty()) {
                        getActorRank.setNull(5, OracleTypes.DATE);
                    } else {
                        getActorRank.setDate(5,
                                new java.sql.Date(dateFormat.parse(periodFromTextField.getText()).getTime()));
                    }
                    if (periodToTextField.getText().isEmpty()) {
                        getActorRank.setNull(6, OracleTypes.DATE);
                    } else {
                        getActorRank.setDate(6,
                                new java.sql.Date(dateFormat.parse(periodToTextField.getText()).getTime()));
                    }
                    if (Objects.equals(rankComboBox.getSelectedItem(), "-")) {
                        getActorRank.setNull(7, OracleTypes.INTEGER);
                    } else {
                        getActorRank.setInt(7, ranks.get(rankComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(competitionComboBox.getSelectedItem(), "-")) {
                        getActorRank.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getActorRank.setInt(8, competitions.get(competitionComboBox.getSelectedItem()));
                    }
                    getActorRank.execute();

                    // get results
                    ResultSet results = (ResultSet) getActorRank.getObject(9);
                    fillTableFromResultSet(resultTable, 1, null, results);
                    results.close();
                    setSuccessMessage(status, resultTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        queryButton2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    // process query
                    if (Objects.equals(actorComboBox.getSelectedItem(), "-")) {
                        getActorRole.setNull(1, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(1, actors.get(actorComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(genderComboBox.getSelectedItem(), "-")) {
                        getActorRole.setNull(2, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(2, genders.get(genderComboBox.getSelectedItem()));
                    }
                    if (ageFromTextField.getText().isEmpty()) {
                        getActorRole.setNull(3, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(3, Integer.parseInt(ageFromTextField.getText()));
                    }
                    if (ageToTextField.getText().isEmpty()) {
                        getActorRole.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(4, Integer.parseInt(ageToTextField.getText()));
                    }
                    if (periodFromTextField.getText().isEmpty()) {
                        getActorRole.setNull(5, OracleTypes.DATE);
                    } else {
                        getActorRole.setDate(5,
                                new java.sql.Date(dateFormat.parse(periodFromTextField.getText()).getTime()));
                    }
                    if (periodToTextField.getText().isEmpty()) {
                        getActorRole.setNull(6, OracleTypes.DATE);
                    } else {
                        getActorRole.setDate(6,
                                new java.sql.Date(dateFormat.parse(periodToTextField.getText()).getTime()));
                    }
                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        getActorRole.setNull(7, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(7, genres.get(genreComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(ageCategoryComboBox.getSelectedItem(), "-")) {
                        getActorRole.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(8, ageCategories.get(ageCategoryComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(directorComboBox.getSelectedItem(), "-")) {
                        getActorRole.setNull(9, OracleTypes.INTEGER);
                    } else {
                        getActorRole.setInt(9, directors.get(directorComboBox.getSelectedItem()));
                    }
                    getActorRole.execute();

                    // get results
                    ResultSet results = (ResultSet) getActorRole.getObject(10);
                    fillTableFromResultSet(resultTable, 1, null, results);
                    results.close();
                    setSuccessMessage(status, resultTable.getRowCount());
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
        ageFromTextField = new JFormattedTextField(numberFormat);
        ageToTextField = new JFormattedTextField(numberFormat);
        periodToTextField = new JFormattedTextField(dateFormat);
        periodFromTextField = new JFormattedTextField(dateFormat);
    }
}