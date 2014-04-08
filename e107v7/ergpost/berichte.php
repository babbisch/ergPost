<?php
if(!defined("e107_INIT")) {
	require_once("../../class2.php");
}

include_once(e_PLUGIN.'/ergpostr/php/dh.func.inc');

require_once(HEADERF);

$liga = $_GET['liga'];
$st = $_GET['st'];
$id = $_GET['id'];

$output = '<div class="dh_ergpost">
<h1>Berichte</h1>
<p align="left">';
$arr = @file(e_BASE.'ergpost/spiele/'.$liga.'-'.$st.'-'.$id);
$str = @implode('',$arr);
$str = convert_text2htmlplus($str);
if (!$str){$str= 'Bericht ist nicht vorhanden';}
$output .= $str;
$output .= '</p>
</div>';

$ns->tablerender("Ergebnisbericht",$output);
require_once(FOOTERF);
?>
