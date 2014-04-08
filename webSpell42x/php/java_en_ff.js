function dh_en_ff(formfield,auswahl,formname) {
if (document.forms[formname].elements[auswahl].value == ''){
document.forms[formname].elements[formfield].disabled = false;
document.forms[formname].elements[formfield].size = 30;
} else {
document.forms[formname].elements[formfield].disabled = true;
document.forms[formname].elements[formfield].size = 1;
};}
