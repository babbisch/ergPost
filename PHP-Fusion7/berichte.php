<?php
require_once "maincore.php";
require_once THEMES."templates/header.php";

error_reporting(E_ALL ^E_NOTICE);

include_once(BASEDIR.'php/dh.func.inc');

$liga = $_GET['liga'];
$st = $_GET['st'];
$id = $_GET['id'];

echo '<div class="dh_ergpost">';
echo '<h1>Berichte</h1>';
echo '<p align="left">';
$arr = @file(BASEDIR.'ergpost/spiele/'.$liga.'-'.$st.'-'.$id);
$str = @implode('',$arr);
$str = convert_text2htmlplus($str);
if (!$str){$str= 'Bericht ist nicht vorhanden';}
echo $str;
echo '</p>';
echo '</div>';

error_reporting(E_ALL);

require_once THEMES."templates/footer.php";
?>
