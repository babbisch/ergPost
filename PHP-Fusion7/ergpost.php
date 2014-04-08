<?php
require_once "maincore.php";
require_once THEMES."templates/header.php";

error_reporting(E_ALL ^E_NOTICE);

echo '<div class="dh_ergpost">';
$dh_id = 'ergpost';
include(BASEDIR.'ergpost/ergpost.dh.inc');
echo '</div>';

error_reporting(E_ALL);

require_once THEMES."templates/footer.php";
?>
