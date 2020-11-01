<html>
    <head>
        <title> z4-3b.php </title>
    </head>
    
    <body>
        <?php
            print "Тестируемый: ". $_POST["user"] . "<br>";

            $right_answers_array=array("находится в городе", "Caнкт-Пeтepбypг", "Moсква",
                                    "Иepуcaлим", "Mилaн", "Пapиж", "Maдpид", 
                                    "Лондон", "Hью-Йopк", "Бepлин");
            $right_answers_array_num=array(6,9,4,1,3,2,5,8,7);
            $right_answers_num=0;
            $i=0;

            foreach ($_POST["answers"] as $value) {
                if($right_answers_array_num[$i] == array_search($value, $right_answers_array)) {
                    $right_answers_num++;
                }
                $i++;
            }

            switch($right_answers_num) {
                case 9: print "великолепно знаете географию"; break;
                case 8: print "отлично знаете географию"; break;
                case 7: print "очень хорошо знаете географию"; break;
                case 6: print "хорошо знаете географию"; break;
                case 5: print "удовлетворительно знаете географию"; break;
                case 4: print "терпимо знаете географию"; break;
                case 3: print "плохо знаете географию"; break;
                case 2: print "очень плохо знаете географию"; break;
                default: print "вообще не знаете географию";
            }
        ?>

    </body>
</html>
