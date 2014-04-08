<?php

//=====FORMULAR FUnktionen=================================================//


//Erzeugt ein Eingabefeld mit Name, Wert und Typ, zusätzliche Attribute müssen im HTML-Format in $attrib angegeben werden , ein Javabefehl kann in $java folgen
//Benötigte Funktionen: -keine-
function dh_input($name,$value,$type='text',$attrib='',$java=''){
    if (!$name AND $name !== 0){return FALSE;}
    if (!in_array($type,array('text','checkbox','radio','submit','image','hidden','file'))){return FALSE;}
    $erg = '<input name="'.$name.'" type="'.$type.'" value="'.$value.'"';
    $erg .= ($attrib) ? ' '.$attrib : '';
    $erg .= ($java) ? ' '.$java : '';
    $erg .= '>';
    return $erg;
}

//attrib
function dh_auswahl($arr,$name='optbox',$selected='',$size=1,$maxselected=1,$attrib='',$keyVal='val',$keyText='text',$keySel='selected'){
    if (!is_array($arr)){return FALSE;}
    if (!is_array($selected)){$selected = array($selected);}
    $opt1 = '<option value="';
    $opt2 = '"';
    $opt2a = '>';
    $opt3 = '</option>';
    $erg ='<select name="'.$name.'" size="'.$size.'"';
    $erg .= ($attrib) ? ' '.$attrib : '';
    $erg .= ($maxselected>1)?' multiple':'';
    $erg .= '>';

    while (list($key,$el) = each ($arr)){
      $erg .= $opt1.$el[$keyVal].$opt2;
      if ($maxselected > 0 AND ($el[$keySel] OR in_array($el[$keyVal],$selected))){$maxselected--; $erg.=' selected';}
      $erg .= $opt2a.$el[$keyText].$opt3;
    }

    $erg .= '</select>';
    $erg .= $ende;
    return $erg;
}



function dh_textarea($name,$value='',$rows=10,$cols=10,$attrib =''){
  $erg.='<textarea name="'.$name.'" rows="'.$rows.'" cols="'.$cols.'"';
  $erg .= ($attrib)? ' .$attrib':'';
  $erg.= '>'.$value.'</textarea>';
  return $erg;
}







?>
