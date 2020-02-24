package org.nsu;

import java.sql.*;
import org.apache.commons.cli.*;
import org.jetbrains.annotations.NotNull;

public class University {

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
        if(databasePassword == null || databaseLogin == null || databaseAddress == null || databaseSID == null) {
            help.printHelp("university-db", options);
            return;
        }

        // load oracle jdbc and connect to database
        Class.forName("oracle.jdbc.driver.OracleDriver");
        try(Connection connection = DriverManager.getConnection("jdbc:oracle:thin:@" + databaseAddress + ":"
                + databaseSID, databaseLogin, databasePassword)) {

            createStudentAutoIncrement(connection);
            System.out.println("Auto increment for STUDENT table is created.");

            createStudentAddingCheck(connection);
            System.out.println("Trigger for STUDENT table is created.");

            createTeacherCascadeDeleting(connection);
            System.out.println("Trigger for TEACHER table is created.");

        } catch (SQLException e) {
            System.err.println(e.getMessage());
        }
    }

    static void createStudentAutoIncrement(@NotNull Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        ResultSet result = statement.executeQuery("SELECT MAX(ID) AS maxID FROM STUDENT");

        int maxID = 0;
        if(result.next()) {
            maxID = Integer.parseInt(result.getString("maxID"));
        }
        try {
            statement.executeUpdate("DROP SEQUENCE st_sequence");
        } catch (SQLException ignored) {}
        statement.executeUpdate("CREATE SEQUENCE st_sequence MINVALUE " + (maxID + 1));
        statement.executeUpdate("CREATE OR REPLACE TRIGGER st_autoinc " +
                                    "BEFORE INSERT ON STUDENT FOR EACH ROW " +
                                    "BEGIN " +
                                        ":NEW.id := st_sequence.NextVal; " +
                                    "END;");
    }

    static void createStudentAddingCheck(@NotNull Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        statement.executeUpdate("CREATE OR REPLACE TRIGGER add_student " +
                                    "BEFORE INSERT ON STUDENT FOR EACH ROW " +
                                    "DECLARE " +
                                        "CURSOR teachers " +
                                        "IS SELECT ID FROM TEACHER " +
                                        "WHERE NAME = :NEW.NAME AND LAST_NAME = :NEW.LAST_NAME; " +
                                        "teacher_id TEACHER.ID%TYPE; " +
                                    "BEGIN " +
                                        "OPEN teachers; " +
                                        "FETCH teachers INTO teacher_id; " +
                                        "IF(teachers%FOUND) THEN " +
                                            "raise_application_error(-20235, 'Student can not be a teacher!'); " +
                                        "END IF; " +
                                    "END;");
    }

    static void createTeacherCascadeDeleting(@NotNull Connection connection) throws SQLException {
        Statement statement = connection.createStatement();
        statement.executeUpdate("CREATE OR REPLACE TRIGGER delete_teacher " +
                                    "BEFORE DELETE ON TEACHER FOR EACH ROW " +
                                    "BEGIN " +
                                        "DELETE FROM ST_TEAC WHERE ID_TEAC = :OLD.ID;" +
                                    "END;");
    }
}