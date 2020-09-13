<html>
    <head>
        <title> z2-4.php </title>

    </head>
    
    <body> 

    <?php

    define ("NUM_E", 2.71828);

    print "Число e равно ".NUM_E."<br>";

    $num_e1 = NUM_E;
    print gettype($num_e1); print "<br>";   // double

    $var2 = (string) $num_e1;
    print gettype($var2);   // string
    print " - $var2<br> ";  // 2.71828

    $var2 = (integer) $num_e1;
    print gettype($var2);   // integer
    print " - $var2<br>";   // 2

    $var2 = (boolean) $num_e1;
    print gettype($var2);    // boolean
    print " - $var2<br>";    // 1
    
    ?>

    </body>
</html>