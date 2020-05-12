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

public class Authors extends Theater {
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final Map<String, Integer> countries = new HashMap<>();
    private final CallableStatement getCountries;
    private final Map<Integer, Integer> authors = new HashMap<>();
    private final CallableStatement getAuthorsInfo;
    private final CallableStatement addAuthor;
    private final CallableStatement saveAuthor;
    private final CallableStatement deleteAuthor;
    private final CallableStatement getAuthorShows;
    private JTable resultTable;
    private JFormattedTextField centuryTo;
    private JButton queryButton;
    private JButton saveButton;
    private JButton deleteButton;
    private JTextField nameTextField;
    private JTextField surnameTextField;
    private JTextField middleNameTextField;
    private JComboBox countryComboBox;
    private JFormattedTextField century;
    private JFormattedTextField dateFrom;
    private JFormattedTextField dateTo;
    private JComboBox genreComboBox;
    private JButton queryButton2;
    private JPanel mainPanel;
    private JButton addButton;
    private JLabel status;
    private JFormattedTextField centuryFrom;

    public Authors(final Connection connection) throws Exception {
        resultTable.getTableHeader().setReorderingAllowed(false);
        resultTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });

        getGenres = connection.prepareCall("{call get_genres_list(?)}");
        getGenres.registerOutParameter("list", OracleTypes.CURSOR);

        getCountries = connection.prepareCall("{call get_countries_list(?)}");
        getCountries.registerOutParameter("list", OracleTypes.CURSOR);

        getAuthorsInfo = connection.prepareCall("{call author_info(?, ?, ?, ?, ?, ?, ?)}");
        getAuthorsInfo.registerOutParameter(7, OracleTypes.CURSOR);

        addAuthor = connection.prepareCall("{call author_insert(?, ?, ?, ?, ?)}");

        saveAuthor = connection.prepareCall("{call author_update(?, ?, ?, ?, ?, ?)}");

        deleteAuthor = connection.prepareCall("{call author_delete(?)}");

        getAuthorShows = connection.prepareCall("{call author_shows(?, ?, ?, ?, ?)}");
        getAuthorShows.registerOutParameter(5, OracleTypes.CURSOR);

        showComboBoxListFromSQL(countryComboBox, getCountries, countries, "id_country",
                "name_country");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");

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
                    resultTable.setEnabled(true);
                    // process query
                    if (nameTextField.getText().isEmpty()) {
                        getAuthorsInfo.setNull(1, OracleTypes.VARCHAR);
                    } else {
                        getAuthorsInfo.setString(1, nameTextField.getText());
                    }
                    if (surnameTextField.getText().isEmpty()) {
                        getAuthorsInfo.setNull(2, OracleTypes.VARCHAR);
                    } else {
                        getAuthorsInfo.setString(2, surnameTextField.getText());
                    }
                    if (middleNameTextField.getText().isEmpty()) {
                        getAuthorsInfo.setNull(3, OracleTypes.VARCHAR);
                    } else {
                        getAuthorsInfo.setString(3, middleNameTextField.getText());
                    }
                    if (centuryFrom.getText().isEmpty()) {
                        getAuthorsInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getAuthorsInfo.setInt(4, Integer.parseInt(centuryFrom.getText()));
                    }
                    if (centuryTo.getText().isEmpty()) {
                        getAuthorsInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getAuthorsInfo.setInt(5, Integer.parseInt(centuryTo.getText()));
                    }
                    if (countryComboBox.getSelectedItem().equals("-")) {
                        getAuthorsInfo.setNull(6, OracleTypes.INTEGER);
                    } else {
                        getAuthorsInfo.setInt(6, countries.get(countryComboBox.getSelectedItem()));
                    }
                    getAuthorsInfo.execute();

                    // get results
                    ResultSet results = (ResultSet) getAuthorsInfo.getObject(7);
                    fillTableFromResultSet(resultTable, 2, authors, results);
                    status.setText("Статус: Успех. Возвращено " + resultTable.getRowCount() + " записей.");
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
                }
            }
        });

        ListSelectionModel selectionModel = resultTable.getSelectionModel();
        selectionModel.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                try {
                    if (resultTable.getSelectedRows().length == 0) {
                        return;
                    }
                    DefaultTableModel model = (DefaultTableModel) resultTable.getModel();
                    int selectedRow = resultTable.getSelectedRows()[0];

                    nameTextField.setText((String) model.getValueAt(selectedRow, 0));
                    surnameTextField.setText((String) model.getValueAt(selectedRow, 1));
                    middleNameTextField.setText((String) model.getValueAt(selectedRow, 2));
                    century.setText((String) model.getValueAt(selectedRow, 3));
                    countryComboBox.setSelectedItem(model.getValueAt(selectedRow, 4));

                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        addButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String name = null;
                    if (!nameTextField.getText().isEmpty()) {
                        name = nameTextField.getText();
                    }
                    String surname = null;
                    if (!surnameTextField.getText().isEmpty()) {
                        surname = surnameTextField.getText();
                    }
                    String middle_name = null;
                    if (!middleNameTextField.getText().isEmpty()) {
                        middle_name = middleNameTextField.getText();
                    }
                    int country = countries.get(countryComboBox.getSelectedItem());
                    String century_str = century.getText();
                    if (name == null || name.isEmpty() || country == 0 || century_str == null
                            || century_str.isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                    } else {
                        addAuthor.setString(1, name);
                        addAuthor.setString(2, surname);
                        addAuthor.setString(3, middle_name);
                        addAuthor.setInt(4, Integer.parseInt(century_str));
                        addAuthor.setInt(5, country);
                        addAuthor.execute();

                        updateResultTable();
                        status.setText("Статус: запись добавлена успешно!");
                    }
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: ошибка добавления записи!");
                }
            }
        });

        saveButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (resultTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выбирете запись для редактирования!",
                                "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    String name = null;
                    if (!nameTextField.getText().isEmpty()) {
                        name = nameTextField.getText();
                    }
                    String surname = null;
                    if (!surnameTextField.getText().isEmpty()) {
                        surname = surnameTextField.getText();
                    }
                    String middle_name = null;
                    if (!middleNameTextField.getText().isEmpty()) {
                        middle_name = middleNameTextField.getText();
                    }
                    int country = countries.get(countryComboBox.getSelectedItem());
                    String century_str = century.getText();
                    int selectedRow = resultTable.getSelectedRows()[0];
                    if (name == null || name.equals("") || country == 0 || century_str == null
                            || century_str.isEmpty()) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                    } else {
                        saveAuthor.setString(1, name);
                        saveAuthor.setString(2, surname);
                        saveAuthor.setString(3, middle_name);
                        saveAuthor.setInt(4, Integer.parseInt(century_str));
                        saveAuthor.setInt(5, country);
                        saveAuthor.setInt(6, authors.get(selectedRow));
                        saveAuthor.execute();

                        updateResultTable();
                        status.setText("Статус: запись обновлена успешно!");
                    }
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: ошибка обновления записи!");
                }
            }
        });

        deleteButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (resultTable.getSelectedRows().length == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выбирете запись для удаления!",
                                "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    int selectedRow = resultTable.getSelectedRows()[0];

                    deleteAuthor.setInt(1, authors.get(selectedRow));
                    deleteAuthor.execute();

                    updateResultTable();
                    status.setText("Статус: запись удалена успешно!");
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                    status.setText("Статус: ошибка удаления записи!");
                }
            }
        });

        queryButton2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    // process query
                    if (dateFrom.getText().isEmpty()) {
                        getAuthorShows.setNull(1, OracleTypes.VARCHAR);
                    } else {
                        getAuthorShows.setDate(1, new java.sql.Date(dateFormat.parse(dateFrom.getText()).getTime()));
                    }
                    if (dateTo.getText().isEmpty()) {
                        getAuthorShows.setNull(2, OracleTypes.VARCHAR);
                    } else {
                        getAuthorShows.setDate(2, new java.sql.Date(dateFormat.parse(dateTo.getText()).getTime()));
                    }
                    if (genreComboBox.getSelectedItem().equals("-")) {
                        getAuthorShows.setNull(3, OracleTypes.INTEGER);
                    } else {
                        getAuthorShows.setInt(3, genres.get(genreComboBox.getSelectedItem()));
                    }
                    if (resultTable.getSelectedRows().length != 0) {
                        getAuthorShows.setInt(4, authors.get(resultTable.getSelectedRows()[0]));
                    } else {
                        getAuthorShows.setNull(4, OracleTypes.INTEGER);
                    }
                    getAuthorShows.execute();

                    // get results
                    ResultSet results = (ResultSet) getAuthorShows.getObject(5);
                    fillTableFromResultSet(resultTable, 2, null, results);
                    status.setText("Статус: Успех. Возвращено " + resultTable.getRowCount() + " записей.");

                    resultTable.setEnabled(false);
                } catch (Exception exception) {
                    exception.printStackTrace();
                    status.setText("Статус: запрос не выполнен.");
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setVisible(true);
    }

    private void updateResultTable() {
        nameTextField.setText(null);
        surnameTextField.setText(null);
        middleNameTextField.setText(null);
        countryComboBox.setSelectedItem("-");
        century.setText(null);
        queryButton.doClick();
    }

    private void createUIComponents() {
        centuryFrom = new JFormattedTextField(NumberFormat.getNumberInstance());
        centuryTo = new JFormattedTextField(NumberFormat.getNumberInstance());
        century = new JFormattedTextField(NumberFormat.getNumberInstance());
        dateFrom = new JFormattedTextField(dateFormat);
        dateTo = new JFormattedTextField(dateFormat);
    }
}
