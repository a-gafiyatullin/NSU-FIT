<html>
    <head>
        <title> z5-3.php </title>
    </head> 
    
    <body>
        <?php
            $user = "root";
            $pass = "linuxlinux";
            $db = "sample";
            $table = "notebook";
            
            $conn = mysqli_connect("localhost", $user, $pass);
            if (!$conn) die("Нет соединения с MySQL!");

            mysqli_select_db($conn, $db) or die ("Нельзя открыть $db: " . mysqli_error($conn));

            $result = mysqli_query($conn, "SELECT * FROM $table");
            $num_rows = mysqli_num_rows($result);

            print "<p>В таблице $table содержится $num_rows строк";

            $num_fields = mysqli_num_fields($result);

            print "<p><table border=1>\n";
            print "<tr>\n";

            for ($x=0; $x < $num_fields; $x++) {
                $name = mysqli_fetch_field_direct($result, $x);
                print "\t<th>$name->name</th>\n";
            }
            print "</tr>\n";

            while ($a_row = mysqli_fetch_row($result)) {
                print "<tr>\n";
                foreach ($a_row as $field)
                print "\t<td>$field</td>\n";
                print "</tr>\n";
            }

            print "</table>\n";
            mysqli_close($conn);
        ?>
    </body>
</html>
