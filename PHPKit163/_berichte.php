<?php
include_once(dirname(__file__).'/php/dh.func.inc');

$liga = $_GET['liga'];
$st = $_GET['st'];
$id = $_GET['id'];

echo '<div class="dh_ergpost">';
echo '<h1>Berichte</h1>';
echo '<p align="left">';
$arr = @file(dirname(__file__).'/ergpost/spiele/'.$liga.'-'.$st.'-'.$id);
$str = @implode('',$arr);
$str = convert_text2htmlplus($str);
if (!$str){$str= 'Bericht ist nicht vorhanden';}
echo $str;
echo '</p>';
echo '</div>';
?>
