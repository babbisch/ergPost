<?php

include_once('php/dh.func.inc');

eval ("\$berichte = \"".gettemplate("berichte")."\";");
echo $berichte;

$liga = $_GET['liga'];
$st = $_GET['st'];
$id = $_GET['id'];

$arr = file(dirname(__file__).'/ergpost/spiele/'.$liga.'-'.$st.'-'.$id);
$str = implode('',$arr);
$str = convert_text2htmlplus($str);
if (!$str){$str= 'Bericht ist nicht vorhanden';}
echo $str;

?>
