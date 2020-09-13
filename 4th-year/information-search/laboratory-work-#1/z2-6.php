<html>
    <head>
        <title> z2-6.php </title>
    </head>
    
    <body> 

    <?php

        switch($_GET["lang"]) {
            case "ru": print "Русский!"; break;
            case "en": print "Английский!"; break;
            case "fr": print "Французский!"; break;
            case "de": print "Немецкий!"; break;
            default: print "Язык неизвестен!";
        }

    ?>

    </body>
</html>