<?php

$error='';
$url='http://'.$_SERVER['HTTP_HOST'].dirname(($_SERVER['SCRIPT_NAME']));

if(isset($_POST['sendliga'])) {

   if($_POST['ligafilename'] == '' || $_POST['liganame'] == '' || $_POST['ligastart'] == '' || $_POST['ligaend'] == '' || $_POST['ligamods'] == '') {
      $error.= "Einige Felder sind leer, bitte alles ausfüllen.";
   }
   echo $error;
   if($error == '') {
      $ligen = fopen("ligen.ini","ab");
      fputs($ligen,"[".$_POST['liganr']."]\r\n");
      fputs($ligen,"datei=".$_POST['ligafilename']."\r\n");
      fputs($ligen,"name=".$_POST['liganame']."\r\n");
      fputs($ligen,"sta=".$_POST['ligastart']."\r\n");
      fputs($ligen,"ste=".$_POST['ligaend']."\r\n");
      fputs($ligen,"result=".$_POST['ligaresults']."\r\n");
      fputs($ligen,"teamstat=".$_POST['ligastats']."\r\n");
      fputs($ligen,"scorer=".$_POST['ligascorer']."\r\n");
      fputs($ligen,"cards=".$_POST['ligacards']."\r\n");
      fputs($ligen,"shots=".$_POST['ligashots']."\r\n");
      fputs($ligen,"bericht=".$_POST['ligareport']."\r\n");
      fputs($ligen,"moreinfo=".$_POST['ligamoreinfo']."\r\n");
      fputs($ligen,"mods=".$_POST['ligamods']."\r\n");
      fclose($ligen);
      $liga = fopen("ligen/".$_POST['ligafilename'], "ab");
      fclose($liga);
      header("Location: ".$url."/ergpost_ligaadmin.php");
   }
} else {
   $ligen = fopen("ligen.ini","rb");
   while($ligen && !feof($ligen)) {
      $zeile = fgets($ligen);
      $zeile = trim(rtrim($zeile));
      if (substr($zeile,0,1) == '[') {
         $intligavalue = substr($zeile,strrpos($zeile,"[")+1,strrpos($zeile,"]")-1) + 1;
      }
      $intligavalue == '' ? $intligavalue = '1' : '';
   }
   fclose($ligen);

   echo "<form name='ligenadmin' method='post' action='ergpost_ligaadmin.php'>
<input type='hidden' name='liganr' value='$intligavalue'>
<table align='center'>
<tr><td>Liganummer</td><td>$intligavalue</td></tr>
<tr><td>Dateiname der Liga</td><td><input type='text' name='ligafilename' class='textbox'></td>
<tr><td>Name der Liga</td><td><input type='text' name='liganame' class='textbox'></td>
<tr><td>Spieltag Start</td><td><input type='text' name='ligastart'  maxlength='2' class='textbox'></td>
<tr><td>Spieltag Ende</td><td><input type='text' name='ligaend' maxlength='2' class='textbox'></td>
<tr><td>Ergebnisse eintragen</td><td><input type='radio' name='ligaresults' value='1' checked> ja <input type='radio' name='ligaresults' value='0'> nein </td>
<tr><td>Teamstatistik eintragen</td><td><input type='radio' name='ligastats' value='1' checked> ja <input type='radio' name='ligastats' value='0'> nein </td>
<tr><td>Torschützen eintragen</td><td><input type='radio' name='ligascorer' value='1' checked> ja <input type='radio' name='ligascorer' value='0'> nein </td>
<tr><td>Karten eintragen</td><td><input type='radio' name='ligacards' value='1' checked> ja <input type='radio' name='ligacards' value='0'> nein </td>
<tr><td>Torsch&uuml;sse eintragen</td><td><input type='radio' name='ligashots' value='1' checked> ja <input type='radio' name='ligashots' value='0'> nein </td>
<tr><td>Bericht eintragen</td><td><input type='radio' name='ligareport' value='1' checked> ja <input type='radio' name='ligareport' value='0'> nein </td>
<tr><td>Minutenangaben/Vorlagengeber</td><td><input type='radio' name='ligamoreinfo' value='1' checked> ja <input type='radio' name='ligamoreinfo' value='0'> nein </td>
<tr><td>Erfasser</td><td><input type='text' name='ligamods' class='textbox'></td>
<tr><td colspan='2'><input type='submit' name='sendliga' value='Ligadaten speichern' class='button'>
</table>
</form>";
}

?>