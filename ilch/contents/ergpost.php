<?php
defined ('main') or die ( 'no direct access' );

$title = $allgAr['title'].' :: ergPost';
$hmenu = 'ergPost';
$design = new design ( $title , $hmenu );
$design->header();

echo '<div class="dh_ergpost">';
$dh_id = 'ergpost';
include('ergpost/ergpost.dh.inc');
echo '</div>';

$design->footer();
?>
