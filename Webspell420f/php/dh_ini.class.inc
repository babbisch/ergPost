<?php

// ----------------------------------------------------------------------------------------
// Global Constants
// ----------------------------------------------------------------------------------------

// If you have longer lines change the value (in bytes) so it fits your requirements.
define("_LineLength_", "16384");

// If you have a different remark symbol in your INI file change it.
define("_REMARK_", ";");

// If you ar not authorized to write to the global temp directory create a own one
// in your space and change the path to your temp directory.
define("_TempDir_", PATH_TO_LMO."/output");

// If you need an other file type (end of line) you can change it. Linefeeds for
// different OS are:
//     Linux:   chr(10) or "\n"
//     Windows: chr(13).chr(10) or "\r\n"
//     MAC:     chr(13) or "\r"
define("_LINEFEED_", "\n");

// If you use different - not INI like - section limiter and value delimiter
// you can change the values. This should not be neccesary.
define("_SectionStart_", "[");
define("_SectionStop_", "]");
define("_ValueDelimiter_", "=");

// If you prefer an other prefix for temporary files change the value. This
// should not be neccesary.
define("_TempPrefix_", "~db-ini_");

// Do NOT change anything here!
define("_READ_", "rb");
define("_WRITE_", "wb+");
define("_APPEND_", "a+b");
/*
###########################################################################################
#
#                D E R H A S I    C L A S S   F O R   I N I - F I L E S
#
###########################################################################################
//by derhasi: Reading via Array, usefull for multiple Data research
//Addition to myinifunctions by Tom Gries - tom@xsurf.net - http://sourceforge.net/projects/myinifunctions/.*/

$dir = dirname(__file__);
include_once("$dir/dh_func.init.inc");

/*CLASS DH_INI
#########################################################################*/
class DH_INI
{
    //Variables
    var $file = '';
    var $original = FALSE;
    var $current = array(); //Current INI Datas
    var $output = '';
    var $exists = TRUE;
    var $sections_arr = array();
    
//####### FUNKTIONEN die Klassenvariablen verändern #############
    // - Constructor
    function DH_INI($inifile){
      if ($this->file == ''){
        if (is_file($inifile)) {
          $this->file = $inifile;
          $this->current = $this->original();
        } else {
          //ERROR: $inifile ist keine gültige Datei, initialisierung der Klasse DH_INI fehlgeschlagen [ENG]
        } //is_file()
      } else {
        //ERROR: Das Objekt wurde bereits initialisiert
      }//this->file
    }

    function original(){
      if ($this->original === FALSE) {
        $this->original = $this->ReadIniCompleteInArr($this->file);
      }
      return $this->original;
    }

    //Ändert und fügt Values hinzu in current (Array)
    function change_add_values($arr){
      $ziel = &$this->current;
      while (list($key,$data) = each($arr)){
        if (isset($data['section']) AND isset($data['item']) AND isset($data['value']))
        $ziel[$data['section']][$data['item']]=$data['value'];
      }
    }

    // - setzt OUTPUT_DATA aus current
    function set_output_data(){
        $a = ($this->output = $this->arr2ini($this->current));
        return $a;
    }

    //Holt den Wert von [section][item]
    function get_val_o($section,$item){
      $temp = &$this->original();
      //echo "<br>SECTION:$section-ITEM:$item";
      return $temp[$section][$item];
    }

    // - Holt Daten aus Original (original)
    function get_data_original($section=TRUE,$item=TRUE,$value=TRUE,$strict=TRUE){
      return $this->get_data($this->original(),$section,$item,$value,$strict);
    }
    
    // - Holt Daten aus Aktuell (current)   //gibt es als Array zurück
    function get_data_current($section=TRUE,$item=TRUE,$value=TRUE,$strict=TRUE){
      return $this->get_data($this->current,$section,$item,$value,$strict);
    }
    
    // - Schreibt Datensatz in aktuellen INI-Array
    function write_data_c($section,$item,$value,$change=TRUE,$create=TRUE){
      return $this->write_data($this->current,$section,$item,$value,$change,$create);
    }
    
    // - Schreibt Daten aus Array (wie es get_data ausgibt) in den aktuellen INI-Array - Gibt Anzahl geänderter und geschriebener Datensätze zurück
    function write_arr_c($arr_input,$change=TRUE,$create=TRUE){
      if (!is_array($arr_input)){return FALSE;}
      $i=0;
      while (list($key,$val) = each($arr_input)){
        if (isset($val['section']) AND isset($val['item'])){
          $i += $this->write_data_c($val['section'],$val['item'],$val['value'],$change,$create);
        }
      }
      return i;
    }
    
    
    // - Speichert die aktualisierte INI-Datei auf dem Server
    function save($datei,$inistring){
       return save_str2file($datei,$inistring);
    }
    
    function save_c(){
      $this->set_output_data();
      return $this->save($this->file,$this->output);
    }


//  ##### Algemeine FUNKTIONEN für die Klasse ######
    // ----------------------------------------------------------------------------------------
    function ReadIniCompleteInArr($inifile) {
    // ----------------------------------------------------------------------------------------
    // This function returns an array with all the values of a given inifile as an array
    // if exists. Items without a '=' are not identified as an item.
    // Whitespaces will be stripped and are allowed. They are allowed also before and after the '='.
    // To mark a line as a comment, the first non whitespace character must be a _REMARK_.
    // Sections are the keys of the first layer. In each section Item work as key and Value is      item
    // e.g. : $arr['Section']['Item'] = Value;
    if (is_file($inifile)) {
        $file = fopen($inifile, _READ_);
        $arr = array();
        while (!feof($file)) {
            $line = trim(fgets($file, _LineLength_));
            if ($this->IsSection($line)) {
              $start_s = strpos($line, _SectionStart_) + 1;
              $stop_s = strrpos($line, _SectionStop_) - 1;
              $line = substr(trim($line), $start_s, $stop_s);
              $section = trim($line);
              $this->sections_arr[]=$section;
            } else {
              $stop = strpos($line, _ValueDelimiter_);
              if (substr($line, 0, 1) != _REMARK_ AND $line) {
                    $key = trim(substr($line, 0, $stop));
                    $arr[$section][$key] = trim(substr($line, $stop + 1));
              } // remarks
            }
        } // while !feof()
        fclose($file);
        return $arr;
      } //if is_file ()
    }
    // ----------------------------------------------------------------------------------------

    //---- Gibt alle Daten aus $arr als array die mit den Kriterien $section,$value,$item übereinstimmen, falls eines der Kriterien FALSE (!array) dann wird dies und evtl. nachfolgende Kriterien nicht mehr geprüft und ebenfalls nicht mehr angezeigt
    function get_data($arr,$section=TRUE,$item=TRUE,$value=TRUE,$strict=TRUE){
      if (!is_array($arr)) {return array();}
      $erg = array();
      while (list($sec,$el) = each($arr)){
        $add = array();
        if ($section !== FALSE AND dh_compare($sec,$section,$strict)){
          $add['section'] = $sec;
          if ($item === FALSE) {
            $erg[] = $add;
          } else {
            while (list($itm,$val) = each ($el)){
              if(dh_compare($itm,$item,$strict)){
                $add['item'] = $itm;
                if ($value === FALSE) {
                  $erg[] = $add;
                } else {
                  if (dh_compare($val,$value,$strict)){
                    $add['value'] = $val;
                    $erg[] = $add;
                  } //if comp val
                }//if value FALSE
              }//if comp itm
            }//while values
          }//if item FALSE
        }//if comp sec
      }//while elements
      return $erg;
    }//func get_data
    
    //Holt den Wert von [section][item]
    function get_val($arr,$section,$item){
      return $arr[$section][$item];
    }
    
    //------------------------ Ändert Datensatz in einem Ini_Array, falls nicht vorhanden wird dieser erstellt  !!! Funktioniert nur mit Referenziertem $arr , Gibt bei Erfolg TRUE zurück - $change gibt an ob der Wert eines bereits vorhandenen Datensatzes bearbeitet werden darf, create gibt an ob ein neuer ertstellt werden darf, falls es ihn nicht gibt
    function write_data(&$arr,$section,$item,$value,$change=TRUE,$create=TRUE){
      if (is_array($section) OR is_array($item) OR is_array($value)){return FALSE;}
      if (!$change AND isset($arr[$section][$item])){return FALSE;}
      if (!$create AND !isset($arr[$section][$item])){return FALSE;}
      //echo $value;
      $arr[$section][$item] = $value;
      return TRUE;
    }
    
    function write_arr(&$arr,$arr_input,$change=TRUE,$create=TRUE){
      if (!is_array($arr_input)){return FALSE;}
      $i=0;
      while (list($key,$val) = each($arr_input)){
        if (isset($val['section']) AND isset($val['item'])){
          $i += $this->write_data($arr,$val['section'],$val['item'],$val['value'],$change,$create);
        }
      }
      return i;
    }
    

    //---------------------------Wandelt Arrray in einen INI-String um
    function arr2ini($arr){
        $glue_k = array('',array(_SectionStop_.chr(10),_SectionStart_),_ValueDelimiter_);
        $glue = array('','',chr(10));
        $key = TRUE;
        $val = TRUE;
        return arr2str($arr,$glue,$glue_k,$key,$val);
    }
// ----------------------------------------------------------------------------------------
function IsSection($line) {
// ----------------------------------------------------------------------------------------
//
// This function checks if a string is a 'section' in an Ini-File. The credentials
// are: it must start with _SectionStart_ and it must end with _SectionStop_.
//
// Whitespaces will be stripped and are allowed. To mark a line as a comment, the first non
// whitespace character must be a _REMARK_.

  if (substr(trim($line), 0, 1) == _REMARK_) {
    return(FALSE);
  }
  $start = strpos(trim($line), _SectionStart_);
  $stop = strpos(strrev(trim($line)), strrev(_SectionStop_));
  return ($start === 0 && $stop === 0);
}
// ----------------------------------------------------------------------------------------
}


?>
