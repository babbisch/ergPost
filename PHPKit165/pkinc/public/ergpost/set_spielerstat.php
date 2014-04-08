<?php
include_once('set_spielerstat.dh.inc');

//Momentan gültige Spielerststatistikauswahl
	  $spielerstatopt = array(	'score'=>1,
	  							'cards'=>0,
								'shots'=>0);

     //Erstellen der Spielerstatistik
      write_spielerstats($_GET['l98'],$spielerstatopt);
?>