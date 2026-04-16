<!DOCTYPE html>
<head>
    <link rel="stylesheet" href="core/main.css">
    <script type="text/javascript" src="core/color.js"></script>
    <script type="text/javascript" src="core/gauge.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/4.5.0/chart.umd.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/date-fns/4.1.0/cdn.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/chartjs-adapter-date-fns/3.0.0/chartjs-adapter-date-fns.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/date-fns/4.1.0/locale/fr/cdn.min.js"></script>
</head>
<body>
    <div id="datetime" class="wrapper header">
        <h1>
        <?php
            $valid = $data['valid']?'&#9989;':'&#10060';
            $location = $data['location'];
            echo _t('gauges', ['measures', $location, 'when'])._d($data['values'][0]['time']).' '.$valid;
        ?>
        </h1>
    </div>
    <div class="wrapper">
        <div id="gauges" class="centered"></div>
    </div>
    <div class="wrapper">
        <div id="temperature" class="centered">
        <canvas id="historyChart" width="900" height="450"></canvas>
        </div>
    </div>

    <script>
    <?php
        echo 'var p_now = '.$data['values'][0]['pressure'].';';
        echo 'var h_now = '.$data['values'][0]['humidity'].';';
        echo 'var t_now = '.$data['values'][0]['temperature'].';';
        echo 'var p_old = '.$data['values'][3]['pressure'].';';
        echo 'var h_old = '.$data['values'][3]['humidity'].';';
        echo 'var t_old = '.$data['values'][3]['temperature'].';';

        $last = count($data['values']) - 1;
        $index = $last;
        while($index) {
        if($index == $last) {
            $xVals = '["'.date_format(date_create($data['values'][$index]['time']),"H:i").'"';
            $ypVals = '['.strval($data['values'][$index]['pressure']);
            $yhVals = '['.strval($data['values'][$index]['humidity']);
            $ytVals = '['.strval($data['values'][$index]['temperature']);
        }
        else {
            $xVals .= ', "'.date_format(date_create($data['values'][$index]['time']),"H:i").'"';
            $ypVals .= ', '.strval($data['values'][$index]['pressure']);
            $yhVals .= ', '.strval($data['values'][$index]['humidity']);
            $ytVals .= ', '.strval($data['values'][$index]['temperature']);
        }
        $index--;
        }
        echo 'xVals = '.$xVals.'];';
        echo 'ypVals = '.$ypVals.'];';
        echo 'yhVals = '.$yhVals.'];';
        echo 'ytVals = '.$ytVals.'];';
    ?>

        // Definition of gradients
        gp = ["black", "black", "lightblue", "deepskyblue", "mediumblue"];
        gh = ["orange", "orange", "white", "deepskyblue", "mediumblue"];
        gt = ["darkblue", "deepskyblue", "green", "orange", "red"];

        // Display 3 gauges for outside pressure, humidity and temperature
        pression = new Gauge({container:"gauges", x:20, y:20, size:240, min:913, max:1113, val:p_now, old:p_old, gradient:gp, caption:"<?php _t('gauges',['pressure'], display:true, up:true); ?>", unit:"hPa"});
        humidity = new Gauge({container:"gauges", x:340, y:20, size:240, min:0, max:100, val:h_now, old:h_old, gradient:gh, caption:"<?php _t('gauges',['humidity'], display:true, up:true); ?>", unit:"%"});
        temperature = new Gauge({container:"gauges", x:660, y:20, size:240, min:-20, max:50, val:t_now, old:t_old, decimal:true, gradient:gt, caption:"<?php _t('gauges',['temperature'], display:true, up:true); ?>", unit:"°C"});
    
        const ctx = document.getElementById('historyChart');
        Chart.defaults.color = 'white';
        new Chart(ctx, {
            type: "line",
            data: {
                labels: xVals,
                datasets: [{ 
                    data: ytVals,
                    yAxisID: 'yTemp',
                    borderColor: "Tomato",
                    pointRadius: 4,
                    tension: 0.2,
                    label: 'Température (°C)',
                    },
                { 
                    data: yhVals,
                    yAxisID: 'yHum',
                    borderColor: "DodgerBlue",
                    pointRadius: 4,
                    tension: 0.2,
                    label: 'Humidité (%)',
                    },
                { 
                    data: ypVals,
                    yAxisID: 'yPres',
                    borderColor: "MediumVioletRed",
                    pointRadius: 4,
                    tension: 0.2,
                    label: 'Pression (hPa)',
                    }]
            },
            options: {
                interaction: { mode: 'index', intersect: false },
                stacked: false,
                legend: {
                    fontColor: "white",
                },
                scales: {
                    yTemp: {
                        type: 'linear',
                        position: 'left',
                        title: { display: true, text: '°C' }
                    },
                    yHum: {
                        type: 'linear',
                        position: 'right',
                        title: { display: true, text: '%' },
                        grid: { drawOnChartArea: false }
                    },
                    yPres: {
                        type: 'linear',
                        position: 'right',
                        title: { display: true, text: 'hPa' },
                        grid: { drawOnChartArea: false },
                        offset: true
                    }
              }
            }
        });
  </script>
</body>