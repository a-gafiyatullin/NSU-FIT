<?php
    $user = "root";
    $pass = "linuxlinux";
    $db = "study";
        
    $conn = mysqli_connect("localhost", $user, $pass);
    if (!$conn) die("Нет соединения с MySQL");

    mysqli_select_db($conn, $db) or die ("Нельзя открыть $db");
?>
