<html>
    <head>
        <title> z2-2.php </title>
    </head>
    
    <body>

    <?php

        $breakfast = "gamburger";
        $$breakfast = " and tea"; // <=> $gamburger = " and tea";

        print "$breakfast $gamburger<br>";  // будет выведено "gamburger and tea", потому что создалась переменная $gamburger со значением " and tea",
                                            // причем скобки "" обращаются к значению переменной, а не ее имени;
        
        print "$breakfast"; print "$$breakfast<br>";// будет выведено "gamburger$gamburger", потому что вместо $breakfast будет подставлено gamburger,
                                                    // дальнейшего получения значения не произойдет(потому что двойные кавычки);

        print $breakfast; print $$breakfast; // будет выведено "gamburger and tea", потому что вместо $$breakfast будет подставлено " and tea"
                                             // (во втором print нет кавычек)
    ?>
    
    </body>
</html>