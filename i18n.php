<?php
$lang = 'fr';
$langPath = __DIR__ . "/i18n/lang.{$lang}";

if (!file_exists($langPath)) {
    die('I18N file not found.');
}

$i18n = parse_ini_file($langPath, true);

function _t($section, array $prompts) {
    global $i18n;
    $constructed = '';
    foreach($prompts as $prompt) {
        if(isset($i18n[$section][$prompt])) $constructed .= $i18n[$section][$prompt];
        else $constructed .= "__i18n__<{$section}><{$prompt}>";
    }
    return $constructed;
}