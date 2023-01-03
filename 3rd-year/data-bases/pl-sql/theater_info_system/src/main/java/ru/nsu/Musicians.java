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

public class Musicians extends DatabaseUtils {
    private final Map<String, Integer> musicians = new HashMap<>();
    private final CallableStatement getMusicians;
    private final Map<String, Integer> instruments = new HashMap<>();
    private final CallableStatement getInstruments;
    private final Map<String, Integer> genders = new HashMap<>();
    private final CallableStatement getGenders;
    private final Map<String, Integer> genres = new HashMap<>();
    private final CallableStatement getGenres;
    private final Map<String, Integer> ageCategories = new HashMap<>();
    private final CallableStatement getAgeCategories;
    private final Map<Integer, Integer> musiciansInfo = new HashMap<>();
    private final CallableStatement getMusiciansInfo;
    private final CallableStatement updateMusician;
    private final CallableStatement getShowInfo;

    private JPanel mainPanel;
    private JTable resultTable;
    private JComboBox musicianComboBox;
    private JComboBox genderComboBox;
    private JComboBox instrumentComboBox;
    private JFormattedTextField ageFromTextField;
    private JFormattedTextField ageToTextField;
    private JButton queryButton;
    private JButton saveButton;
    private JComboBox genreComboBox;
    private JComboBox ageComboBox;
    private JFormattedTextField periodFromTextField;
    private JFormattedTextField periodToTextField;
    private JButton queryButton2;
    private JLabel status;

    public Musicians(final Connection connection, String role) throws Exception {
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

        getMusicians = connection.prepareCall("{call get_musicians_list(?)}");
        getMusicians.registerOutParameter("list", OracleTypes.CURSOR);

        getInstruments = connection.prepareCall("{call get_musical_instruments_list(?)}");
        getInstruments.registerOutParameter("list", OracleTypes.CURSOR);

        getGenres = connection.prepareCall("{call get_genres_list(?)}");
        getGenres.registerOutParameter("list", OracleTypes.CURSOR);

        getAgeCategories = connection.prepareCall("{call get_age_categories_list(?)}");
        getAgeCategories.registerOutParameter("list", OracleTypes.CURSOR);

        getGenders = connection.prepareCall("{call get_genders_list(?)}");
        getGenders.registerOutParameter("list", OracleTypes.CURSOR);

        getMusiciansInfo = connection.prepareCall("{call musician_info(?, ?, ?, ?, ?, ?)}");
        getMusiciansInfo.registerOutParameter(6, OracleTypes.CURSOR);

        updateMusician = connection.prepareCall("{call musician_update(?, ?)}");

        getShowInfo = connection.prepareCall("{call musician_shows(?, ?, ?, ?, ?, ?)}");
        getShowInfo.registerOutParameter(6, OracleTypes.CURSOR);

        showComboBoxListFromSQL(musicianComboBox, getMusicians, musicians, "id_employee", "name");
        showComboBoxListFromSQL(genderComboBox, getGenders, genders, "id_gender", "name_gender");
        showComboBoxListFromSQL(instrumentComboBox, getInstruments, instruments, "id_instrument",
                "name_instrument");
        showComboBoxListFromSQL(genreComboBox, getGenres, genres, "id_genre", "name_genre");
        showComboBoxListFromSQL(ageComboBox, getAgeCategories, ageCategories, "id_age_category",
                "name_age_category");

        musicianComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(musicianComboBox, getMusicians, musicians, "id_employee", "name");
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

        instrumentComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(instrumentComboBox, getInstruments, instruments, "id_instrument",
                        "name_instrument");
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
                    if (Objects.equals(musicianComboBox.getSelectedItem(), "-")) {
                        getMusiciansInfo.setNull(1, OracleTypes.INTEGER);
                    } else {
                        getMusiciansInfo.setInt(1, musicians.get((musicianComboBox.getSelectedItem())));
                    }
                    if (Objects.equals(genderComboBox.getSelectedItem(), "-")) {
                        getMusiciansInfo.setNull(2, OracleTypes.INTEGER);
                    } else {
                        getMusiciansInfo.setInt(2, genders.get(genderComboBox.getSelectedItem()));
                    }
                    if (ageFromTextField.getText().isEmpty()) {
                        getMusiciansInfo.setNull(3, OracleTypes.INTEGER);
                    } else {
                        getMusiciansInfo.setInt(3, Integer.parseInt(ageFromTextField.getText()));
                    }
                    if (ageToTextField.getText().isEmpty()) {
                        getMusiciansInfo.setNull(4, OracleTypes.INTEGER);
                    } else {
                        getMusiciansInfo.setInt(4, Integer.parseInt(ageToTextField.getText()));
                    }
                    if (Objects.equals(instrumentComboBox.getSelectedItem(), "-")) {
                        getMusiciansInfo.setNull(5, OracleTypes.INTEGER);
                    } else {
                        getMusiciansInfo.setInt(5, instruments.get(instrumentComboBox.getSelectedItem()));
                    }
                    getMusiciansInfo.execute();

                    // get results
                    ResultSet results = (ResultSet) getMusiciansInfo.getObject(6);
                    fillTableFromResultSet(resultTable, 2, musiciansInfo, results);
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

                    instrumentComboBox.setSelectedItem(model.getValueAt(selectedRow, 10));

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
                    int instrument = 0;
                    if (!Objects.equals(instrumentComboBox.getSelectedItem(), "-")) {
                        instrument = instruments.get(instrumentComboBox.getSelectedItem());
                    }
                    if (instrument == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                "Ошибка редактирования!", JOptionPane.ERROR_MESSAGE);
                    } else {
                        updateMusician.setInt(1, musiciansInfo.get(selectedRow));
                        updateMusician.setInt(2, instrument);
                        updateMusician.execute();

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
                    JOptionPane.showMessageDialog(mainPanel, "Выберите музыканта!",
                            "Ошибка запроса", JOptionPane.ERROR_MESSAGE);
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
                    getShowInfo.setInt(5, musiciansInfo.get(selectedRow));
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
        queryButton.doClick();
    }
}
