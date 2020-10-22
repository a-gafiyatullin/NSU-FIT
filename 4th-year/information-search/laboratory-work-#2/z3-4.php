<html>
    <head>
        <title> Задание 4 </title>
    </head>
    
    <body>

        <?php
            $size = 7;

            function WeekDay($day, $color) {
                global $size;
                print "<p><font color=$color size=$size>$day</font>";
            }

            $days = array("Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскрeсенье");
            $colors = array("black", "grey", "blue", "yellow", "red", "pink", "purple");

            for($i = 0; $i < 7; $i++) {
                WeekDay($days[$i], $colors[$i]);
                $size--;
            }
        ?>
    </body>
</html>
