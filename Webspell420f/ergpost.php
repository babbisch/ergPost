<?php
include("_mysql.php");
include("_settings.php");
include("_functions.php");
require_once("lmo/init.php");
echo '<div class="dh_ergpost">';
$dh_id = 'ergpost';
include(dirname(__file__).'/ergpost/ergpost.dh.inc');
echo '</div>';
?>
