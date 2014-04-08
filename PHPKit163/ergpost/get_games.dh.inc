<?php

/*
0=>file
1=>spieltag
2=>spielid
3=>teamA
4=>teamB
5=>trainerA
6=>trainerB
7=>goalsA
8=>goalsB
9=>Liganame
*/

function ergpost_last_results($source,$count=0,$lmo='',$template='',$bericht='/berichte.php?'){
	$quelle = file($source.'/ergpost/allgames.txt');
	if ($lmo == ''){$lmo = $source.'/lmo/lmo.php?';}
	elseif (substr($lmo,0,7) != "http://"){
		$lmo = $source.$lmo;
	}
	if (strpos($lmo,'?')== FALSE) {
		$lmo .= '?';
	}
	
	if($template==''){
		$template = '<!--iconTeamA--> - <!--iconTeamB--> <a href="<!--Berichtlink-->" title="<!--Liganame-->-<!--Spieltag-->-<!--SpielID-->"><!--goalsA-->:<!--goalsB--></a><br>';
	}
	$erg = "";
	$i=0;
	while($el = array_pop($quelle) AND ($i<$count OR $count == 0)){
		$temp = explode(';',trim($el));
		//Filter folgt noch
		$i++;
		$replace_arr = array(   '<!--Datei-->' => $temp[0],
								'<!--Spieltag-->' => $temp[1],
								'<!--SpielID-->' => $temp[2],
								'<!--teamA-->' => htmlentities($temp[3]),
								'<!--teamB-->' => htmlentities($temp[4]),
								'<!--trainerA-->' => htmlentities($temp[5]),
								'<!--trainerB-->' => htmlentities($temp[6]),
								'<!--goalsA-->' => $temp[7],
								'<!--goalsB-->' => $temp[8],
								'<!--Liganame-->' => htmlentities($temp[9]),
								'<!--Berichtlink-->' => $source.$bericht.'&liga='.rawurlencode($temp[0]).'&st='.$temp[1].'&id='.$temp[2],
								'<!--Spieltaglink-->' => $lmo.'file='.$temp[0].'&action=results&st='.$temp[1],
								'<!--Tabelle nach Spieltag-->' =>  $lmo.'file='.$temp[0].'&action=table&st='.$temp[1],
								'<!--Tabelle-->' =>  $lmo.'file='.$temp[0].'&action=table',
								'<!--iconTeamA-->' => '<img src="'.$source.'/lmo/img/teams/small/'.rawurlencode($temp[3]).'.gif" title="'.htmlentities($temp[3]).' ('.htmlentities($temp[5]).')">',
								'<!--iconTeamB-->' => '<img src="'.$source.'/lmo/img/teams/small/'.rawurlencode($temp[4]).'.gif" title="'.htmlentities($temp[4]).' ('.htmlentities($temp[6]).')">'								
		);
		
		
		$erg .= str_replace(array_keys($replace_arr),array_values($replace_arr),$template);
	}
	return $erg;
}

//echo ergpost_last_results('http://epl.ep.funpic.de',5);


?>