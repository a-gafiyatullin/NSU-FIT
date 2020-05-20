package ru.nsu;

import javafx.util.Pair;
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
import java.sql.ResultSetMetaData;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class ShowEditing extends DatabaseUtils {
    private final Map<String, Integer> shows = new HashMap<>();
    private final CallableStatement getShowTitles;
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final Map<String, Integer> ageCategories = new HashMap<>();
    private final CallableStatement getAgeCategories;
    private final Map<String, Integer> authors = new HashMap<>();
    private final CallableStatement getAuthors;
    private final Map<String, Integer> directors = new HashMap<>();
    private final CallableStatement getDirectors;
    private final Map<String, Integer> conductors = new HashMap<>();
    private final CallableStatement getConductors;
    private final Map<String, Integer> productionDesigners = new HashMap<>();
    private final CallableStatement getProductionDesigners;
    private final CallableStatement getShowInfo;
    private final CallableStatement deleteShow;
    private final CallableStatement updateShow;
    private final CallableStatement insertShow;
    private final Map<String, Integer> instruments = new HashMap<>();
    private final CallableStatement getInstruments;
    private final Map<Integer, Integer> musicians = new HashMap<>();
    private final CallableStatement getMusicians;
    private final CallableStatement setMusicianForShow;
    private final CallableStatement deleteMusicianFromShow;
    private final Map<String, Integer> roles = new HashMap<>();
    private final CallableStatement getRoles;
    private final CallableStatement insertRole;
    private final CallableStatement deleteRole;
    private final CallableStatement updateRole;
    private final CallableStatement getRoleInfo;
    private final Map<Integer, Integer> roleCharacteristics = new HashMap<>();
    private final CallableStatement getRoleCharacteristics;
    private final CallableStatement insertRoleCharacteristic;
    private final CallableStatement deleteRoleCharacteristic;
    private final Map<String, Integer> actors = new HashMap<>();
    private final CallableStatement getActorsForRole;
    private final CallableStatement setActorsToRole;
    private final CallableStatement deleteActorsFromRole;
    private final Map<Integer, Integer> performances = new HashMap<>();
    private final CallableStatement getPerformancesDate;
    private final CallableStatement insertPerformanceDate;
    private final CallableStatement deletePerformanceDate;
    private final Map<String, Integer> characteristics = new HashMap<>();
    private final CallableStatement getCharacteristics;
    private final Map<Integer, Pair<Integer, Integer>> showsActorsInfo = new HashMap<>();
    private final CallableStatement getShowActorInfo;

    private JComboBox titleComboBox;
    private JButton editTitleButton;
    private JButton addTitleButton;
    private JFormattedTextField titleCenturyTextField;
    private JComboBox directorComboBox;
    private JComboBox productionDesignerComboBox;
    private JComboBox conductorComboBox;
    private JComboBox authorComboBox;
    private JComboBox ageComboBox;
    private JTextField titleNameTextField;
    private JComboBox genreComboBox;
    private JButton saveTitleButton;
    private JFormattedTextField premierDateTextField;
    private JComboBox characteristicsComboBox;
    private JTable characteristicsTable;
    private JFormattedTextField characteristicValue;
    private JButton saveRoleButton;
    private JButton addRoleButton;
    private JButton deleteTitleButton;
    private JTextField roleNameTextField;
    private JComboBox roleComboBox;
    private JButton editRoleButton;
    private JButton deleteRoleButton;
    private JButton addCharacteristicButton;
    private JButton deleteCharacteristicButton;
    private JCheckBox isMainRoleCheckBox;
    private JComboBox actorComboBox;
    private JButton addAsUnderstudyActorButton;
    private JButton addAsMainActorButton;
    private JComboBox instrumentsComboBox;
    private JTable musiciansTable;
    private JButton addMusicianToTitleButton;
    private JButton deleteMusicianFromTitleButton;
    private JTable performancesTable;
    private JFormattedTextField dateTextField;
    private JButton deleteDateButton;
    private JButton addDateButton;
    private JPanel mainPanel;
    private JComboBox rolesForActorsComboBox;
    private JButton getMusiciansButton;
    private JLabel status;
    private JTable actorsTable;
    private JButton deleteActorFromRoleButton;

    public ShowEditing(final Connection connection) throws Exception {
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        performancesTable.getTableHeader().setReorderingAllowed(false);
        performancesTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        musiciansTable.getTableHeader().setReorderingAllowed(false);
        musiciansTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        characteristicsTable.getTableHeader().setReorderingAllowed(false);
        characteristicsTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        actorsTable.getTableHeader().setReorderingAllowed(false);
        actorsTable.setModel(new DefaultTableModel() {

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

        getDirectors = connection.prepareCall("{call get_directors_list(?)}");
        getDirectors.registerOutParameter("list", OracleTypes.CURSOR);

        getConductors = connection.prepareCall("{call get_conductors_list(?)}");
        getConductors.registerOutParameter("list", OracleTypes.CURSOR);

        getProductionDesigners = connection.prepareCall("{call get_designers_list(?)}");
        getProductionDesigners.registerOutParameter("list", OracleTypes.CURSOR);

        getShowInfo = connection.prepareCall("{call show_info(?, ?)}");
        getShowInfo.registerOutParameter(2, OracleTypes.CURSOR);
        deleteShow = connection.prepareCall("{call show_delete(?)}");
        updateShow = connection.prepareCall("{call show_update(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        insertShow = connection.prepareCall("{call show_insert(?, ?, ?, ?, ?, ?, ?, ?, ?)}");

        getInstruments = connection.prepareCall("{call get_musical_instruments_list(?)}");
        getInstruments.registerOutParameter("list", OracleTypes.CURSOR);

        getMusicians = connection.prepareCall("{call get_all_musicians_for_show(?, ?, ?)}");
        getMusicians.registerOutParameter(3, OracleTypes.CURSOR);

        setMusicianForShow = connection.prepareCall("{call set_musician_for_show(?, ?)}");
        deleteMusicianFromShow = connection.prepareCall("{call delete_musician_from_show(?, ?)}");

        getRoles = connection.prepareCall("{call get_roles_for_show(?, ?)}");
        getRoles.registerOutParameter("list", OracleTypes.CURSOR);
        deleteRole = connection.prepareCall("{call role_delete(?)}");
        insertRole = connection.prepareCall("{call role_insert(?, ?, ?)}");
        updateRole = connection.prepareCall("{call role_update(?, ?, ?)}");
        getRoleInfo = connection.prepareCall("{call role_info(?, ?)}");
        getRoleInfo.registerOutParameter(2, OracleTypes.INTEGER);

        getRoleCharacteristics = connection.prepareCall("{call get_role_characteristics(?, ?)}");
        getRoleCharacteristics.registerOutParameter(2, OracleTypes.CURSOR);

        getCharacteristics = connection.prepareCall("{call get_characteristics_list(?)}");
        getCharacteristics.registerOutParameter("list", OracleTypes.CURSOR);
        insertRoleCharacteristic = connection.prepareCall("{call role_characteristic_insert(?, ?, ?)}");
        deleteRoleCharacteristic = connection.prepareCall("{call role_characteristic_delete(?, ?)}");

        getActorsForRole = connection.prepareCall("{call get_actors_for_role(?, ?)}");
        getActorsForRole.registerOutParameter("list", OracleTypes.CURSOR);
        setActorsToRole = connection.prepareCall("{call set_actor_to_role(?, ?, ?)}");

        insertPerformanceDate = connection.prepareCall("{call performance_insert(?, ?)}");
        deletePerformanceDate = connection.prepareCall("{call performance_delete(?)}");

        getPerformancesDate = connection.prepareCall("{call get_performances_date_by_show(?, ?)}");
        getPerformancesDate.registerOutParameter(2, OracleTypes.CURSOR);

        getShowActorInfo = connection.prepareCall("{call actors_roles_in_show_info(?, ?)}");
        getShowActorInfo.registerOutParameter(2, OracleTypes.CURSOR);
        deleteActorsFromRole = connection.prepareCall("{call delete_actor_from_role(?, ?)}");

        showComboBoxListFromSQL(titleComboBox, getShowTitles, shows, "id_show", "name_show");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageComboBox, getAgeCategories, ageCategories, "id_age_category",
                "name_age_category");
        showComboBoxListFromSQL(authorComboBox, getAuthors, authors, "id_author", "name_author");
        showComboBoxListFromSQL(directorComboBox, getDirectors, directors, "id_employee", "name");
        showComboBoxListFromSQL(conductorComboBox, getConductors, conductors, "id_employee", "name");
        showComboBoxListFromSQL(productionDesignerComboBox, getProductionDesigners, productionDesigners,
                "id_employee", "name");
        showComboBoxListFromSQL(instrumentsComboBox, getInstruments, instruments, "id_instrument",
                "name_instrument");
        showComboBoxListFromSQL(characteristicsComboBox, getCharacteristics, characteristics, "id_characteristic",
                "type_characteristic");

        instrumentsComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(instrumentsComboBox, getInstruments, instruments, "id_instrument",
                        "name_instrument");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

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

        ageComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(ageComboBox, getAuthors, authors, "id_author", "name_author");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        characteristicsComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(characteristicsComboBox, getCharacteristics, characteristics,
                        "id_characteristic", "type_characteristic");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        roleComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                try {
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль для редактирования!",
                                "Ошибка получения ролей!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        getRoles.setInt("id_show", shows.get(titleComboBox.getSelectedItem()));
                        showComboBoxListFromSQL(roleComboBox, getRoles, roles, "id_role", "name_role");
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка получения ролей!", JOptionPane.ERROR_MESSAGE);
                }
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {

            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {

            }
        });

        rolesForActorsComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                try {
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль для редактирования!",
                                "Ошибка получения ролей!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        getRoles.setInt("id_show", shows.get(titleComboBox.getSelectedItem()));
                        showComboBoxListFromSQL(rolesForActorsComboBox, getRoles, roles, "id_role", "name_role");
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка получения ролей!", JOptionPane.ERROR_MESSAGE);
                }
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {

            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {

            }
        });

        actorComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                try {
                    if (Objects.equals(rolesForActorsComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите роль!",
                                "Ошибка получения актеров!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        getActorsForRole.setInt("id_role", roles.get(rolesForActorsComboBox.getSelectedItem()));
                        showComboBoxListFromSQL(actorComboBox, getActorsForRole, actors, "id_employee",
                                "Актер");
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка получения актеров!", JOptionPane.ERROR_MESSAGE);
                }
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {

            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {

            }
        });

        editTitleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберете спектакль для редактирования!",
                                "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                    } else {
                        int id_show = shows.get(titleComboBox.getSelectedItem());
                        getShowInfo.setInt(1, id_show);
                        updatePerformancesTable(id_show);
                        updateMusiciansTable(id_show);
                        updateActorTable(id_show);
                    }
                    getShowInfo.execute();

                    ResultSet results = ((ResultSet) getShowInfo.getObject(2));
                    if (!results.next()) {
                        status.setText("Статус: Успех.");
                        return;
                    }
                    titleNameTextField.setText(results.getString("Название"));
                    directorComboBox.setSelectedItem(results.getString("Режиссер"));
                    conductorComboBox.setSelectedItem(results.getString("Диpижеp"));
                    productionDesignerComboBox.setSelectedItem(results.getString("Художник"));
                    authorComboBox.setSelectedItem(results.getString("Автор"));
                    genreComboBox.setSelectedItem(results.getString("Жанр"));
                    titleCenturyTextField.setText(results.getString("Век спектакля"));
                    premierDateTextField.setText(results.getString("Дата премьеры"));
                    ageComboBox.setSelectedItem(results.getString("Возраст"));

                    results.close();
                    status.setText("Статус: Успех.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: Неуспех!");
                }
            }
        });

        deleteTitleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберете спектакль для редактирования!",
                                "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        deleteShow.setInt(1, shows.get(titleComboBox.getSelectedItem()));
                    }
                    deleteShow.execute();

                    status.setText("Статус: Успех.");
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: Спектакль не удален!.");
                }
            }
        });

        addTitleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String name = titleNameTextField.getText();

                    int century = -1;
                    if (!titleCenturyTextField.getText().isEmpty()) {
                        century = Integer.parseInt(titleCenturyTextField.getText());
                    }

                    int conductor = -1;
                    if (!conductorComboBox.getSelectedItem().equals("-")) {
                        conductor = conductors.get(conductorComboBox.getSelectedItem());
                    }

                    int productionDesigner = -1;
                    if (!productionDesignerComboBox.getSelectedItem().equals("-")) {
                        productionDesigner = productionDesigners.get(productionDesignerComboBox.getSelectedItem());
                    }

                    int director = -1;
                    if (!directorComboBox.getSelectedItem().equals("-")) {
                        director = directors.get(directorComboBox.getSelectedItem());
                    }

                    int genre = -1;
                    if (!genreComboBox.getSelectedItem().equals("-")) {
                        genre = genres.get(genreComboBox.getSelectedItem());
                    }

                    int age = -1;
                    if (!ageComboBox.getSelectedItem().equals("-")) {
                        age = ageCategories.get(ageComboBox.getSelectedItem());
                    }

                    int author = -1;
                    if (!authorComboBox.getSelectedItem().equals("-")) {
                        author = authors.get(authorComboBox.getSelectedItem());
                    }

                    String premierDate = premierDateTextField.getText();

                    if (name == null || premierDate == null || author == -1 || age == -1 || genre == -1 || director == -1
                            || productionDesigner == -1 || conductor == -1 || century == -1) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                        return;
                    }

                    insertShow.setString(1, name);
                    insertShow.setInt(2, century);
                    insertShow.setInt(3, conductor);
                    insertShow.setInt(4, productionDesigner);
                    insertShow.setInt(5, director);
                    insertShow.setInt(6, genre);
                    insertShow.setInt(7, age);
                    insertShow.setInt(8, author);
                    insertShow.setDate(9, new java.sql.Date(dateFormat.parse(premierDate).getTime()));

                    insertShow.execute();

                    status.setText("Статус: Успех. Спектакль добавлен!");
                    clearTitle();
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: спектакль не добавлен!.");
                }
            }
        });

        saveTitleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (titleComboBox.getSelectedItem().equals("-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберете спектакль для редактирования!",
                                "Ошибка обновления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                    }

                    String name = titleNameTextField.getText();

                    int century = -1;
                    if (!titleCenturyTextField.getText().isEmpty()) {
                        century = Integer.parseInt(titleCenturyTextField.getText());
                    }

                    int conductor = -1;
                    if (!conductorComboBox.getSelectedItem().equals("-")) {
                        conductor = conductors.get(conductorComboBox.getSelectedItem());
                    }

                    int productionDesigner = -1;
                    if (!productionDesignerComboBox.getSelectedItem().equals("-")) {
                        productionDesigner = productionDesigners.get(productionDesignerComboBox.getSelectedItem());
                    }

                    int director = -1;
                    if (directorComboBox.getSelectedItem().equals("-")) {
                        director = directors.get(directorComboBox.getSelectedItem());
                    }

                    int genre = -1;
                    if (!genreComboBox.getSelectedItem().equals("-")) {
                        genre = genres.get(genreComboBox.getSelectedItem());
                    }

                    int age = -1;
                    if (!ageComboBox.getSelectedItem().equals("-")) {
                        age = ageCategories.get(ageComboBox.getSelectedItem());
                    }

                    int author = -1;
                    if (!authorComboBox.getSelectedItem().equals("-")) {
                        author = authors.get(authorComboBox.getSelectedItem());
                    }

                    String premierDate = premierDateTextField.getText();

                    if (name == null || premierDate == null || author == -1 || age == -1 || genre == -1 || director == -1
                            || productionDesigner == -1 || conductor == -1 || century == -1) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка обновления!", JOptionPane.ERROR_MESSAGE);
                    }

                    updateShow.setInt(1, id_show);
                    updateShow.setString(2, name);
                    updateShow.setInt(3, century);
                    updateShow.setInt(4, conductor);
                    updateShow.setInt(5, productionDesigner);
                    updateShow.setInt(6, director);
                    updateShow.setInt(7, genre);
                    updateShow.setInt(8, age);
                    updateShow.setInt(9, author);
                    updateShow.setDate(10, new java.sql.Date(dateFormat.parse(premierDate).getTime()));

                    updateShow.execute();

                    status.setText("Статус: Успех. Спектакль обновлен!");
                    clearRole();
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка обновления!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: Неудача. Спектакль не обновлен!");
                }
            }
        });

        getMusiciansButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль!",
                                "Ошибка выбора музыкантов!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        getMusicians.setInt(1, shows.get(titleComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(instrumentsComboBox.getSelectedItem(), "-")) {
                        getMusicians.setNull(2, OracleTypes.INTEGER);
                    } else {
                        getMusicians.setInt(2, instruments.get(instrumentsComboBox.getSelectedItem()));
                    }
                    getMusicians.execute();

                    ResultSet results = (ResultSet) getMusicians.getObject(3);
                    fillTableFromResultSet(musiciansTable, 2, musicians, results);
                    results.close();
                    setSuccessMessage(status, musiciansTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        addMusicianToTitleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль!",
                                "Ошибка выбора музыкантов!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                        setMusicianForShow.setInt(1, id_show);
                    }
                    if (musiciansTable.getSelectedRow() == -1) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите музыканта!",
                                "Ошибка выбора музыкантов!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        setMusicianForShow.setInt(2, musicians.get(musiciansTable.getSelectedRow()));
                    }
                    setMusicianForShow.execute();

                    updateMusiciansTable(id_show);
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка назначения музыканта на спектакль!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        deleteMusicianFromTitleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль!",
                                "Ошибка удаления музыканта из спектакля!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                        deleteMusicianFromShow.setInt(1, id_show);
                    }
                    if (musiciansTable.getSelectedRow() == -1) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите музыканта!",
                                "Ошибка удаления музыканта из спектакля!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        deleteMusicianFromShow.setInt(2, musicians.get(musiciansTable.getSelectedRow()));
                    }
                    deleteMusicianFromShow.execute();

                    updateMusiciansTable(id_show);
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления музыканта из спектакля!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        editRoleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(roleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите роль!",
                                "Ошибка редактирования роли спектакля!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        getRoleCharacteristics.setInt(1, roles.get(roleComboBox.getSelectedItem()));
                        getRoleInfo.setInt(1, roles.get(roleComboBox.getSelectedItem()));
                    }
                    getRoleCharacteristics.execute();

                    ResultSet results = (ResultSet) getRoleCharacteristics.getObject(2);
                    fillTableFromResultSet(characteristicsTable, 2, roleCharacteristics, results);
                    roleNameTextField.setText((String) roleComboBox.getSelectedItem());

                    getRoleInfo.execute();
                    int is_main = getRoleInfo.getInt(2);
                    isMainRoleCheckBox.setSelected(is_main == 1);

                    results.close();
                    setSuccessMessage(status, characteristicsTable.getRowCount());
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка редактирования роли спектакля!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        deleteRoleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(roleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберете роль!",
                                "Ошибка удаления роли спектакля!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        deleteRole.setInt(1, roles.get(roleComboBox.getSelectedItem()));
                    }
                    deleteRole.execute();

                    status.setText("Статус: Успех. Роль удалена!");
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления роли спектакля!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: Неуспех. Роль не удалена!");
                }
            }
        });

        addRoleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (titleComboBox.getSelectedItem().equals("-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберете спектакль!",
                                "Ошибка добавления роли спектакля!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        insertRole.setInt(2, shows.get(titleComboBox.getSelectedItem()));
                    }
                    if (roleNameTextField.getText().isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Введите название!",
                                "Ошибка добавления роли спектакля!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        insertRole.setString(1, roleNameTextField.getText());
                    }
                    insertRole.setInt(3, isMainRoleCheckBox.isSelected() ? 1 : 0);

                    insertRole.execute();
                    status.setText("Статус: Успех. Роль добавлена!");
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления роли спектакля!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: Роль не добавлена!");
                }
            }
        });

        saveRoleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (Objects.equals(roleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите роль!",
                                "Ошибка удаления роли спектакля!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        updateRole.setInt(1, roles.get(roleComboBox.getSelectedItem()));
                    }
                    if (roleNameTextField.getText().isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Введите название!",
                                "Ошибка редактирования роли спектакля!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        updateRole.setString(2, roleNameTextField.getText());
                    }
                    updateRole.setInt(3, isMainRoleCheckBox.isSelected() ? 1 : 0);

                    updateRole.execute();
                    status.setText("Статус: Успех. Роль отредактирована!");
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка редактирования роли спектакля!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: Роль не отредактирования!");
                }
            }
        });

        addCharacteristicButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_role = -1;
                    if (Objects.equals(roleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберете роль!",
                                "Ошибка добавления характеристики роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        id_role = roles.get(roleComboBox.getSelectedItem());
                        insertRoleCharacteristic.setInt(1, id_role);
                    }
                    if (Objects.equals(characteristicsComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите характеристику!",
                                "Ошибка добавления характеристики роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        insertRoleCharacteristic.setInt(2,
                                characteristics.get(characteristicsComboBox.getSelectedItem()));
                    }
                    if (characteristicValue.getText().isEmpty()) {
                        insertRoleCharacteristic.setNull(3, OracleTypes.DOUBLE);
                    } else {
                        insertRoleCharacteristic.setDouble(3,
                                Double.parseDouble(characteristicValue.getText()));
                    }
                    insertRoleCharacteristic.execute();

                    updateCharacteristicsTable(id_role);
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления характеристики роли!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        deleteCharacteristicButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_role = -1;
                    if (Objects.equals(roleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите роль!",
                                "Ошибка удаления характеристики роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        id_role = roles.get(roleComboBox.getSelectedItem());
                        deleteRoleCharacteristic.setInt(1, id_role);
                    }
                    if (characteristicsTable.getSelectedRow() == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите характеристику!",
                                "Ошибка удаления характеристики роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        deleteRoleCharacteristic.setInt(2,
                                roleCharacteristics.get(characteristicsTable.getSelectedRow()));
                    }
                    deleteRoleCharacteristic.execute();

                    updateCharacteristicsTable(id_role);
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления характеристики роли!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        addAsMainActorButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль!",
                                "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                    }
                    int role = -1;
                    if (Objects.equals(rolesForActorsComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите роль для назначения!",
                                "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        role = roles.get(rolesForActorsComboBox.getSelectedItem());
                    }
                    int actor = -1;
                    if (Objects.equals(actorComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера для назначения!",
                                "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        actor = actors.get(actorComboBox.getSelectedItem());
                    }
                    setActorsToRole.setInt(1, role);
                    setActorsToRole.setInt(2, actor);
                    setActorsToRole.setInt(3, 0);
                    setActorsToRole.execute();

                    updateActorTable(id_show);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        addAsUnderstudyActorButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль!",
                                "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                    }
                    int role = -1;
                    if (Objects.equals(rolesForActorsComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите роль для назначения!",
                                "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        role = roles.get(rolesForActorsComboBox.getSelectedItem());
                    }
                    int actor = -1;
                    if (Objects.equals(actorComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера для назначения!",
                                "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        actor = actors.get(actorComboBox.getSelectedItem());
                    }
                    setActorsToRole.setInt(1, role);
                    setActorsToRole.setInt(2, actor);
                    setActorsToRole.setInt(3, 1);
                    setActorsToRole.execute();

                    updateActorTable(id_show);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка назначения роли!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        addDateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль для редактирования!",
                                "Ошибка добавления выступления!", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                    }
                    if (dateTextField.getText().isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Введите дату выступления!",
                                "Ошибка добавления выступления!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        insertPerformanceDate.setInt(1, id_show);
                        insertPerformanceDate.setDate(2,
                                new java.sql.Date(dateFormat.parse(dateTextField.getText()).getTime()));
                        insertPerformanceDate.execute();

                        updatePerformancesTable(id_show);
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления выступления!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        deleteDateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (performancesTable.getSelectedRow() == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите выступление!",
                                "Ошибка удаления выступления!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        deletePerformanceDate.setInt(1,
                                performances.get(performancesTable.getSelectedRow()));
                        deletePerformanceDate.execute();

                        DefaultTableModel model = (DefaultTableModel) performancesTable.getModel();
                        model.removeRow(performancesTable.getSelectedRow());
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления выступления!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setLocationRelativeTo(null);
        setVisible(true);

        deleteActorFromRoleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_show = -1;
                    if (Objects.equals(titleComboBox.getSelectedItem(), "-")) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите спектакль для редактирования!",
                                "ООшибка снятия актера с роли", JOptionPane.ERROR_MESSAGE);
                        return;
                    } else {
                        id_show = shows.get(titleComboBox.getSelectedItem());
                    }
                    if (actorsTable.getSelectedRow() != -1) {
                        deleteActorsFromRole.setInt(1, showsActorsInfo.get(actorsTable.getSelectedRow()).getValue());
                        deleteActorsFromRole.setInt(2, showsActorsInfo.get(actorsTable.getSelectedRow()).getKey());
                        deleteActorsFromRole.execute();
                    }

                    updateActorTable(id_show);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка снятия актера с роли!", JOptionPane.ERROR_MESSAGE);
                }
            }
        });
    }

    private void createUIComponents() {
        premierDateTextField = new JFormattedTextField(dateFormat);
        dateTextField = new JFormattedTextField(dateFormat);
        titleCenturyTextField = new JFormattedTextField(numberFormat);
        characteristicValue = new JFormattedTextField(numberFormat);
    }

    void updatePerformancesTable(int id_show) throws Exception {
        getPerformancesDate.setInt(1, id_show);
        getPerformancesDate.execute();

        ResultSet results = (ResultSet) getPerformancesDate.getObject(2);
        fillTableFromResultSet(performancesTable, 2, performances, results);
        results.close();
    }

    void updateMusiciansTable(int id_show) throws Exception {
        getMusicians.setInt(1, id_show);
        getMusicians.setNull(2, OracleTypes.INTEGER);
        getMusicians.execute();

        ResultSet results = (ResultSet) getMusicians.getObject(3);
        fillTableFromResultSet(musiciansTable, 2, musicians, results);
        results.close();
    }

    void updateActorTable(int id_show) throws Exception {
        getShowActorInfo.setInt(1, id_show);
        getShowActorInfo.execute();

        ResultSet results = (ResultSet) getShowActorInfo.getObject(2);
        ResultSetMetaData metaData = results.getMetaData();
        int column_num = metaData.getColumnCount();
        DefaultTableModel model = (DefaultTableModel) actorsTable.getModel();
        model.getDataVector().removeAllElements();
        model.setColumnCount(0);
        model.fireTableDataChanged();
        // set column names
        for (int i = 3; i <= column_num; i++) {
            model.addColumn(metaData.getColumnName(i));
        }
        // set table data
        int j = 0;
        while (results.next()) {
            Object[] row = new Object[column_num];
            showsActorsInfo.put(j, new Pair(results.getInt(1), results.getInt(2)));
            for (int i = 3; i <= column_num; i++) {
                row[i - 3] = results.getString(i);
            }
            model.addRow(row.clone());
            j++;
        }
        results.close();
    }

    void clearTitle() {
        titleNameTextField.setText(null);
        titleCenturyTextField.setText(null);
        directorComboBox.setSelectedItem("-");
        authorComboBox.setSelectedItem("-");
        productionDesignerComboBox.setSelectedItem("-");
        actorComboBox.setSelectedItem("-");
        genreComboBox.setSelectedItem("-");
        premierDateTextField.setText(null);
        conductorComboBox.setSelectedItem("-");
    }

    void clearRole() {
        roleNameTextField.setText(null);
        isMainRoleCheckBox.setSelected(false);
    }

    void updateCharacteristicsTable(int id_role) throws Exception {
        getRoleCharacteristics.setInt(1, roles.get(roleComboBox.getSelectedItem()));
        getRoleCharacteristics.execute();

        ResultSet results = (ResultSet) getRoleCharacteristics.getObject(2);
        fillTableFromResultSet(characteristicsTable, 2, roleCharacteristics, results);
        results.close();
    }
}
