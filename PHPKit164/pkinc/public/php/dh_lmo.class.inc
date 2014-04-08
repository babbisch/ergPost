<?php

include_once("dh_ini.class.inc");

class DH_LMO extends DH_INI
{
  var $games = array();
  var $game = array();

   function DH_LMO($datei){
     $this->DH_INI($datei);
   }
   
   function set_games($spieltag,$not){
     $this->games=$this->get_games($pieltag,$not);
   }
   
   function set_game($gameid){
     $this->game = $this->select_game($gameid);

     return !empty($this->game);
   }
   
   function game_data_as_string($game=''){
     if(!is_array($game)){$game = $this->game;}
     $erg .= '<b>Spieltag'.$game['spieltag'].'<br>';
     $erg .= $game['teamA'].'</b> ['.$game['trainerA'].'] - ['.$game['trainerB'].'] <b>'.$game['teamB'].'</b><br>';

     $erg .= ($game['gespielt'])? 'gespielt: '.$game['date'].' '.$game['time'].'<br>Ergebnis: '.$game['GA'].':'.$game['GB'].'<br>':'';
     return $erg;
   }

   function select_game($gameid,$games=""){
     if (!is_array($games)){$games = $this->games;}
     if (isset($games[$gameid])){return $games[$gameid];} else {return FALSE;}
   }

   function get_teams_by_not($not){
     $temp = $this->get_data_original(array(1=>'Team'),'NOT',$not);
     $erg = array();
     if (!empty($temp)){
     foreach ($temp as $el) {
       $erg[] = 1*substr($el['section'],4);
     }
     }
     return $erg;
   }

   //gibt int aus
   function get_gameid_in_st_by_team($teamid,$spieltag){
    $temp = $this->get_data_original("Round$spieltag",array(1=>array('TA','TB')),$teamid,FALSE);
    return 1*substr($temp[0]['item'],2);
   }
   
   //
   function get_teamname_by_id($teamid){
     return $this->get_val_o('Teams',$teamid);
   }
   
   function get_games($spieltag,$not){
     $erg = array();
     foreach ($this->get_teams_by_not($not) as $teamid){
       $gameid = $this->get_gameid_in_st_by_team($teamid,$spieltag);
       $TA = $this->get_val_o("Round$spieltag","TA$gameid");  //Team A
       $TB = $this->get_val_o("Round$spieltag","TB$gameid");  //Team B
       $GA = $this->get_val_o("Round$spieltag","GA$gameid");  //Goals A
       $GB = $this->get_val_o("Round$spieltag","GB$gameid");  //Goals B
       $NT = $this->get_val_o("Round$spieltag","NT$gameid");  //Notiz
       $BE = $this->get_val_o("Round$spieltag","BE$gameid");  //Berichtslink
       $TI = $this->get_val_o("Round$spieltag","TI$gameid");
       $SP = $this->get_val_o("Round$spieltag","SP$gameid");
       $AT = $this->get_val_o("Round$spieltag","AT$gameid");  //Anstosszeit in Timestamp
       $teamA = $this->get_teamname_by_id($TA);
       $teamB = $this->get_teamname_by_id($TB);
       $trainerA = $this->get_val_o("Team$TA",'NOT');
       $trainerB = $this->get_val_o("Team$TB",'NOT');
       $gespielt = $this->gespielt($GA,$GB);
       if (!$AT) {$AT = time();}
       $AT = (int) $AT; //to int for php5
       $date = date('Y-m-d',$AT);
       $year = date('Y',$AT);
       $month = date('n',$AT);
       $day = date('j',$AT);
       $time = date('H:i',$AT);
       $hour = date('G',$AT);
       $minute = 1*date('i',$AT);
       $add = array(
            'spieltag'=>$spieltag,
            'gameid'=> $gameid,
            'teamid'=> $teamid,
            'not' => $not,
            'TA'=> $TA,
            'TB'=> $TB,
            'GA'=> $GA,
            'GB'=> $GB,
            'NT'=> $NT,
            'BE'=> $BE,
            'TI'=> $TI,
            'AT'=> $AT,
            'teamA'=> $teamA,
            'teamB'=> $teamB,
            'trainerA'=>$trainerA,
            'trainerB'=>$trainerB,
            'gespielt'=> $gespielt,
            'date'=> $date,
            'time'=> $time,
            'hour'=> $hour,
            'minute'=> $minute,
            'year' => $year,
            'month' => $month,
            'day' => $day
        );
        $erg[$gameid]=$add;
     }
     return $erg;
   }

   //Prüft anhand der Tore ob das Spiel bereits gespielt wurde
   function gespielt($ga,$gb){
     if ($ga == -1 AND $gb == -1){
       return FALSE;
     } else {
       return TRUE;
     }
   }

  
  
  

}
?>
