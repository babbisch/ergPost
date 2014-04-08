<?php

@error_reporting(E_ALL);

// Für später, momentan nicht realisiert
$filelist777=array('/copysave','/ligen','/spiele','/spieler','/vereine');
$filelist666=array('/config.dh.inc','/ligen.ini');

$errors = array();
$error = '';

$install_step=isset($_REQUEST['install_step'])?$_REQUEST['install_step']:0;

$ergpost_dir = dirname(__FILE__);

$url='http://'.$_SERVER['HTTP_HOST'].dirname(($_SERVER['SCRIPT_NAME']));
$urlshort = substr($url,0,-20);
$urlshort2 = substr($url,0,-7);

?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html lang="de">
  <head>
    <title>Installation ErgPost für LMO</title>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <style type="text/css">
      @media all {
        body     {  max-width: 60em;margin: 0.5em auto;padding: 0 0.9em;font-size: 85%;background-color: #fff;color: #000;font-family: Tahoma, Verdana, sans-serif;border: 1px solid #999;-moz-border-radius: 8px;}
        h1       {  font-size: 150%; text-align: center; }
        table    {  margin: auto; }
        .error   {  border:1px solid #d99;background:#ffe7e0; }
      }
    </style>
  </head>
  <body>
  <h1>Installation ErgPost für LMO</h1>
  <table width="90%">
<?php
if ($install_step==0) {
?>
    <tr>
      <td>
        Die Installation von ErgPost erfolgt vollautomatisch. Bitte dazu die Installation starten.
      </td>
    </tr>
    <tr>
      <td>
        <form action="<?php echo $_SERVER['PHP_SELF']?>" method="post">
        <input type="hidden" name="install_step" value="1">
        <input type="submit" value="Installation starten">
        </form>
      </td>
    </tr>
<?php
} elseif ($install_step == 1) {
?>
    <tr>
      <td>
        Es folgen diverse Prüfungen. Bitte Fehler beachten, korrigieren und <a href="#" onclick="location.reload();return false;">ggf. erneut aufrufen</a>.
      </td>
    </tr>
  <?php
  if(!file_exists("config.dh.inc"))
    array_push($errors, "<div class='error'>Datei <strong>config.dh.inc</strong> exisitiert nicht.</div>");
  if(!is_writeable("config.dh.inc"))
    array_push($errors, "<div class='error'>Datei <strong>config.dh.inc</strong> hat kein CHMOD 666.</div>");
  ?>
    <tr>
      <td id='error'>
<?php
   foreach($errors as $error) {
      echo $error."<br />";
   }
?>
        </div>
    </tr>
  <?php
  if ($error == '') {

    $config = "<?php
define('_ergposthilfe_','http://www.derhasi.de/drupal/ergpost');
define('_berichtescript_','".$urlshort."include.php?path=berichte&');
define('_ergpostscript_','".$urlshort."include.php?path=ergpost');
define('_lmophp_','".$urlshort2."/lmo/lmo.php?');
define('_userlogin_','".$urlshort."include.php?path=login');
define('_userlogout_','".$urlshort."include.php?logout=1');
define('_userid_',"."$"."_SESSION['USER']['id']);
define('_username_',"."$"."_SESSION['USER']['name']);
?>";

    $configfile = fopen("config.dh.inc","w");
    fwrite($configfile,$config);
    fclose($configfile);

  ?>
    <tr>
      <td>
        Die Installation erfolgte. Jetzt kann die <a href="ergpost_ligaadmin.php">Ligenadministration</a> aufgerufen werden.
      </td>
    </tr>
  <?php
  }
  ?>
<?php
}
?>
  </table>
</body>
</html>