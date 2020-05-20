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

public class Directors extends DatabaseUtils {
    private final Map<String, Integer> directors = new HashMap<>();
    private final CallableStatement getDirectors;
    private final Map<String, Integer> genders = new HashMap<>();
    private final CallableStatement getGenders;
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final Map<String, Integer> ageCategories = new HashMap<>();
    private final CallableStatement getAgeCategories;
    private final Map<String, Integer> directorTypes = new HashMap<>();
    private final CallableStatement getDirectorTypes;
    private final Map<Integer, Integer> employees = new HashMap<>();
    private final CallableStatement getEmployeesInfo;
    private final CallableStatement updateEmployee;
    private final CallableStatement getShowInfo;

    private JPanel mainPanel;
    private JTable resultTable;
    private JComboBox directorComboBox;
    private JComboBox genderComboBox;
    private JComboBox jobTypeComboBox;
    private JFormattedTextField ageFromTextField;
    private JFormattedTextField ageToTextField;
    private JButton queryButton;
    private JButton saveButton;
    private JComboBox genreComboBox;
    private JComboBox ageComboBox;
    private JFormattedTextField periodFromTextField;
    private JFormattedTextField periodToTextField;
    private JLabel status;
    private JButton queryButton2;
    private int id_director_type;

    Directors(final Connection connection, String role) throws Exception {
        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        if (!role.equals("headmaster")) {
            saveButton.setVisible(false);
        }

        resultTable.getTableHeader().setReorderingAllowed(false);
        resultTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });

        getGenres = connection.prepareCall("{call get_genres_list(?)}");
        getGenres.registerOutParameter("list", OracleTypes.CURSOR);

        getAgeCategories = connection.prepareCall("{call get_age_categories_list(?)}");
        getAgeCategories.registerOutParameter("list", OracleTypes.CURSOR);

        getGenders = connection.prepareCall("{call get_genders_list(?)}");
        getGenders.registerOutParameter("list", OracleTypes.CURSOR);

        getDirectorTypes = connection.prepareCall("{call get_director_types_list(?, ?)}");
        getDirectorTypes.registerOutParameter("list", OracleTypes.CURSOR);
        getDirectorTypes.registerOutParameter("id_director_type", OracleTypes.INTEGER);

        getDirectors = connection.prepareCall("{call get_all_types_directors_list(?)}");
        getDirectors.registerOutParameter("list", OracleTypes.CURSOR);

        getEmployeesInfo
                = connection.prepareCall("{call employee_info(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getEmployeesInfo.registerOutParameter(18, OracleTypes.CURSOR);

        updateEmployee = connection.prepareCall("{call employee_update(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");

        getShowInfo = connection.prepareCall("{call director_shows(?, ?, ?, ?, ?, ?)}");
        getShowInfo.registerOutParameter(6, OracleTypes.CURSOR);

        showComboBoxListFromSQL(directorComboBox, getDirectors, directors, "id_employee", "name");
        showComboBoxListFromSQL(genderComboBox, getGenders, genders, "id_gender", "name_gender");
        showComboBoxListFromSQL(jobTypeComboBox, getDirectorTypes, directorTypes, "id_job_type", "name_job_type");
        id_director_type = getDirectorTypes.getInt("id_director_type");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageComboBox, getAgeCategories, ageCategories, "id_age_category", "name_age_category");

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

        genderComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(genderComboBox, getGenders, genders, "id_gender", "name_gender");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        jobTypeComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(jobTypeComboBox, getDirectorTypes, directorTypes, "id_job_type",
                        "name_job_type");
                try {
                    id_director_type = getDirectorTypes.getInt("id_director_type");
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
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

        queryButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    resultTable.setEnabled(true);
                    // process query
                    if (Objects.equals(directorComboBox.getSelectedItem(), "-")) {
                        getEmployeesInfo.setNull(1, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(1, directors.get((directorComboBox.getSelectedItem())));
                    }
                    getEmployeesInfo.setNull(2, OracleTypes.VARCHAR);
                    getEmployeesInfo.setNull(3, OracleTypes.VARCHAR);
                    getEmployeesInfo.setNull(4, OracleTypes.VARCHAR);
                    if (Objects.equals(genderComboBox.getSelectedItem(), "-")) {
                        getEmployeesInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(5, genders.get(genderComboBox.getSelectedItem()));
                    }
                    getEmployeesInfo.setNull(6, OracleTypes.VARCHAR);
                    getEmployeesInfo.setNull(7, OracleTypes.VARCHAR);
                    if (ageFromTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(8, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(8, Integer.parseInt(ageFromTextField.getText()));
                    }
                    if (ageToTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(9, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(9, Integer.parseInt(ageToTextField.getText()));
                    }
                    getEmployeesInfo.setNull(10, OracleTypes.INTEGER);
                    getEmployeesInfo.setNull(11, OracleTypes.INTEGER);
                    getEmployeesInfo.setNull(12, OracleTypes.INTEGER);
                    getEmployeesInfo.setNull(13, OracleTypes.INTEGER);
                    getEmployeesInfo.setNull(14, OracleTypes.DOUBLE);
                    getEmployeesInfo.setNull(15, OracleTypes.DOUBLE);
                    getEmployeesInfo.setNull(16, OracleTypes.INTEGER);
                    if (Objects.equals(jobTypeComboBox.getSelectedItem(), "-")) {
                        getEmployeesInfo.setInt(17, id_director_type);
                    } else {
                        getEmployeesInfo.setInt(17, directorTypes.get(jobTypeComboBox.getSelectedItem()));
                    }
                    getEmployeesInfo.execute();

                    // get results
                    ResultSet results = (ResultSet) getEmployeesInfo.getObject(18);
                    fillTableFromResultSet(resultTable, 2, employees, results);
                    results.close();
                    setSuccessMessage(status, resultTable.getRowCount());
                } catch (Exception exception) {
                    exception.printStackTrace();
                    setFailMessage(status);
                }
            }
        });

        ListSelectionModel selectionModel = resultTable.getSelectionModel();
        selectionModel.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent e) {
                try {
                    if (resultTable.getSelectedRow() == -1) {
                        return;
                    }
                    DefaultTableModel model = (DefaultTableModel) resultTable.getModel();
                    int selectedRow = resultTable.getSelectedRow();

                    jobTypeComboBox.setSelectedItem(model.getValueAt(selectedRow, 9));
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        saveButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    if (resultTable.getSelectedRow() == -1) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите запись для редактирования!",
                                "Ошибка редактирования!", JOptionPane.ERROR_MESSAGE);
                        return;
                    }
                    int selectedRow = resultTable.getSelectedRow();
                    int job = directorTypes.get(jobTypeComboBox.getSelectedItem());
                    if (job == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка редактирования!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        updateEmployee.setNull(1, OracleTypes.VARCHAR);
                        updateEmployee.setNull(2, OracleTypes.VARCHAR);
                        updateEmployee.setNull(3, OracleTypes.VARCHAR);
                        updateEmployee.setNull(4, OracleTypes.INTEGER);
                        updateEmployee.setNull(5, OracleTypes.DATE);
                        updateEmployee.setNull(6, OracleTypes.DATE);
                        updateEmployee.setNull(7, OracleTypes.INTEGER);
                        updateEmployee.setNull(8, OracleTypes.DOUBLE);
                        updateEmployee.setNull(9, OracleTypes.INTEGER);
                        updateEmployee.setInt(10, job);
                        updateEmployee.setInt(11, employees.get(selectedRow));
                        updateEmployee.execute();

                        updateResultTable();
                    }
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка редактирования!", JOptionPane.ERROR_MESSAGE);
                    exception.printStackTrace();
                }
            }
        });

        queryButton2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (resultTable.getSelectedRow() == -1) {
                    JOptionPane.showMessageDialog(mainPanel, "Выберите постановщика!",
                            "Ошибка запроса!", JOptionPane.ERROR_MESSAGE);
                    setFailMessage(status);
                    return;
                }
                int selectedRow = resultTable.getSelectedRow();
                resultTable.setEnabled(false);
                try {
                    if (periodFromTextField.getText().isEmpty()) {
                        getShowInfo.setNull(1, OracleTypes.DATE);
                    } else {
                        getShowInfo.setDate(1,
                                new java.sql.Date(dateFormat.parse(periodFromTextField.getText()).getTime()));
                    }
                    if (periodToTextField.getText().isEmpty()) {
                        getShowInfo.setNull(2, OracleTypes.DATE);
                    } else {
                        getShowInfo.setDate(2,
                                new java.sql.Date(dateFormat.parse(periodToTextField.getText()).getTime()));
                    }
                    if (Objects.equals(genreComboBox.getSelectedItem(), "-")) {
                        getShowInfo.setNull(3, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(3, genres.get(genreComboBox.getSelectedItem()));
                    }
                    if (Objects.equals(ageComboBox.getSelectedItem(), "-")) {
                        getShowInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getShowInfo.setInt(4, ageCategories.get(ageComboBox.getSelectedItem()));
                    }
                    getShowInfo.setInt(5, employees.get(selectedRow));
                    getShowInfo.execute();

                    ResultSet results = (ResultSet) getShowInfo.getObject(6);
                    fillTableFromResultSet(resultTable, 2, null, results);
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
        ageToTextField = new JFormattedTextField(numberFormat);
        ageFromTextField = new JFormattedTextField(numberFormat);
        periodToTextField = new JFormattedTextField(dateFormat);
        periodFromTextField = new JFormattedTextField(dateFormat);
    }

    private void updateResultTable() {
        genderComboBox.setSelectedItem("-");
        jobTypeComboBox.setSelectedItem("-");
        queryButton.doClick();
    }
}