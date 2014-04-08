<?php
include_once(pkPUBLICPHP.'dh.func.inc');
include(pkPUBLICPHP.'dh_anker.inc');
echo '<div class="dh_ergpost">';
echo '<h1>Trainer: '.$dh_anker.'</h1>';

echo 'Das angeklickte Team wird von '.$dh_anker.' trainiert.';

echo '<br><br>Informationen zu '.$dh_anker.' findest Du im '.dh_link('http://lmo.pes-freaks.de/include.php?path=userinfo&id='.$dh_anker,'Benutzerprofil');
echo '</div>';

?>
