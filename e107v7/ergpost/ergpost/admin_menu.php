<?php
/*
+------------------------------------------------------------------------------+
|     ergPost - a plugin for LMO 4.0.x
|
|     find LMO at http://www.liga-manager-online.de
|
|	Plugin Support Site and latest versions: ergpost.svn.sf.net
|
|	For the e107 website system visit http://e107.org
|
|	Released under the terms and conditions of the
|	GNU General Public License (http://gnu.org).
+------------------------------------------------------------------------------+
*/
if(!defined("e107_INIT")){ exit; }
$eplug_admin = true;

require_once('../../../class2.php');
require_once(e_ADMIN.'auth.php');

/// Get language file (assume that the English language file is always present)
include_lan(e_PLUGIN.'ergpost/languages/'.e_LANGUAGE.'.php');

global $pageid;

$action = basename($_SERVER['PHP_SELF'], '.php');

$var['admin_menu_01']['text'] = ERGPOST_MENU_01;
$var['admin_menu_01']['link'] = 'admin_config.php';

$var['admin_menu_02']['text'] = ERGPOST_MENU_02;
$var['admin_menu_02']['link'] = 'admin_install.php';


// Show the admin menu with a caption from the language file
$caption = ERGPOST_MENU_00;

show_admin_menu($caption, $pageid, $var);
?>