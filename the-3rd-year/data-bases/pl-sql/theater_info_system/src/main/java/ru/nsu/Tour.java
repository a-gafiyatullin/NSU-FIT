package ru.nsu;

import javafx.util.Pair;
import oracle.jdbc.OracleTypes;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.*;
import java.util.HashMap;
import java.util.Map;

public class Tour extends Theater {
    private final CallableStatement getEmployees;
    private final Map<String, Integer> employees = new HashMap<>();
    private final Map<String, Integer> shows = new HashMap<>();
    private final CallableStatement getShowTitles;
    private final Map<String, Integer> jobs = new HashMap<>();
    private final CallableStatement getJobTypes;
    private final Map<Integer, Pair<Integer, Integer>> tours = new HashMap<>();
    private final CallableStatement getTours;
    private final CallableStatement deleteTour;
    private final CallableStatement insertTour;
    private JPanel mainPanel;
    private JTable resultTable;
    private JComboBox employeeComboBox;
    private JComboBox showComboBox;
    private JComboBox jobTypeComboBox;
    private JComboBox tourTypeComboBox;
    private JFormattedTextField periodFrom;
    private JFormattedTextField periodTo;
    private JLabel status;
    private JButton addButton;
    private JButton queryButton;
    private JButton deleteButton;

    public Tour(final Connection connection) throws Exception {
        resultTable.getTableHeader().setReorderingAllowed(false);
        resultTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });

        getEmployees = connection.prepareCall("{call get_art_employees_list(?)}");
        getEmployees.registerOutParameter("list", OracleTypes.CURSOR);

        getShowTitles = connection.prepareCall("{call get_shows_list(?)}");
        getShowTitles.registerOutParameter("list", OracleTypes.CURSOR);

        getJobTypes = connection.prepareCall("{call get_job_types_list(?)}");
        getJobTypes.registerOutParameter("list", OracleTypes.CURSOR);

        getTours = connection.prepareCall("{call tours_info(?, ?, ?, ?, ?, ?, ?)}");
        getTours.registerOutParameter(7, OracleTypes.CURSOR);
        deleteTour = connection.prepareCall("{call tours_delete(?, ?, ?, ?)}");
        insertTour = connection.prepareCall("{call tours_insert(?, ?, ?, ?, ?)}");

        showComboBoxListFromSQL(employeeComboBox, getEmployees, employees, "id_employee", "name");
        showComboBoxListFromSQL(showComboBox, getShowTitles, shows, "id_show", "name_show");
        showComboBoxListFromSQL(jobTypeComboBox, getJobTypes, jobs, "id_job_type", "name_job_type");

        employeeComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(employeeComboBox, getEmployees, employees, "id_employee", "name");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        showComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(showComboBox, getShowTitles, shows, "id_show", "name_show");
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
                showComboBoxListFromSQL(jobTypeComboBox, getJobTypes, jobs, "id_job_type", "name_job_type");
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
                    if (employeeComboBox.getSelectedItem().equals("-")) {
                        getTours.setNull(1, OracleTypes.INTEGER);
                    } else {
                        getTours.setInt(1, employees.get(employeeComboBox.getSelectedItem()));
                    }
                    if (showComboBox.getSelectedItem().equals("-")) {
                        getTours.setNull(2, OracleTypes.INTEGER);
                    } else {
                        getTours.setInt(2, shows.get(showComboBox.getSelectedItem()));
                    }
                    if (periodFrom.getText().isEmpty()) {
                        getTours.setNull(3, OracleTypes.DATE);
                    } else {
                        getTours.setDate(3, new java.sql.Date(dateFormat.parse(periodFrom.getText()).getTime()));
                    }
                    if (periodTo.getText().isEmpty()) {
                        getTours.setNull(4, OracleTypes.DATE);
                    } else {
                        getTours.setDate(4, new java.sql.Date(dateFormat.parse(periodTo.getText()).getTime()));
                    }
                    if (tourTypeComboBox.getSelectedItem().equals("-")) {
                        getTours.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getTours.setInt(5, (tourTypeComboBox.getSelectedItem().equals("выездные") ? 1 : 0));
                    }
                    if (jobTypeComboBox.getSelectedItem().equals("-")) {
                        getTours.setNull(6, OracleTypes.INTEGER);
                    } else {
                        getTours.setInt(6, jobs.get(jobTypeComboBox.getSelectedItem()));
                    }
                    getTours.execute();

                    ResultSet results = (ResultSet) getTours.getObject(7);
                    ResultSetMetaData metaData = results.getMetaData();
                    int column_num = metaData.getColumnCount();
                    DefaultTableModel model = (DefaultTableModel) resultTable.getModel();
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
                        tours.put(j, new Pair(results.getInt(1), results.getInt(2)));
                        for (int i = 3; i <= column_num; i++) {
                            row[i - 3] = results.getString(i);
                        }
                        model.addRow(row.clone());
                        j++;
                    }
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

                    employeeComboBox.setSelectedItem(model.getValueAt(selectedRow, 0));
                    jobTypeComboBox.setSelectedItem(model.getValueAt(selectedRow, 1));
                    showComboBox.setSelectedItem(model.getValueAt(selectedRow, 2));
                    periodFrom.setText((String) model.getValueAt(selectedRow, 3));
                    periodTo.setText((String) model.getValueAt(selectedRow, 4));
                    tourTypeComboBox.setSelectedItem(model.getValueAt(selectedRow, 5));
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });

        addButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_employee = 0;
                    if (!employeeComboBox.getSelectedItem().equals("-")) {
                        id_employee = employees.get(employeeComboBox.getSelectedItem());
                    }
                    int id_show = 0;
                    if (!showComboBox.getSelectedItem().equals("-")) {
                        id_show = shows.get(showComboBox.getSelectedItem());
                    }
                    Date dateFrom = null;
                    if (!periodFrom.getText().isEmpty()) {
                        getTours.setNull(3, OracleTypes.DATE);
                        dateFrom = new java.sql.Date(dateFormat.parse(periodFrom.getText()).getTime());
                    }
                    Date dateTo = null;
                    if (!periodTo.getText().isEmpty()) {
                        dateTo = new java.sql.Date(dateFormat.parse(periodTo.getText()).getTime());
                    }
                    int type = -1;
                    if (!tourTypeComboBox.getSelectedItem().equals("-")) {
                        type = (tourTypeComboBox.getSelectedItem().equals("выездные") ? 1 : 0);
                    }

                    if (id_employee * id_show == 0 || dateFrom == null || dateTo == null || type == -1) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                    } else {

                        insertTour.setInt(1, id_employee);
                        insertTour.setInt(2, id_show);
                        insertTour.setDate(3, dateFrom);
                        insertTour.setDate(4, dateTo);
                        insertTour.setInt(5, type);
                        insertTour.execute();

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

                    deleteTour.setInt(1, tours.get(selectedRow).getKey());
                    deleteTour.setInt(2, tours.get(selectedRow).getValue());
                    deleteTour.setDate(3,
                            new java.sql.Date(dateFormat.parse((String) resultTable.getValueAt(selectedRow, 3)).getTime()));
                    deleteTour.setDate(4,
                            new java.sql.Date(dateFormat.parse((String) resultTable.getValueAt(selectedRow, 4)).getTime()));
                    deleteTour.execute();

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

    private void createUIComponents() {
        periodTo = new JFormattedTextField(dateFormat);
        periodFrom = new JFormattedTextField(dateFormat);
    }

    private void updateResultTable() {
        employeeComboBox.setSelectedItem("-");
        jobTypeComboBox.setSelectedItem("-");
        showComboBox.setSelectedItem("-");
        periodFrom.setText(null);
        periodTo.setText(null);
        tourTypeComboBox.setSelectedItem("-");

        queryButton.doClick();
    }
}
