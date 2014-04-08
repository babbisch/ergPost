<?php
defined ('main') or die ( 'no direct access' );

$title = $allgAr['title'].' :: Trainer';
$hmenu = 'Trainer';
$design = new design ( $title , $hmenu );
$design->header();

include_once(dirname(__file__).'/php/dh.func.inc');
include(dirname(__file__).'/php/dh_anker.inc');
echo '<div class="dh_ergpost">';
echo '<h1>Trainer: '.$dh_anker.'</h1>';

echo 'Das angeklickte Team wird von '.$dh_anker.' trainiert.';

echo '</div>';

$design->footer();
?>
