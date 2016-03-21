<?php
$dir = dirname(__file__);
require_once(dirname(__file__).'/../maincore.php');
include_once(dirname(__file__).'/dh_lmo.class.inc');
include_once(dirname(__file__).'/../ergpost/set_spielerstat.dh.inc');
include_once(dirname(__file__).'/../ergpost/config.dh.inc');
define('_ligen_',dirname(__file__)."/../ergpost/ligen.ini");
define('_spielerdir_',dirname(__file__)."/../ergpost/spieler/");
define('_lmoligendir_',dirname(__file__)."/../lmo/ligen/");
define('_lmoligentempdir_',dirname(__file__)."/../ergpost/ligen/");
define('_berichtedir_',dirname(__file__)."/../ergpost/spiele/");
define('_teamstatsdir_',dirname(__file__)."/../ergpost/vereine/");
define('_formname_',"ergpost");
define('_copysavedir_',dirname(__file__)."/../ergpost/copysave/");
define('_allgamestxt_',dirname(__file__)."/../ergpost/allgames.txt");

class DH_ERGPOST extends DH_LMO
{
//Bei INIT
  var $games = array(); //Spieldaten aus LMO-Datei
  var $ligenini = array(); //aus ini.datei
  var $l98 = FALSE; //Name der Datei ohne pfad: pfad = _lmoligendir_
  var $liganame = FALSE;//zb Serie A
  var $spieltag = FALSE;
  var $username = FALSE;
  var $get_games_username = FALSE;
  var $sta = FALSE;
  var $ste = FALSE;
  var $stufen = array();
  var $ko_system = FALSE;
  var $ligamods = array();
  var $topic_id = FALSE;
  var $dh_submit = FALSE;//Zeigt an welche Schaltflaeche zuletzt gedrueckt wurde
  var $dh_submit_post = FALSE; //Gibt den Standardwert fuer hidden : dh_submit an
  var $fehler = array();//Nimmt die Fehleranzeigen bei der Pruefung auf, und gibt sie bei der Ausgabe aus
  var $form_depth = FALSE;//Gibt an wieviel "Felder"(Kopf,Spieldaten,Tore,..,Torschuesse) des Formulars angezeigt werden
  var $st_arr = array(); //Spieltags array
  var $ligen_arr = array(); //Array fuer Ligaauswahl
//Nach Spielauswahl (1)
  var $gameid = FALSE;
  var $not = FALSE;
  var $TA = FALSE;
  var $TB = FALSE;
  var $GA = FALSE;
  var $GB = FALSE;
  var $NT = FALSE;
  var $BE = FALSE;
  var $TI = FALSE;
  var $AT = FALSE;
  var $teamA = FALSE;
  var $teamB = FALSE;
  var $trainerA = FALSE;
  var $trainerB = FALSE;
  var $gespielt = FALSE;
  var $date = FALSE;
  var $time = FALSE;
  var $hour = FALSE;
  var $minute = FALSE;
  var $year = FALSE;
  var $month = FALSE;
  var $day = FALSE;
//Spielerdaten
  var $all_spieler = array();
  var $spieler = array();
  var $spieler_a = array();
//Nach Datum Tor und Teamstat (2)
  var $goalsA = FALSE;
  var $goalsB = FALSE;
  var $y = FALSE;
  var $m = FALSE;
  var $d = FALSE;
  var $h = FALSE;
  var $mi = FALSE;
  var $sdate = FALSE;
  //var $torenvA = 0;//Nach Verlaengerung
  var $torschusseA = FALSE;
  var $torschusseB = FALSE;
  var $auftorA = FALSE;
  var $auftorB = FALSE;
  var $penaltyA = FALSE;
  var $penaltyB = FALSE;
  var $freekicksA = FALSE;
  var $freekicksB = FALSE;
  var $cornersA = FALSE;
  var $cornersB = FALSE;
  var $foulsA = FALSE;
  var $foulsB = FALSE;
  var $offsideA = FALSE;
  var $offsideB = FALSE;
  var $yellowA = FALSE;
  var $yellowB = FALSE;
  var $yelredA = FALSE;
  var $yelredB = FALSE;
  var $redA = FALSE;
  var $redB = FALSE;
  var $posessionA = FALSE;
  var $posessionB = FALSE;
//Torschuetzen Vorlagengeber und Karten
  var $goals_count = 0;
  var $goals_arr = array(); //jeder Wert hat keys -> homeaway,min,schuetze,verein,vorlage,typ(='',EM,FS,ET)=>stand
  var $cards = array();
//Torsch&uuml;sse
  var $shots = array();

  var $torsch_h = 0;//gibt an wieviele Torschussspieler fuer Heimteam angezeigt werden
  var $torsch_a = 0;//gibt an wieviele Torschussspieler fuer Auswaertsteam angezeigt werden
  var $bigtemp = array();

  var $formular = '';

//##################################
  function __constructor($username){
    //Auswahl Liga in ergpostdh.inc
    //Auswahl Spieltag ergpost.dh.inc
    //$this->topic_id=$topic_id;
    $this->exists = TRUE;
    $this->username=_username_;
    if(!$this->username){$this->exists = FALSE;}
    //$dat = $this->get_liga();//$datei und $spieltag aus ligen.dat holen
    //if (!$dat){$this->exists = FALSE;return false;};
    $this->post_auswerten_dh_submit();
    $temp = $this->set_all_ligen();
    if (empty($this->ligenini)){$this->exists = FALSE;return false;}
    $this->get_liga();
    $this->admin_edit();
     //-->$games holen mit datei spieltag und UN
    $this->change_shooter_count(); //Passt die Anzahl der Anzeige der Torschussspieler an
    $this->DH_LMO(_lmoligendir_.$this->l98);
    $this->games = $this->get_games($this->spieltag,$this->get_games_username);
    $this->get_from_game();//Holt Spieldaten aus LMO-Datei und bereitet sie ggf fuers Formular vor
    $this->proof_vars();
    if (empty($this->current)){return false;}
    $this->code_bericht();
    //if (empty($this->datei) OR empty($this->games)){$this->exists = FALSE;}//falls datei oder games nicht existieren
  }

//#############################################################################
  //Holt LigaDaten fuer Topic: Ligadatei,-name und Spieltag
  function get_all_ligen(){
    $this->ligenini = $this->ReadIniCompleteInArr(_ligen_);
    //print_array($this->ligenini);
  }
//#############################################################################
  function set_all_ligen(){
    $this->get_all_ligen();
    $ids = array_keys($this->ligenini);
    foreach ($ids as $el){
      $this->ligen_arr[]=array('val'=>$el,'text'=>$this->get_val($this->ligenini,$el,'name'));
    }
    //print_array($this->ligen_arr);
    return $this->ligen_arr;
  }
//#############################################################################
  function set_spieltage(){
    $i = $this->sta;
    while ($i <= $this->ste){
      $arr[]=array('val'=>$i,'text'=>'Spieltag '.$i);
      $i++;
    }
    $this->st_arr = $arr;
    return $arr;
  }
//#############################################################################
  function get_liga(){
    $this->l98 = $this->get_val($this->ligenini,$this->ligaid,'datei');
    $this->liganame = $this->get_val($this->ligenini,$this->ligaid,'name');
    $this->sta = $this->get_val($this->ligenini,$this->ligaid,'sta');
    $this->ste = $this->get_val($this->ligenini,$this->ligaid,'ste');
    $this->stufen['result'] = $this->get_val($this->ligenini,$this->ligaid,'result');
    $this->stufen['teamstat'] = $this->get_val($this->ligenini,$this->ligaid,'teamstat');
    $this->stufen['scorer'] = $this->get_val($this->ligenini,$this->ligaid,'scorer');
    $this->stufen['cards'] = $this->get_val($this->ligenini,$this->ligaid,'cards');
    $this->stufen['shots'] = $this->get_val($this->ligenini,$this->ligaid,'shots');
    $this->stufen['bericht'] = $this->get_val($this->ligenini,$this->ligaid,'bericht');
    $this->stufen['moreinfo'] = $this->get_val($this->ligenini,$this->ligaid,'moreinfo');
    if ($this->stufen['cards']!=FALSE){$this->stufen['teamstat']= TRUE;}
	  if ($this->stufen['shots']!=FALSE){$this->stufen['teamstat']= TRUE;}
    $this->ko_system = $this->get_val($this->ligenini,$this->ligaid,'ko');
    $this->ligamods = explode(',',$this->get_val($this->ligenini,$this->ligaid,'mods'));

    //$this->spieltag = $this->get_val($this->topic2ligen,'TOPIC'.$this->topic_id,'ST');
    //$this->ko_system = $this->get_val($this->topic2ligen,'TOPIC'.$this->topic_id,'KO');
    if (!$this->l98) {return FALSE;} else {return TRUE;}
  }

  function admin_edit(){
    if (is_array($this->ligamods) AND in_array($this->username,$this->ligamods)){
      $this->get_games_username = TRUE;
    } else {
      $this->get_games_username = $this->username;
    }
  }


//#############################################################################
  function post_auswerten_dh_submit(){
    switch(FALSE){
      case !$_POST['dh_submit_all']:
      case !$_POST['dh_state']: //Status blieb erhalten
      case !$_POST['dh_set_all']:
      case !$_POST['dh_set_text']:
      case !$_POST['dh_change_text']:
        $this->bericht = $_POST['dh_bericht'];
      case !$_POST['dh_set_shooter']:
      case !$_POST['dh_add_shooter_h']://Heim-Torschuss-Schuetze soll hinzugefuegt werden
      case !$_POST['dh_add_shooter_a']://Auswaerts-Torschuss-Schuetze
      case !$_POST['dh_del_shooter_h']://letzter Heim-Torschuss-Schuetze soll entfernt werden
      case !$_POST['dh_del_shooter_a']://letzter Auswaerts-Torschuss-Schuetze
      case !$_POST['dh_change_shooter']://L?t Torschuss wieder bearbeiten
        $this->torsch_h = $_POST['dh_torsch_h'];
        $this->torsch_a = $_POST['dh_torsch_a'];
        $i = 0;
        while ($i < $this->torsch_h){
          $i++;
          $this->shots['h'][$i]['a'] = stripslashes($_POST['dh_shotah'.$i]);
          $this->shots['h'][$i]['t'] = stripslashes($_POST['dh_shotth'.$i]);
          $this->shots['h'][$i]['c'] = $_POST['dh_shotch'.$i];
          $this->shots['h'][$i]['m'] = $_POST['dh_shotmh'.$i];
        }
        $i = 0;
        while ($i < $this->torsch_a){
          $i++;
          $this->shots['a'][$i]['a'] = stripslashes($_POST['dh_shotaa'.$i]);
          $this->shots['a'][$i]['t'] = stripslashes ($_POST['dh_shotta'.$i]);
          $this->shots['a'][$i]['c'] = $_POST['dh_shotca'.$i];
          $this->shots['a'][$i]['m'] = $_POST['dh_shotma'.$i];
        }


//      case 'dh_set_home': //Setzt alle Heim Shooter
//      case 'dh_set_away': //Setzt alle Auswaerts Shooter
      case !$_POST['dh_set_scorers']://Torschuetzen, Vorlagengeber und Karten eingegeben
      case !$_POST['dh_change_scorers']:// Torschuetzen, Vorlagengeber und Karten aendern
        $this->yellowA = 1*$_POST['dh_yellowA'];
        $this->yellowB = 1*$_POST['dh_yellowB'];
        $this->yelredA = 1*$_POST['dh_yelredA'];
        $this->yelredB = 1*$_POST['dh_yelredB'];
        $this->redA = 1*$_POST['dh_redA'];
        $this->redB = 1*$_POST['dh_redB'];
        $i=0;
        while($i < $this->yellowA){
          $i++;
          $this->cards['h']['y'][$i]['a'] = stripslashes($_POST['dh_crd_hya'.$i]);
          $this->cards['h']['y'][$i]['t'] = stripslashes ($_POST['dh_crd_hyt'.$i]);
          $this->cards['h']['y'][$i]['m'] = $_POST['dh_crd_hym'.$i];
        }
        $i=0;
        while($i < $this->yelredA){
          $i++;
          $this->cards['h']['yr'][$i]['a'] = stripslashes($_POST['dh_crd_hyra'.$i]);
          $this->cards['h']['yr'][$i]['t'] = stripslashes ($_POST['dh_crd_hyrt'.$i]);
          $this->cards['h']['yr'][$i]['m'] = $_POST['dh_crd_hyrm'.$i];
        }
        $i=0;
        while($i < $this->redA){
          $i++;
          $this->cards['h']['r'][$i]['a'] = stripslashes($_POST['dh_crd_hra'.$i]);
          $this->cards['h']['r'][$i]['t'] = stripslashes ($_POST['dh_crd_hrt'.$i]);
          $this->cards['h']['r'][$i]['m'] = $_POST['dh_crd_hrm'.$i];
        }
        $i=0;
        while($i < $this->yellowB){
          $i++;
          $this->cards['a']['y'][$i]['a'] = stripslashes($_POST['dh_crd_aya'.$i]);
          $this->cards['a']['y'][$i]['t'] = stripslashes ($_POST['dh_crd_ayt'.$i]);
          $this->cards['a']['y'][$i]['m'] = $_POST['dh_crd_aym'.$i];
        }
        $i=0;
        while($i < $this->yelredB){
          $i++;
          $this->cards['a']['yr'][$i]['a'] = stripslashes($_POST['dh_crd_ayra'.$i]);
          $this->cards['a']['yr'][$i]['t'] = stripslashes ($_POST['dh_crd_ayrt'.$i]);
          $this->cards['a']['yr'][$i]['m'] = $_POST['dh_crd_ayrm'.$i];
        }
        $i=0;
        while($i < $this->redB){
          $i++;
          $this->cards['a']['r'][$i]['a'] = stripslashes($_POST['dh_crd_ara'.$i]);
          $this->cards['a']['r'][$i]['t'] = stripslashes ($_POST['dh_crd_art'.$i]);
          $this->cards['a']['r'][$i]['m'] = $_POST['dh_crd_arm'.$i];
        }
        //print_array($this->cards);
        $this->goals_count = $_POST['dh_goals_count'];
        $i=0;
        while($i < $this->goals_count){
          $i++;
          $temp = array();
          $temp['h_a']= $_POST['dh_torh_a'.$i];
          $temp['min']= $_POST['dh_tormin'.$i];
          $temp['spa']= stripslashes($_POST['dh_torspa'.$i]);
          $temp['spt']= stripslashes ($_POST['dh_torspt'.$i]);
          $temp['typ']= $_POST['dh_tortyp'.$i];
          $temp['voa']= stripslashes($_POST['dh_torvoa'.$i]);
          $temp['vot']= stripslashes ($_POST['dh_torvot'.$i]);
          $this->goals_arr[$i] = $temp;
          unset($temp);
        }
      case !$_POST['dh_set_result']://Wenn Tore und Teamstats eingegeben wurden
      case !$_POST['dh_change_result']://Um Tore/Teamstats und Datum zu aendern, ohne Daten zu verlieren
        $this->y = 1*$_POST['dh_jahr'];
        $this->m = 1*$_POST['dh_monat'];
        $this->d = 1*$_POST['dh_tag'];
        $this->h = 1*$_POST['dh_stunde'];
        $this->mi = 1*$_POST['dh_minute'];
        $this->goalsA = 1*$_POST['dh_goalsA'];
        $this->goalsB = 1*$_POST['dh_goalsB'];
        $this->torschusseA = 1*$_POST['dh_torschusseA'];
        $this->torschusseB = 1*$_POST['dh_torschusseB'];
        $this->auftorA = 1*$_POST['dh_auftorA'];
        $this->auftorB = 1*$_POST['dh_auftorB'];
        $this->penaltyA = 1*$_POST['dh_penaltyA'];
        $this->penaltyB = 1*$_POST['dh_penaltyB'];
        $this->freekicksA = 1*$_POST['dh_freekicksA'];
        $this->freekicksB = 1*$_POST['dh_freekicksB'];
        $this->cornersA = 1*$_POST['dh_cornersA'];
        $this->cornersB = 1*$_POST['dh_cornersB'];
        $this->foulsA = 1*$_POST['dh_foulsA'];
        $this->foulsB = 1*$_POST['dh_foulsB'];
        $this->offsideA = 1*$_POST['dh_offsideA'];
        $this->offsideB = 1*$_POST['dh_offsideB'];
        $this->yellowA = 1*$_POST['dh_yellowA'];
        $this->yellowB = 1*$_POST['dh_yellowB'];
        $this->yelredA = 1*$_POST['dh_yelredA'];
        $this->yelredB = 1*$_POST['dh_yelredB'];
        $this->redA = 1*$_POST['dh_redA'];
        $this->redB = 1*$_POST['dh_redB'];
        $this->posessionA = 1*$_POST['dh_posessionA'];
        $this->posessionB = 1*$_POST['dh_posessionB'];
      case !$_POST['dh_select_game']://Wenn Game ausgewaehlt wurde
        $temp = 1*$_POST['dh_gameid'];
        if ($temp>0) {$this->gameid = $temp;}
      case !$_POST['dh_change_game']:
      case !$_POST['dh_select_spieltag']:
        $this->spieltag = $_POST['dh_spieltag'];
      case !$_POST['dh_change_spieltag']:
      case !$_POST['dh_select_liga']:
        $this->ligaid = $_POST['dh_liga'];
      case !$_POST['dh_change_liga']:
      case !$_POST['dh_undo']:
      default:
    }
  }


//#############################################################################
  function get_from_game(){
    if (!$this->set_game($this->gameid)){return FALSE;}
    $this->not = $this->game['not'];
    $this->TA = $this->game['TA'];
    $this->TB = $this->game['TB'];
    $this->GA = $this->game['GA'];
    $this->GB = $this->game['GB'];
    $this->NT = $this->game['NT'];
    $this->BE = $this->game['BE'];
    $this->TI = $this->game['TI'];
    $this->AT = $this->game['AT'];
    $this->teamA = $this->game['teamA'];
    $this->teamB = $this->game['teamB'];
    $this->trainerA = $this->game['trainerA'];
    $this->trainerB = $this->game['trainerB'];
    $this->gespielt = $this->game['gespielt'];
    $this->date = $this->game['date'];
    $this->time = $this->game['time'];
    $this->hour = $this->game['hour'];
    $this->minute = $this->game['minute'];
    $this->year = $this->game['year'];
    $this->month = $this->game['month'];
    $this->day = $this->game['day'];
    if (empty($this->goalsA) AND $this->goalsA !== '0' AND $this->goalsA !== 0){$this->goalsA = $this->GA;}
    if (empty($this->goalsB) AND $this->goalsB !== '0' AND $this->goalsB !== 0){$this->goalsB = $this->GB;}
    if ($this->y === FALSE){$this->y = $this->year;}
    if ($this->m === FALSE){$this->m = $this->month;}
    if ($this->d === FALSE){$this->d = $this->day;}
    if ($this->h === FALSE){$this->h = $this->hour;}
    if ($this->mi === FALSE){$this->mi = $this->minute;}
  }


//################
    function change_shooter_count(){
      switch (FALSE){
        case !$_POST['dh_add_shooter_h']:
            $this->torsch_h++; break;
        case !$_POST['dh_add_shooter_a']:
            $this->torsch_a++; break;
        case !$_POST['dh_del_shooter_h']:
            unset($this->shots['h'][$this->torsch_h]);
            $this->torsch_h--;
            break;
        case !$_POST['dh_del_shooter_a']:
            unset($this->shots['a'][$this->torsch_a]);
            $this->torsch_a--; break;
        case !$_POST['dh_set_scorers']:
            $this->torsch_h = 0;
            $this->torsch_a = 0;
            break;
      }
    }

//###################################
    function code_bericht(){
      if ($this->bericht){
        switch (FALSE){
          case !$_POST['dh_state']: //Status blieb erhalten
          case !$_POST['dh_set_all']:
          case !$_POST['dh_set_text']:
          case !$_POST['dh_change_text']:
            $this->bericht = convert_text($this->bericht,'',FALSE);
            $this->bericht = stripslashes($this->bericht);
            break;
          case 'str2html':
            break;
          case 'html2str':
            break;
        }
      }
    }

//#############################################################################
  //Prueft die Daten und gibt an wie weit das Formular angezeigt wird
  function proof_vars(){
    if (!$this->exists OR !$this->username){return FALSE;}
        $this->form_depth = 9;//##########Auswahl Liga###############################################
    if (empty($this->ligen_arr)){$this->fehler[10].='Es sind keine Ligen ausw&auml;hlbar. Bitte wenden Sie sich an den Admin<br>';}
    if (empty($this->ligaid) OR $_POST['dh_change_liga']){return FALSE;}
    if (!$this->l98){$this->fehler[10].='Es konnte keine Ligadatei ausgew&auml;hlt werden. Bitte wenden Sie sich an den Admin<br>';}
    if (empty($this->current)){$this->fehler[10].='Es konnte keine Ligadatei gefunden werden. Bitte wenden Sie sich an den Admin<br>';}
    if ($this->fehler[10]){return false;}
      $this->form_depth = 10; //#######Auswahl Spieltag##################################################
    $this->set_spieltage();
    if (empty($this->st_arr)){$this->fehler[11].='Es sind (noch) keine Spieltage ausw&auml;hlbar.<br>';}
    if ($_POST['dh_select_liga']){return FALSE;}
    if ($_POST['dh_change_spieltag']){return FALSE;}
    if ($this->spieltag < $this->sta OR $this->spieltag > $this->ste){$this->fehler[11].='Es wurde kein g&uumlltiger Spieltag ausgew&auml;hlt.<br>';}
    if ($this->fehler[11]){return false;}
      $this->form_depth = 11; //#########################################################
    if (empty($this->games)){$this->fehler[12].='Sie haben/hatten kein Spiel in der '.$this->liganame.'am Spieltag '.$this->spieltag.'. Aus diesem Grund steht ihnen die Eingabemaske nicht zur Verf&uuml;gung.<br>';}//User nicht in Liga/Spieltag
    if ($this->fehler[12]){return FALSE;}
      $this->form_depth = 12;   //#########################################################
    if (!$this->gameid OR !$this->set_game($this->gameid)){return FALSE;}//wenn gameid nicht angegeben oder nicht passend
    if ($this->gespielt) {$this->fehler[13].='Bereits gespielt, keine Eingabe m&ouml;glich';}
    if ($this->fehler[13]){return FALSE;}
    $this->form_depth = 15;//#########################################################
    if ($_POST['dh_select_game'] AND $this->stufen['result']){return FALSE;}
    if ($this->y<2006){$this->fehler[14].='Die Jahresangabe ist keine vierstellige Zahl gr&ouml;&szlig;er 2005.<br>';}
    if ($this->m < 1 OR $this->m > 12){$this->fehler[14].='Monat liegt nicht innerhalb von 1 bis 12.<br>';}
    if ($this->d < 1 OR $this->d > 31){$this->fehler[14].='Tag liegt nicht im g&uuml;ltigen Bereich.<br>';}
    if ($this->h < 0 OR $this->h > 23){$this->fehler[14].='Stunde nicht im g&uuml;ltigen Bereich [0-23]<br>';}
    if ($this->mi < 0 OR $this->mi > 59){$this->fehler[14].='Minute nicht im g&uuml;ltigen Bereich [0-59]<br>';}
    if ($this->fehler[14]){return FALSE;}
    $this->sdate = mktime($this->h,$this->mi,0,$this->m,$this->d,$this->y);
    $this->form_depth = 15;  //#########################################################
    if ($this->stufen['result']){ //Ergebnis
        if ($this->goalsA < 0){$this->fehler[15].='Der Wert der Heimtore ist nicht numerisch.<br>';}
        if ($this->goalsB < 0){$this->fehler[15].='Der Wert der Ausw&auml;rtstore ist nicht numerisch.<br>';}
    }
    if ($this->fehler[15]){return FALSE;}
    $this->form_depth = 15;   //#########################################################
    $this->get_all_teamstats();
    if ($this->stufen['teamstat']){
        if ($this->torschusseA < 0 OR $this->torschusseA ===''){$this->fehler[16].='Ung&uuml;ltig: Torsch&uuml;sse - Heim<br>';}
        if ($this->torschusseB < 0 OR $this->torschusseB ===''){$this->fehler[16].='Ung&uuml;ltig: Torsch&uuml;sse - Ausw&auml;rts<br>';}
        if ($this->auftorA < 0 OR $this->auftorA ===''){$this->fehler[16].='Ung&uuml;ltig: Schuesse aufs Tor - Heim<br>';}
        if ($this->auftorB < 0 OR $this->auftorB ===''){$this->fehler[16].='Ung&uuml;ltig: Schuesse aufs Tor - Ausw&auml;rts<br>' ;}
        if ($this->penaltyA < 0 OR $this->penaltyA ===''){$this->fehler[16].='Ung&uuml;ltig: Elfmeter - Heim<br>';}
        if ($this->penaltyB < 0 OR $this->penaltyB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Elfmeter - Ausw&auml;rts<br>';}
        if ($this->freekicksA < 0 OR $this->freekicksA ===''){$this->fehler[16].='Ung&uuml;ltig: Freistoss - Heim<br>';}
        if ($this->freekicksB < 0 OR $this->freekicksB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Freistoss - Ausw&auml;rts<br>';}
        if ($this->cornersA < 0 OR $this->cornersA ===''){$this->fehler[16].='Ung&uuml;ltig: Ecken - Heim<br>';}
        if ($this->cornersB < 0 OR $this->cornersB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Ecken - Ausw&auml;rts<br>';}
        if ($this->foulsA < 0 OR $this->foulsA ===''){$this->fehler[16].= 'Ung&uuml;ltig: Fouls - Heim<br>';}
        if ($this->foulsB < 0 OR $this->foulsB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Fouls - Ausw&auml;rts<br>';}
        if ($this->offsideA < 0 OR $this->offsideA ===''){$this->fehler[16].= 'Ung&uuml;ltig: Abseits - Heim<br>';}
        if ($this->offsideB < 0 OR $this->offsideB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Abseits - Ausw&auml;rts<br>';}
        if ($this->yellowA < 0 OR $this->yellowA ===''){$this->fehler[16].= 'Ung&uuml;ltig: Gelbe Karten - Heim<br>';}
        if ($this->yellowB < 0 OR $this->yellowB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Gelbe Karten - Ausw&auml;rts<br>' ;}
        if ($this->yelredA < 0 OR $this->yelredA ===''){$this->fehler[16].= 'Ung&uuml;ltig: Gelb/Rote Karten - Heim<br>';}
        if ($this->yelredB < 0 OR $this->yelredB ===''){$this->fehler[16].= 'Ung&uuml;ltig: Gelb/Rote Karten - Ausw&auml;rts<br>';}
        if ($this->redA < 0 OR $this->redA ===''){$this->fehler[16].= 'Ung&uuml;ltig: Rote Karten - Heim<br>';}
        if ($this->redB < 0 OR $this->redB ===''){$this->fehler[16].=  'Ung&uuml;ltig: Rote Karten - Ausw&auml;rts<br>';}
        if ($this->posessionA < 0 OR $this->posessionA > 100){$this->fehler[16].= 'Ung&uuml;ltig: Ballbesitz - Heim<br>';}
        if ($this->posessionB < 0 OR $this->posessionB > 100){$this->fehler[16].= 'Ung&uuml;ltig: Ballbesitz - Ausw&auml;rts<br>';}
        if ($this->stufen['moreinfo']) {
          if ($this->posessionA + $this->posessionB != 100){$this->fehler[16].='Die Addition des Ballbesitzes ergibt nicht 100<br>';}
        }
    }
    if ($this->fehler[16] OR $_POST['dh_change_result']){return FALSE;}
    $this->form_depth = 17;   //#########################################################
    $this->get_team_spieler();
    if ($this->stufen['scorer']){//Torschuetzen
        //Pruefen der Angaben fuer Torschuetzen und Vorlagen
        if ($_POST['dh_set_result']){return false;}
        $this->fehler[17]=$this->proof_scorer();
    }
    if ($this->fehler[17] OR $_POST['dh_change_scorers']){return FALSE;}
    if ($this->stufen['cards']){//KARTEN
        //Pruefen der Angaben fuer Karten
        $this->fehler[18] = $this->proof_all_suender();
    }
    if ($this->fehler[18]){return FALSE;}
    $this->form_depth = 18;      //#########################################################
    if ($this->stufen['shots'] AND ($_POST['dh_set_scorers'] OR $_POST['dh_add_shooter_h'] OR $_POST['dh_del_shooter_h'] OR $_POST['dh_add_shooter_a'] OR $_POST['dh_del_shooter_a'])){return FALSE;}
    if ($this->stufen['shots']){ //SCHUESSE
        //Pruefen der Schuesse Heim
        $add ='';
        $i=0;
        $shoots_h = 0;
        while (is_array($this->shots['h']) AND list($key,$homeshot) = each ($this->shots['h'])){
          $i++;
          if (!$homeshot['a'] == !$homeshot['t']){
            $add.='Spieler muss entweder ausgew&auml;hlt ODER eingegeben werden, jedoch nicht beides oder keines.<br>';
          } elseif ($homeshot['a']){
            $temp = explode(';',$homeshot['a']);
            if (count($temp)!=2){$add.="Die Spielerauswahl ist nicht im Format 'Spieler;Team'. Bitte wenden Sie sich an den Administrator oder geben Sie im Textfeld einen Spieler ein.<br>";
            } else {
            if (!$temp[0]){$add.="Der ausgew&auml;hlte Spielername darf nicht leer sein. Wenden Sie sich bitte an den Administrator.<br>";}
            if ($temp[1]!= $this->teamA){$add.="Der ausgew&auml;hlte Spieler geh&ouml;rt nicht $this->teamA an. Wenden Sie sich bitte an den Administrator.<br>";}
            }
          } elseif ($homeshot['t']){
            $temp = explode(';',$homeshot['t']);
            if (count($temp)!=2){
                if (count($temp) == 1){
                $this->shots['h'][$key]['t'] .= ';'.$this->teamA;
                $add.="Die Spielereingabe war nicht im Format 'Spieler;Team'.<br>";
                } else {
                $add.="Die Spielereingabe ist nicht im Format 'Spieler;Team'.<br>";
                }
            } else {
            if (!$temp[0]){$add.="Der eingegebene Spielername darf nicht leer sein.<br>";}
            if ($temp[1]!= $this->teamA){$add.="Das Team des eingegeben Spielers ist nicht $this->teamA.<br>";}
            }
          }
          if ($homeshot['c']<1){$add.='Die Anzahl der Torsch&uuml;sse muss gr&ouml;&szlig;er Null sein. Falls vom Spieler nicht aufs Tor geschossen wurde muss dieser mit der Schaltfl&auml;che gel&ouml;scht werden.<br>';
          } else {
            $shoots_h += 1*$homeshot['c'];
          }
          if (!$homeshot['m']){$add.='Bitte geben Sie die Minutenangabe an.<br>';} else {
            $exp = explode(',',$homeshot['m']);
            $cm = count($exp);
            if ($cm != $homeshot['c']){
              $add .= 'Die Anzahl der (kommagetrennten) Minutenangaben ['.$cm.'] entspricht nicht der angegeben Toschussanzahl ['.$homeshot['c'].'] .';
            }
          }
          $this->fehler[19] .= ($add)?'<b>Heimteam-Spieler Nr.'.$key.'</b><br>'.$add:'';
          $add ="";
        }
        //Pruefen der Schuesse Ausw&auml;rts
        $add = '';
        $shoots_a = 0;
        $i = 0;
        while (is_array($this->shots['a']) AND list($key,$homeshot) = each ($this->shots['a'])){
         $i++;
          if (!$homeshot['a'] == !$homeshot['t']){
           $add.='Spieler muss entweder ausgew&auml;hlt ODER eingegeben werden, jedoch nicht beides oder keines.<br>';
         } elseif ($homeshot['a']){
            $temp = explode(';',$homeshot['a']);
            if (count($temp)!=2){$add.="Die Spielerauswahl ist nicht im Format 'Spieler;Team'. Bitte wenden Sie sich an den Administrator oder geben sie im Textfeld einen Spieler ein.<br>";
            } else {
            if (!$temp[0]){$add.="Der ausgew&auml;hlte Spielername darf nicht leer sein. Wenden Sie sich bitte an den Administrator.<br>";}
           if ($temp[1]!= $this->teamB){$add.="Der ausgew&auml;hlte Spieler geh&ouml;hrt nicht $this->teamB an. Wenden Sie sich bitte an den Administrator.<br>";}
            }
          } elseif ($homeshot['t']){
            $temp = explode(';',$homeshot['t']);
            if (count($temp)!=2){
                if (count($temp) == 1){
                $this->shots['a'][$key]['t'] .= ';'.$this->teamB;
               $add.="Die Spielereingabe war nicht im Format 'Spieler;Team'.<br>";
                } else {
                $add.="Die Spielereingabe ist nicht im Format 'Spieler;Team'.<br>";
                }
            } else {
                if (!$temp[0]){$add.="Der eingegebene Spielername darf nicht leer sein.<br>";}
                if ($temp[1]!= $this->teamB){$add.="Das Team des eingegeben Spielers ist nicht $this->teamB.<br>";}
            }
          }
          if ($homeshot['c']<1){$add.='Die Anzahl der Torsch&uuml;sse  muss gr&ouml;&szlig;er Null sein. Falls vom Spieler nicht aufs Tor geschossen wurde muss dieser mit der Schaltfl&auml;che gel&ouml;scht werden.<br>';
          } else {
            $shoots_a += 1*$homeshot['c'];
          }
          if (!$homeshot['m']){$add.='Bitte geben sie die Minutenangabe an.<br>';}
          $this->fehler[19] .= ($add)?'<b>Ausw&auml;rtsteam-Spieler Nr.'.$key.'</b><br>'.$add:'';
          $add ="";
        }
        if ($this->torsch_h > $this->torschusseA){$this->fehler[19] .='<b>Es sind mehr Heimspieler-Auswahlfelder ['.$this->torsch_h.'] als Heimtorsch&uuml;sse ['.$this->torschusseA.'] angegeben.</b><br>';}
        if ($this->torsch_a > $this->torschusseB){$this->fehler[19] .='<b>Es sind mehr Ausw&auml;rtsspieler-Auswahlfelder ['.$this->torsch_a.'] als Ausw&auml;rtstorsch&uuml;sse ['.$this->torschusseB.'] angegeben.</b><br>';}
        if ($shoots_h != $this->torschusseA){$this->fehler[19] .='<b>Die Anzahl der Heimtorschue? ['.$shoots_h.'] stimmt nicht mit den angegebenen ['.$this->torschusseA.'] &uuml;berein.</b><br>';}
        if ($shoots_a != $this->torschusseB){$this->fehler[19] .='<b>Die Anzahl der Ausw&auml;rtstorschue? ['.$shoots_a.'] stimmt nicht mit den angegebenen ['.$this->torschusseB.'] &uuml;berein.</b><br>';}
    }
    if ($this->fehler[19] OR $_POST['dh_change_shooter']){return FALSE;}
    $this->form_depth = 19; //#########################################################
    //if ($_POST['dh_change_text'] OR $_POST['dh_set_shooter']){return FALSE;}
    if ($_POST['dh_change_text']){return FALSE;}
    if ($this->stufen['bericht']){
        if (strlen($this->bericht)< 30){$this->fehler[20] .= 'Bitte gib zumindest einen kurzen Bericht (1-2 S&auml;tze,  30 Zeichen) ein.<br>';}
    }
    if ($this->fehler[20]){return FALSE;}
    $this->form_depth = 20; //#########################################################
    if (!$_POST['dh_submit_all']){return FALSE;}
    //uebermitteln der Daten
    $this->submit_all();
    if ($this->fehler[21]){return FALSE;}
    $this->form_depth = 21;
    return TRUE;
  }

//#############################################################################
//Ausgelagerte Pruefung der Torschuetzen und Vorlagen
  function proof_scorer(){
    $i = 1;
    $goalsh=0;
    $goalsa=0;

    $erg ='';
    while ($i<=$this->goals_count){
        $temp = $this->goals_arr[$i];
        if (!$temp['h_a']){$erg.="Tor $i: Es wurde weder das Tor f&uuml;r 'Heim' noch f&uuml;r 'Ausw&auml;rts' ausgew&auml;hlt.<br>";} else {$h_a = $temp['h_a'];}
        if ($this->stufen['moreinfo']!=FALSE) {
          if ($temp['min']<1 OR $temp['min']>120){$erg.="Tor $i: Die Minutenangabe ist keine Zahl oder liegt nicht zwischen 1 und 120.<br>";}
        }
        //Schuetzenangabe
        if (!$temp['spa'] == !$temp['spt']){$erg.="Tor $i: Schu&uuml;tzenangabe - Entweder einen Spieler im Auswahlfeld angeben ODER im Textfeld, jedoch nicht beides oder keines.<br>";
        } else {//spa !=spt FALSE
            if ($temp['spa']){
            $arra = explode(';',$temp['spa']);
            if (count($arra)!=2){$erg.="Tor $i: Sch&uuml;tzenangabe: Die Spielerauswahl ist nicht im Format 'Spieler;Team'. Bitte wenden Sie sich an den Administrator oder geben sie im Textfeld einen Spieler ein.<br>";
            } else {
              if (!$arra[0]){$erg.="Tor $i: Der ausgew&auml;hlte Spielername darf nicht leer sein. Wenden sie sich bitte an den Administrator.<br>";}  else {$schuetze = $arra[0];}
              if ($arra[1]!= $this->teamA AND $arra[1]!= $this->teamB){$erg.="Tor $i: Der ausgew&auml;hlte Sch&uuml;tze geh&ouml;rt nicht einem der beiden Teams des Spieles an.<br>";} else {$team = $arra[1];}
            }
            } else {// spa TRUE
              $arra = explode(';',$temp['spt']);
            if (count($arra)!=2){$erg.="Tor $i: Sch&uuml;tzenangabe: Die Spielerangabe ist nicht im Format 'Spieler;Team'. Der Spielername muss mit einem Semikolon ';' vom Team getrennt werden.<br>";
            } else {//count arra spt != 2
              if (!$arra[0]){$erg.="Tor $i: Der eingegeben Spielername darf nicht leer sein.<br>";}  else {$schuetze = $arra[0];}
              if ($arra[1]!= $this->teamA AND $arra[1]!= $this->teamB){$erg.="Tor $i: Das Team des eingegebenen Sch&uuml;tzen wurde falsch geschrieben/angegeben.<br>";} else {$team = $arra[1];}
            } //count arra spt = 2
        }//spa FALSE

        }//spa != spt TRUE
        //Typ
        if ($temp['typ'] == '' OR $temp['typ'] == 'ET' OR $temp['typ'] == 'EM' OR $temp['typ'] == 'FS'){
          if ($temp['typ'] == 'ET') {$et = TRUE;} else {$et = FALSE;}
        } else {
          $erg.="Tor $i: Der Tortyp ist Ung&uuml;ltig, Bitte wenden Sie sich an den Admin.<br>";
        }
        if ($h_a == 'home' AND !$et AND $team != $this->teamA){$erg.="Tor $i: Es wurde ein Spieler vom falschen Team f&uuml;r dieses Heimtor ausgew&auml;hlt oder das Tor nicht als Eigentor deklariert.<br>";}
        if ($h_a == 'home' AND $et AND $team != $this->teamB){$erg.="Tor $i: Es wurde ein Spieler vom falschen Team f&uuml;r dieses Heimtor ausgew&auml;hlt oder das Tor f&auml;lschlicherweise als Eigentor deklariert.<br>";}
        if ($h_a == 'away' AND !$et AND $team != $this->teamB){$erg.="Tor $i: Es wurde ein Spieler vom falschen Team f&uuml;r dieses Ausw&auml;rtstor ausgew&auml;hlt oder das Tor nicht als Eigentor deklariert.<br>";}
        if ($h_a == 'away' AND $et AND $team != $this->teamA){$erg.="Tor $i: Es wurde ein Spieler vom falschen Team f&uuml;r dieses Ausw&auml;rtstor ausgew&auml;hlt oder das Tor f&auml;lschlicherweise als Eigentor deklariert.<br>";}

        //Vorlagengeber
        if ($this->stufen['moreinfo']!=FALSE) {
          if (!$temp['voa'] == !$temp['vot']){$erg.="Tor $i: Vorlagengeber - Entweder einen Spieler im Auswahlfeld angeben ODER im Textfeld, jedoch nicht beides oder keines.<br>";
          } else {//voa !=vot FALSE
            if ($temp['voa'] AND $temp['voa'] != -1){
              $arra = explode(';',$temp['voa']);
              if (count($arra)!=2){$erg.="Tor $i: Vorlagengeber: Die Spielerauswahl ist nicht im Format 'Spieler;Team'. Bitte wenden Sie sich an den Administrator oder geben sie im Textfeld einen Spieler ein.<br>";
              } else {
                if (!$arra[0]){$erg.="Tor $i: Vorlagengeber: Der ausgew&auml;hlte Spielername darf nicht leer sein. Wenden sie sich bitte an den Administrator.<br>";} else {$vorlagg = $arra[0];}
                if ($arra[1]!= $this->teamA AND $arra[1]!= $this->teamB){$erg.="Tor $i: Der ausgew&auml;hlte Vorlagengeber geh&ouml;rt nicht einem der beiden Teams des Spieles an.<br>";} else {$teamB = $arra[1];}
              }
            } elseif ($temp['voa'] != -1) {// voa TRUE
              $arra = explode(';',$temp['vot']);
              if (count($arra)!=2){$erg.="Tor $i: Vorlagengeber: Die Spielerangabe ist nicht im Format 'Spieler;Team'. Der Spielername muss mit einem Semikolon ';' vom Team getrennt werden.<br>";
              } else {//count arra vot != 2
                if (!$arra[0]){$erg.="Tor $i: Der eingegeben Spielername darf nicht leer sein.<br>";}  else {$vorlagg = $arra[0];}
                if ($arra[1]!= $this->teamA AND $arra[1]!= $this->teamB){$erg.="Tor $i: Das Team des eingegebenen Schuetzen wurde falsch geschrieben/angegeben.<br>";} else {$teamB = $arra[1];}
              } //count arra vot = 2
            }//voa FALSE
          }//voa != vot TRUE

          if ($et AND ($temp['voa']!='-1' OR $temp['vot'])){$erg.="Tor $i: Vorlagengeber: Bei einem Eigentor kann es keinen Vorlagengeber geben. W&auml;hlen sie deshalb bitte -kein Spieler- und lassen sie die Texteingabe f&uuml;r den Vorlagengeber leer.<br>";}

          if ($temp['voa'] != -1 AND $team != $teamB){$erg.="Tor $i: Der Vorlagengeber muss vom gleichen Verein wie der Sch&uuml;tze sein.<br>";}
          if ($this->goals_arr[$i]['min'] < $this->goals_arr[$i-1]['min']){$erg .= "Tor $i: Die Minutenangabe von Tor $i ist kleiner als die von Tor ". ($i-1) .".<br>";}
        } //stufen['moreinfo']

        switch ($h_a){
          case 'away': $goalsa++; $ha = 'a'; break;
          case 'home': $goalsh++; $ha = 'h'; break;
        }
        $this->goals_arr[$i]['stand']=$goalsh.':'.$goalsa;
        $this->goals_arr[$i]['verein']=$team;
        $this->goals_arr[$i]['schuetze']=$schuetze;
        if ($this->stufen['moreinfo']!=FALSE) {
          $this->goals_arr[$i]['vorlagengeber']=$vorlagg;
        }
        if ($schuetze AND $team AND ($vorlagg OR $temp['voa'] == -1)){
        if (!is_array($this->spieler[strtoupper($ha)])){$this->spieler[strtoupper($ha)]=array();}
        if (!in_array($schuetze.';'.$team,$this->spieler[strtoupper($ha)])){$this->spieler[strtoupper($ha)][]=$schuetze.';'.$team;}
        if ($this->stufen['moreinfo']!=FALSE) {
          if (!in_array($vorlagg.';'.$team,$this->spieler[strtoupper($ha)]) AND $temp['voa'] != -1){$this->spieler[strtoupper($ha)][]=$vorlagg.';'.$team;}
        }
      }

        $vorlagg='';
        $schuetze='';
        $team='';
        $i++;

    }//while

    if ($goalsh != $this->goalsA){$erg.="Die Anzahl der Heimtore stimmt nicht mit der Ergebnisangabe ueberein.<br>";}
    if ($goalsa != $this->goalsB){$erg.="Die Anzahl der Ausw&auml;rtstore stimmt nicht mit der Ergebnisangabe ueberein.<br>";}
    return $erg;
  }

//#############################################################################
  function proof_suender($h_a,$y_yr_r,$i){
    $vala = $this->cards[$h_a][$y_yr_r][$i]['a'];
    $valt = $this->cards[$h_a][$y_yr_r][$i]['t'];
    if (!$vala == !$valt){
      $erg.= 'Ein Spieler muss ausgew&auml;hlt oder eingegeben werden (aber nicht beides oder keines).<br>';
    } elseif ($vala){
      $arra = explode(';',$vala);
      if (count($arra)!=2){
        $erg.="Es muss der Spieler im Format 'Spieler;Verein' angegeben werden. Bitte wenden Sie sich an den Admin.<br>";
      } elseif (empty($arra[0])){
        $erg.="Es muss ein Spielername angegeben sein. Bitte wenden Sie sich an den Admin.<br>";
      } elseif (empty($arra[1])){
        $erg.="Es muss ein Vereinsname angegeben sein. Bitte wenden Sie sich an den Admin.<br>";
      } elseif (($h_a == 'h' AND $arra[1]!=$this->teamA) OR ($h_a == 'a' AND $arra[1]!=$this->teamB)){
        $erg.="Es wurde ein Spieler des falschen Teams ausgew&auml;hlt. Bitte wenden Sie sich an den Admin.<br>";
      }
    } elseif ($valt){
      $arra = explode(';',$valt);
      if (count($arra)!=2){
        $erg.="Es muss der Spieler im Format 'Spieler;Verein' angegeben werden.<br>";
        if (count($arra) == 1){
          switch ($h_a){
           case 'h':
            $this->cards[$h_a][$y_yr_r][$i]['t'] .= ';'.$this->teamA;break;
           case 'a':
            $this->cards[$h_a][$y_yr_r][$i]['t'] .= ';'.$this->teamB;break;
          }
        }
      } elseif (empty($arra[0])){
        $erg.="Es ein Spielername angegeben sein.<br>";
      } elseif (empty($arra[1])){
        $erg.="Es muss ein Vereinsname angegeben sein.<br>";
      } elseif (($h_a == 'h' AND $arra[1]!=$this->teamA) OR ($h_a == 'a' AND $arra[1]!=$this->teamB)){
        $erg.="Es wurde ein Spieler des falschen Teams ausgew&auml;hlt.<br>";
      }
    }
    if (is_array($this->cards[$h_a]['sp']) AND in_array($arra[0].';'.$arra[1],$this->cards[$h_a]['sp'])){
      $erg.="Dem Spieler wurde in diesem Match bereits eine Karte zugewiesen.<br>";
    }
    if ($this->stufen['moreinfo']!=FALSE) {
    	$this->cards[$h_a]['sp'][]=$arra[0].';'.$arra[1];
    		if ($this->cards[$h_a][$y_yr_r][$i]['m'] < 1 OR $this->cards[$h_a][$y_yr_r][$i]['m'] > 120){
      		$erg .= "Die Minutenangabe fehlt oder ist nicht im richtigen Bereich (1-120)<br>";
    	}
    }
    if ($erg){
        switch ($h_a){
          case 'a':$add .= 'Ausw&auml;rtsteam-';break;
          case 'h':$add .= 'Heimteam-';break;
        }
        switch ($y_yr_r){
          case 'y': $add .='Gelbe Karte Nr.';break;
          case 'yr': $add .='Gelb/Rote Karte Nr.';break;
          case 'r': $add .='Rote Karte Nr.';break;
        }
        $add = '<b>'.$add.$i.'</b><br>';
    }
    if (!is_array($this->spieler[strtoupper($h_a)])){$this->spieler[strtoupper($h_a)]=array();}
    if (!($erg)){
        if (!in_array($vala.$valt,$this->spieler[strtoupper($h_a)])){$this->spieler[strtoupper($h_a)][]=$vala.$valt;}
        }
    return $add.$erg;
  }

//#############################################################################
  function proof_all_suender(){
    $i=0;
        while($i < $this->yellowA){
          $i++;
          $erg .= $this->proof_suender('h','y',$i);
        }
        $i=0;
        while($i < $this->yelredA){
          $i++;
          $erg .= $this->proof_suender('h','yr',$i);
        }
        $i=0;
        while($i < $this->redA){
          $i++;
          $erg .= $this->proof_suender('h','r',$i);
        }
        $i=0;
        while($i < $this->yellowB){
          $i++;
          $erg .= $this->proof_suender('a','y',$i);
        }
        $i=0;
        while($i < $this->yelredB){
          $i++;
          $erg .= $this->proof_suender('a','yr',$i);
        }
        $i=0;
        while($i < $this->redB){
          $i++;
          $erg .= $this->proof_suender('a','r',$i);
        }
    return $erg;
  }


function get_all_spieler(){
  $this->all_spieler = $this->ReadIniCompleteInArr(_spielerdir_.$this->l98.'.players');
  //print_array($this->all_spieler);
}

function get_team_spieler(){
  $this->get_all_spieler();
  $temp = $this->get_data($this->all_spieler,TRUE,'VER',$this->teamA);
  $arr = array('H'=>array(),'A'=>array());
  foreach($temp as $el){
    $arr['H'][]=$el['section'];
  }
  sort ($arr['H']);
  $temp = $this->get_data($this->all_spieler,TRUE,'VER',$this->teamB);
  foreach($temp as $el){
    $arr['A'][]=$el['section'];
  }
  sort ($arr['A']);
  $this->spieler = $arr;
  return $arr;
}

function get_all_teamstats(){
  $this->all_teamstats = $this->ReadIniCompleteInArr(_teamstatsdir_.$this->l98.'.teams');
  //print_array($this->all_teamstats);

}

//#############################################################################
  //Gibt die Spieler der entsprechenden Teams als Auswahlfeldkompatibel aus
  function set_spieler($home=TRUE,$away=TRUE,$new=TRUE,$none=FALSE){
    $arr=array();
    $arr1=array();
    $arr2=array();
	//$this->get_team_spieler();
    foreach($this->spieler['H'] as $sp){
      $arr1[]=array('text'=>$sp,'val'=>$sp);
    }
    foreach($this->spieler['A'] as $sp){
      $arr2[]=array('text'=>$sp,'val'=>$sp);
    }
    $spieler_a['NONE']=array(0=>array('text'=>'-kein Spieler-','val'=>"-1"));
    $spieler_a['NEW']=array(0=>array('text'=>'-neuer Spieler-','val'=>''));
    $spieler_a['HOME']=$arr1;
    $spieler_a['AWAY']=$arr2;
    //print_array($this->spieler_a);
    if ($none) {$arr = $spieler_a['NONE'];}
    if ($new) {$arr = array_merge($arr,$spieler_a['NEW']);}
    if ($home) {$arr = array_merge($arr,$spieler_a['HOME']);}
    if ($away) {$arr = array_merge($arr,$spieler_a['AWAY']);}
    return $arr;
  }

//#############################################################################
  function tor_typen(){
    $arr = array(
        array('val'=>'','text'=>''),
        array('val'=>'EM','text'=>'Elfmeter'),
        array('val'=>'FS','text'=>'Freistoss'),
        array('val'=>'ET','text'=>'Eigentor')
    );
    return $arr;
  }

//######################################
function form_liga_select(){
  if ($this->form_depth < 10){
    $erg.= 'W&auml;hlen sie eine Liga aus:<br>';
    $erg.= dh_auswahl($this->ligen_arr,'dh_liga');
    $erg.= '<br><br>'.dh_input('dh_select_liga','Liga ausw&auml;hlen','submit');
  } else {
    $erg.= dh_input('dh_liga',$this->ligaid,'hidden');
    $erg.= $this->liganame;
    $erg.= '<br><br>'.dh_input('dh_change_liga','Liga &auml;ndern','submit');
  }
  return $erg;
}
//#######################################
function form_spieltag_select(){
  if ($this->form_depth < 11){
    $erg.= 'W&auml;hlen sie einen Spieltag aus:<br>';
    $erg.= dh_auswahl($this->set_spieltage(),'dh_spieltag');
    $erg.= '<br><br>'.dh_input('dh_select_spieltag','Spieltag ausw&auml;hlen','submit');
  } else {
    $erg.= dh_input('dh_spieltag',$this->spieltag,'hidden');
    $erg.= 'Spieltag '.$this->spieltag;
    $erg.= '<br><br>'.dh_input('dh_change_spieltag','Spieltag &auml;ndern','submit');
  }
  return $erg;
}

//#############################################################################
  function form_game_select(){ //Inhalt des Spielauswahlfeldes
    if ($this->form_depth<12){//Wenn keine Spiele gefunden wurden
      $erg.= 'Sie haben/hatten kein Spiel in der '.$this->liganame.'am Spieltag '.$this->spieltag.'. Aus diesem Grund steht ihnen die Eingabemaske nicht zur Verfuegung.<br><br><br>';

    } else {//sonst
      if ($this->form_depth < 13){//wenn gameid nicht angegeben oder nicht passend
        //$erg .= ($this->gameid)?'<font color="red">Gew&auml;hltes Spiel ist nicht g&uuml;ltig.</font><br>':'';
        $arr = array(array('val'=>'','text'=>'-kein Spiel ausgew&auml;hlt-'));
        foreach ($this->games as $game){
          $add['val'] = $game['gameid'];
          $add['text'] = $game['teamA'].'['.$game['trainerA'].'] gegen '.$game[teamB].'['.$game['trainerB'].']';
          $arr[]=$add;
        }
        reset($this->games);
        $erg .= dh_auswahl($arr,'dh_gameid');
        $erg .= dh_input('dh_select_game','Spiel ausw&auml;hlen!','submit');
        $erg.='<br><br><br>';
      } else {
        $erg.= dh_input('dh_gameid',$this->gameid,'hidden');
        $erg.= $this->game_data_as_string();
        $erg .= dh_input('dh_change_game','Anderes Spiel w&auml;hlen!','submit');
        $erg.='<br>';
      }
    }
    return $erg;
}

//#############################################################################
  function form_date(){
      if ($this->form_depth<16){
        $erg .= 'Tag: '.dh_input('dh_tag',$this->d,'text','maxlength="2" size="2"');
        $erg .= ' Monat: '.dh_input('dh_monat',$this->m,'text','maxlength="2" size="2"');
        $erg .= ' Jahr: '.dh_input('dh_jahr',$this->y,'text','maxlength="4" size="4"').'<br>';
        $erg .= 'Stunde: '.dh_input('dh_stunde',$this->h,'text','maxlength="2" size="2"');
        $erg .= ' Minute: '.dh_input('dh_minute',$this->mi,'text','maxlength="2" size="2"');
      } else {
        $erg .= dh_input('dh_jahr',$this->y,'hidden');
        $erg .= dh_input('dh_monat',$this->m,'hidden');
        $erg .= dh_input('dh_tag',$this->d,'hidden');
        $erg .= dh_input('dh_stunde',$this->h,'hidden');
        $erg .= dh_input('dh_minute',$this->mi,'hidden');
        $dat = date_time_german(mktime($this->h,$this->mi,0,$this->m,$this->d,$this->y));
        $erg .= 'gespielt am:'.$dat['date'].' um '.$dat['time'].' &nbsp;&nbsp;';
        $erg .= dh_input('dh_change_result','Datums- und Zeitangabe &auml;ndern','submit').'<br>';
      }
      return $erg;
  }

//#############################################################################
  function form_result(){
    if (!$this->stufen['result']){return "Eingabe nicht n&ouml;tig!";}
    if ($this->form_depth<16){
      $erg .= $this->teamA.' '.dh_input('dh_goalsA',$this->goalsA,'text','size="2"');
      $erg .= ' - '.dh_input('dh_goalsB',$this->goalsB,'text','size="2"').$this->teamB;
      $erg .= '<br><br>'.dh_input('dh_set_result','Zur Torsch&uuml;tzeneingabe','submit');
    } else {
      $erg .= dh_input('dh_goalsA',$this->goalsA,'hidden');
      $erg .= dh_input('dh_goalsB',$this->goalsB,'hidden');
      $erg .= $this->teamA.' <b>'.$this->goalsA.' - '.$this->goalsB.'</b> '.$this->teamB.' &nbsp;&nbsp;';
      $erg .= dh_input('dh_change_result','Ergebnis &auml;ndern','submit');
    }
    return $erg;
  }

//#############################################################################
  function form_teamstats(){
    if (!$this->stufen['teamstat']){return "Eingabe nicht n&ouml;tig!";}
    if ($this->form_depth<16){
      $erg .= '<b>'.$this->teamA.' &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$this->teamB.'</b><br>';
      if ($this->stufen['moreinfo']) {
        $erg .= dh_input('dh_posessionA',$this->posessionA,'text','maxlength="2" size="2"').' Ballbesitz in % '.dh_input('dh_posessionB',$this->posessionB,'text','maxlength="2" size="2"').'<br><br>';
        $erg .= dh_input('dh_freekicksA',$this->freekicksA,'text','size="2"').' Freistoss';
        $erg .= dh_input('dh_freekicksB',$this->freekicksB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_cornersA',$this->cornersA,'text','size="2"').' Eckb&auml;lle ';
        $erg .= dh_input('dh_cornersB',$this->cornersB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_foulsA',$this->foulsA,'text','size="2"').' Fouls ';
        $erg .= dh_input('dh_foulsB',$this->foulsB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_offsideA',$this->offsideA,'text','size="2"').' Abseits ';
        $erg .= dh_input('dh_offsideB',$this->offsideB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_torschusseA',$this->torschusseA,'text','size="2"').' Torsch&uuml;sse ';
        $erg .= dh_input('dh_torschusseB',$this->torschusseB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_auftorA',$this->auftorA,'text','size="2"').' aufs Tor ';
        $erg .= dh_input('dh_auftorB',$this->auftorB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_penaltyA',$this->penaltyA,'text','size="2"').' Elfmeter ';
        $erg .= dh_input('dh_penaltyB',$this->penaltyB,'text','size="2"').'<br>';
      }
      if ($this->stufen['cards']) {
        $erg .= dh_input('dh_yellowA',$this->yellowA,'text','size="2"'). ' Gelbe Karten ';
        $erg .= dh_input('dh_yellowB',$this->yellowB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_yelredA',$this->yelredA,'text','size="2"'). ' Gelb/Rote Karten ';
        $erg .= dh_input('dh_yelredB',$this->yelredB,'text','size="2"').'<br>';
        $erg .= dh_input('dh_redA',$this->redA,'text','size="2"').' Rote Karten ';
        $erg .= dh_input('dh_redB',$this->redB,'text','size="2"').'<br>';
      }
      $erg .= dh_input('dh_set_result','Zur Torsch&uuml;tzeneingabe','submit');
    }else {
      if ($this->stufen['moreinfo']) {
        $erg .= dh_input('dh_posessionA',$this->posessionA,'hidden');
        $erg .= dh_input('dh_posessionB',$this->posessionB,'hidden');
        $erg .= dh_input('dh_torschusseA',$this->torschusseA,'hidden');
        $erg .= dh_input('dh_torschusseB',$this->torschusseB,'hidden');
        $erg .= dh_input('dh_auftorA',$this->auftorA,'hidden');
        $erg .= dh_input('dh_auftorB',$this->auftorB,'hidden');
        $erg .= dh_input('dh_penaltyA',$this->penaltyA,'hidden');
        $erg .= dh_input('dh_penaltyB',$this->penaltyB,'hidden');
        $erg .= dh_input('dh_freekicksA',$this->freekicksA,'hidden');
        $erg .= dh_input('dh_freekicksB',$this->freekicksB,'hidden');
        $erg .= dh_input('dh_cornersA',$this->cornersA,'hidden');
        $erg .= dh_input('dh_cornersB',$this->cornersB,'hidden');
        $erg .= dh_input('dh_foulsA',$this->foulsA,'hidden');
        $erg .= dh_input('dh_foulsB',$this->foulsB,'hidden');
        $erg .= dh_input('dh_offsideA',$this->offsideA,'hidden');
        $erg .= dh_input('dh_offsideB',$this->offsideB,'hidden');
      }
      if ($this->stufen['cards']) {
        $erg .= dh_input('dh_yellowA',$this->yellowA,'hidden');
        $erg .= dh_input('dh_yellowB',$this->yellowB,'hidden');
        $erg .= dh_input('dh_yelredA',$this->yelredA,'hidden');
        $erg .= dh_input('dh_yelredB',$this->yelredB,'hidden');
        $erg .= dh_input('dh_redA',$this->redA,'hidden');
        $erg .= dh_input('dh_redB',$this->redB,'hidden');
      }
      $erg .= '<b>'.$this->teamA.' &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$this->teamB.'<br>';
      if ($this->stufen['moreinfo']) {
        $erg .= $this->torschusseA.'</b> Torsch&uuml;sse <b>'.$this->torschusseB.'<br>';
        $erg .= $this->auftorA.'</b> Aufs Tor <b>'.$this->auftorB.'<br>';
        $erg .= $this->penaltyA.'</b> Elfmeter <b>'.$this->penaltyB.'<br>';
        $erg .= $this->freekicksA.'</b> Freist&ouml;sse<b>'.$this->freekicksB.'<br>';
        $erg .= $this->cornersA.'</b> Eckb&auml;lle <b>'.$this->cornersB.'<br>';
        $erg .= $this->foulsA.'</b> Fouls <b>'.$this->foulsB.'<br>';
        $erg .= $this->offsideA.'</b> Abseits <b>'.$this->offsideB.'<br>';
        $erg .= $this->posessionA.'%</b> Ballbesitz <b>'.$this->posessionB.'%</b><br><br>';
      }
      if ($this->stufen['cards']) {
        $erg .= $this->yellowA.'</b> Gelbe Karten <b>'.$this->yellowB.'<br>';
        $erg .= $this->yelredA.'</b> Gelb/Rote Karten <b>'.$this->yelredB.'<br>';
        $erg .= $this->redA.'</b> Rote Karten <b>'.$this->redB.'<br>';
      }
      $erg .= dh_input('dh_change_result','Ergebnis &auml;ndern','submit');
    }
    return $erg;
  }


//#############################################################################
  function form_scorer(){
    if (!$this->stufen['scorer']){return "Eingabe nicht n&ouml;tig!";}
    $i = 0;
    $c_g = $this->goalsA + $this->goalsB;
    $erg.= dh_input('dh_goals_count',$c_g,'hidden');
    if ($this->form_depth<18){
    while ($i<$c_g){
      $erg .= chr(10).'<br>';
      $erg .= $this->input_scorer($i+1);
      $erg .= '';
      $i++;
    }
    $erg .= ($c_g>0)?'':'Es wurden keine Tore verzeichnet.';
    $erg .= '<br>'.dh_input('dh_set_scorers','Zur Torschusseingabe','submit');
    } else {
      while ($i<$c_g){
      $erg .= chr(10).'<br>';
      $erg .= $this->input_scorer($i+1,TRUE);
      $erg .= '';
      $i++;
      }
      $erg .= ($c_g>0)?'':'Es wurden keine Tore verzeichnet.';
      $erg .= '<br>'.dh_input('dh_change_scorers','Torsch&uuml;tzen & Vorlagen &auml;ndern','submit');

    }
    return $erg;
}

//#############################################################################
  function input_scorer($id,$onlyshow=FALSE){
    if (!$onlyshow) {
    $erg .= "<b>Tor $id</b><br>";
    $erg .= dh_input("dh_torh_a$id",'home','radio',($this->goals_arr[$id]['h_a']=='home')?' checked':'').'Heim ';
    $erg .= dh_input("dh_torh_a$id",'away','radio',($this->goals_arr[$id]['h_a']=='away')?' checked':'').'Ausw&auml;rts';
    if ($this->stufen['moreinfo']!=FALSE) {
      $erg .= ' &nbsp;&nbsp;'.dh_input("dh_tormin$id",$this->goals_arr[$id]['min'],'text','size="3" maxlength="3"').'.Minute';
    }
    $erg .='<br>Torschuetze:';
    $erg .= dh_auswahl($this->set_spieler(1,1,1,0),"dh_torspa$id",$this->goals_arr[$id]['spa']);
    $erg .= ' oder '.dh_input("dh_torspt$id",$this->goals_arr[$id]['spt']);
    //$erg .= dh_auswahl_plus_text($this->tor_typen(),$name='dh_tortyp'.$id);
    if ($this->stufen['moreinfo']!=FALSE) {
    	$erg .= ' Art:';
      $erg .= dh_auswahl($this->tor_typen(),"dh_tortyp$id",$this->goals_arr[$id]['typ']);
      $erg .= '<br>Vorlagengeber:'.dh_auswahl($this->set_spieler(1,1,1,1),"dh_torvoa$id",$this->goals_arr[$id]['voa']);
      $erg .= ' oder '.dh_input("dh_torvot$id",$this->goals_arr[$id]['vot']).'<br><br>';
    }
    } else {
      $erg .= dh_input("dh_torh_a$id",$this->goals_arr[$id]['h_a'],'hidden');
      $erg .= dh_input("dh_tormin$id",$this->goals_arr[$id]['min'],'hidden');
      $erg .= dh_input("dh_torspa$id",$this->goals_arr[$id]['spa'],'hidden');
      $erg .= dh_input("dh_torspt$id",$this->goals_arr[$id]['spt'],'hidden');
      $erg .= dh_input("dh_tortyp$id",$this->goals_arr[$id]['typ'],'hidden');
      $erg .= dh_input("dh_torvoa$id",$this->goals_arr[$id]['voa'],'hidden');
      $erg .= dh_input("dh_torvot$id",$this->goals_arr[$id]['vot'],'hidden');
      $erg .= '<br><b>'.$this->goals_arr[$id]['stand'].'</b><br>';
      $erg .= 'Schuetze: '.$this->goals_arr[$id]['schuetze'].' '.$this->goals_arr[$id]['verein'];
      $erg .= ($this->goals_arr[$id]['typ'])?' ('.$this->goals_arr[$id]['typ'].')':'';
      if ($this->stufen['moreinfo']!=FALSE) {
        $erg .= ' in der '.$this->goals_arr[$id]['min'].'.Minute<br>';
        $erg .= ($this->goals_arr[$id]['vorlagengeber']) ? 'Vorlage von '.$this->goals_arr[$id]['vorlagengeber'].'<br>':'-keine Vorlage-<br>';
      }
    }
    return $erg;
  }

//#############################################################################
  function form_cards(){
    if (!$this->stufen['cards']){return "Eingabe nicht n&ouml;tig!";}
    $c_yh = $this->yellowA;
    $c_yrh = $this->yelredA;
    $c_rh = $this->redA;
    $c_ya = $this->yellowB;
    $c_yra = $this->yelredB;
    $c_ra = $this->redB;
    //Heim
    $erg.='<b>'.$this->teamA.':</b><br>';
    //HGelb
    $i = 0;
    while ($i < $c_yh){
      $i++;
      if ($this->form_depth<18){
      $erg.='Gelbe Karte:'.dh_auswahl($this->set_spieler(1,0,1,0),'dh_crd_hya'.$i,$this->cards['h']['y'][$i]['a']);
      $erg.= ' oder '.dh_input('dh_crd_hyt'.$i,$this->cards['h']['y'][$i]['t']);
      $erg.= ' &nbsp;&nbsp; Minute:'.dh_input('dh_crd_hym'.$i,$this->cards['h']['y'][$i]['m'],'text','maxlength="3" size="3"').'<br>';
      } else {
        $erg.=dh_input('dh_crd_hya'.$i,$this->cards['h']['y'][$i]['a'],'hidden');
        $erg.=dh_input('dh_crd_hyt'.$i,$this->cards['h']['y'][$i]['t'],'hidden');
        $erg.=dh_input('dh_crd_hym'.$i,$this->cards['h']['y'][$i]['m'],'hidden');
        $sundera = explode(';',$this->cards['h']['y'][$i]['a']);
        $sundert = explode(';',$this->cards['h']['y'][$i]['t']);
        $erg.='Gelbe Karte: '.$sundera[0].$sundert[0];
        $erg.=' in der '.$this->cards['h']['y'][$i]['m'].'.Minute<br>';
      }
    }
    //HGelbRot
    $i = 0;
    while ($i < $c_yrh){
      $i++;
      if ($this->form_depth<18){
      $erg.='Gelb/Rote Karte:'.dh_auswahl($this->set_spieler(1,0,1,0),'dh_crd_hyra'.$i,$this->cards['h']['yr'][$i]['a']);
      $erg.= ' oder '.dh_input('dh_crd_hyrt'.$i,$this->cards['h']['yr'][$i]['t']);
      $erg.= ' &nbsp;&nbsp; Minute:'.dh_input('dh_crd_hyrm'.$i,$this->cards['h']['yr'][$i]['m'],'text','maxlength="3" size="3"').'<br>';
      } else {
        $erg.=dh_input('dh_crd_hyra'.$i,$this->cards['h']['yr'][$i]['a'],'hidden');
        $erg.=dh_input('dh_crd_hyrt'.$i,$this->cards['h']['yr'][$i]['t'],'hidden');
        $erg.=dh_input('dh_crd_hyrm'.$i,$this->cards['h']['yr'][$i]['m'],'hidden');
        $sundera = explode(';',$this->cards['h']['yr'][$i]['a']);
        $sundert = explode(';',$this->cards['h']['yr'][$i]['t']);
        $erg.='Gelb/Rote Karte: '.$sundera[0].$sundert[0];
        $erg.=' in der '.$this->cards['h']['yr'][$i]['m'].'.Minute<br>';
      }
    }
    //HRot
    $i = 0;
    while ($i < $c_rh){
      $i++;
      if ($this->form_depth<18){
      $erg.='Rote Karte:'.dh_auswahl($this->set_spieler(1,0,1,0),'dh_crd_hra'.$i,$this->cards['h']['r'][$i]['a']);
      $erg.= ' oder '.dh_input('dh_crd_hrt'.$i,$this->cards['h']['r'][$i]['t']);
      $erg.= ' &nbsp;&nbsp; Minute:'.dh_input('dh_crd_hrm'.$i,$this->cards['h']['r'][$i]['m'],'text','maxlength="3" size="3"').'<br>';
      } else {
        $erg.=dh_input('dh_crd_hra'.$i,$this->cards['h']['r'][$i]['a'],'hidden');
        $erg.=dh_input('dh_crd_hrt'.$i,$this->cards['h']['r'][$i]['t'],'hidden');
        $erg.=dh_input('dh_crd_hrm'.$i,$this->cards['h']['r'][$i]['m'],'hidden');
        $sundera = explode(';',$this->cards['h']['r'][$i]['a']);
        $sundert = explode(';',$this->cards['h']['r'][$i]['t']);
        $erg.='Rote Karte: '.$sundera[0].$sundert[0];
        $erg.=' in der '.$this->cards['h']['r'][$i]['m'].'.Minute<br>';
      }
    }
    if ($c_yh+$c_yrh+$c_rh < 1){
      $erg.=$this->teamA.' erhielt keine Karten.<br>';
    }
    //Ausw&auml;rts
    $erg.='<b>'.$this->teamB.':</b><br>';
    //AGelb
    $i = 0;
    while ($i < $c_ya){
      $i++;
      if ($this->form_depth<18){
      $erg.='Gelbe Karte:'.dh_auswahl($this->set_spieler(0,1,1,0),'dh_crd_aya'.$i,$this->cards['a']['y'][$i]['a']);
      $erg.= ' oder '.dh_input('dh_crd_ayt'.$i,$this->cards['a']['y'][$i]['t']);
      $erg.= ' &nbsp;&nbsp; Minute:'.dh_input('dh_crd_aym'.$i,$this->cards['a']['y'][$i]['m'],'text','maxlength="3" size="3"').'<br>';
      } else {
        $erg.=dh_input('dh_crd_aya'.$i,$this->cards['a']['y'][$i]['a'],'hidden');
        $erg.=dh_input('dh_crd_ayt'.$i,$this->cards['a']['y'][$i]['t'],'hidden');
        $erg.=dh_input('dh_crd_aym'.$i,$this->cards['a']['y'][$i]['m'],'hidden');
        $sundera = explode(';',$this->cards['a']['y'][$i]['a']);
        $sundert = explode(';',$this->cards['a']['y'][$i]['t']);
        $erg.='Gelbe Karte: '.$sundera[0].$sundert[0];
        $erg.=' in der '.$this->cards['a']['y'][$i]['m'].'.Minute<br>';
      }
    }
    //AGelbRot
    $i = 0;
    while ($i < $c_yra){
      $i++;
      if ($this->form_depth<18){
      $erg.='Gelb/Rote Karte:'.dh_auswahl($this->set_spieler(0,1,1,0),'dh_crd_ayra'.$i,$this->cards['a']['yr'][$i]['a']);
      $erg.= ' oder '.dh_input('dh_crd_ayrt'.$i,$this->cards['a']['yr'][$i]['t']);
      $erg.= ' &nbsp;&nbsp; Minute:'.dh_input('dh_crd_ayrm'.$i,$this->cards['a']['yr'][$i]['m'],'text','maxlength="3" size="3"').'<br>';
      } else {
        $erg.=dh_input('dh_crd_ayra'.$i,$this->cards['a']['yr'][$i]['a'],'hidden');
        $erg.=dh_input('dh_crd_ayrt'.$i,$this->cards['a']['yr'][$i]['t'],'hidden');
        $erg.=dh_input('dh_crd_ayrm'.$i,$this->cards['a']['yr'][$i]['m'],'hidden');
        $sundera = explode(';',$this->cards['a']['yr'][$i]['a']);
        $sundert = explode(';',$this->cards['a']['yr'][$i]['t']);
        $erg.='Gelb/Rote Karte: '.$sundera[0].$sundert[0];
        $erg.=' in der '.$this->cards['a']['yr'][$i]['m'].'.Minute<br>';
      }
    }
    //ARot
    $i = 0;
    while ($i < $c_ra){
      $i++;
      if ($this->form_depth<18){
      $erg.='Rote Karte:'.dh_auswahl($this->set_spieler(0,1,1,0),'dh_crd_ara'.$i,$this->cards['a']['r'][$i]['a']);
      $erg.= ' oder '.dh_input('dh_crd_art'.$i,$this->cards['a']['r'][$i]['t']);
      $erg.= ' &nbsp;&nbsp; Minute:'.dh_input('dh_crd_arm'.$i,$this->cards['a']['r'][$i]['m'],'text','maxlength="3" size="3"').'<br>';
      } else {
        $erg.=dh_input('dh_crd_ara'.$i,$this->cards['a']['r'][$i]['a'],'hidden');
        $erg.=dh_input('dh_crd_art'.$i,$this->cards['a']['r'][$i]['t'],'hidden');
        $erg.=dh_input('dh_crd_arm'.$i,$this->cards['a']['r'][$i]['m'],'hidden');
        $sundera = explode(';',$this->cards['a']['r'][$i]['a']);
        $sundert = explode(';',$this->cards['a']['r'][$i]['t']);
        $erg.='Rotee Karte: '.$sundera[0].$sundert[0];
        $erg.=' in der '.$this->cards['a']['r'][$i]['m'].'.Minute<br>';
      }
    }
    if ($c_ya+$c_yra+$c_ra < 1){
      $erg.=$this->teamB.' erhielt keine Karten.<br>';
    }
    if ($this->form_depth<18){
      $erg.='<br>'.dh_input('dh_set_scorers','zur Torschusseingabe','submit').'<br>';
      } else {
      $erg.='<br>'.dh_input('dh_change_scorers','Karten &auml;ndern','submit').'<br>';
    }
    return $erg;
  }
//############################################################
  function form_shots(){
    if (!$this->stufen['shots']){return "Eingabe nicht n&ouml;tig!";}
    $erg .= dh_input('dh_torsch_h',$this->torsch_h,'hidden');
    $erg .= dh_input('dh_torsch_a',$this->torsch_a,'hidden');
    $onlyshow = ($this->form_depth < 19);
    $onlyshow = !$onlyshow;
    $erg.='<b>Sch&uuml;tzen '.$this->teamA.'</b><br>';
    $i = 0;
    if ($this->torsch_h < 1){$erg.='<br>-keine Spieler zur Eingabe-<br>';}
    while ($i < $this->torsch_h){
      $i++;
      $erg.= $this->input_shot($i,'h',$onlyshow);
    }
    if (!$onlyshow){
      $erg.= '<br>'.dh_input('dh_add_shooter_h','Weiteren Spieler von '.$this->teamA.' hinzuf&uuml;gen','submit');
      $erg.= ($this->torsch_h > 0) ? dh_input('dh_del_shooter_h','Letzten Spieler  von '.$this->teamA.' entfernen','submit') : '';
      $erg.='<br>';
    }
    $erg.='<br><br><b>Sch&uuml;tzen '.$this->teamB.'</b><br>';
    $i = 0;
    if ($this->torsch_a < 1){$erg.='<br>-keine Spieler zur Eingabe-<br>';}
    while ($i < $this->torsch_a){
      $i++;
      $erg.= $this->input_shot($i,'a',$onlyshow);
    }
    if (!$onlyshow){
      $erg.= '<br>'.dh_input('dh_add_shooter_a','Weiteren Spieler von '.$this->teamB.' hinzuf&uuml;gen','submit');
      $erg.= ($this->torsch_a > 0) ? dh_input('dh_del_shooter_a','Letzten Spieler  von '.$this->teamB.' entfernen','submit') : '';
      $erg.='<br>';
      $erg.='<br>'.dh_input('dh_set_shooter','Zur Berichteingabe','submit');
    } else {
      $erg.= '<br><br>'.dh_input('dh_change_shooter','Torsch&uuml;sse &auml;ndern','submit').'<br>';
    }
    return $erg;
  }


//####################################################
  function input_shot($i,$h_a,$onlyshow=FALSE){

    if (!$onlyshow){
    $erg.= '<br><b>Spieler '.$i.':</b>';
    $erg.= dh_auswahl($this->set_spieler($h_a=='h',$h_a=='a',1,0),'dh_shota'.$h_a.$i,$this->shots[$h_a][$i]['a']);
    $erg.= ' oder ';
    $erg.= dh_input('dh_shott'.$h_a.$i,$this->shots[$h_a][$i]['t']);
    $erg.= '<br>Anzahl Sch&uuml;sse:';
    $erg.= dh_input('dh_shotc'.$h_a.$i,$this->shots[$h_a][$i]['c'],'text','size="2"');
    $erg.= " Minutenangaben (z.B. 14,45,53):";
    $erg.= dh_input('dh_shotm'.$h_a.$i,$this->shots[$h_a][$i]['m']);
    $erg.= '<br>';
    } else {
    $erg.= dh_input('dh_shota'.$h_a.$i,$this->shots[$h_a][$i]['a'],'hidden');
    $erg.= dh_input('dh_shott'.$h_a.$i,$this->shots[$h_a][$i]['t'],'hidden');
    $erg.= dh_input('dh_shotc'.$h_a.$i,$this->shots[$h_a][$i]['c'],'hidden');
    $erg.= dh_input('dh_shotm'.$h_a.$i,$this->shots[$h_a][$i]['m'],'hidden');
    $erg.= $this->shots[$h_a][$i]['a'].$this->shots[$h_a][$i]['t'];
    $erg.= '<i> - '.$this->shots[$h_a][$i]['c'];
    $erg.= ($this->shots[$h_a][$i]['t']==1)?' Torschuss-':' Torsch&uuml;sse -';
    $erg.= ' Minuten:'.$this->shots[$h_a][$i]['m'].'</i><br>';
    }
    return $erg;
  }

  function form_textarea(){
    if (!$this->stufen['bericht']){return "Eingabe nicht n&ouml;tig!";}
    if ($this->form_depth<20){
      $erg.= '<table><tr><td>';
      $erg.= dh_textarea('dh_bericht',$this->bericht,20,60).'<br>';
      $erg.= '</td><td><span class="gensmall"><b>Bei der Eingabe des Berichtes k&ouml;nnen folgende Tags verwendet werden:</b><br><br>';
      $erg.= '[b]fett[/b] -> <b>fett</b><br>';
      $erg.= '[i]kursiv[/i] -> <i>kursiv</i><br>';
      $erg.= '[u]unterstrichen[/u] -> <u>unterstrichen</u><br>';
      $erg.= '[list]Aufz&auml;hlung1[/list][list]Aufz&auml;hlung2[/list] -> <ul>Aufz&auml;hlung 1</ul><ul>Aufz&auml;hlung 2</ul><br>';
      $erg.= 'Zeilenumbr&uuml;che werden erhalten bleiben. Mehrfache Leerzeichen nicht.<br>';
      $erg.= '</span></td></tr></table>';
      $erg.= dh_input('dh_set_text','Bericht eingeben','submit');
    } else {
    $erg.='<hr><br>';
    $erg.= dh_input('dh_bericht',convert_text($this->bericht),'hidden');
    $erg .= convert_text2htmlplus($this->bericht).'<br><hr><br>';
    $erg .= dh_input('dh_change_text','Bericht &auml;ndern','submit');
    }
    return $erg;
  }

//####################################
  function form_submit_all(){
    $erg.= dh_input('dh_submit_all','Spiel und Statistik komplett &uuml;bermitteln.','submit');
    return $erg;
  }

  function form_erfolgreich(){
        $erg = '<tr><th class="thHead" colspan="2">Ergebniseingabe</th></tr>'.chr(13);
        $erg .= '<tr><td class="row1" colspan="2">';
        $erg.='<p>&nbsp;</p><h2>Das Spiel wurde erfolgreich gespeichert.</h2>';
        $erg.='Der Bericht kann unter '.dh_link(_berichtescript_.'liga='.$this->l98.'&st='.$this->spieltag.'&id='.$this->gameid,'diesem Link').' aufgerufen werden.<br>Zudem ist er in der '.dh_link(_lmophp_.'&file='.$this->l98.'&action=results&tabtype=0&st='.$this->spieltag,'Spieltags&uuml;bersicht').' verlinkt.<br><br>'.dh_link(_ergpostscript_,'Neues Spielergebnis eingeben.');
        $erg .= '</td></tr>'.chr(10);

    return $erg;
  }

//#############################################################################
  function form_to_include_in_table(){
     switch ($this->form_depth){
        case 21://uebermittlungsphase
            $erg = $this->form_erfolgreich();
            break;
        case 20://Gesamtansicht
            $erg20 .= '<tr><td class="row2"><span class="gen"><b>Spiel &uuml;bermitteln</b></span></td>';
            $erg20 .= '<td align="left" class="row2"><span class="genmed">';
            $erg20 .= ($this->fehler[21])?'<span class="gensmall"><font color="red">'.$this->fehler[21].'</font></span>':'';
            $erg20 .= '<br>'.$this->form_submit_all().'';
            $erg20 .= '</span></td></tr>';
        case 19://Alle Eingaben korrekt
            $erg19 .= '<tr><th class="thHead" colspan="2">Spielbericht</th></tr>'.chr(13);
            $erg19 .= '<tr><td class="row1" colspan="2"><span class="gensmall">Nachfolgend k&ouml;nnen Sie einen Bericht zum Spiel verfassen.</span></td></tr>'.chr(13);
            $erg19 .= '<tr><td class="row1" colspan="2"><span class="genmed">';
            $erg19 .= ($this->fehler[20])?'<span class="gensmall"><font color="red">'.$this->fehler[20].'</font></span>':'';
            $erg19 .= '<br>'.$this->form_textarea().'';
            $erg19 .= '</span></td></tr>';

        case 18://Torschuesse
            $erg18 .= '<tr><td class="row2"><span class="gen"><b>Torsch&uuml;sse</b></span></td>';
            $erg18 .= '<td align="left" class="row2"><span class="genmed">';
            $erg18 .= ($this->fehler[19])?'<span class="gensmall"><font color="red">'.$this->fehler[19].'</font></span>':'';
            $erg18 .= '<br>'.$this->form_shots().'';
            $erg18 .= '</span></td></tr>';
        case 17://Karten
            $erg17 .= '<tr><td class="row1"><span class="gen"><b>Karten</b></span></td>';
            $erg17 .= '<td align="left" class="row1"><span class="genmed">';
            $erg17 .= ($this->fehler[18])?'<span class="gensmall"><font color="red">'.$this->fehler[18].'</font></span>':'';
            $erg17 .= '<br>'.$this->form_cards().'';
            $erg17 .= '</span></td></tr>';
        case 16://Torschuetzen & Vorlagen
            $erg16 .= '<tr><td class="row2"><span class="gen"><b>Torsch&uuml;tzen und Vorlagen</b></span></td>';
            $erg16 .= '<td align="left" class="row2"><span class="genmed">';
            $erg16 .= ($this->fehler[17])?'<span class="gensmall"><font color="red">'.$this->fehler[17].'</font></span>':'';
            $erg16 .= '<br>'.$this->form_scorer().'';
            $erg16 .= '</span></td></tr>';
        case 15://Teamstats
            $erg15 .= '<tr><td class="row1"><span class="gen"><b>Teamstatistik</b></span></td>';
            $erg15 .= '<td align="left" class="row1"><span class="genmed">';
            $erg15 .= ($this->fehler[16])?'<span class="gensmall"><font color="red">'.$this->fehler[16].'</font></span>':'';
            $erg15 .= '<br>'.$this->form_teamstats().'';
            $erg15 .= '</span></td></tr>';
        case 14://Tore
            $erg14 .= '<tr><td class="row2"><span class="gen"><b>Ergebnis</b></span></td>';
            $erg14 .= '<td class="row2"><span class="genmed">';
            $erg14 .= ($this->fehler[15])?'<span class="gensmall"><font color="red">'.$this->fehler[15].'</font></span>':'';
            $erg14 .= '<br>'.$this->form_result().'';
            $erg14 .= '</span></td></tr>';
        case 13://Datum,Zeit
            $erg13 .= '<tr><td class="row1"><span class="gen"><b>Spielzeitpunkt</b></span></td>';
            $erg13 .= '<td class="row1"><span class="genmed">';
            $erg13 .= ($this->fehler[14])?'<span class="gensmall"><font color="red">'.$this->fehler[14].'</font></span>':'';
            $erg13 .= '<br>'.$this->form_date().'';
			$erg13 .= '</span></td></tr>';
        case 12://Spieldaten
            //Auswahlfeld Spiele
            $erg12 .= '<tr><td class="row2"><span class="gen"><b>Spieldaten</b></span></td>';
            $erg12 .= '<td class="row2"><span class="genmed">';
            $erg12 .= ($this->fehler[13])?$this->fehler[13]:$this->form_game_select();
            $erg12 .= '</span></td></tr>';
        case 11: //Pruefung ob User in Liga
            $erg11 .= ($this->fehler[12])?'<tr><td class="row2" colspan="2"><span class="gensmall">'.$this->fehler[12].'</span></td></tr>':'';
        case 10: //Spieltagsauswahl
            $erg10 .= '<tr><td class="row1"><span class="gen"><b>Spieltag</b></span></td>';
            $erg10 .= '<td class="row1"><span class="genmed">';
            $erg10 .= ($this->fehler[11])?'<span class="gensmall"><font color="red">'.$this->fehler[11].'</font></span>':'';
            $erg10 .= '<br>'.$this->form_spieltag_select().'';
            $erg10 .= '</span></td></tr>';
        case 9:  //Ligaauswahl//Kopf und Erkl?ng
            $erg9 = '<tr><th class="thHead" colspan="2">Spielergebnis eintragen!</th></tr>'.chr(13);
            $erg9 .= '<tr><td class="row1" colspan="2"><span class="gensmall">Im nachfolgenden kann jeder Trainer seine Spielergebnisse eingeben. Bei einer Eingabe muessen die Daten komplett gem&auml;&szlig; Wettbewerbsregeln eingegeben werden.<br></span>';
            $erg9 .= '</td></tr>'.chr(10);
            $erg9 .= '<tr><td class="row2"><span class="gen"><b>Liga</b></span></td>';
            $erg9 .= '<td class="row2"><span class="genmed">';
            $erg9 .= ($this->fehler[10])?'<span class="gensmall"><font color="red">'.$this->fehler[10].'</font></span>':'';
            $erg9 .= '<br>'.$this->form_liga_select().'';
            $erg9 .= '</span></td></tr>';


            $erg1 = $erg9.$erg10.$erg11.$erg12.$erg13.$erg14.$erg15.$erg16.$erg17.$erg18.$erg19.$erg20;
            $erg .= $erg1;

            break;
    }
            return $erg;
  }

  function bericht_ausgabe(){
  	$str.= '[i]Eingabe von '.$this->username.' am '.date_time_german(mktime(),'datetime').'[/i]'.chr(10).chr(10);
   $str.= '[u][b]'.strtoupper($this->liganame).'[/b][/u]'.chr(10);
   $str.= '[b]Spieltag '.$this->spieltag.'[/b]'.chr(10);
   $str.= '[b]'.$this->teamA.'[/b] ('.$this->trainerA.') -  ('.$this->trainerB.') [b]'.$this->teamB.'[/b]'.chr(10);
   $str.= 'Spielzeitpunkt: '.date_time_german($this->sdate,'datetime').chr(10);
   if($this->stufen['result'] == 1)
   	$str.= '[u]Ergebnis:[/u] [b]'.$this->goalsA.' - '.$this->goalsB.'[/b]'.chr(10);
   if($this->stufen['teamstat'] == 1) {
   	//Teamstat
   	$str .= chr(10).'[u][b]Teamstatistiken:[/b][/u]'.chr(10);
   	$str .= '[b]'.$this->teamA.' : '.$this->teamB.'[/b]'.chr(10);
        if ($this->stufen['moreinfo']) {
   	  $str .= 'Torsch&uuml;sse [b]'.$this->torschusseA.' : '.$this->torschusseB.'[/b]'.chr(10);
     	  $str .= 'Aufs Tor [b]'.$this->auftorA.' : '.$this->auftorB.'[/b]'.chr(10);
   	  $str .= 'Elfmeter [b]'.$this->penaltyA.' : '.$this->penaltyB.'[/b]'.chr(10);
   	  $str .= 'Freist&ouml;sse[b]'.$this->freekicksA.' : '.$this->freekicksB.'[/b]'.chr(10);
   	  $str .= 'Eckb&auml;lle [b]'.$this->cornersA.' : '.$this->cornersB.'[/b]'.chr(10);
   	  $str .= 'Fouls [b]'.$this->foulsA.' : '.$this->foulsB.'[/b]'.chr(10);
   	  $str .= 'Abseits [b]'.$this->offsideA.' : '.$this->offsideB.'[/b]'.chr(10);
   	  $str .= 'Ballbesitz [b]'.$this->posessionA.'% : '.$this->posessionB.'%[/b]'.chr(10);
        }
        if ($this->stufen['cards']) {
   	  $str .= 'Gelbe Karten [b]'.$this->yellowA.' : '.$this->yellowB.'[/b]'.chr(10);
   	  $str .= 'Gelb/Rote Karten [b]'.$this->yelredA.' : '.$this->yelredB.'[/b]'.chr(10);
   	  $str .= 'Rote Karten [b]'.$this->redA.' : '.$this->redB.'[/b]'.chr(10);
        }
   }
   if($this->stufen['scorer'] == 1) {
   	//Tore & Vorlagen
   	$str.= chr(10).'[u][b]Tore & Vorlagen:[/b][/u]'.chr(10);
   	$id = 0;
   	$c_g = $this->goalsA + $this->goalsB;
   	while ($id<$c_g){
      	$id++;
      	$str .= '[b]'.$this->goals_arr[$id]['stand'];
      	$str .= ' '.$this->goals_arr[$id]['schuetze'].'[/b] [i]'.$this->goals_arr[$id]['verein'].'[/i]';
      	$str .= ($this->goals_arr[$id]['typ'])?' ('.$this->goals_arr[$id]['typ'].')':'';
      	if ($this->stufen['moreinfo']!=FALSE) {
      	  $str .= ' in der '.$this->goals_arr[$id]['min'].'.Minute'.chr(10);
        	$str .= ($this->goals_arr[$id]['vorlagengeber']) ? ' - Vorlage von '.$this->goals_arr[$id]['vorlagengeber'].chr(10):' - keine Vorlage -'.chr(10);
        }
        $str .= chr(10);
   	}
   	$str .= ($c_g>0)?'':'- Es wurden keine Tore verzeichnet. -'.chr(10);
   }
   if($this->stufen['cards'] == 1) {
   	//Karten
   	$str.= chr(10).'[u][b]S&uuml;nder[/b][/u]'.chr(10);
   	$c_yh = $this->yellowA;
   	$c_yrh = $this->yelredA;
   	$c_rh = $this->redA;
   	$c_ya = $this->yellowB;
   	$c_yra = $this->yelredB;
   	$c_ra = $this->redB;
   	//Heim
   	$str.='[b]'.$this->teamA.':[/b]'.chr(10);
   	//HGelb
   	$i = 0;
   	while ($i < $c_yh){
     		$i++;
     		$str.='Gelbe Karte:'.$this->cards['h']['y'][$i]['a'].$this->cards['h']['y'][$i]['t'];
     		$str.=' in '.$this->cards['h']['y'][$i]['m'].'.Minute'.chr(10);
   	}
   	//HGelb/Rot
   	$i = 0;
   	while ($i < $c_yrh){
     		$i++;
     		$str.='Gelb/Rote Karte:'.$this->cards['h']['yr'][$i]['a'].$this->cards['h']['yr'][$i]['t'];
     		$str.=' in '.$this->cards['h']['yr'][$i]['m'].'.Minute'.chr(10);
   	}
   	//HRot
   	$i = 0;
   	while ($i < $c_ra){
     		$i++;
     		$str.='Rote Karte:'.$this->cards['a']['r'][$i]['a'].$this->cards['a']['r'][$i]['t'];
     		$str.=' in '.$this->cards['a']['r'][$i]['m'].'.Minute'.chr(10);
   	}
   	if ($c_yh+$c_yrh+$c_rh < 1){
      	$str.=$this->teamA.' erhielt keine Karten.'.chr(10);
   	}
   	//Ausw&auml;rts
   	$str.='[b]'.$this->teamB.':[/b]'.chr(10);
   	//AGelb
   	$i = 0;
   	while ($i < $c_ya){
     		$i++;
     		$str.='Gelbe Karte:'.$this->cards['a']['y'][$i]['a'].$this->cards['a']['y'][$i]['t'];
     		$str.=' in '.$this->cards['a']['y'][$i]['m'].'.Minute'.chr(10);
   	}
   	//AGelb/Rot
   	$i = 0;
   	while ($i < $c_yra){
     		$i++;
     		$str.='Gelb/Rote Karte:'.$this->cards['a']['yr'][$i]['a'].$this->cards['a']['yr'][$i]['t'];
     		$str.=' in '.$this->cards['a']['yr'][$i]['m'].'.Minute'.chr(10);
   	}
   	//ARot
   	$i = 0;
   	while ($i < $c_ra){
     		$i++;
     		$str.='Rote Karte:'.$this->cards['a']['r'][$i]['a'].$this->cards['a']['r'][$i]['t'];
     		$str.=' in '.$this->cards['a']['r'][$i]['m'].'.Minute'.chr(10);
   	}
   	if ($c_ya+$c_yra+$c_ra < 1){
      	$str.=$this->teamB.' erhielt keine Karten.'.chr(10);
   	}
   }
   if($this->stufen['shots'] == 1) {
   	//Torsch?   	$str.= chr(10).'[u][b]Torsch?b][/u]'.chr(10);
   	$str.='[b]Sch?'.$this->teamA.'[/b]'.chr(10);
   	$i = 0;
   	if ($this->torsch_h < 1){$str.='-keine Spieler die einen Torschuss abfeuerten-'.chr(10);}
   	while ($i < $this->torsch_h){
     		$i++;
     		$str.= $this->shots['h'][$i]['a'].$this->shots['h'][$i]['t'];
     		$str.= '[i] - '.$this->shots['h'][$i]['c'];
     		$str.= ($this->shots['h'][$i]['t']==1)?' Torschuss -':' Torsch&uuml;sse';
     		$str.= ' Minuten:'.$this->shots['h'][$i]['m'].'[/i]'.chr(10);
   	}
   	$str.='[b]Sch?'.$this->teamB.'[/b]'.chr(10);
   	$i = 0;
   	if ($this->torsch_a < 1){$str.='-keine Spieler die einen Torschuss abfeuerten-'.chr(10);}
   	while ($i < $this->torsch_a){
     		$i++;
     		$str.= $this->shots['a'][$i]['a'].$this->shots['a'][$i]['t'];
     		$str.= '[i] - '.$this->shots['a'][$i]['c'];
     		$str.= ($this->shots['a'][$i]['t']==1)?' Torschuss -':' Torsch&uuml;sse';
     		$str.= ' Minuten:'.$this->shots['a'][$i]['m'].'[/i]'.chr(10);
   	}
   }
   if($this->stufen['bericht'] == 1) {
   	$str.= chr(10).'[b][u]Spielbericht von '.$this->username.':[/u][/b]'.chr(10);
   	$str.= $this->bericht;
   	$this->bigtemp['bericht'] = $str;
   }
   return $str;
  }

    function submit_all(){
      //Speichern des Berichts
      $strl = strlen($this->bericht_ausgabe());
      $err = save_str2file(_berichtedir_.$this->l98.'-'.$this->spieltag.'-'.$this->gameid,$this->bericht_ausgabe());
      /*$err += save_str2file(_copysavedir_.$this->l98.'-'.$this->spieltag.'-'.$this->gameid.'-'.time(),$this->bericht_ausgabe());*/
      if ($err != $strl){$this->fehler[21].='Der Bericht konnte nicht erfolgreich gespeichert werden.<br>';}

      //Speichern der Teamstats
      $arr = $this->teamstat_ausgabe_array();
      $str = $this->arr2ini($arr);
      $err = save_str2file(_teamstatsdir_.$this->l98.'.teams',$str);
      if ($err != strlen($str)){$this->fehler[21].='Die Teamstatistiken konnten nicht erfolgreich gespeichert werden.<br>';}

      //Speichern der Spielerstats
      $arr = $this->playerstats_ausgabe();
      $str = $this->arr2ini($arr);
      $err = save_str2file(_spielerdir_.$this->l98.'.players',$str);
      if ($err != strlen($str)){$this->fehler[21].='Die Spielerstatistiken konnten nicht erfolgreich gespeichert werden.<br>';}

      //Speichern der LMO-Daten
      $lmo = $this->lmodats_ausgabe();
      $str = $this->arr2ini($lmo['temp']);
      $err = save_str2file(_lmoligentempdir_.$this->l98.'.temp',$str);
      if ($err != strlen($str)){$this->fehler[21].='Die LMO-Temp-Daten konnten nicht erfolgreich gespeichert werden.<br>';}

      $str = $this->arr2ini($lmo['cur']);
      //echo $str;
      $err = save_str2file(_lmoligendir_.$this->l98,$str);
      if ($err != strlen($str)){$this->fehler[21].='Die LMO-Orig-Daten konnten nicht erfolgreich gespeichert werden.<br>';}
      //Speichern der TEMP-Datei mit allen 4 Teilen
      $str ='';
      $str .= '::LMO::'.chr(10);
      $str .= $this->arr2ini($this->bigtemp['lmo']);
      $str .= chr(10).'::TEAMSTAT::'.chr(10);
      $str .= $this->arr2ini($this->bigtemp['teamstats']);
      $str .= chr(10).'::SCORER & KARTEN::'.chr(10);
      $str .= $this->arr2ini($this->bigtemp['scorer']);
      $str .= chr(10).'::BERICHT::'.chr(10);
      $str .= $this->bigtemp['bericht'];
      $err = save_str2file(_copysavedir_.$this->l98.'-'.$this->spieltag.'-'.$this->gameid.'-'.time(),$str);
      if ($err != strlen($str)){$this->fehler[21].='Die Tempor&auml;ren Dateien konnten nicht erfolgreich gespeichert werden.<br>';}


	  //Momentan g? Spielerststatistikauswahl
	  $spielerstatopt = array(	'score'=>$this->stufen['scorer'],
	  							'cards'=>$this->stufen['cards'],
								'shots'=>$this->stufen['shots']);

     //Erstellen der Spielerstatistik
      write_spielerstats($this->l98,$spielerstatopt);

	 //Speichern der allgames.txt
	 $str=$this->allgames_txt_ausgabe();
     $err = save_str2file(_allgamestxt_,$str);
      if ($err != strlen($str)){$this->fehler[21].='Die allgames-Datei konnte nicht aktualisiert werden. Bitte wende Dich deshalb an einen Admin.<br>';}
      //Aktualsieren der LMO HTML-Dateien, z.B. Minitabelle
      include dirname(__file__).'/../lmo/lmo-savehtml.php';
      include dirname(__file__).'/../lmo/addon/mini/lmo-minitab.php';
    }

     function teamstat_ausgabe_array(){
       $secA = $this->teamA;
       $secB = $this->teamB;
       $arr[$secA]['VER'] = $this->teamA;
       $arr[$secA]['ID'] = $this->TA;
       $arr[$secA]['HA'.$this->spieltag] = 'H';
       $arr[$secA]['OID'.$this->spieltag] = $this->TB;
       $arr[$secA]['STR'.$this->spieltag] = $this->trainerA;
       $arr[$secA]['OTR'.$this->spieltag] = $this->trainerB;
       $arr[$secA]['ONM'.$this->spieltag] = $this->teamB;
       $arr[$secA]['GID'.$this->spieltag] = $this->gameid;
       $arr[$secA]['GCS'.$this->spieltag] = $this->goalsA;
       $arr[$secA]['GCO'.$this->spieltag] = $this->goalsB;
       $arr[$secA]['SHS'.$this->spieltag] = $this->torschusseA;
       $arr[$secA]['SHO'.$this->spieltag] = $this->torschusseB;
       $arr[$secA]['SGS'.$this->spieltag] = $this->auftorA;
       $arr[$secA]['SGO'.$this->spieltag] = $this->auftorB;
       $arr[$secA]['EMS'.$this->spieltag] = $this->penaltyA;
       $arr[$secA]['EMO'.$this->spieltag] = $this->penaltyB;
       $arr[$secA]['FSS'.$this->spieltag] = $this->freekicksA;
       $arr[$secA]['FSO'.$this->spieltag] = $this->freekicksB;
       $arr[$secA]['COS'.$this->spieltag] = $this->cornersA;
       $arr[$secA]['COO'.$this->spieltag] = $this->cornersB;
       $arr[$secA]['FOS'.$this->spieltag] = $this->foulsA;
       $arr[$secA]['FOO'.$this->spieltag] = $this->foulsB;
       $arr[$secA]['OFS'.$this->spieltag] = $this->offsideA;
       $arr[$secA]['OFO'.$this->spieltag] = $this->offsideB;
       $arr[$secA]['YCS'.$this->spieltag] = $this->yellowA;
       $arr[$secA]['YCO'.$this->spieltag] = $this->yellowB;
       $arr[$secA]['YRS'.$this->spieltag] = $this->yelredA;
       $arr[$secA]['YRO'.$this->spieltag] = $this->yelredB;
       $arr[$secA]['RCS'.$this->spieltag] = $this->redA;
       $arr[$secA]['RCO'.$this->spieltag] = $this->redB;
       $arr[$secA]['POS'.$this->spieltag] = $this->posessionA;
       $arr[$secA]['POO'.$this->spieltag] = $this->posessionB;
       //$arr[$secA]['ETS'.$this->spieltag] = $this->
       //$arr[$secA]['ETO'.$this->spieltag] = $this->
       $arr[$secB]['VER'] = $this->teamB;
       $arr[$secA]['ID'] = $this->TB;
       $arr[$secB]['HA'.$this->spieltag] = 'A';
       $arr[$secB]['OID'.$this->spieltag] = $this->TA;
       $arr[$secB]['STR'.$this->spieltag] = $this->trainerB;
       $arr[$secB]['OTR'.$this->spieltag] = $this->trainerA;
       $arr[$secB]['ONM'.$this->spieltag] = $this->teamA;
       $arr[$secB]['GID'.$this->spieltag] = $this->gameid;
       $arr[$secB]['GCS'.$this->spieltag] = $this->goalsB;
       $arr[$secB]['GCO'.$this->spieltag] = $this->goalsA;
       $arr[$secB]['SHS'.$this->spieltag] = $this->torschusseB;
       $arr[$secB]['SHO'.$this->spieltag] = $this->torschusseA;
       $arr[$secB]['SGS'.$this->spieltag] = $this->auftorB;
       $arr[$secB]['SGO'.$this->spieltag] = $this->auftorA;
       $arr[$secB]['EMS'.$this->spieltag] = $this->penaltyB;
       $arr[$secB]['EMO'.$this->spieltag] = $this->penaltyA;
       $arr[$secB]['FSS'.$this->spieltag] = $this->freekicksB;
       $arr[$secB]['FSO'.$this->spieltag] = $this->freekicksA;
       $arr[$secB]['COS'.$this->spieltag] = $this->cornersB;
       $arr[$secB]['COO'.$this->spieltag] = $this->cornersA;
       $arr[$secB]['FOS'.$this->spieltag] = $this->foulsB;
       $arr[$secB]['FOO'.$this->spieltag] = $this->foulsA;
       $arr[$secB]['OFS'.$this->spieltag] = $this->offsideB;
       $arr[$secB]['OFO'.$this->spieltag] = $this->offsideA;
       $arr[$secB]['YCS'.$this->spieltag] = $this->yellowB;
       $arr[$secB]['YCO'.$this->spieltag] = $this->yellowA;
       $arr[$secB]['YRS'.$this->spieltag] = $this->yelredB;
       $arr[$secB]['YRO'.$this->spieltag] = $this->yelredA;
       $arr[$secB]['RCS'.$this->spieltag] = $this->redB;
       $arr[$secB]['RCO'.$this->spieltag] = $this->redA;
       $arr[$secB]['POS'.$this->spieltag] = $this->posessionB;
       $arr[$secB]['POO'.$this->spieltag] = $this->posessionA;
       //$arr[$secA]['ETS'.$this->spieltag] = $this->
       //$arr[$secA]['ETO'.$this->spieltag] = $this->
       $this->bigtemp['teamstats'] = $arr;

       $arr_2=$this->get_data($arr,TRUE,TRUE,TRUE);
       $orig = $this->all_teamstats;
       $this->write_arr($orig,$arr_2,TRUE,TRUE);
       return $orig;
     }

     function playerstats_ausgabe(){
       //Alle Spieler der Teams werden vorerst f? Spieltag auf Null gesetzt
        foreach (array_merge($this->spieler['H'],$this->spieler['A']) as $player){
            $tmppl = explode(';',$player);
            $arr[$player]['NAM'] = $tmppl[0];
            $arr[$player]['VER'] = $tmppl[1];
            $arr[$player]['GC'.$this->spieltag] = 0;
            $arr[$player]['GM'.$this->spieltag] = '';
            $arr[$player]['FS'.$this->spieltag] = 0;
            $arr[$player]['EM'.$this->spieltag] = 0;
            $arr[$player]['ET'.$this->spieltag] = 0;
            $arr[$player]['VC'.$this->spieltag] = 0;
            $arr[$player]['VM'.$this->spieltag] = '';
            $arr[$player]['SC'.$this->spieltag] = 0;
            $arr[$player]['SM'.$this->spieltag] = '';
            $arr[$player]['CC'.$this->spieltag] = 0;
            $arr[$player]['CM'.$this->spieltag] = '';
        }
        $i = 0;
        $c_g = $this->goalsA + $this->goalsB;
        while ($i<$c_g){
            $i++;
            //Sch? Vorlagen
            $player = $this->goals_arr[$i]['spa'].$this->goals_arr[$i]['spt'];
            $arr[$player]['NAM'] = $this->goals_arr[$i]['schuetze'];
            $arr[$player]['VER'] = $this->goals_arr[$i]['verein'];
            switch($this->goals_arr[$i]['typ']){
              case 'ET':
                $arr[$player]['ET'.$this->spieltag]++;
                break;
              case 'FS':
                $arr[$player]['FS'.$this->spieltag]++;
              case 'EM':
                $arr[$player]['EM'.$this->spieltag]++;
              default:
                $arr[$player]['GC'.$this->spieltag]++;
            }
            $arr[$player]['GM'.$this->spieltag] .= ($arr[$player]['GM'.$this->spieltag])?',':'';
            $arr[$player]['GM'.$this->spieltag] .= $this->goals_arr[$i]['min'].$this->goals_arr[$i]['typ'];
            if ($this->stufen['moreinfo']!=FALSE) {
              if ($this->goals_arr[$i]['voa'] != -1){
                $vorlgg = $this->goals_arr[$i]['voa'].$this->goals_arr[$i]['vot'];
                $arr[$vorlgg]['NAM'] = $this->goals_arr[$i]['vorlagengeber'];
                $arr[$vorlgg]['VER'] = $this->goals_arr[$i]['verein'];
                $arr[$vorlgg]['VC'.$this->spieltag]++;
                $arr[$vorlgg]['VM'.$this->spieltag] .= ($arr[$vorlgg]['VM'.$this->spieltag])?',':'';
                $arr[$vorlgg]['VM'.$this->spieltag] .= $this->goals_arr[$i]['min'];
              }
            }
        }
        //Torsch?       $i = 0;
        while ($i < $this->torsch_h){
            $i++;
            $shooter = $this->shots['h'][$i]['a'].$this->shots['h'][$i]['t'];
            $shtmp = explode(';',$shooter);
            $arr[$shooter]['NAM'] = $shtmp[0];
            $arr[$shooter]['VER'] = $shtmp[1];
            $arr[$shooter]['SC'.$this->spieltag] += $this->shots['h'][$i]['c'];
            $arr[$shooter]['SM'.$this->spieltag] .= ($arr[$shooter]['SM'.$this->spieltag])?',':'';
            $arr[$shooter]['SM'.$this->spieltag] .= $this->shots['h'][$i]['m'];
        }
        $i  = 0;
        while ($i < $this->torsch_a){
            $i++;
            $shooter = $this->shots['a'][$i]['a'].$this->shots['a'][$i]['t'];
            $shtmp = explode(';',$shooter);
            $arr[$shooter]['NAM'] = $shtmp[0];
            $arr[$shooter]['VER'] = $shtmp[1];
            $arr[$shooter]['SC'.$this->spieltag] += $this->shots['a'][$i]['c'];
            $arr[$shooter]['SM'.$this->spieltag] .= ($arr[$shooter]['SM'.$this->spieltag])?',':'';
            $arr[$shooter]['SM'.$this->spieltag] .= $this->shots['a'][$i]['m'];
        }
        //Karten
        $c_yh = $this->yellowA;
        $c_yrh = $this->yelredA;
        $c_rh = $this->redA;
        $c_ya = $this->yellowB;
        $c_yra = $this->yelredB;
        $c_ra = $this->redB;
        //Heim
        //HGelb
        $i = 0;
        while ($i < $c_yh){
            $i++;
            $sunder = $this->cards['h']['y'][$i]['a'].$this->cards['h']['y'][$i]['t'];
            $sutmp = explode(';',$sunder);
            $arr[$sunder]['NAM'] = $sutmp[0];
            $arr[$sunder]['VER'] = $sutmp[1];
            $arr[$sunder]['CC'.$this->spieltag] = 'G';
            $arr[$sunder]['CM'.$this->spieltag] = $this->cards['h']['y'][$i]['m'];
        }
         $i = 0;
        while ($i < $c_yrh){
            $i++;
            $sunder = $this->cards['h']['yr'][$i]['a'].$this->cards['h']['yr'][$i]['t'];
            $sutmp = explode(';',$sunder);
            $arr[$sunder]['NAM'] = $sutmp[0];
            $arr[$sunder]['VER'] = $sutmp[1];
            $arr[$sunder]['CC'.$this->spieltag] = 'GR';
            $arr[$sunder]['CM'.$this->spieltag] = $this->cards['h']['yr'][$i]['m'];
        }
         $i = 0;
        while ($i < $c_rh){
            $i++;
            $sunder = $this->cards['h']['r'][$i]['a'].$this->cards['h']['r'][$i]['t'];
            $sutmp = explode(';',$sunder);
            $arr[$sunder]['NAM'] = $sutmp[0];
            $arr[$sunder]['VER'] = $sutmp[1];
            $arr[$sunder]['CC'.$this->spieltag] = 'R';
            $arr[$sunder]['CM'.$this->spieltag] = $this->cards['h']['r'][$i]['m'];
        }
        $i = 0;
        while ($i < $c_ya){
            $i++;
            $sunder = $this->cards['a']['y'][$i]['a'].$this->cards['a']['y'][$i]['t'];
            $sutmp = explode(';',$sunder);
            $arr[$sunder]['NAM'] = $sutmp[0];
            $arr[$sunder]['VER'] = $sutmp[1];
            $arr[$sunder]['CC'.$this->spieltag] = 'G';
            $arr[$sunder]['CM'.$this->spieltag] = $this->cards['a']['y'][$i]['m'];
        }
        $i = 0;
        while ($i < $c_yra){
            $i++;
            $sunder = $this->cards['a']['yr'][$i]['a'].$this->cards['a']['yr'][$i]['t'];
            $sutmp = explode(';',$sunder);
            $arr[$sunder]['NAM'] = $sutmp[0];
            $arr[$sunder]['VER'] = $sutmp[1];
            $arr[$sunder]['CC'.$this->spieltag] = 'GR';
            $arr[$sunder]['CM'.$this->spieltag] = $this->cards['a']['yr'][$i]['m'];
        }
        $i = 0;
        while ($i < $c_ra){
            $i++;
            $sunder = $this->cards['a']['r'][$i]['a'].$this->cards['a']['r'][$i]['t'];
            $sutmp = explode(';',$sunder);
            $arr[$sunder]['NAM'] = $sutmp[0];
            $arr[$sunder]['VER'] = $sutmp[1];
            $arr[$sunder]['CC'.$this->spieltag] = 'R';
            $arr[$sunder]['CM'.$this->spieltag] = $this->cards['a']['r'][$i]['m'];
        }

        //Array der vorzunehmenden ?erungen in Quellarray integrieren
        $this->bigtemp['scorer']=$arr;
        $arr_2=$this->get_data($arr,TRUE,TRUE,TRUE);
        $orig = $this->all_spieler;
        $this->write_arr($orig,$arr_2,TRUE,TRUE);
        return $orig;
      }

      function lmodats_ausgabe(){
        $sec = 'Round'.$this->spieltag;
        $arr[$sec]['TA'.$this->gameid] = $this->TA;
        $arr[$sec]['TB'.$this->gameid] = $this->TB;
        $arr[$sec]['GA'.$this->gameid] = $this->goalsA;
        $arr[$sec]['GB'.$this->gameid] = $this->goalsB;
        $arr[$sec]['TI'.$this->gameid] = 0;
        $arr[$sec]['BE'.$this->gameid] = _berichtescript_.'liga='.$this->l98.'&st='.$this->spieltag.'&id='.$this->gameid;
        $arr[$sec]['AT'.$this->gameid] = $this->sdate;

        $this->bigtemp['lmo'] = $arr;
        //Array der vorzunehmenden ?erungen in Quellarray integrieren
        $arr_2=$this->get_data($arr,TRUE,TRUE,TRUE);
        //Tempor? Datei
        $lmotemp = $this->ReadIniCompleteInArr(_lmoligentempdir_.$this->l98.'.temp');
        $this->write_arr($lmotemp,$arr_2,TRUE,TRUE);
        $erg['temp'] = $lmotemp;
        //Originale
        $lmocur = $this->current;
        $this->write_arr($lmocur,$arr_2,TRUE,TRUE);
        $erg['cur'] = $lmocur;
        return $erg;

      }
      //allgames.txt hat folgendes Format: liga;st;nr;teamA;teamB;trainerA;trainerB;goalsA;goalsB
	  //Array hat liga-st-nr=>array(liga,st,nr,teamA,teamB,trainerA,trainerB,goalsA,goalsB)
      function allgames_txt_ausgabe(){
	       $arr = array();
	       $arr0 = array();
			if (file_exists(_allgamestxt_)){
			 $arr0 = file(_allgamestxt_);
			 while (list($k,$str) = each($arr0)){
			  if (substr($str,0,strlen($this->l98.';'.$this->spieltag.';'.$this->gameid))!=$this->l98.';'.$this->spieltag.';'.$this->gameid){
				$arr[]=trim($str);
			  }
		   }
		 }
		$arr[]=implode(";",array($this->l98,$this->spieltag,$this->gameid,$this->teamA,$this->teamB,$this->trainerA,$this->trainerB,$this->goalsA,$this->goalsB,$this->liganame));
		return implode(chr(10),$arr);
	  }
}

/*Um abzubrechen bevor abgesendet wird.
if(isset($_POST['post'])){
  unset($_POST['post']);
  unset($submit);
  $preview = TRUE;
  $refresh = TRUE;
}*/

?>