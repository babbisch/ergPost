<?php

function spieltagsname_arr($file,$path='/configs'){
 $pfaddatei = dirname(__file__).$path.'/'.$file.'.spieltage';
 if (!file_exists($pfaddatei)){
  return FALSE;
 }
 $arr1 = file($pfaddatei);	
 $arrout = array();
 foreach($arr1 as $el){
	$el = trim($el);
	$temparr = explode(';',$el);
	$arr[$temparr[0]]=$temparr;
 }
 return $arr;

}

?>