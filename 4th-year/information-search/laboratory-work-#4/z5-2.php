<html>
    <head>
        <title> z5-2.php </title>
    </head>
    
    <body>
        <?php
            function Add_to_database($name, $city, $address, $birthday, $email, &$dberror) {
                $user = "root";
                $pass = "linuxlinux";
                $db = "sample";
                $table = "notebook";

                $conn = mysqli_connect("localhost", $user, $pass);
                if (!$conn ) {
                    $dberror = "Нет соединения с MySQL сервером";
                    return false;
                }

                if (!mysqli_select_db($conn, $db)) {
                    $dberror = mysqli_error($conn);
                    return false;
                }

                $birthday = ($birthday == "" ? NULL : $birthday);
                $query = "INSERT INTO $table (name, city, address, birthday, mail) VALUES('$name', '$city', '$address'," . 
                    ($birthday == "" ? "NULL" : "'$birthday'") . ", '$email')";

                if (!mysqli_query($conn, $query)) {
                    $dberror = mysqli_error($conn);
                    return false;
                }

                return true;
            }
            
            function Write_form() {
                print "<form action=" . $_SERVER['PHP_SELF'] . " method='POST'>\n";
                
                    print "<p>Введите фамилию и имя[*]: ";
                    print "<input type='text' name='name'>";

                    print "<p>Введите город: ";
                    print "<input type='text' name='city'>";

                    print "<p>Введите адрес: ";
                    print "<input type='text' name='address'>";

                    print "<p>Введите дату рождения в форме ГГГГ-ММ-ДД: ";
                    print "<input type='text' name='birthday'>";

                    print "<p>Введите email[*]: ";
                    print "<input type='text' name='mail'>";

                    print "<p><input type='submit' value='Записать! '>\n
                </form>\n";

                    print "Поля, помеченные [*] обязательны для заполнения!<br>";
            }

            if ($_POST['name'] != "" && $_POST['mail'] != "") {
                $dberror = "";
                $ret = Add_to_database($_POST['name'], $_POST['city'], $_POST['address'], $_POST['birthday'], $_POST['mail'], $dberror);

                if (!$ret) {
                    print "Ошибка: $dberror<br>";
                }
                else {
                    print "Спасибо";
                }
            }
            else {
                Write_form();
            }
        ?>
    </body>
</html>
