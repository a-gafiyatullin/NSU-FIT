package ru.nsu.fit;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.cli.*;
import ru.nsu.fit.queries.*;

public class MainForm extends JFrame {
    private JButton theDiskWithTheMostSongsButton;
    private JButton diskWithSongButton;
    private JButton composerSDisksButton;
    private JButton songsOnTheDiskButton;
    private JButton theBandWithTheMostMembersButton;
    private JButton bandMembersButton;
    private JButton authorSBandsButton;
    private JButton authorSSongsButton;
    private JPanel mainPanel;

    public MainForm(final Connection connection) {
        setTitle("Music Market Database");
        theDiskWithTheMostSongsButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT DISK_NAME, COUNT(*) AS SONG_AMOUNT " +
                                "FROM DISKS_SONGS " +
                                "INNER JOIN DISK D USING(DISK_ID) " +
                                "GROUP BY DISK_NAME " +
                                "HAVING COUNT(*) = (SELECT MAX(SONG_AMOUNT) " +
                                                   "FROM (SELECT COUNT(*) AS SONG_AMOUNT " +
                                                   "FROM DISKS_SONGS " +
                                                   "GROUP BY DISK_ID))";

                try {
                    new QueryResultForm(connection, new HashMap<Integer, String>(), query);
                } catch (SQLException ignored) {}
            }
        });

        diskWithSongButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT DISK_NAME " +
                               "FROM DISKS_SONGS " +
                               "INNER JOIN DISK USING(DISK_ID) " +
                               "INNER JOIN SONG USING (SONG_ID) " +
                               "WHERE SONG_NAME LIKE ?";

                try {
                    Map<Integer, String> queryArgs = new HashMap<>();
                    queryArgs.put(1, "Song name:");
                    new QueryResultForm(connection, queryArgs, query);
                } catch (SQLException ignored) { }
            }
        });

        composerSDisksButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT UNIQUE DISK_NAME " +
                               "FROM SONG " +
                                "INNER JOIN AUTHOR A on SONG.COMPOSER_ID = A.AUTHOR_ID " +
                                "INNER JOIN OCCUPATION USING(OCCUPATION_ID) " +
                                "RIGHT JOIN DISKS_SONGS USING(SONG_ID) " +
                                "INNER JOIN DISK USING(DISK_ID) " +
                                "WHERE OCCUPATION_NAME LIKE 'композитор' " +
                                "AND AUTHOR_NAME LIKE ? AND (AUTHOR_SURNAME LIKE ? OR AUTHOR_SURNAME IS NULL) " +
                                "AND (AUTHOR_MIDNAME LIKE ? OR AUTHOR_MIDNAME IS NULL)";
                try {
                    Map<Integer, String> queryArgs = new HashMap<>();
                    queryArgs.put(1, "Name:");
                    queryArgs.put(2, "Surname:");
                    queryArgs.put(3, "Midname:");
                    new QueryResultForm(connection, queryArgs, query);
                } catch (SQLException ignored) {}
            }
        });

        songsOnTheDiskButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT SONG_NAME " +
                               "FROM DISKS_SONGS " +
                               "INNER JOIN SONG USING(SONG_ID) " +
                               "INNER JOIN DISK USING(DISK_ID) " +
                               "WHERE DISK_NAME = ?";

                try {
                    Map<Integer, String> queryArgs = new HashMap<>();
                    queryArgs.put(1, "Disk:");
                    new QueryResultForm(connection, queryArgs, query);
                } catch (SQLException ignored) {}
            }
        });

        bandMembersButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT AUTHOR_SURNAME, AUTHOR_NAME, AUTHOR_MIDNAME " +
                               "FROM BANDS_AUTHORS " +
                               "INNER JOIN BAND USING(BAND_ID) " +
                               "INNER JOIN AUTHOR USING(AUTHOR_ID) " +
                               "WHERE BAND_NAME LIKE ?";

                try {
                    Map<Integer, String> queryArgs = new HashMap<>();
                    queryArgs.put(1, "Band name:");
                    new QueryResultForm(connection, queryArgs, query);
                } catch (SQLException ignored) {}
            }
        });

        theBandWithTheMostMembersButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT BAND_NAME, COUNT(*) AS MEMBERS_AMOUNT " +
                               "FROM BANDS_AUTHORS " +
                               "INNER JOIN BAND USING(BAND_ID) " +
                               "GROUP BY BAND_NAME " +
                               "HAVING COUNT(*) = (SELECT MAX(MEMBERS_AMOUNT) " +
                                                  "FROM (SELECT COUNT(*) AS MEMBERS_AMOUNT " +
                                                  "FROM BANDS_AUTHORS " +
                                                  "INNER JOIN BAND USING(BAND_ID) " +
                                                  "GROUP BY BAND_NAME))";

                try {
                    new QueryResultForm(connection, new HashMap<Integer, String>(), query);
                } catch (SQLException ignored) {}
            }
        });

        authorSSongsButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT SONG_NAME " +
                               "FROM AUTHORS_SONGS " +
                               "INNER JOIN SONG USING(SONG_ID) " +
                               "INNER JOIN AUTHOR USING(AUTHOR_ID) " +
                               "WHERE AUTHOR_NAME LIKE ? " +
                               "AND (AUTHOR_SURNAME LIKE ? OR AUTHOR_SURNAME IS NULL) " +
                               "AND (AUTHOR_MIDNAME LIKE ? OR AUTHOR_MIDNAME IS NULL)";
                try {
                    Map<Integer, String> queryArgs = new HashMap<>();
                    queryArgs.put(1, "Name:");
                    queryArgs.put(2, "Surname:");
                    queryArgs.put(3, "Midname:");
                    new QueryResultForm(connection, queryArgs, query);
                } catch (SQLException ignored) {}
            }
        });

        authorSBandsButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String query = "SELECT BAND_NAME " +
                               "FROM BANDS_AUTHORS " +
                               "INNER JOIN BAND USING(BAND_ID) " +
                               "INNER JOIN AUTHOR USING(AUTHOR_ID) " +
                               "WHERE AUTHOR_NAME LIKE ? " +
                               "AND(AUTHOR_SURNAME LIKE ? OR AUTHOR_SURNAME IS NULL) " +
                               "AND (AUTHOR_MIDNAME LIKE ? OR AUTHOR_MIDNAME IS NULL)";
                try {
                    Map<Integer, String> queryArgs = new HashMap<>();
                    queryArgs.put(1, "Name:");
                    queryArgs.put(2, "Surname:");
                    queryArgs.put(3, "Midname:");
                    new QueryResultForm(connection, queryArgs, query);
                } catch (SQLException ignored) {}
            }
        });
    }

    public static void main(String[] args) throws Exception {
        // parse command line args
        Options options = new Options();
        options.addOption("a", "address", true, "ip address of the database (ip:port)")
                .addOption("l", "login", true, "login for accessing to database")
                .addOption("p", "password", true, "password for login")
                .addOption("s", "sid", true, "Oracle database SID");

        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = parser.parse(options, args);

        HelpFormatter help = new HelpFormatter();
        String databaseAddress = cmd.getOptionValue("a");
        String databaseLogin = cmd.getOptionValue("l");
        String databasePassword = cmd.getOptionValue("p");
        String databaseSID = cmd.getOptionValue("s");
        if (databasePassword == null || databaseLogin == null || databaseAddress == null || databaseSID == null) {
            help.printHelp("music-market-db", options);
            return;
        }

        // load oracle jdbc and connect to database
        Class.forName("oracle.jdbc.driver.OracleDriver");
        Connection connection = DriverManager.getConnection("jdbc:oracle:thin:@" + databaseAddress + ":"
                + databaseSID, databaseLogin, databasePassword);

        // start application
        UIManager.setLookAndFeel("com.sun.java.swing.plaf.gtk.GTKLookAndFeel");
        MainForm mainForm = new MainForm(connection);
        mainForm.setContentPane(mainForm.mainPanel);
        mainForm.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainForm.pack();
        mainForm.setVisible(true);
    }
}