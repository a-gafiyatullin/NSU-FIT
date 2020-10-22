<html>
    <head>
        <title> Задание 2 </title>
    </head>
    
    <body>

        <?php
            $color="blue";

            print "<table cellpadding=5 border=1>\n";
            for ($y=1;  $y <= 10;  $y++) {
                print "<tr>\n";
                for ($x=1;  $x <= 10;  $x++) {
                    print "\t<td>";

                    if($x == 1 && $y == 1) {
                        print"<font color=\"red\">";
                        print "+";
                        print"</font>";
                    } else if($x == 1 || $y == 1) {
                        print"<font color=$color>";
                        print ($x * $y);
                        print"</font>";
                    } else {
                        print ($x + $y);
                    }
                    print "</td>\n";
                }
                print "</tr>\n";
            }
            print "</table>";

        ?>
    </body>
</html>
