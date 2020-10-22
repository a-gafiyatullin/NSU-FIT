<html>
    <head>
        <title> Задание 3 </title>
    </head>
    
    <body>

        <?php
            function Ru($color) { print "<p><font color=$color>Здравствуйте!</font>"; }
            function En($color) { print "<p><font color=$color>Hello!</font>"; }
            function Fr($color) { print "<p><font color=$color>Bonjour!</font>"; }
            function De($color) { print "<p><font color=$color>Guten Tag!</font>"; }
      
            $color=$_GET["color"];
            $lang= $_GET["lang"];
            $lang($color);
        ?>
    </body>
</html>
