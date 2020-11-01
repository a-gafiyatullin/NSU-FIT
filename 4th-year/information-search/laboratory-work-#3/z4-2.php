<html>
    <head>
        <title> z4-2.php </title>
    </head>
    
    <body>

        <?php

            $align=$_GET["align"];
            $valign=$_GET["valign"];

            if (!isset ($align)) {print $align; $align="left";}
            if (!isset ($valign)) {$valign="top";}

            print "<body bgcolor=silver text=black link=white
                    alink=maroon>\n";

            print "<p><table border=1 width=100 height=100 align=center>\n";
            print "<tr><td align=\"$align\" valign=\"$valign\">Текст<br></td>
                    </tr> </table>\n";

        ?>

        <form action="<?php print $_SERVER['PHP_SELF']; ?>" method="GET">
            <p><b>Choose horizontal alignment:</b>
            <p><input type="radio" name="align" value="left">left
            <p><input type="radio" name="align" value="center">center
            <p><input type="radio" name="align" value="right">right
            <p><b>Choose vertical alignment:</b></p>
            <p><input type="checkbox" name="valign" value="top">top
            <p><input type="checkbox" name="valign" value="middle">middle
            <p><input type="checkbox" name="valign" value="bottom">bottom
            <p><input type="submit" value="Execute">
        </form>
    
    </body>
</html>

