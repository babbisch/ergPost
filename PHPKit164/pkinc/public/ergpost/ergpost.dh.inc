<?php
include_once('config.dh.inc');
include_once(pkPUBLICPHP.'dh_ini.class.inc');
include_once(pkPUBLICPHP.'dh_func.init.inc');

echo '<h1>Eingabe der Ergebnisse</h1>'.chr(10);

//Einbinden der Sessionverwaltung erledigt phpkit
//Prüfen username
if ( _userid_ <= 0)
{
	echo ' <p><br><br>Sie müssen '.dh_link(_userlogin_,'eingeloggt','_self').' sein, um ERGPOST benutzen zu können!!</p>';
} else
{
	echo (strlen(_ergposthilfe_)>0)?dh_link(_ergposthilfe_,'Hilfe zur Ergebniseingabe','_blank'):'';
	echo ' | ';
	echo dh_link(_userlogout_,'Logout','_self').'<br>';

	//Formular zur eingabe
	echo '<form name="ergpost" method="post" action="'._ergpostscript_.'">';
	echo dh_input('dh_submit','dh_state','hidden');
	echo '<table border="0" cellpadding="3" cellspacing="1" width="100%" class="forumline">';
	$data = new DH_ERGPOST(_username_);
	echo $data->form_to_include_in_table();
	echo '</table>';
echo '</form>';
}
?>
