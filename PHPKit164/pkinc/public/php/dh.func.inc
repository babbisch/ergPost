<?php

include_once('dh_func.init.inc');
//ARRAY//////////////////////////////////////////////////////////////////
// Gibt ein Array visuell aus
function print_array($arr,$LayerDepthSgn='==>',$sepNewKey='<br>',$sepVal=':',$endVal='',$keyQ=true,$i=1) {
  if (is_array($arr)){
    while (list ($key, $val) = each ($arr)){
      echo $sepNewKey.str_repeat($LayerDepthSgn,$i);
      echo ($keyQ)?$key.$sepVal:'';
      print_array($val,$LayerDepthSgn,$sepNewKey,$sepVal,$endVal,$keyQ,$i+1);
    }
  } else {
    echo $arr.$endVal;
  }
  
}

//Gibt einen String eines Arrays aus
//Bei mehrdimensionalen Arrays, kann glue maximal zwei-dimensionales Array sein.
//$glue 1dimensional: einem Wert der Ebene X wird der String $arr[X] angehängt
//$glue 2dimensional: einem Wert der Ebene X wird der String $arr[X][1] vorangestellt, der String $arr[X][0] angehängt
//$glue kein Array, dann wird jedem auszugebenden Wert $glue angehängt
//Falls $key = TRUE, werden auch die Schlüssel in den String vor den Wert geschrieben
// Falls $key ein Array dann wird für die Ebene X der Wert geschrieben falls $key[X] wahr ist
//für $glue_key gilt das gleiche wie für $glue, nur werden die Strings auf die Schlüssel angewandt.
//$ebene darf nicht angegeben werden

function arr2str($arr,$glue='',$glue_key='',$key=FALSE,$val=TRUE,$ebene=0,$dg=0,$dgk=0){
   return arr2str_help($arr,$glue,$glue_key,$key,$val,$ebene,$dg,$dgk);
}


function arr2str_help($arr,$glue,$glue_key,$key,$val,$ebene,&$dg,&$dgk){
  //Dimensionsprüfung
  if (!$ebene) { //Falls Dimension von glue und glue_key schon geprüft wurden, werden sie nicht mehr geprüft
    $dg = dim_of_array($glue);
    $dgk = dim_of_array($glue_key);
  }
  if ($dg > 2){ //Falls Dimension von glue >2 dann glue ungültig
    $dg = 0;
    $glue = '';
  }
  if ($dgk > 2){//Falls Dimension von glue_key >2 dann glue ungültig
    $dgk = 0;
    $glue_key='';
  }
  //key & val Ebene(n)
  if (!is_array($key)){$key = array($key);}
  if (!is_array($val)){$val = array($val);}
  
  //DP Ende
  if (is_array($arr)){
    $ebene++;
    $kQ = (in_array(TRUE,$key,TRUE) OR in_array($ebene,$key,TRUE));
    while(list($k,$el) = each($arr)){
      if ($kQ){
        switch ($dgk){
          case 0:
            $erg .= $k.$glue_key;
            break;
          case 1:
            $erg .= $k.$glue_key[$ebene];
            break;
          case 2:
            $at = (is_array($glue_key[$ebene])) ? $glue_key[$ebene] : array($glue_key[$ebene]);
            $erg .= $at[1].$k.$at[0];
            break;
        }
      }
      $erg .= arr2str_help($el,$glue,$glue_key,$key,$val,$ebene,$dg,$dgk);
    }
  } else {
    if (in_array(TRUE,$val,TRUE) OR in_array($ebene,$val,TRUE)){
      switch ($dg){
        case 0:
          $erg .= $arr.$glue;
          break;
        case 1:
          $erg .= $arr.$glue[$ebene];
          break;
        case 2:
          $at = (is_array($glue[$ebene])) ? $glue[$ebene] : array($glue[$ebene]);
          $erg .= $at[1].$arr.$at[0];
          break;
      }
    }
  }
  return $erg;
}

//Gibt die Dimension eines Arrays aus (maxArr darf nicht angegeben werden), leere Werte werden ignoriert
function dim_of_array($arr,$maxDim=0){
  $a = $maxDim;
  if (is_array($arr)){
    $a++;
    while(list($key,$el)= each($arr)){
      $dim = dim_of_array($el,$a);
      if ($maxDim < $dim) {
        $maxDim = $dim;
      }
    }
  }
  return $maxDim*1;
}

//Holt eine Datei in ein Array
//Schlüsseldefinition in Zeile 1
//Ebene 1: neue Zeile ==> neuer Wert | in Ebene 2: durch ";" getrennt ==> neuer Wert | Ebene 3: falls Schlüssel in list2arr: durch "," getrennt ==> neuer Wert
//$file = Dateipfad (wenn globalvar = FALSE) oder Veriablenname der Variable die den Dateipfad enthält (wenn globalvar = TRUE 'standard' )
//$globalvar, bei TRUE stellt $file den Variablennamen einer globalen Variablen dar, der den Dateipfad der zu lesenden Datei enthält, bei FALSE stellt $file direkt den Dateipfad dar
//$list2arr falls in Ebene einer der Schlüssel definiert wird der in der kommagetrennten Liste list2arr auftritt, wird der Wert als kommagetrennte Liste in ein Array geschrieben
function DH_catch_file($file='DH_TERMINE',$globalvar=true,$list2arr='groups,lfnr'){
    if ($globalvar) {
        GLOBAL $$file;
        $file = $$file;
    }
    $val2arr = explode(',',$list2arr);
    $arr = file($file);
    while (list($key, $val) = each($arr)){
      $val = trim($val);
      $arr[$key] = explode(';',$val);
      while (list($key2,$val2)= each($arr[$key])) {
        if (!is_array($arr[0][$key2])) {
            $key_temp = ''.$arr[0][$key2];
            $arr[$key][$key_temp] = $arr[$key][$key2];
            if ($list2arr!="" AND in_array($key_temp,$val2arr)) {
                $arr[$key][$key_temp] = explode(',',$arr[$key][$key_temp]);
            }
        }
      }
    }
    reset($arr);
    return $arr;
}

//Prüft ob $val entweder $vglVal entspricht oder falls is_array im Wert von Schlüssel $keySearch den Wert $vglwert hat oder falls dieser wiederum array, dieses $vglwert enthält (falls schlüssel ="" werden alle schlüssel durchsucht) und gibt falls zuftreffend den $val als Wert in AusgabeArr
function proofVal($val,$key="",$userdata){
    $vglVal=&$userdata[0];
    $keySearch=&$userdata[1];
    $ausgabeArr=&$userdata[2];
    $gleich=&$userdata[3];
    $erg = false;
    if (is_array($val)){
      if ($keySearch){
        if (array_key_exists($keySearch,$val)){
          if (is_array($val[$keySearch])) {
            $erg = (in_array($vglVal,$val[$keySearch])) ? TRUE : FALSE;
          } else {
            $erg = ($vglVal == $val[$keySearch]) ? TRUE : FALSE;
          }
        }
      } else {
        if (in_array($vglVal,$val)) {
          $erg = TRUE;
        } else {
          while (list($k,$v) = each($val) AND $erg = FALSE) {
            $erg = (is_array($v) AND in_array($vglVal,$v)) ? TRUE : FALSE;
          }
        }
      }
    } else {
      $erg = ($val == $vglVal) ? TRUE : FALSE;
    }
    if ($erg == $gleich) {$ausgabeArr[]=$val;}
    return $erg;
}

//Gibt ein Array aus, welches Elemente enthält welche den Prüfbedingungen entspricht (falls gleich = FALSE, alle die nicht den Bedingungen entsprechen)
function filter_arr($arr,$vglVal,$SearchInKey="",$gleich=TRUE) {
  if (!is_array($arr)) {return array();}
  $arr_result = array();
  $userdata = array($vglVal,$SearchInKey,&$arr_result,$gleich);
  array_walk($arr,'proofVal',$userdata);
  reset($arr);
  return $arr_result;
}

//Falls gleich = FALSE, werden nur Datensätze zurückgegeben, die keinen der Angegeben Filterwerte enthalten
function filter_multi_arr($arr,$filter,$gleich=TRUE){
  if (!is_array($arr)) {return array();}
  $arr_res = $arr;
  while (list($key,$val) = each($filter)){
    $arr_res = filter_arr($arr_res,$val,$key,$gleich);
    reset($arr_res);
  }
  return $arr_res;
}

//Sortiert ein 2-dimensionales Array nach den Schlüsseln (müssen in kleinbuchstaben gehalten sein) in sort_keys (falls ein Schlüssel in Großbuchstaben angegeben wird, wird dieser Schlüssel absteigend sortiert, sonst aufsteigend)
//Schlüssel eines arrays die in Großbuchstaben geschrieben wurden werden ignoriert, Numerische Schlüssel werden immeraufsteigend sortiert
function dh_sort_arr($arr,$sort_keys=array()){
  $arr = $s_arr;
  if (is_array($sort_keys)){
    $s_k = array_values($sort_keys);
  } else {
    $s_k = array($sort_keys);
  }
  GLOBAL $dh_sort_keys_dh1984;
  $dh_sort_keys_dh1984 = $s_k;
  usort($s_arr,'DH_SORT_cmpfunc');
  return $s_arr;
}

function DH_SORT_cmpfunc($a,$b){
  GLOBAL $dh_sort_keys_dh1984;
  $s_k = array_values($dh_sort_keys_dh1984);
  $vgl = 0;
  while (list($k,$val)= each($s_k) AND $vgl==0){
    $vval = strtolower($val);
    if ($a[$vval]!=$b[$vval]){
      $vgl = ($a[$vval]>$b[$vval])? 1 : -1;
      $vgl = ($val == strtoupper($val) AND $val !=strtolower($val)) ? $vgl *= -1 : $vgl;
    }
  }
  return $vgl;
}

//Teilt arr in arrays auf, in denen alle in groups angegeben schlüssel den gleichen wert besitzen, die arrays, werdenals elemente im ausgabearray ausgegeben
//arr_help und i dürfen bei aufruf durch nutzer nicht angegeben werden, sin rekursivvariablen
/*function arr_split_in_groups($arr,$groups=array(),&$arr_help=array(),$i=0){
  if (is_array($groups)){
    $groups = array_values($groups);
  } else {
    $groups = $array($groups);
  }
  if (isset($groups[$i])){
    while (list($key,$el) = each($arr)){
      if (!isset($same) OR (isset($same) AND $same == $el[$groups[$i]])){
        $arr_temp[]=$el;
      } else {
        $arr_help[] = arr_split_in_groups($arr_temp,$groups,$arr_help,$i+1);
        unset($arr_temp);
        $same = $el[$groups[$i]];
      }
    }
  } else {
    return $arr;
  }
  if (!$i){
    return $arr_help;
  }
}*/



//Tabelle

function table_row($dats_arr,$class='',$th = FALSE){
  $tag = ($th) ? 'th' : 'td';
  $erg .= "<tr";
  $erg .= ($class) ? ' class="'.$class.'">' : '>';
  if (is_array($dats_arr)){
    while ( list ($k,$el) = each ($dats_arr)){
      $erg .= "<$tag>$el</$tag>";
      }
  } else {
    $erg.= "<$tag>$dats_arr</$tag>";
  }
  $erg .= "</tr>".chr(10);
  return $erg;
}

function table($arr_dats,$t_class='',$r_classes=array(''),$arr_th=array(),$th_class='',$tableTag=TRUE){
  if ($tableTag) {
    $erg = '<table';
    $erg .= ($t_class) ? ' class="'.$t_class.'">':'>';
  }
  $erg .= chr(10);
  $erg .= table_row($arr_th,$th_class,TRUE);

  $r_classes = (!is_array($r_classes)) ? array('') : array_values($r_classes);
  $i_m = count($r_classes);
  $i = 0;
  while (list($key,$val) = each($arr_dats)){
    $erg .= table_row($val,$r_classes[$i]);
    $i++;
    $i = ($i_m == $i) ? 0 : $i;
  }
  $erg .= chr(10);
  $erg .= ($tableTag)?'</table>':'';
  return $erg;
}


//Prüft ob $str in $comp beinhaltet ist
//bei Angabe von $comp als TRUE (und kein Array) ergibt die Function immer TRUE, auch wenn $str = FALSE;
//bei im array $comp wird nach den dortigen Schlüsseln unterschieldich geprüft
// Schlüssel 0 (also $comp[0]): wenn $comp[0] oder falls Array $comp[0] einer der Werte gleich $ str ==> TRUE
//analog:
// 1: wenn (einer) der Wert(e) von $comp mit $str beginnt
// 2: wenn (einer) der Wert(e) von $comp mit $str endet
// 3: wenn (einer) der Wert(e) von $comp $str enthält
//$strict gilt nur für Komplettvergleiche (also Key=0)
function dh_compare($str,$comp,$strict = FALSE){
 if (!is_array($comp)){
   return ($comp === TRUE OR c_s($str,$comp,$strict));
 } else {
   while (list($typ,$vgls) = each($comp)){
     switch ($typ){
       case 0:
         if (!is_array($vgls)) {
           $even = c_s($str,$vgls,$strict);
         } else {
            $even = in_array($str,$vgls,$strict);
         }
         break;
       case 1:
         $even = c_p_of_str($str,$vgls,1);
         break;
       case 2:
         $even = c_p_of_str($str,$vgls,-1);
         break;
       case 3:
         $even = c_p_of_str($str,$vgls,0);
         break;
     }
     if ($even) {return true;}
   }// /while
 }// /if !is_arr
 return FALSE;
}// /function

//Vergleicht eventuell strict
function c_s($val1,$val2,$strict=FALSE){
    if ($strict) {
      return ($val1 === $val2);
    } else {
      return ($val1 == $val2);
    }
}

//0 beinhaltet, 1 str am anfang, -1 str am ende , 2 str an strpos 1, -2 str an strpos 1 von hinten,... compare part of string
function c_p_of_str($str,$vgls,$type=0){
  if (is_array($vgls)){
    while ($erg !== TRUE AND list($key,$vgl) = each($vgls)){
        $erg = c_p_of_str($str,$vgl,$type);
    }
  } else {
    $pos = ($type>=0) ? strpos($str,$vgls) : strpos(strrev($strR),strrev($vgls));
    switch ($type){
      case 0:
        $erg = ($pos !== FALSE);
        break;
      default:
        $erg = ($pos !== FALSE AND $pos === abs($type)-1);
        break;
    }
  }
  return $erg;
}

//detail gibt an bis zu welchen daten angegeben werden muss (3~$d muss angegben werden,4~ $h,...)
function get_timestamp_proof($detail=5,$y='',$m='',$d='',$h='',$mi='',$s=''){
    $erg = FALSE;
    switch(1*$detail){
      case 6:
        $erg = ($s !== '' AND $s == $s*1);
      case 5:
        $erg = ($mi !== '' AND $mi == $mi*1);
      case 4:
        $erg = ($h !== '' AND $h == $h*1);
      case 3:
        $erg = ($d !== '' AND $d == $d*1);
      case 2:
        $erg = ($m !== '' AND $m == $m*1);
      case 1:
        $erg = ($y !== '' AND $y == $y*1);
      break;
    }
    if (!$erg) {return FALSE;}
    return mktime($h,$mi,$s,$m,$d,$y);
}

function date_time_german($timestamp,$datetime='array'){
  setlocale(LC_TIME,array('de_DE', 'de', 'ge'));
  $erg['date']=strftime('%d. %b %Y',$timestamp);
  $erg['time']=strftime('%R',$timestamp);
  switch ($datetime){
    case 'array':
        break;
    case 'date':
        $erg = $erg['date'];
        break;
    case 'time':
        $erg = $erg['time'];
        break;
    case 'datetime':
        $erg = $erg['date'].' um '.$erg['time'];
        break;
    case 'timedate':
        $erg = $erg['time'].' am '.$erg['date'];
        break;
  }
  return $erg;
}

function convert_text2htmlplus($str,$convplusarr=''){
  if (!is_array($convplusarr)){
    $convplusarr = array(
        '[b]'=>'<b>',
        '[/b]'=>'</b>',
        '[i]'=>'<i>',
        '[/i]'=>'</i>',
        '[u]'=>'<u>',
        '[/u]'=>'</u>',
        '[list]'=>'<ul>',
        '[/list]'=>'</ul>',
        'ä'=>'&auml;',
        'ö'=>'&ouml;',
        'ü'=>'&uuml;',
        'Ä'=>'&Auml;',
        'Ö'=>'&Ouml;',
        'Ü'=>'&Uuml;',
        'ß'=>'&szlig;',
        '"'=>'&quot;'
    );
  }
  $str = stripslashes($str);
  $str = htmlentities($str);
  $str = strtr($str,$convplusarr);
  $str = nl2br($str);
  return $str;
}

function convert_text($str,$convarr='',$encode=TRUE){
  if (!is_array($convarr)){
    $convarr = array(
        '"'=>'&quot;'
    );
  }
  if ($encode){
    $str = str_replace(array_keys($convarr),array_values($convarr),$str);
  } else {
    $str = str_replace(array_values($convarr),array_keys($convarr),$str);
  }
  return $str;
}

function dh_link($href='',$inhalt='',$target='',$title='',$attrib=''){
if ($inhalt === ''){$inhalt = $href;}
$erg.= '<a href="'.$href.'"';
$erg.=($target)?' target="'.$target.'"':'';
$erg.=($title)?' title="'.$title.'"':'';
$erg.=($attrib)?' target="'.$attrib.'"':'';
$erg.='>'.$inhalt.'</a>';
return $erg;
}

//Speichert einen String in einer Datei
function save_str2file($datei,$str){
       $handle = fopen($datei,'wb+');
       $erg = fwrite($handle,$str,strlen($str));
       fclose($handle);
       return $erg;
    }

//Sortiert array, nach einem wert eines bestimmten schlüssels ($dh_sort_key),l mit asc(1) oder desc(-1)
function sort_arr_arr($a,$b){
  GLOBAL $dh_sort_key;
  GLOBAL $dh_sort_asc; // 1 ansteigend, -1 abfallend
  $dhsa = ($dh_sort_asc) ? $dh_sort_asc : 1;
  $a2 = (isset($dh_sort_key)) ? $a[$dh_sort_key] : $a;
  $b2 = (isset($dh_sort_key)) ? $b[$dh_sort_key] : $b;
  switch (true) {
    case $a2 > $b2 : return $dhsa;
    case $a2 == $b2 : return 0;
    case $a2 < $b2 : return -1*$dhsa;
  }
}
?>
