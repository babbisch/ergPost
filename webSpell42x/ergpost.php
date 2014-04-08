<?php
include_once("_mysql.php");
include_once("_settings.php");
include_once("_functions.php");

require_once("lmo/init.php");

eval ("\$ergpost = \"".gettemplate("ergpost")."\";");
echo $ergpost;

$dh_id = 'ergpost';
include(dirname(__file__).'/ergpost/ergpost.dh.inc');

?>
