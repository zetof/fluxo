<!DOCTYPE html>
<head>
  <link rel="stylesheet" href="core/main.css">
  <script type="text/javascript" src="core/color.js"></script>
  <script type="text/javascript" src="core/gauge.js"></script>
</head>
<body>
  <script>

    // Definition of gradients
    gp = ["black", "black", "lightblue", "deepskyblue", "mediumblue"];
    gh = ["orange", "orange", "white", "deepskyblue", "mediumblue"];
    gt = ["darkblue", "deepskyblue", "green", "orange", "red"];
    <?php
    $stmt = $pdo->prepare('SELECT * FROM `measures` WHERE sensor_id=? ORDER BY id DESC LIMIT 4');
    $stmt->execute([2]);
    $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
    foreach ($rows as $key=>$value) {
      switch($key){
        case 0:
          echo 'var p_now = '.$value['sensor_pressure'].';';
          echo 'var h_now = '.$value['sensor_humidity'].';';
          echo 'var t_now = '.$value['sensor_temp'].';';
          break;
        case 3:  
          echo 'var p_old = '.$value['sensor_pressure'].';';
          echo 'var h_old = '.$value['sensor_humidity'].';';
          echo 'var t_old = '.$value['sensor_temp'].';';
      }
    }
    ?>
    // Display 3 gauges for outside pressure, humidity and temperature
    pression = new Gauge({x:20, y:20, size:300, min:913, max:1113, val:p_now, old:p_old, gradient:gp, caption:"PRESSION", unit:"hPa"});
    outsideHumidity = new Gauge({x:340, y:20, size:300, min:0, max:100, val:h_now, old:h_old, gradient:gh, caption:"HUMIDITE", unit:"%"});
    outsideTemp = new Gauge({x:660, y:20, size:300, min:-20, max:50, val:t_now, old:t_old, decimal:true, gradient:gt, caption:"TEMPERATURE", unit:"°C"});
  </script>
</body>