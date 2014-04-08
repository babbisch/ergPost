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
// If e107 is not running we won't run this plugin program
if( ! defined('e107_INIT')){ exit(); }

// Use this folder during install
$eplug_folder = "ergpost";

// Get language file (assume that the English language file is always present)
include_lan(e_PLUGIN.'ergpost/languages/'.e_LANGUAGE.'.php');

// To keep plugin name multilangual: store the constant name in the plugin table
// Plugin name is multi-language (from included language file)
// Otherwise for a fixed language term use the language constant name without the quotes
$eplug_name = 'ERGPOST_NAME';
$eplug_version = "1.0";
$eplug_author = "DwB";
$eplug_url = "http://ergpost.sf.net/";
$eplug_email = "dummwiebrot@gmail.com";
$eplug_description = ERGPOST_DESC;
$eplug_compatible = "e107v0.7+";
$eplug_compliant = TRUE; // indicator if plugin is XHTML compliant, shows icon

$eplug_menu_name = ERGPOST_MENU;
$eplug_conffile = "ergpost/admin_config.php";
$eplug_icon = $eplug_folder."/images/lmo_32.png";
$eplug_icon_small = $eplug_folder."/images/lmo_16.png";
$eplug_caption = ERGPOST_CAPTION;

// List of preferences ---------------------------------------------------------
// this stores a default value(s) in the preferences. 0 = Off , 1= On
// Preferences are saved with plugin folder name as prefix to make preferences unique and recognisable
$eplug_prefs = array(
	$eplug_folder."_name" => "ergPost",
    $eplug_folder."_image_path" => "images/"
);

// Create a link in main menu (yes=TRUE, no=FALSE) ---------------------------
$eplug_link = TRUE;

// To keep link multilangual: store the constant name in the links table
$eplug_link_name = ERGPOST_LINK_NAME;
// $plugins_directory can be named differently than the default e107_plugins in the e107_config
$eplug_link_url = e_PLUGIN."ergpost/ergpost.php";
$eplug_done = ERGPOST_DONE1." ".ERGPOST_NAME." v".$eplug_version." ".ERGPOST_DONE2;

// upgrading ... //
$upgrade_add_prefs = "";
$upgrade_remove_prefs = "";
$upgrade_alter_tables = "";



$eplug_upgrade_done = ERGPOST_DONE3." ".ERGPOST_NAME." v".$eplug_version.".";
?>