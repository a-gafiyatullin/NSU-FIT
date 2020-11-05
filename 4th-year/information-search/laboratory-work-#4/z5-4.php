<html>
    <head>
        <title> z5-4.php </title>
    </head>
    
    <body>
        <?php
            $user = "root";
            $pass = "linuxlinux";
            $db = "sample";
            $table = "notebook";
        
            $conn = mysqli_connect("localhost", $user, $pass);
            if (!$conn) die("Нет соединения с MySQL");

            mysqli_select_db($conn, $db) or die ("Нельзя открыть $db");

            if ($_POST['id'] != "" && $_POST['field_name'] != "") {
                $value = ($_POST['field_value'] == "" ? NULL : $_POST['field_value']);
                $query = "UPDATE $table SET " . $_POST['field_name'] . " = '$value' WHERE id = " . $_POST['id'];
                if (!mysqli_query($conn, $query)) {
                    print mysqli_error($conn);
                }

                print "<p align=center><a href='z5-3.php'>Результаты обновлены</a>";
                exit;
            }

            if ($_POST['id'] != "") {
                $result = mysqli_query($conn, "SELECT * FROM $table WHERE id = " . $_POST['id']);
                $a_row = mysqli_fetch_row($result);

                ?>
                <form action = "<?php print $_SERVER['PHP_SELF']; ?>" method="post">
                    <select name="field_name">";
                        <option value = "name"><?php print $a_row[1]; ?>
                        <option value = "city"><?php print $a_row[2]; ?>
                        <option value = "address"><?php print $a_row[3]; ?>
                        <option value = "birthday"><?php print $a_row[4]; ?>
                        <option value = "mail"><?php print $a_row[5]; ?>
                    </select>

                    <input type=hidden name=id value = <?php print $_POST['id']; ?>> 
                    <input type="text" name="field_value">

                    <p><input type="submit" value="Заменить">
                </form>
                
                <?php
                exit;
            }
        ?>

        <?php
            $result = mysqli_query($conn, "SELECT * FROM $table");

            $num_fields = mysqli_num_fields($result);

            print "<p><table border=1>\n";
            print "<tr>\n";

            for ($x=0; $x < $num_fields; $x++) {
                $name = mysqli_fetch_field_direct($result, $x);
                print "\t<th>$name->name</th>\n";
            }
            print "\t<th>Исправить</th>\n";
            print "</tr>\n";
        ?>

        <p><form action="<?php print $_SERVER['PHP_SELF'] ?>" method='POST'>
            <?php
                while($a_row = mysqli_fetch_row($result)) {
                    print "<tr>\n";
                    foreach ($a_row as $field)
                        print "\t<td>$field</td>\n";
                    print "\t<td><input type=\"radio\" name=\"id\" value=" . $a_row[0] . "></td>\n";
                    print "</tr>\n";
                }
                print "</table>\n";
            ?>
            <p><input type="submit" value="Выбрать">
        </form>
    </body>
</form>
