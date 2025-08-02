#ifndef INDEX_H
#define INDEX_H

const char INDEX[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>INDEX Page</title>
    <script>
        var defaultIP = "http://esp8266.local";

        function showPopup() {
            if (confirm("Do you want to open the Wi-Fi setup page in 3 seconds?")) {
                fetch(defaultIP + "/router_setup").catch(error => console.error("Error fetching data:", error));
                setTimeout(function () {
                    window.location.href = defaultIP;
                }, 3000);
            }
        }
    </script>
</head>

<body>
    <div style="display: flex;justify-content: center;">
        <button onclick="showPopup()">Wi-Fi Setup</button>
    </div>
</body>

</html>
)=====";

#endif
