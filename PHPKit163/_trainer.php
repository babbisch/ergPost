<?php
include_once(dirname(__file__).'/php/dh.func.inc');
include(dirname(__file__).'/php/dh_anker.inc');
echo '<div class="dh_ergpost">';
echo '<h1>Trainer: '.$dh_anker.'</h1>';

echo 'Das angeklickte Team wird von '.$dh_anker.' trainiert.';

echo '<br><br>Informationen zu '.$dh_anker.' findest Du im '.dh_link('http://epl.ep.funpic.de/include.php?path=login/userinfo.php&name='.$dh_anker,'Benutzerprofil');
echo '</div>';

?>
