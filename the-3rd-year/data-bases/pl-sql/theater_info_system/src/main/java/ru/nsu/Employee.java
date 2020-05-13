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

public class Employee extends Theater {
    private final Map<String, Integer> genders = new HashMap<>();
    private final CallableStatement getGenders;
    private final Map<String, Integer> educations = new HashMap<>();
    private final CallableStatement getEducation;
    private final Map<String, Integer> jobs = new HashMap<>();
    private final CallableStatement getJobs;
    private final Map<Integer, Integer> employees = new HashMap<>();
    private final CallableStatement getEmployeesInfo;
    private final CallableStatement addEmployee;
    private final CallableStatement updateEmployee;
    private final CallableStatement deleteEmployee;
    private JTable resultTable;
    private JTextField nameTextField;
    private JTextField surnameTextField;
    private JTextField middleNameTextField;
    private JComboBox genderComboBox;
    private JFormattedTextField birthdayTextField;
    private JFormattedTextField hireDateTextField;
    private JFormattedTextField childrenTextField;
    private JFormattedTextField salaryTextField;
    private JComboBox educationComboBox;
    private JComboBox jobTypeComboBox;
    private JFormattedTextField ageToTextField;
    private JFormattedTextField ageFromTextField;
    private JFormattedTextField birthdayFromTextField;
    private JFormattedTextField birthdayToTextField;
    private JFormattedTextField experienceFromTextField;
    private JFormattedTextField experienceToTextField;
    private JFormattedTextField childrenFromTextField;
    private JFormattedTextField childrenToTextField;
    private JFormattedTextField salaryFromTextField;
    private JFormattedTextField salaryToTextField;
    private JButton deleteButton;
    private JButton saveButton;
    private JButton addButton;
    private JButton queryButton;
    private JPanel mainPanel;
    private JLabel status;

    public Employee(final Connection connection) throws Exception {
        resultTable.getTableHeader().setReorderingAllowed(false);
        resultTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });

        getGenders = connection.prepareCall("{call get_genders_list(?)}");
        getGenders.registerOutParameter("list", OracleTypes.CURSOR);

        getEducation = connection.prepareCall("{call get_education_list(?)}");
        getEducation.registerOutParameter("list", OracleTypes.CURSOR);

        getJobs = connection.prepareCall("{call get_job_types_list(?)}");
        getJobs.registerOutParameter("list", OracleTypes.CURSOR);

        getEmployeesInfo = connection.prepareCall("{call employee_info(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
        getEmployeesInfo.registerOutParameter(18, OracleTypes.CURSOR);

        addEmployee = connection.prepareCall("{call employee_insert(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");

        updateEmployee = connection.prepareCall("{call employee_update(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");

        deleteEmployee = connection.prepareCall("{call employee_delete(?)}");

        showComboBoxListFromSQL(genderComboBox, getGenders, genders, "id_gender", "name_gender");
        showComboBoxListFromSQL(educationComboBox, getEducation, educations, "id_education", "name_education");
        showComboBoxListFromSQL(jobTypeComboBox, getJobs, jobs, "id_job_type",
                "name_job_type");

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

        educationComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(educationComboBox, getEducation, educations, "id_education",
                        "name_education");
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
                showComboBoxListFromSQL(jobTypeComboBox, getJobs, jobs, "id_job_type", "name_job_type");
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
                    getEmployeesInfo.setNull(1, OracleTypes.INTEGER);
                    if (nameTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(2, OracleTypes.VARCHAR);
                    } else {
                        getEmployeesInfo.setString(2, nameTextField.getText());
                    }
                    if (surnameTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(3, OracleTypes.VARCHAR);
                    } else {
                        getEmployeesInfo.setString(3, surnameTextField.getText());
                    }
                    if (middleNameTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(4, OracleTypes.VARCHAR);
                    } else {
                        getEmployeesInfo.setString(4, middleNameTextField.getText());
                    }
                    if (genderComboBox.getSelectedItem().equals("-")) {
                        getEmployeesInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(5, genders.get(genderComboBox.getSelectedItem()));
                    }
                    if (birthdayFromTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(6, OracleTypes.VARCHAR);
                    } else {
                        getEmployeesInfo.setDate(6,
                                new java.sql.Date(dateFormat.parse(birthdayFromTextField.getText()).getTime()));
                    }
                    if (birthdayToTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(7, OracleTypes.VARCHAR);
                    } else {
                        getEmployeesInfo.setDate(7,
                                new java.sql.Date(dateFormat.parse(birthdayToTextField.getText()).getTime()));
                    }
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
                    if (experienceFromTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(10, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(10, Integer.parseInt(experienceFromTextField.getText()));
                    }
                    if (experienceToTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(11, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(11, Integer.parseInt(experienceToTextField.getText()));
                    }
                    if (childrenFromTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(12, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(12, Integer.parseInt(childrenFromTextField.getText()));
                    }
                    if (childrenToTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(13, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(13, Integer.parseInt(childrenToTextField.getText()));
                    }
                    if (salaryFromTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(14, OracleTypes.DOUBLE);
                    } else {
                        getEmployeesInfo.setDouble(14, Double.parseDouble(salaryFromTextField.getText()));
                    }
                    if (salaryToTextField.getText().isEmpty()) {
                        getEmployeesInfo.setNull(15, OracleTypes.DOUBLE);
                    } else {
                        getEmployeesInfo.setDouble(15, Double.parseDouble(salaryToTextField.getText()));
                    }
                    if (educationComboBox.getSelectedItem().equals("-")) {
                        getEmployeesInfo.setNull(16, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(16, educations.get(educationComboBox.getSelectedItem()));
                    }
                    if (jobTypeComboBox.getSelectedItem().equals("-")) {
                        getEmployeesInfo.setNull(17, OracleTypes.INTEGER);
                    } else {
                        getEmployeesInfo.setInt(17, jobs.get(jobTypeComboBox.getSelectedItem()));
                    }
                    getEmployeesInfo.execute();

                    // get results
                    ResultSet results = (ResultSet) getEmployeesInfo.getObject(18);
                    fillTableFromResultSet(resultTable, 2, employees, results);
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
                    genderComboBox.setSelectedItem(model.getValueAt(selectedRow, 3));
                    birthdayTextField.setText((String) model.getValueAt(selectedRow, 4));
                    hireDateTextField.setText((String) model.getValueAt(selectedRow, 5));
                    childrenTextField.setText((String) model.getValueAt(selectedRow, 6));
                    salaryTextField.setText((String) model.getValueAt(selectedRow, 7));
                    educationComboBox.setSelectedItem(model.getValueAt(selectedRow, 8));
                    jobTypeComboBox.setSelectedItem(model.getValueAt(selectedRow, 9));

                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        addButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String name = nameTextField.getText();
                    String surname = surnameTextField.getText();
                    String middle_name = middleNameTextField.getText();
                    int gender = genders.get(genderComboBox.getSelectedItem());
                    String birthday = birthdayTextField.getText();
                    String hire_date = hireDateTextField.getText();
                    String children_amount = childrenTextField.getText();
                    String salary = salaryTextField.getText();
                    int education = educations.get(educationComboBox.getSelectedItem());
                    int job = jobs.get(jobTypeComboBox.getSelectedItem());
                    if (name.isEmpty() || gender == 0 || birthday.isEmpty() || hire_date.isEmpty()
                            || children_amount.isEmpty() || salary.isEmpty()
                            || education == 0 || job == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                    } else {

                        addEmployee.setString(1, name);
                        addEmployee.setString(2, surname);
                        addEmployee.setString(3, middle_name);
                        addEmployee.setInt(4, gender);
                        addEmployee.setDate(5, new java.sql.Date(dateFormat.parse(birthday).getTime()));
                        addEmployee.setDate(6, new java.sql.Date(dateFormat.parse(hire_date).getTime()));
                        addEmployee.setInt(7, Integer.parseInt(children_amount));
                        addEmployee.setDouble(8, Double.parseDouble(salary));
                        addEmployee.setInt(9, education);
                        addEmployee.setInt(10, job);
                        addEmployee.execute();

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
                    int selectedRow = resultTable.getSelectedRows()[0];
                    String name = nameTextField.getText();
                    String surname = surnameTextField.getText();
                    String middle_name = middleNameTextField.getText();
                    int gender = genders.get(genderComboBox.getSelectedItem());
                    String birthday = birthdayTextField.getText();
                    String hire_date = hireDateTextField.getText();
                    String children_amount = childrenTextField.getText();
                    String salary = salaryTextField.getText();
                    int education = educations.get(educationComboBox.getSelectedItem());
                    int job = jobs.get(jobTypeComboBox.getSelectedItem());
                    if (name.isEmpty() || gender == 0 || birthday.isEmpty() || hire_date.isEmpty()
                            || children_amount.isEmpty() || salary.isEmpty()
                            || education == 0 || job == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                    } else {
                        updateEmployee.setString(1, name);
                        updateEmployee.setString(2, surname);
                        updateEmployee.setString(3, middle_name);
                        updateEmployee.setInt(4, gender);
                        updateEmployee.setDate(5, new java.sql.Date(dateFormat.parse(birthday).getTime()));
                        updateEmployee.setDate(6, new java.sql.Date(dateFormat.parse(hire_date).getTime()));
                        updateEmployee.setInt(7, Integer.parseInt(children_amount));
                        updateEmployee.setDouble(8, Double.parseDouble(salary));
                        updateEmployee.setInt(9, education);
                        updateEmployee.setInt(10, job);
                        updateEmployee.setInt(11, employees.get(selectedRow));
                        updateEmployee.execute();

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

                    deleteEmployee.setInt(1, employees.get(selectedRow));
                    deleteEmployee.execute();

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

        setContentPane(mainPanel);
        pack();
        setVisible(true);
    }

    private void updateResultTable() {
        nameTextField.setText(null);
        surnameTextField.setText(null);
        middleNameTextField.setText(null);
        genderComboBox.setSelectedItem("-");
        birthdayTextField.setText(null);
        hireDateTextField.setText(null);
        childrenTextField.setText(null);
        salaryTextField.setText(null);
        educationComboBox.setSelectedItem("-");
        jobTypeComboBox.setSelectedItem("-");
        queryButton.doClick();
    }

    private void createUIComponents() {
        birthdayTextField = new JFormattedTextField(dateFormat);
        hireDateTextField = new JFormattedTextField(dateFormat);
        childrenTextField = new JFormattedTextField(numberFormat);
        salaryTextField = new JFormattedTextField(numberFormat);
        ageToTextField = new JFormattedTextField(numberFormat);
        ageFromTextField = new JFormattedTextField(numberFormat);
        birthdayFromTextField = new JFormattedTextField(dateFormat);
        birthdayToTextField = new JFormattedTextField(dateFormat);
        experienceFromTextField = new JFormattedTextField(numberFormat);
        experienceToTextField = new JFormattedTextField(numberFormat);
        childrenFromTextField = new JFormattedTextField(numberFormat);
        childrenToTextField = new JFormattedTextField(numberFormat);
        salaryFromTextField = new JFormattedTextField(numberFormat);
        salaryToTextField = new JFormattedTextField(numberFormat);
    }
}
