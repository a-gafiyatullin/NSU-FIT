package ru.nsu;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.sql.CallableStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Map;

public abstract class Theater extends JFrame {

    protected static DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");

    // get and update entries list from database
    protected static void showComboBoxListFromSQL(JComboBox comboBox, CallableStatement procedure, Map entries,
                                                  String id_str, String name_str) {
        try {
            procedure.execute();
            comboBox.removeAllItems();
            entries.clear();
            ResultSet resultSet = (ResultSet) procedure.getObject("list");
            comboBox.addItem("-");
            while (resultSet.next()) {
                String title = resultSet.getString(name_str);
                entries.put(title, resultSet.getInt(id_str));
                comboBox.addItem(title);
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
    }

    protected static void fillTableFromResultSet(JTable table, int start_idx, Map<Integer, Integer> map, ResultSet set)
            throws Exception {
        // get results
        ResultSetMetaData metaData = set.getMetaData();
        int column_num = metaData.getColumnCount();
        DefaultTableModel model = (DefaultTableModel) table.getModel();
        model.getDataVector().removeAllElements();
        model.setColumnCount(0);
        model.fireTableDataChanged();
        // set column names
        for (int i = start_idx; i <= column_num; i++) {
            model.addColumn(metaData.getColumnName(i));
        }
        // set table data
        int j = 0;
        if (map != null) {
            map.clear();
        }
        while (set.next()) {
            Object[] row = new Object[column_num];
            if (map != null) {
                map.put(j, set.getInt(1));
            }
            for (int i = start_idx; i <= column_num; i++) {
                row[i - start_idx] = set.getString(i);
            }
            model.addRow(row.clone());
            j++;
        }
    }
}