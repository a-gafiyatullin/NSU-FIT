package ru.nsu;

import oracle.jdbc.OracleTypes;

import javax.swing.*;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.Date;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Map;

public class ActorsEditing extends Theater {
    private final Map<String, Integer> actors = new HashMap<>();
    private final CallableStatement getActors;
    private final Map<String, Integer> ranks = new HashMap<>();
    private final CallableStatement getRanks;
    private final Map<String, Integer> competitions = new HashMap<>();
    private final CallableStatement getCompetitions;
    private final Map<String, Integer> characteristics = new HashMap<>();
    private final CallableStatement getCharacteristics;
    private final Map<Integer, Integer> actorCharacteristics = new HashMap<>();
    private final CallableStatement getActorCharacteristics;
    private final Map<Integer, Integer> actorRank = new HashMap<>();
    private final CallableStatement getActorRank;
    private final CallableStatement insertActorCharacteristics;
    private final CallableStatement deleteActorRank;
    private final CallableStatement deleteActorCharacteristics;
    private final CallableStatement insertActorRank;
    private JPanel mainPanel;
    private JComboBox actorComboBox;
    private JButton editButton;
    private JComboBox rankComboBox;
    private JComboBox competitionComboBox;
    private JFormattedTextField dateTextField;
    private JButton rankAddButton;
    private JButton rankDeleteButton;
    private JTable rankTable;
    private JComboBox characteristicComboBox;
    private JFormattedTextField valueTextField;
    private JButton characteristicAddButton;
    private JButton characteristicDeleteButton;
    private JTable characteristicTable;
    private JLabel statusField;

    public ActorsEditing(final Connection connection) throws Exception {
        characteristicTable.getTableHeader().setReorderingAllowed(false);
        rankTable.getTableHeader().setReorderingAllowed(false);
        characteristicTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });
        rankTable.setModel(new DefaultTableModel() {

            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        });


        getActors = connection.prepareCall("{call get_actors_list(?)}");
        getActors.registerOutParameter("list", OracleTypes.CURSOR);

        getCompetitions = connection.prepareCall("{call get_competitions_list(?)}");
        getCompetitions.registerOutParameter("list", OracleTypes.CURSOR);

        getRanks = connection.prepareCall("{call get_rank_list(?)}");
        getRanks.registerOutParameter("list", OracleTypes.CURSOR);

        getCharacteristics = connection.prepareCall("{call get_characteristics_list(?)}");
        getCharacteristics.registerOutParameter("list", OracleTypes.CURSOR);

        getActorRank = connection.prepareCall("{call actor_rank_list(?, ?)}");
        getActorRank.registerOutParameter(2, OracleTypes.CURSOR);

        getActorCharacteristics = connection.prepareCall("{call actor_characteristic_list(?, ?)}");
        getActorCharacteristics.registerOutParameter(2, OracleTypes.CURSOR);

        insertActorRank = connection.prepareCall("{call actor_rank_insert(?, ?, ?, ?)}");
        deleteActorRank = connection.prepareCall("{call actor_rank_delete(?, ?)}");

        insertActorCharacteristics = connection.prepareCall("{call actor_characteristic_insert(?, ?, ?)}");

        deleteActorCharacteristics = connection.prepareCall("{call actor_characteristic_delete(?, ?)}");

        showComboBoxListFromSQL(actorComboBox, getActors, actors, "id_employee", "name");
        showComboBoxListFromSQL(competitionComboBox, getCompetitions, competitions, "id_competition",
                "name_competition");
        showComboBoxListFromSQL(characteristicComboBox, getCharacteristics, characteristics,
                "id_characteristic", "type_characteristic");
        showComboBoxListFromSQL(rankComboBox, getRanks, ranks, "id_rank", "name_rank");

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

        characteristicComboBox.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
                showComboBoxListFromSQL(characteristicComboBox, getCharacteristics, characteristics,
                        "id_characteristic", "type_characteristic");
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
                showComboBoxListFromSQL(rankComboBox, getRanks, ranks, "id_rank", "name_rank");
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });

        editButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_actor = 0;
                    if (!actorComboBox.getSelectedItem().equals("-")) {
                        id_actor = actors.get(actorComboBox.getSelectedItem());
                    }

                    if (id_actor == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера!",
                                "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                    } else {
                        getActorRank.setInt(1, id_actor);
                        getActorRank.execute();

                        ResultSet results = (ResultSet) getActorRank.getObject(2);
                        fillTableFromResultSet(rankTable, 2, actorRank, results);

                        getActorCharacteristics.setInt(1, id_actor);
                        getActorCharacteristics.execute();

                        results = (ResultSet) getActorCharacteristics.getObject(2);
                        fillTableFromResultSet(characteristicTable, 2, actorCharacteristics, results);
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка редактирования", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        rankAddButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_actor = 0;
                    if (!actorComboBox.getSelectedItem().equals("-")) {
                        id_actor = actors.get(actorComboBox.getSelectedItem());
                    }

                    if (id_actor == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера!",
                                "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                    } else {
                        int id_rank = 0;
                        if (!rankComboBox.getSelectedItem().equals("-")) {
                            id_rank = ranks.get(rankComboBox.getSelectedItem());
                        }
                        int id_competition = 0;
                        if (!competitionComboBox.getSelectedItem().equals("-")) {
                            id_competition = competitions.get(competitionComboBox.getSelectedItem());
                        }
                        Date date = null;
                        if (!dateTextField.getText().isEmpty())
                            date = new java.sql.Date(dateFormat.parse(dateTextField.getText()).getTime());
                        if (id_rank * id_competition == 0) {
                            JOptionPane.showMessageDialog(mainPanel, "Не все поля заполнены!",
                                    "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                        } else {
                            insertActorRank.setInt(1, id_actor);
                            insertActorRank.setInt(2, id_rank);
                            insertActorRank.setDate(3, date);
                            insertActorRank.setInt(4, id_competition);
                            insertActorRank.execute();

                            editButton.doClick();
                            statusField.setText("Статус: Успешно.");
                        }
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    statusField.setText("Статус: Ошибка!");
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        rankDeleteButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_actor = 0;
                    if (!actorComboBox.getSelectedItem().equals("-")) {
                        id_actor = actors.get(actorComboBox.getSelectedItem());
                    }

                    if (id_actor == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера!",
                                "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                    } else {
                        int id_rank = 0;
                        if (rankTable.getSelectedRows().length != 0) {
                            id_rank = actorRank.get(rankTable.getSelectedRows()[0]);
                        }
                        if (id_rank == 0) {
                            JOptionPane.showMessageDialog(mainPanel, "Выберете звание!",
                                    "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                        } else {
                            deleteActorRank.setInt(1, id_actor);
                            deleteActorRank.setInt(2, id_rank);
                            deleteActorRank.execute();

                            editButton.doClick();
                            statusField.setText("Статус: Успешно.");
                        }
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    statusField.setText("Статус: Ошибка!");
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        characteristicAddButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_actor = 0;
                    if (!actorComboBox.getSelectedItem().equals("-")) {
                        id_actor = actors.get(actorComboBox.getSelectedItem());
                    }

                    if (id_actor == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера!",
                                "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                    } else {
                        int id_characteristic = 0;
                        if (!characteristicComboBox.getSelectedItem().equals("-")) {
                            id_characteristic = characteristics.get(characteristicComboBox.getSelectedItem());
                        }
                        double value = 0;
                        if (!valueTextField.getText().isEmpty()) {
                            value = Double.parseDouble(valueTextField.getText());
                        }
                        if (id_characteristic == 0) {
                            JOptionPane.showMessageDialog(mainPanel, "Выберите характеристику!",
                                    "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                        } else {
                            insertActorCharacteristics.setInt(1, id_actor);
                            insertActorCharacteristics.setInt(2, id_characteristic);
                            insertActorCharacteristics.setDouble(3, value);
                            insertActorCharacteristics.execute();

                            editButton.doClick();
                            statusField.setText("Статус: Успешно.");
                        }
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    statusField.setText("Статус: Ошибка.");
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка добавления", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        characteristicDeleteButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    int id_actor = 0;
                    if (!actorComboBox.getSelectedItem().equals("-")) {
                        id_actor = actors.get(actorComboBox.getSelectedItem());
                    }

                    if (id_actor == 0) {
                        JOptionPane.showMessageDialog(mainPanel, "Выберите актера!",
                                "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                    } else {
                        int id_characteristic = 0;
                        if (characteristicTable.getSelectedRows().length != 0) {
                            id_characteristic = actorCharacteristics.get(characteristicTable.getSelectedRows()[0]);
                        }
                        if (id_characteristic == 0) {
                            JOptionPane.showMessageDialog(mainPanel, "Выберите характеристику!",
                                    "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                        } else {
                            deleteActorCharacteristics.setInt(1, id_actor);
                            deleteActorCharacteristics.setInt(2, id_characteristic);
                            deleteActorCharacteristics.execute();

                            editButton.doClick();
                            statusField.setText("Статус: Успешно.");
                        }
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                    statusField.setText("Статус: Ошибка.");
                    JOptionPane.showMessageDialog(mainPanel, exception.getMessage().split("\n", 2)[0],
                            "Ошибка удаления", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        setContentPane(mainPanel);
        pack();
        setVisible(true);
    }

    private void createUIComponents() {
        dateTextField = new JFormattedTextField(dateFormat);
        valueTextField = new JFormattedTextField(numberFormat);
    }
}