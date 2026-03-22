<!DOCTYPE html>
<head>
  <link rel="stylesheet" href="core/main.css">
  <script type="text/javascript" src="core/color.js"></script>
  <script type="text/javascript" src="core/gauge.js"></script>
</head>
<body>
  <div id="datetime" class="wrapper header">
    <h1>
      <?php
      $valid = $data['valid']?'ok':'ko';
      echo _t('gauges', ['measures'])._d($data['values'][0]['time']).' <span class="'.$valid.'">&#128959;</span>';
      ?>
    </h1>
  </div>
  <div class="wrapper">
    <div id="gauges" class="centered"></div>
  </div>
  <script>
    // Definition of gradients
    gp = ["black", "black", "lightblue", "deepskyblue", "mediumblue"];
    gh = ["orange", "orange", "white", "deepskyblue", "mediumblue"];
    gt = ["darkblue", "deepskyblue", "green", "orange", "red"];

    <?php
    echo 'var p_now = '.$data['values'][0]['pressure'].';';
    echo 'var h_now = '.$data['values'][0]['humidity'].';';
    echo 'var t_now = '.$data['values'][0]['temperature'].';';
    echo 'var p_old = '.$data['values'][3]['pressure'].';';
    echo 'var h_old = '.$data['values'][3]['humidity'].';';
    echo 'var t_old = '.$data['values'][3]['temperature'].';';
    ?>
    
    // Display 3 gauges for outside pressure, humidity and temperature
    pression = new Gauge({container:"gauges", x:20, y:20, size:300, min:913, max:1113, val:p_now, old:p_old, gradient:gp, caption:"<?php _t('gauges',['pressure'], display:true, up:true); ?>", unit:"hPa"});
    outsideHumidity = new Gauge({container:"gauges", x:340, y:20, size:300, min:0, max:100, val:h_now, old:h_old, gradient:gh, caption:"<?php _t('gauges',['humidity'], display:true, up:true); ?>", unit:"%"});
    outsideTemp = new Gauge({container:"gauges", x:660, y:20, size:300, min:-20, max:50, val:t_now, old:t_old, decimal:true, gradient:gt, caption:"<?php _t('gauges',['temperature'], display:true, up:true); ?>", unit:"°C"});
  </script>
</body>