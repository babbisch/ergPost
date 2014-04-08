<?php

include_once(dirname(__file__)."/../php/dh_ini.class.inc");
include_once(dirname(__file__)."/../php/dh_func.init.inc");

//define('_lmospielerstatdir_',"$dir/../lmo/addon/spieler/stats/");
define('_lmospielerstatdir_',dirname(__file__)."/../lmo/addon/spieler/stats/");
define('_spielerstatssource_',dirname(__file__)."/spieler/");

define('_valSep_','§');


function write_spielerstats($l98,$sso){
 //(array) $sso : spielerstatoptions
 	if (!is_array($sso)){$sso=array('all'=>1);}
    $arr = array();
    $ini = new DH_INI(_spielerstatssource_.$l98.'.players');
    $secs = $ini->get_data_original(TRUE,FALSE,FALSE);
    //print_array($ini->get_data_original(TRUE,TRUE,TRUE));
    $addtitle = array();
    
	$addtitle = array();
    $addtitle['name']='Name';
    $addtitle['verein']='Verein';
    if ($sso['all']== TRUE OR $sso['score']== TRUE){$addtitle['tore']='Tore';}
    if ($sso['all']== TRUE OR $sso['score']== TRUE){$addtitle['vorlagen']='Vorlagen';}
    if ($sso['all']== TRUE OR $sso['shots']== TRUE){$addtitle['schuesse']='Torschüße';}
    if ($sso['all']== TRUE OR $sso['score']== TRUE){$addtitle['score']='Score';}
    if ($sso['all']== TRUE OR $sso['shots']== TRUE){$addtitle['torquote']='Torquote (%)';}
    if ($sso['all']== TRUE OR $sso['cards']== TRUE){$addtitle['gelb']='Gelb';}
    if ($sso['all']== TRUE OR $sso['cards']== TRUE){$addtitle['gelbrot']='GelbRot';}
    if ($sso['all']== TRUE OR $sso['cards']== TRUE){$addtitle['rot']='Rot';}
    if ($sso['all']== TRUE OR $sso['cards']== TRUE){$addtitle['fairness']='Fairness';}
    if ($sso['all']== TRUE OR $sso['score']== TRUE){$addtitle['freistoss']='Freistoßtore';}
    if ($sso['all']== TRUE OR $sso['score']== TRUE){$addtitle['elfmeter']='Elfmetertore';}
    if ($sso['all']== TRUE OR $sso['score']== TRUE){$addtitle['eigentore']='Eigentore';}
    /*$addline1 = array(
                'name'=>0,
                'verein'=>0,
                'tore'=>0,
                'vorlagen'=>0,
                'schuesse'=>0,
                'score'=>0,
                'torquote'=>0,
                'gelb'=>0,
                'gelbrot'=>0,
                'rot'=>0,
                'fairness'=>0,
                'freistoss'=>0,
                'elfmeter'=>0,
                'eigentore'=>0
                );*/ //needed for formulas
    $arr[]=$addtitle;
    //$arr[]=$addline1;
    $arr_rf = array_keys($addtitle);
    foreach($secs as $secarr){
        $add = array();
        $sec = $secarr['section'];
        $add['spid'] = $sec['section'];
        if($addtitle['name']!=FALSE){$add['name'] = $ini->get_val_o($sec,'NAM');}
        if($addtitle['verein']!=FALSE){$add['verein'] = $ini->get_val_o($sec,'VER');}
        if($addtitle['tore']!=FALSE){$add['tore'] = summe_ueber_item($ini,'GC',$sec);}
        if($addtitle['vorlagen']!=FALSE){$add['vorlagen'] = summe_ueber_item($ini,'VC',$sec);}
        if($addtitle['schuesse']!=FALSE){$add['schuesse'] = summe_ueber_item($ini,'SC',$sec);}
        if($addtitle['freistoss']!=FALSE){$add['freistoss'] = summe_ueber_item($ini,'FS',$sec);}
        if($addtitle['elfmeter']!=FALSE){$add['elfmeter'] = summe_ueber_item($ini,'EM',$sec);}
        if($addtitle['eigentore']!=FALSE){$add['eigentore'] = summe_ueber_item($ini,'ET',$sec);}
        if($addtitle['gelb']!=FALSE){$add['gelb'] = count_item_with($ini,'G','CC',$sec);}
        if($addtitle['gelbrot']!=FALSE){$add['gelbrot'] = count_item_with($ini,'GR','CC',$sec);}
        if($addtitle['rot']!=FALSE){$add['rot'] = count_item_with($ini,'R','CC',$sec);}
        if($addtitle['score']!=FALSE){$add['score'] = $add['tore'] + $add['vorlagen'];}
        if($addtitle['torquote']!=FALSE){$add['torquote'] = round(($add['schuesse']<=$add['tore'])? 0 : ($add['tore']/$add['schuesse']),2)*100;}
        if($addtitle['fairness']!=FALSE){$add['fairness'] = -($add['gelb'] + $add['gelbrot']*2 + $add['rot']*3);}
        $arr[]= $add;
    }
    $erg = "";
    foreach ($arr as $line){
      $addline = "";
      foreach ($arr_rf as $key){
        if ($addline !=''){$addline .= _valSep_;}
        $addline .= $line[$key];
      }
      $erg.= $addline.chr(10);
    }
    $filetemp = explode('.l98',$l98);
    unset($filetemp[count($filetemp)-1]);
    $file = implode('.l98',$filetemp);
    save_str2file(_lmospielerstatdir_.$file.'.stat',$erg);
}

function summe_ueber_item(&$ini_ref,$itembeginn,$section){
  $arr = $ini_ref->get_data_original($section,array('1'=>array($itembeginn)),TRUE);
  $sum = 0;
  foreach ($arr as $el){
    $sum += $el['value'];
  }
  return $sum;
}

function count_item_with($ini_ref,$vgl,$itembeginn,$section){
  $arr = $ini_ref->get_data_original($section,array(1=>array($itembeginn)),TRUE);
  $count = 0;
  foreach ($arr as $el){
    if ($el['value']== $vgl){
      $count++;
    }
  }
  return $count;
}



/*
CC1=G Karte
CM1=75
GC1=0  Tore
GM1=
FS1=0  Freistoßtore
EM1=0  Elfmetertore
ET1=0  Eigentore
VC1=0  Vorlagen
VM1=
SC1=0  Schüße
SM1=
*/


?>
