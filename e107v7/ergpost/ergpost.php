<?php
if(!defined("e107_INIT")) {
	require_once("../../class2.php");
}

include_once(e_PLUGIN.'/ergpost/php/dh.func.inc');

require_once(HEADERF);

echo '<div class="dh_ergpost">';
$dh_id = 'ergpost';
include(e_PLUGIN.'ergpost/ergpost/ergpost.dh.inc');
echo '</div>';

require_once(FOOTERF);

?>

