<html>
    <head>
        <title> Задание 1 </title>
    </head>
    
    <body>

        <?php
            $color="silver";

            print "<table cellpadding=5 border=1>\n";
            for ($y=1;  $y <= 10;  $y++) {
                print "<tr>\n";
                for ($x=1;  $x <= 10;  $x++) {
                    if($x == $y) {
                        print "\t<td bgcolor=$color>";
                    } else {
                        print "\t<td>";
                    }
                    print ($x*$y);
                    print "</td>\n";
                }
                print "</tr>\n";
            }
            print "</table>";

        ?>
    </body>
</html>
