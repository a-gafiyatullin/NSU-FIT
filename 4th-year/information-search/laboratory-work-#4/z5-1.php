<html>
    <head>
        <title> z5-1.php </title>
    </head>
    
    <body>
        <?php
            $mysql_user = "root";  
            $mysql_password = "linuxlinux";
            $conn = mysqli_connect("localhost", $mysql_user, $mysql_password);
            if (!$conn) {
                die("Нет соединения с MySQL!");
            }

            $database = "sample";
            mysqli_select_db($conn, $database) or die ("Нельзя открыть $database");

            $query = "DROP TABLE IF EXISTS notebook";
            $result = mysqli_query($conn, $query) or die("<p>Ошибка: ".mysqli_error($conn));

            $query = "create table notebook
                (   id INT NOT NULL AUTO_INCREMENT,
                        PRIMARY KEY(id),
                    name VARCHAR(50),
                    city VARCHAR(50),
                    address VARCHAR(50),
                    birthday DATE,
                    mail VARCHAR(20)
                )";
            $result = mysqli_query($conn, $query) or die("<p>Нельзя создать таблицу notebook: ".mysqli_error($conn));
        ?>
    </body>
</html> 
