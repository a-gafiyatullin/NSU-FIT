<html>
    <head>
        <title> Задание 5 </title>
    </head>
    
    <body>

        <?php
            // 1
            for($i = 1; $i <= 10; $i++) {
                $treug[$i - 1] = $i * ($i + 1) / 2;
            }
            for($i = 0; $i < 10; $i++) {
                print "$treug[$i]  ";
            }
            print "<br>";
            // 2
            for($i = 1; $i <= 10; $i++) {
                $kvd[$i - 1] = $i * $i;
            }
            for($i = 0; $i < 10; $i++) {
                print "$kvd[$i]  ";
            }
            print "<br>";
            // 3
            $rez = array_merge($treug, $kvd);
            for($i = 0; $i < 20; $i++) {
                print "$rez[$i]  ";
            }
            print "<br>";
            // 4
            sort($rez);
            for($i = 0; $i < 20; $i++) {
                print "$rez[$i]  ";
            }
            print "<br>";
            // 5
            array_shift($rez);
            for($i = 0; $i < 19; $i++) {
                print "$rez[$i]  ";
            }
            print "<br>";
            // 6
            $rez1 = array_unique($rez);
            for($i = 0; $i < 19; $i++) {
                print "$rez[$i]  ";
            }
            print "<br>";
        ?>
    </body>
</html>
