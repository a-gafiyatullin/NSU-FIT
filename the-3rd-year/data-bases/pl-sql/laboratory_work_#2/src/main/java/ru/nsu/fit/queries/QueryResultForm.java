package ru.nsu.fit.queries;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.*;
import java.util.Map;

public class QueryResultForm extends JFrame {
    private JPanel mainPanel;

    private JLabel queryInfo1;
    private JTextField queryText1;

    private JLabel queryInfo2;
    private JTextField queryText2;

    private JLabel queryInfo3;
    private JTextField queryText3;

    private JTable queryResults;
    private JButton queryProcess;
    private JLabel queryStatus;

    private boolean isReport;
    private PreparedStatement queryStatement;

    public QueryResultForm(Connection connection, Map<Integer, String> labelsText, String preparedStatement) throws SQLException {
        setTitle("Query");
        if(labelsText.containsKey(1)) {
            queryInfo1.setVisible(true);
            queryText1.setVisible(true);
            queryInfo1.setText(labelsText.get(1));
        } else {
            queryInfo1.setVisible(false);
            queryText1.setVisible(false);
        }
        if(labelsText.containsKey(2)) {
            queryInfo2.setVisible(true);
            queryText2.setVisible(true);
            queryInfo2.setText(labelsText.get(2));
        } else {
            queryInfo2.setVisible(false);
            queryText2.setVisible(false);
        }
        if(labelsText.containsKey(3)) {
            queryInfo3.setVisible(true);
            queryText3.setVisible(true);
            queryInfo3.setText(labelsText.get(3));
        } else {
            queryInfo3.setVisible(false);
            queryText3.setVisible(false);
        }
        if(labelsText.isEmpty()) {
            isReport = true;
            queryProcess.setVisible(false);
        }

        queryStatement = connection.prepareStatement(preparedStatement);

        queryProcess.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    execute();
                } catch (SQLException ex) {
                    queryStatus.setText("Error: " + ex.getMessage());
                }
            }
        });

        setMinimumSize(new Dimension(400, 500));
        setContentPane(mainPanel);
        setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        pack();

        if(isReport) {
            execute();
        }

        setLocationRelativeTo(null);
        setVisible(true);
    }

    public void execute() throws SQLException {
        queryStatus.setText("Status: OK");
        if (!isReport) {
            int index = 1;
            if (queryText1.isVisible()) {
                queryStatement.setString(index, queryText1.getText());
                index++;
            }
            if (queryText2.isVisible()) {
                queryStatement.setString(index, queryText2.getText());
                index++;
            }
            if (queryText3.isVisible()) {
                queryStatement.setString(index, queryText3.getText());
            }
        }

        ResultSet results = queryStatement.executeQuery();
        ResultSetMetaData metaData = results.getMetaData();
        int column_num = metaData.getColumnCount();
        DefaultTableModel model = (DefaultTableModel) queryResults.getModel();
        model.getDataVector().removeAllElements();
        model.setColumnCount(0);
        model.fireTableDataChanged();
        // set column names
        for (int i = 1; i <= column_num; i++) {
            model.addColumn(metaData.getColumnName(i));
        }
        // set table data
        while (results.next()) {
            Object[] row = new Object[column_num];
            for (int i = 1; i <= column_num; i++) {
                row[i - 1] = results.getString(i);
            }
            model.addRow(row.clone());
        }
    }
}
