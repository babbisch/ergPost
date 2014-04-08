<?php
include_once(dirname(__file__).'/config.dh.inc');
include_once(dirname(__file__).'/../php/dh_ini.class.inc');
include_once(dirname(__file__).'/../php/dh_func.init.inc');

$output = '<h1>Eingabe der Ergebnisse</h1>'.chr(10);

//Einbinden der Sessionverwaltung erledigt phpkit
//Prüfen username
if ( _userid_ <= 0)
{
	$output .= ' <p><br><br>Sie müssen '.dh_link(_userlogin_,'eingeloggt','_self').' sein, um ERGPOST benutzen zu können!!</p>';
} else
{
	$output .= (strlen(_ergposthilfe_)>0)?dh_link(_ergposthilfe_,'Hilfe zur Ergebniseingabe','_blank'):'';
	$output .=  ' | ';
	$output .=  dh_link(_userlogout_,'Logout','_self').'<br>';

	//Formular zur eingabe
	$output .=  '<form name="ergpost" method="post" action="'._ergpostscript_.'">';
	$output .=  dh_input('dh_submit','dh_state','hidden');
	$output .=  '<table border="0" cellpadding="3" cellspacing="1" width="100%" class="forumline">';
	$data = new DH_ERGPOST(_username_);
	$output .=  $data->form_to_include_in_table();
	$output .=  '</table>';
$output .=  '</form>';
}
$ns->tablerender("Ergebniseingabe",$output);
?>
