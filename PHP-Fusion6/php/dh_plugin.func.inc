<?php

//Bindet Javadatei ein, falls noch nicht geschehen
//benötigte Funktionen: -keine-
function java_init($js_file){
  GLOBAL $DH_JAVA_INIT;
  if (!$DH_JAVA_INIT[$js_file]){
    $arr = file($js_file);
    $erg .= chr(10).'<script language="javascript" type="text/javascript">'.chr(10).'<!--';
    $erg .= implode(chr(10),$arr);
    $erg .= '//-->'.chr(10).'</script>'.chr(10);
    $DH_JAVA_INIT[$js_file]=TRUE;
  }
  return $erg;
}




?>
