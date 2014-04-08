<script language="javascript" type="text/javascript">
<?php
GLOBAL $dh_get;
$dh_get = $_GET;
unset($_GET);
if (!isset($dh_anker))
{
echo "if (window.location.search.length == 0)";
echo " parm = '?';";
echo "else parm = '&';";
echo "var dh_anker = window.location.hash.substr(1,window.location.hash.length -1);";
echo "var cloc = \"". $_SERVER["SCRIPT_URI"] ."\"+window.location.search;";
echo "if (window.location.hash.length > 0)";
echo "window.location.href = cloc + parm + 'dh_anker=' + dh_anker + window.location.hash;";
}
?>
</script>
