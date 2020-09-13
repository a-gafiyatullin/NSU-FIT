<html>
    <head>
        <title> z2-3.php </title>
    </head>
    
    <body>

    <?php

    $breakfast = "gamburger";
    $breakfast2 = &$breakfast;

    print "До: $breakfast2<br>"; // gamburger
    $breakfast = "tea";
    print "После: $breakfast2<br>"; // tea

    // такое поведение, потому что breakfast2 является псевдонимом для ячейки памяти с именем breakfast.
    // Работает аналогично ссылкам C++ 

    ?>
    
    </body>
</html>