package org.nsu;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import java.sql.*;
import static org.junit.Assert.*;

public class UniversityTest {
    // test database credentials
    public static String databaseAddress = "192.168.56.2:1521";
    public static String databaseSID = "orcl";
    public static String databaseLogin = "c##gafiyatullin";
    public static String databasePassword = "accesssosat";
    public static Connection connection;

    @BeforeClass
    public static void connectAndModifyDatabase() throws Exception {
        // load oracle jdbc and connect to database
        Class.forName("oracle.jdbc.driver.OracleDriver");
        connection = DriverManager.getConnection("jdbc:oracle:thin:@" + databaseAddress + ":"
                + databaseSID, databaseLogin, databasePassword);

        // delete triggers
        try {
            Statement statement = connection.createStatement();
            statement.executeUpdate("DROP TRIGGER st_autoinc");
            statement.executeUpdate("DROP TRIGGER add_student");
            statement.executeUpdate("DROP TRIGGER delete_teacher");
        } catch (SQLException ignore) {}

        University.main(new String[]{"-a", databaseAddress, "-s", databaseSID, "-l", databaseLogin, "-p", databasePassword});
    }

    @AfterClass
    public static void closeConnection() throws SQLException {
        connection.close();
    }

    @Test
    public void studentAutoIncrementTest() throws SQLException{
        Statement statement = connection.createStatement();

        ResultSet result = statement.executeQuery("SELECT MAX(ID) AS maxID FROM STUDENT");
        int maxID_before = 0;
        if(result.next()) {
            maxID_before = Integer.parseInt(result.getString("maxID"));
        }

        statement.executeUpdate("INSERT INTO STUDENT VALUES(1, 'Ivanov', 'Ivan')");

        result = statement.executeQuery("SELECT MAX(ID) AS maxID FROM STUDENT");
        int maxID_after = 0;
        if(result.next()) {
            maxID_after = Integer.parseInt(result.getString("maxID"));
        }

        assertTrue(maxID_before == maxID_after || maxID_before + 1 == maxID_after);

        statement.executeUpdate("DELETE FROM STUDENT WHERE ID = " + maxID_after);
    }

    @Test
    public void studentInsertionTest() throws SQLException {
        Statement statement = connection.createStatement();

        try {
            statement.executeUpdate("INSERT INTO STUDENT VALUES(6, 'Zheglov', 'Gleb')");
        } catch (SQLException e) {
            return;
        }

        fail();
    }

    @Test
    public void teacherDeletingTest() throws SQLException {
        Statement statement = connection.createStatement();

        statement.executeUpdate("DELETE FROM TEACHER WHERE ID = 2");
        ResultSet result_after = statement.executeQuery("SELECT * FROM ST_TEAC");

        while (result_after.next()) {
            assertNotEquals("2", result_after.getString("ID_TEAC"));
        }
    }
}