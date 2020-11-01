<?php
    if($_POST['site'] != "") {
        header("Location: http://" . $_POST['site']);
        exit;
    } else {
        $list_sites = array("www.yandex.ru", "www.rambler.ru", "www.google.com", "www.yahoo.com", "www.altavista.com");
        $array_size = count($list_sites);
        $i = 0;
?>

<html>
    <head>
        <title> z4-5.php </title>
    </head>
    
    <body>
        
        <form action = "<?php print $_SERVER['PHP_SELF'] ?>" method="post">
        <select name="site">
        <option value = "">Поисковые системы
    
    </body>
</html>
        
<?php
    while($i < $array_size) {
        print "<option value = \"$list_sites[$i]\">$list_sites[$i]";
        $i++;
    }
?>

<html>
    <body>        
        
        </select>
        <input type="submit" value="Перейти">
        </form>
    
    </body>
</html>
    
<?php
        }
?>
