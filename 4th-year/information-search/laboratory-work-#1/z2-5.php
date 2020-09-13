<html>
    <head>
        <title> z2-5.php </title>
    </head>
    
    <body> 

    <?php

        if ($_GET["lang"] == "ru") {print "Русский!";}
        elseif ($_GET["lang"] == "en") {print "Английский!";}
        elseif ($_GET["lang"] == "fr") {print "Французский!";}
        elseif ($_GET["lang"] == "de") {print "Немецкий!";}
        else {print "Язык неизвестен!";}

    ?>

    </body>
</html>