<?php
require_once("maincore.php");
require_once("lmo/init.php");

require_once("subheader.php");
require_once("side_left.php");

echo '<div class="dh_ergpost">';
$dh_id = 'ergpost';
include(dirname(__file__).'/ergpost/ergpost.dh.inc');
echo '</div>';

require_once("side_right.php");
require_once("footer.php");
?>
