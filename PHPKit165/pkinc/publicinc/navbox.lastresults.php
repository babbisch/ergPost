<?php
include(pkPUBLICERGPOST.'get_games.dh.inc');
return array(ergpost_last_results('http://'.$_SERVER['HTTP_HOST'].dirname($_SERVER['SCRIPT_NAME']).'/pkinc/public',5));
?>