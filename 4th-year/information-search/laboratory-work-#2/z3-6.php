<html>
    <head>
        <title> Задание 6 </title>
    </head>
    
    <body>

        <?php
            // 1
            $cust["cnum"] = 2001;
            $cust["cname"] = "Hoffman";
            $cust["city"] = "London";
            $cust["snum"] = 1001;
            $cust["rating"] = 100;

            foreach ($cust as $key=>$val) { print "$key = $val<br>"; }
            print "<br>";
            //2
            asort($cust);
            foreach ($cust as $key=>$val) { print "$key = $val<br>"; }
            print "<br>";
            
            //3
            ksort($cust);
            foreach ($cust as $key=>$val) { print "$key = $val<br>"; }
            print "<br>";

            //4
            sort($cust);
            foreach ($cust as $key=>$val) { print "$key = $val<br>"; }
            print "<br>";
        ?>
    </body>
</html>
